import LemLib
/-
  Filesystem model for Cerberus: THE tcc -run IMAGE'S FILE SYSTEM (ship-your-tcc F1b, DESIGN-COMPILER.md §3.4
  "pathleak").
  Corresponds to: c/src/htif.c (the image's in-memory file system: `fs_init`, `resolve`, `_open`, `_close`,
  `_read`, `_write`, `_lseek`, `_fstat`, `_stat`, `_unlink`, `_link`) under newlib 4.5.0's system-call wrappers
  (libc.a's `open`/`read`/... → `_open_r`/`_read_r`/... → htif.c; an error is -1 with errno set from htif.c's
  code), which is exactly what a program run by `tcc -run` in the image calls (c/src/main.c
  `tcc_baremetal_syms`). Upstream maps this module to SibylFS (`sibylfs/generated/sibylfs.ml` `run_*`); the
  fork refused every operation from 2026-09-28 ([USER 2026-09-28] contract D2, after the raw-path-string
  report: `docs/2026-09-28_cerbfs-path-hotfix-record.md`). This model replaces both by the image's own file
  system, so CSem's file-system meaning is the one `tcc -run` gives a program.

  The model (pure and total; no IO, no `partial`):
  * a node table of `maxFiles` = 256 slots (htif.c `MAX_FILES`); slot 0 is the root directory; a node is a
    file (bytes, `Array UInt8`) or a directory; a node is the entry `name` of directory `parent` while
    `linked`, and lives on unlinked while a descriptor refers to it (`opens`), then its slot is freed
    (htif.c `release`). Embedded files are read-only in the image until first written and then copied
    (`make_writable`); that copy is unobservable, so here they are plain mutable files.
  * a descriptor table of `maxFds` = 32 entries: 0, 1, 2 are the console streams (stdin, stdout, stderr), a
    file descriptor records its node, offset, access mode and O_APPEND; `open` takes the smallest free number;
    a free or out-of-range number is EBADF.
  * path resolution is htif.c `resolve`, component by component from the root (the working directory is `/`,
    so relative paths start there): empty components (repeated or trailing `/`) and `.` stay, `..` goes to
    the parent (the root's parent is the root), a missing last component is "absent" (an `open` with O_CREAT
    may create it), a missing inner component is ENOENT, a file followed by more path is ENOTDIR. There is no
    lexical normalisation: `/usr/../prog.c` is ENOENT when `/usr` does not exist, as in the image.
  * stdin is empty: `read` on fd 0 returns 0 bytes (htif.c `_read`: no console input over HTIF); `write` on fd
    0 and `read` on fds 1/2 are EBADF. Console output never reaches this module: driver.lem routes `write` on a
    descriptor that is a console stream (`fs_console_fd`) to the stdout/stderr records.
  * `stat`/`fstat` fill only `st_mode` (S_IFREG|0644 or S_IFDIR|0755, S_IFCHR for a console stream),
    `st_nlink` (1/0 for a linked/unlinked file, 2 + subdirectories for a directory, 1 for a console) and
    `st_size` (files); every other field is 0 (htif.c `fill_stat`). `fstat` has a
    separate Core builtin and descriptor operation; a literal `/proc/self/fd/<n>`
    path receives ordinary path resolution, never descriptor semantics.
  * `link` is EMLINK (htif.c `_link`), and so is `rename`: newlib's `_rename_r` without HAVE_RENAME is
    `_link_r` then `_unlink_r` (libc.a's renamer.o references both), so the first step fails with EMLINK.
  * O_* flags arrive in Cerberus's encoding (runtime/libc/include/posix/fcntl.h, from fs_spec.lem) and are
    decoded to what the program asked for (`OpenFlags.decode`); O_RDONLY is newlib's 0, so no access bit
    means read-only and O_WRONLY|O_RDWR is EINVAL (newlib's access mode 3).

  Refused (fail-closed, `failwithI`): the operations the image does not give a program — `mkdir`, `rmdir`,
  `chdir`, `umask`, `chmod`, `chown`, `readlink`, `symlink`, `truncate`, `lstat`, `pread`, `pwrite`,
  `opendir`/`readdir`/`rewinddir`/`closedir`. newlib's libc.a in the image defines none of them and
  `tcc_baremetal_syms` exports none (it exports `_mkdir`, reachable only from libc's unexported
  `_mkdir_r`), so `tcc -run` fails to link a program that calls one: there is no tcc meaning to model.
  Not modelled (resource limits, the theorem's `Fits`): htif.c's ENOSPC when `malloc`/`realloc` fails, and
  offsets beyond 2^63.

  Errors are `FsError`s; driver.lem `store_error` maps `fs_string_of_error` through builtins.lem
  `translate_errno` (Cerberus's errno.h numbering) into errno and the call returns -1.
  This is a leaf module.
-/

namespace CerbFS

/-- Filesystem stat information -/
structure FsStat where
  dev : Int := 0
  ino : Int := 0
  mode : Int := 0
  nlink : Int := 0
  uid : Int := 0
  gid : Int := 0
  rdev : Int := 0
  size : Int := 0
  atime : Int := 0
  mtime : Int := 0
  ctime : Int := 0
  deriving Inhabited, BEq

/-- Filesystem error: the errno names htif.c returns (plus the historical constructors). -/
inductive FsError where
  | enoent : FsError    -- No such file or directory
  | eacces : FsError    -- Permission denied
  | eexist : FsError    -- File exists
  | ebadf  : FsError    -- Bad file descriptor
  | enosys : FsError    -- Not implemented
  | enotdir : FsError   -- Not a directory
  | eisdir : FsError    -- Is a directory
  | einval : FsError    -- Invalid argument
  | emfile : FsError    -- Too many open files
  | enospc : FsError    -- No space left on device (node table full)
  | espipe : FsError    -- Illegal seek
  | emlink : FsError    -- Too many links
  | other : String → FsError
  deriving Inhabited, BEq, Repr, Ord

/-- `panic!` needs an inhabitant at the refusal sites below; the
    default is an enosys error (never observed: the driver refuses to run
    without LEAN_ABORT_ON_PANIC, so the panic aborts first). -/
instance (α : Type) : Inhabited (Sum FsError α) := ⟨Sum.inl .enosys⟩

/-- htif.c `MAX_FILES`: node slots, files and directories; slot 0 is the root. -/
def maxFiles : Nat := 256
/-- htif.c `MAX_FDS`. -/
def maxFds : Nat := 32

/-- A file or directory (htif.c `struct mfile`). -/
structure Node where
  dir : Bool
  /-- it is the entry `name` of directory `parent` -/
  linked : Bool
  parent : Nat
  name : List Char
  /-- a file's contents -/
  data : Array UInt8
  /-- descriptors referring to it -/
  opens : Nat
  deriving Inhabited, BEq, Repr, Ord

/-- An access mode (newlib's O_RDONLY/O_WRONLY/O_RDWR). -/
inductive Access where
  | rdonly | wronly | rdwr
  deriving Inhabited, BEq, Repr, Ord

/-- A descriptor on a node (htif.c `struct mfd` of kind FD_FILE). -/
structure OpenFile where
  node : Nat
  pos : Nat
  acc : Access
  append : Bool
  deriving Inhabited, BEq, Repr, Ord

/-- A descriptor table entry (htif.c `struct mfd`). -/
inductive Fd where
  | free | stdin | stdout | stderr
  | file (f : OpenFile)
  deriving Inhabited, BEq, Repr, Ord

/-- The image's file system: the node table and the descriptor table. -/
structure FsState where
  nodes : Array (Option Node)
  fds : Array Fd
  deriving BEq, Repr, Ord

/-- The root directory alone (htif.c `fs_init` before the embedded files), console streams on 0–2. -/
def emptyState : FsState where
  nodes := #[some { dir := true, linked := false, parent := 0, name := [], data := #[], opens := 0 }] ++
    Array.replicate (maxFiles - 1) none
  fds := #[.stdin, .stdout, .stderr] ++ Array.replicate (maxFds - 3) .free

instance : Inhabited FsState := ⟨emptyState⟩

-- Stat field accessors
def fs_dev  (s : FsStat) : Int := s.dev
def fs_ino  (s : FsStat) : Int := s.ino
def fs_mode (s : FsStat) : Int := s.mode
def fs_nlink (s : FsStat) : Int := s.nlink
def fs_uid  (s : FsStat) : Int := s.uid
def fs_gid  (s : FsStat) : Int := s.gid
def fs_rdev (s : FsStat) : Int := s.rdev
def fs_size (s : FsStat) : Int := s.size
def fs_atime (s : FsStat) : Int := s.atime
def fs_mtime (s : FsStat) : Int := s.mtime
def fs_ctime (s : FsStat) : Int := s.ctime

def fs_string_of_error : FsError → String
  | .enoent => "ENOENT"
  | .eacces => "EACCES"
  | .eexist => "EEXIST"
  | .ebadf  => "EBADF"
  | .enosys => "ENOSYS"
  | .enotdir => "ENOTDIR"
  | .eisdir => "EISDIR"
  | .einval => "EINVAL"
  | .emfile => "EMFILE"
  | .enospc => "ENOSPC"
  | .espipe => "ESPIPE"
  | .emlink => "EMLINK"
  | .other s => s

def string_of_fs_state (_ : FsState) : String := "<fs_state>"

/-! ## Bytes and characters

Lem's `list char` carries bytes: `Char.ofNat b` for byte `b` (CerbUtils.encode_character_constant), so
`c.toNat < 256`. -/

def bytesOfChars (cs : List Char) : List UInt8 := cs.map fun c => c.toNat.toUInt8
def charsOfBytes (bs : List UInt8) : List Char := bs.map fun b => Char.ofNat b.toNat

/-! ## Node table -/

namespace FsState

def node? (st : FsState) (i : Nat) : Option Node := (st.nodes[i]?).join

def isDir (st : FsState) (i : Nat) : Bool := (st.node? i).any (·.dir)

/-- The parent of directory `d` (the root's parent is the root). -/
def parentOf (st : FsState) (d : Nat) : Nat := ((st.node? d).map (·.parent)).getD 0

/-- The entry `n` of directory `d` (htif.c `child`). The root is nobody's entry (`linked = false`). -/
def child (st : FsState) (d : Nat) (n : List Char) : Option Nat :=
  st.nodes.findIdx? fun o => match o with
    | some f => f.linked && f.parent == d && f.name == n
    | none => false

/-- The number of subdirectories of `d` (htif.c `nsubdirs`). -/
def nsubdirs (st : FsState) (d : Nat) : Nat :=
  st.nodes.foldl (fun k o => match o with
    | some f => if f.linked && f.dir && f.parent == d then k + 1 else k
    | none => k) 0

def modifyNode (st : FsState) (i : Nat) (f : Node → Node) : FsState :=
  { st with nodes := st.nodes.modify i (Option.map f) }

/-- A new entry `n` of `d` in the first free slot (htif.c `new_node`); `none` when the table is full. -/
def newNode (st : FsState) (d : Nat) (n : List Char) (dir : Bool) (data : Array UInt8) :
    Option (FsState × Nat) :=
  match st.nodes.findIdx? Option.isNone with
  | some i =>
    let node : Node := { dir := dir, linked := true, parent := d, name := n, data := data, opens := 0 }
    some ({ st with nodes := st.nodes.modify i (fun _ => some node) }, i)
  | none => none

/-- Free node `i` once it is neither linked nor open (htif.c `release`; the root stays). -/
def release (st : FsState) (i : Nat) : FsState :=
  match st.node? i with
  | some f => if i != 0 && !f.linked && f.opens == 0 then
      { st with nodes := st.nodes.modify i fun _ => none } else st
  | none => st

/-- Remove the entry naming `i` (htif.c `unlink_node`). -/
def unlinkNode (st : FsState) (i : Nat) : FsState :=
  (st.modifyNode i fun f => { f with linked := false }).release i

/-- The open descriptor `fd` (htif.c `getfd`). -/
def getFd (st : FsState) (fd : Int) : Option Fd :=
  if fd < 0 then none else
  match st.fds[fd.toNat]? with
  | some .free => none
  | o => o

def setFd (st : FsState) (fd : Nat) (e : Fd) : FsState :=
  { st with fds := st.fds.modify fd fun _ => e }

end FsState

/-! ## Path resolution (htif.c `resolve`) -/

/-- `String.splitOn "/"` on characters: `a//b/` ↦ `[a, "", b, ""]`; never empty. -/
def splitSlash : List Char → List (List Char)
  | [] => [[]]
  | c :: cs =>
    let rest := splitSlash cs
    if c == '/' then [] :: rest
    else match rest with
      | w :: ws => (c :: w) :: ws
      | [] => [[c]]

/-- What a path names (htif.c `struct res`'s kind). -/
inductive Target where
  /-- an existing directory -/
  | dir (d : Nat)
  /-- an existing file -/
  | file (node : Nat)
  /-- nothing, as the entry `name` of the existing directory `d` (the last component) -/
  | absent (d : Nat) (name : List Char)
  | error (e : FsError)
  deriving Inhabited, BEq, Repr, Ord

/-- The result of resolving a path. -/
structure Resolution where
  target : Target
  /-- the path ends with `/` -/
  slash : Bool
  deriving Inhabited, BEq, Repr, Ord

/-- htif.c `resolve`'s loop from directory `d` over the components; the C loop's `rest == NULL` is
`rest = []`, its `last` is "only empty components follow". -/
def walk (st : FsState) (d : Nat) : List (List Char) → Target
  | [] => .dir d
  | comp :: rest =>
    if comp.isEmpty || comp == ['.'] then walk st d rest
    else if comp == ['.', '.'] then walk st (st.parentOf d) rest
    else match st.child d comp with
      | none => if rest.all List.isEmpty then .absent d comp else .error .enoent
      | some c =>
        if st.isDir c then walk st c rest
        else if rest.isEmpty then .file c
        else .error .enotdir

def resolve (st : FsState) (path : String) : Resolution :=
  let cs := path.toList
  { target := if cs.isEmpty then .error .enoent else walk st 0 (splitSlash cs)
    slash := cs.getLast? == some '/' }

/-! ## The initial state: the image's embedded files (htif.c `fs_init`) -/

/-- Add one embedded file along its path's components from `d`: every component followed by `/` is a
directory (created when missing; the walk stops at a file), empty components are skipped, the last one is
the file (created when missing, its contents replaced when it is a file, skipped when a directory; a full
table skips it). Paths are taken literally, as gen_embed.sh writes them (no `.`/`..`). -/
def embed (data : Array UInt8) : FsState → Nat → List (List Char) → FsState
  | st, _, [] => st
  | st, d, c :: rest =>
    if c.isEmpty then embed data st d rest
    else if rest.isEmpty then
      match st.child d c with
      | none => match st.newNode d c false data with
        | some (st', _) => st'
        | none => st
      | some i => if st.isDir i then st else st.modifyNode i fun f => { f with data }
    else
      match st.child d c with
      | none => match st.newNode d c true #[] with
        | some (st', i) => embed data st' i rest
        | none => st
      | some i => if st.isDir i then embed data st i rest else st

/-- **The image's initial file system** for the embedded file table `files` (absolute paths, in table
order; a later entry for the same path replaces the earlier contents), with console streams on 0–2. -/
def fs_initial_state_of (files : List (String × List UInt8)) : FsState :=
  files.foldl (fun st (p, bs) => embed bs.toArray st 0 (splitSlash p.toList)) emptyState

/-- The fork's own lanes: the root directory alone. -/
def fs_initial_state : FsState := emptyState

/-! ## Operations -/

/-- The flags of an `open`, decoded from Cerberus's O_* encoding (runtime/libc/include/posix/fcntl.h). -/
structure OpenFlags where
  /-- `none`: not a valid access mode (EINVAL) -/
  acc : Option Access
  creat : Bool
  excl : Bool
  trunc : Bool
  append : Bool
  directory : Bool

def testBit (x : Int) (b : Nat) : Bool := Int.emod (Int.ediv x ((2 : Int) ^ b)) 2 == 1

/-- O_WRONLY = 0o4 (bit 2), O_RDWR = 0o10 (bit 3), O_CREAT = 0o40, O_EXCL = 0o100, O_TRUNC = 0o400,
O_APPEND = 0o1000, O_DIRECTORY = 0o40000; O_RDONLY (0o2) is newlib's 0, so it carries no information. -/
def OpenFlags.decode (oflag : Int) : OpenFlags where
  acc := match testBit oflag 2, testBit oflag 3 with
    | false, false => some .rdonly
    | true, false => some .wronly
    | false, true => some .rdwr
    | true, true => none
  creat := testBit oflag 5
  excl := testBit oflag 6
  trunc := testBit oflag 8
  append := testBit oflag 9
  directory := testBit oflag 14

def Target.isAbsent : Target → Bool | .absent .. => true | _ => false
def Target.isDir : Target → Bool | .dir _ => true | _ => false
def Target.isFile : Target → Bool | .file _ => true | _ => false

/-- htif.c `_open`'s checks after the flag checks, in its order. -/
def openCheck (fl : OpenFlags) (wr : Bool) (r : Resolution) : Option FsError :=
  match r.target with
  | .error e => some e
  | t =>
    if t.isAbsent && !fl.creat then some .enoent
    else if fl.creat && r.slash then some .eisdir
    else if (wr || fl.trunc) && t.isDir then some .eisdir
    else if fl.directory && t.isFile then some .enotdir
    else if fl.creat && fl.excl && !t.isAbsent then some .eexist
    else if fl.creat && t.isDir then some .eisdir
    else none

def fs_open (st : FsState) (path : String) (oflag : Int) (_mode : Option Int) : FsState × (Sum FsError Nat) :=
  let fl := OpenFlags.decode oflag
  match fl.acc with
  | none => (st, .inl .einval)
  | some acc =>
    if fl.creat && fl.directory then (st, .inl .einval) else
    let r := resolve st path
    match openCheck fl (acc != .rdonly) r with
    | some e => (st, .inl e)
    | none =>
      match st.fds.findIdx? (· == .free) with
      | none => (st, .inl .emfile)
      | some fd =>
        let created : Option (FsState × Nat) := match r.target with
          | .dir d => some (st, d)
          | .file n => some (st, n)
          | .absent d n => st.newNode d n false #[]
          | .error _ => none
        match created with
        | none => (st, .inl .enospc)
        | some (st1, node) =>
          let st2 := st1.modifyNode node fun f =>
            { f with data := if fl.trunc && !f.dir then #[] else f.data, opens := f.opens + 1 }
          (st2.setFd fd (.file { node, pos := 0, acc, append := fl.append }), .inr fd)

def fs_close (st : FsState) (fd : Int) : FsState × (Sum FsError Nat) :=
  match st.getFd fd with
  | none => (st, .inl .ebadf)
  | some e =>
    let st1 := match e with
      | .file f => (st.modifyNode f.node fun n => { n with opens := n.opens - 1 }).release f.node
      | _ => st
    (st1.setFd fd.toNat .free, .inr 0)

def fs_read (st : FsState) (fd : Int) (count : Int) : FsState × (Sum FsError (List Char)) :=
  match st.getFd fd with
  | none | some .free | some .stdout | some .stderr => (st, .inl .ebadf)
  | some .stdin => (st, .inr [])
  | some (.file f) =>
    if f.acc == .wronly then (st, .inl .ebadf) else
    match st.node? f.node with
    | none => (st, .inl .ebadf)
    | some n =>
      if n.dir then (st, .inl .eisdir) else
      let len := min count.toNat (n.data.size - f.pos)
      let bytes := (n.data.extract f.pos (f.pos + len)).toList
      (st.setFd fd.toNat (.file { f with pos := f.pos + len }), .inr (charsOfBytes bytes))

/-- `a` with `bs` written at `pos`, zero-filled up to `pos` (htif.c `_write`'s memset + memcpy). -/
def writeAt (a : Array UInt8) (pos : Nat) (bs : List UInt8) : Array UInt8 :=
  let a' := if a.size < pos then a ++ Array.replicate (pos - a.size) 0 else a
  a'.extract 0 pos ++ bs.toArray ++ a'.extract (pos + bs.length) a'.size

/-- The refusal for an operation that cannot happen here. -/
private def fsRefusal (op : String) (why : String) : String :=
  s!"CerbFS refusal (fail-closed fs-model boundary): {op} — {why}"

def fs_write (st : FsState) (fd : Int) (data : List Char) (count : Int) : FsState × (Sum FsError Nat) :=
  match st.getFd fd with
  | none | some .free | some .stdin => (st, .inl .ebadf)
  | some .stdout | some .stderr =>
    failwithI (fsRefusal s!"write fd {fd} count {count}"
      "a console stream reached the file model (driver.lem routes console writes by fs_console_fd)")
  | some (.file f) =>
    if f.acc == .rdonly then (st, .inl .ebadf) else
    let bs := bytesOfChars (data.take count.toNat)
    if bs.isEmpty then (st, .inr 0) else
    match st.node? f.node with
    | none => (st, .inl .ebadf)
    | some n =>
      let pos := if f.append then n.data.size else f.pos
      let st1 := st.modifyNode f.node fun n => { n with data := writeAt n.data pos bs }
      (st1.setFd fd.toNat (.file { f with pos := pos + bs.length }), .inr bs.length)

/-- 1 when `fd` is the stdout stream, 2 when it is the stderr stream, 0 otherwise (driver.lem routes
`write` on a console stream to its stdout/stderr records). -/
def fs_console_fd (st : FsState) (fd : Int) : Nat :=
  match st.getFd fd with
  | some .stdout => 1
  | some .stderr => 2
  | _ => 0

def fs_lseek (st : FsState) (fd offset whence : Int) : FsState × (Sum FsError Nat) :=
  match st.getFd fd with
  | none => (st, .inl .ebadf)
  | some e =>
    if !(whence == 0 || whence == 1 || whence == 2) then (st, .inl .einval) else
    match e with
    | .file f =>
      match st.node? f.node with
      | none => (st, .inl .ebadf)
      | some n =>
        if whence == 2 && n.dir then (st, .inl .einval) else
        let base : Int := if whence == 0 then 0 else if whence == 1 then f.pos else n.data.size
        if offset < 0 && base < -offset then (st, .inl .einval) else
        let pos := (base + offset).toNat
        (st.setFd fd.toNat (.file { f with pos }), .inr pos)
    | _ => (st, .inl .espipe)

/-- htif.c `fill_stat`. -/
def fillStat (st : FsState) (i : Nat) : FsStat :=
  match st.node? i with
  | some n =>
    if n.dir then { mode := 0o40755, nlink := 2 + st.nsubdirs i }
    else { mode := 0o100644, nlink := if n.linked then 1 else 0, size := n.data.size }
  | none => {}

/-- htif.c `_fstat`. -/
def fstat (st : FsState) (fd : Int) : Sum FsError FsStat :=
  match st.getFd fd with
  | none => .inl .ebadf
  | some (.file f) => .inr (fillStat st f.node)
  | some _ => .inr { mode := 0o20000, nlink := 1 }

/-- Descriptor metadata, independently of path spelling. -/
def fs_fstat (st : FsState) (fd : Int) : FsState × (Sum FsError FsStat) :=
  (st, fstat st fd)

def fs_stat (st : FsState) (path : String) : FsState × (Sum FsError FsStat) :=
  match (resolve st path).target with
  | .error e => (st, .inl e)
  | .absent .. => (st, .inl .enoent)
  | .dir d => (st, .inr (fillStat st d))
  | .file n => (st, .inr (fillStat st n))

def fs_unlink (st : FsState) (path : String) : FsState × (Sum FsError Nat) :=
  let r := resolve st path
  match r.target with
  | .error e => (st, .inl e)
  | .absent .. => (st, .inl (if r.slash then .enotdir else .enoent))
  | .dir _ => (st, .inl .eisdir)
  | .file n => (st.unlinkNode n, .inr 0)

/-- htif.c `_link`: always EMLINK. -/
def fs_link (st : FsState) (_oldP _newP : String) : FsState × (Sum FsError Nat) := (st, .inl .emlink)

/-- newlib's `_rename_r` without HAVE_RENAME: `_link_r` (EMLINK, so it stops there). -/
def fs_rename (st : FsState) (oldP newP : String) : FsState × (Sum FsError Nat) := fs_link st oldP newP

/-- The refusal for an operation the image does not give a program. -/
private def notInImage (op : String) : String :=
  fsRefusal op "not provided to the program by the tcc -run image (newlib 4.5.0's libc.a defines no such \
    function and c/src/main.c tcc_baremetal_syms exports none), so tcc fails to link a program that calls \
    it and there is no tcc meaning to model (CerbFS.lean header)"

def fs_mkdir (_ : FsState) (path : String) (mode : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"mkdir '{path}' mode {mode}")
def fs_pwrite (_ : FsState) (fd : Int) (data : List Char) (count offset : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"pwrite fd {fd} count {count} offset {offset} ({data.length}-byte buffer)")
def fs_pread (_ : FsState) (fd : Int) (count off : Int) : FsState × (Sum FsError (List Char)) :=
  failwithI (notInImage s!"pread fd {fd} count {count} offset {off}")
def fs_umask (_ : FsState) (mask : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"umask {mask}")
def fs_chmod (_ : FsState) (path : String) (mode : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"chmod '{path}' mode {mode}")
def fs_chdir (_ : FsState) (dir : String) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"chdir '{dir}'")
def fs_chown (_ : FsState) (path : String) (uid gid : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"chown '{path}' uid {uid} gid {gid}")
def fs_readlink (_ : FsState) (path : String) : FsState × (Sum FsError (List Char)) :=
  failwithI (notInImage s!"readlink '{path}'")
def fs_symlink (_ : FsState) (target lpath : String) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"symlink '{target}' <- '{lpath}'")
def fs_rmdir (_ : FsState) (path : String) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"rmdir '{path}'")
def fs_truncate (_ : FsState) (path : String) (len : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"truncate '{path}' to {len}")
def fs_lstat (_ : FsState) (path : String) : FsState × (Sum FsError FsStat) :=
  failwithI (notInImage s!"lstat '{path}'")
def fs_opendir (_ : FsState) (path : String) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"opendir '{path}'")
def fs_readdir (_ : FsState) (dir : Int) : FsState × (Sum FsError (List Char)) :=
  failwithI (notInImage s!"readdir {dir}")
def fs_rewinddir (_ : FsState) (dir : Int) : FsState :=
  failwithI (notInImage s!"rewinddir {dir}")
def fs_closedir (_ : FsState) (dir : Int) : FsState × (Sum FsError Nat) :=
  failwithI (notInImage s!"closedir {dir}")

end CerbFS
