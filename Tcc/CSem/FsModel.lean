import CerbFS

/-!
# Unit checks of CSem's image file-system model (F1b, DESIGN-COMPILER.md §3.4 pathleak)

`CerbFS` (csem/lean_frontend/CerbFS.lean) models c/src/htif.c under newlib's wrappers. These `#guard`s
evaluate it on a small image: path resolution (the same file under several spellings, no lexical
normalisation, ENOENT/ENOTDIR), the descriptor table (smallest free number, EMFILE at 32), reads,
writes with zero fill, O_APPEND, O_TRUNC, lseek, stat/fstat and unlink. The differential evidence against
tcc on Sail is c/tests/fs/ and results/csem/f1b-fs.md.
-/

namespace Tcc.CSem.FsModel

open CerbFS

private def bytes (s : String) : List UInt8 := s.toUTF8.toList

/-- A small image: `/prog.c`, a header below `/lib/tcc/include`, one in `/include`. -/
def img : FsState :=
  fs_initial_state_of [("/prog.c", bytes "int main;\n"), ("/lib/tcc/include/x.h", bytes "x"),
    ("/include/a.h", bytes "aa")]

-- Cerberus's O_* encoding (runtime/libc/include/posix/fcntl.h)
private def O_RDONLY : Int := 0o2
private def O_WRONLY : Int := 0o4
private def O_RDWR : Int := 0o10
private def O_CREAT : Int := 0o40
private def O_EXCL : Int := 0o100
private def O_TRUNC : Int := 0o400
private def O_APPEND : Int := 0o1000

def target (p : String) : Target := (resolve img p).target

def progNode : Nat := match target "/prog.c" with | .file n => n | _ => 0

#guard progNode != 0
#guard ["/prog.c", "./prog.c", "//prog.c", "prog.c", "/./prog.c", "/../prog.c", "../../prog.c",
        "/include/../prog.c", "/lib/tcc/../../prog.c"].all (target · == .file progNode)
-- no lexical normalisation: /usr does not exist
#guard target "/usr/../prog.c" == .error .enoent
#guard target "/prog.c/x" == .error .enotdir
#guard target "/prog.c/" == .error .enotdir
#guard target "/prog.c/." == .error .enotdir
#guard target "" == .error .enoent
#guard target "/missing" == .absent 0 "missing".toList
#guard target "/missing/" == .absent 0 "missing".toList
#guard target "/missing/x" == .error .enoent
#guard target "/" == .dir 0
#guard target "/.." == .dir 0
#guard (match target "/lib//tcc///include/x.h", target "lib/tcc/include/../include/./x.h" with
        | .file a, .file b => a == b | _, _ => false)

/-- Open, then read `n` bytes. -/
def openRead (st : FsState) (p : String) (n : Int) : Option (List Char) :=
  match fs_open st p O_RDONLY none with
  | (st', .inr fd) => match fs_read st' fd n with
    | (_, .inr cs) => some cs
    | _ => none
  | _ => none

#guard openRead img "/prog.c" 3 == some "int".toList
#guard openRead img "prog.c" 100 == some "int main;\n".toList
#guard (fs_open img "/prog.c" O_RDONLY none).2 == .inr 3
#guard (fs_open img "/prog.c" (O_CREAT + O_EXCL + O_WRONLY) none).2 == .inl .eexist
#guard (fs_open img "/new/" (O_CREAT + O_WRONLY) none).2 == .inl .eisdir
#guard (fs_open img "/lib" O_WRONLY none).2 == .inl .eisdir
#guard (fs_open img "/nodir/x" (O_CREAT + O_WRONLY) none).2 == .inl .enoent
#guard (fs_open img "/prog.c" (O_WRONLY + O_RDWR) none).2 == .inl .einval
-- stdin is empty; fd 0 is not writable, fds 1/2 not readable
#guard (fs_read img 0 10).2 == .inr []
#guard (fs_write img 0 ['x'] 1).2 == .inl .ebadf
#guard (fs_read img 1 1).2 == .inl .ebadf
#guard fs_console_fd img 1 == 1 && fs_console_fd img 2 == 2 && fs_console_fd img 0 == 0
#guard (fs_close img 7).2 == .inl .ebadf
#guard (fs_lseek img 1 0 0).2 == .inl .espipe

/-- Open `/prog.c` repeatedly: 29 descriptors (3–31), then EMFILE. -/
def openMany : Nat → FsState → List (Sum FsError Nat)
  | 0, _ => []
  | k + 1, st => match fs_open st "/prog.c" O_RDONLY none with
    | (st', r) => r :: openMany k st'

#guard ((openMany 30 img).filter (· matches .inr _)).length == 29
#guard (openMany 30 img).getLast? == some (.inl .emfile)

/-- Create `/n.txt`, write "hello", seek to 8, write "X", seek to 0, read 20. -/
def rw : Option (List Char × Nat) := do
  let (st, .inr fd) := fs_open img "/n.txt" (O_CREAT + O_RDWR) none | none
  let (st, .inr 5) := fs_write st fd "hello".toList 5 | none
  let (st, .inr 8) := fs_lseek st fd 8 0 | none
  let (st, .inr 1) := fs_write st fd ['X'] 1 | none
  let (st, .inr 0) := fs_lseek st fd 0 0 | none
  let (st, .inr cs) := fs_read st fd 20 | none
  let (_, .inr s) := fs_stat st "/n.txt" | none
  pure (cs, s.size.toNat)

#guard rw == some ("hello".toList ++ [Char.ofNat 0, Char.ofNat 0, Char.ofNat 0, 'X'], 9)

/-- O_APPEND writes at the end whatever the offset; O_TRUNC empties. -/
def appendTrunc : Option (List Char) := do
  let (st, .inr fd) := fs_open img "/include/a.h" (O_WRONLY + O_APPEND) none | none
  let (st, .inr 0) := fs_lseek st fd 0 0 | none
  let (st, .inr 1) := fs_write st fd ['b'] 1 | none
  let (st, .inr 0) := fs_close st fd | none
  let s1 ← openRead st "/include/a.h" 10
  let (st, .inr fd) := fs_open st "/include/a.h" (O_WRONLY + O_TRUNC) none | none
  let (st, .inr 0) := fs_close st fd | none
  let s2 ← openRead st "/include/a.h" 10
  pure (s1 ++ ['|'] ++ s2)

#guard appendTrunc == some "aab|".toList

-- stat: S_IFREG|0644 with size; a directory is S_IFDIR|0755 with 2 + subdirectories links
#guard (match (fs_stat img "/prog.c").2 with
        | .inr s => s.mode == 0o100644 && s.size == 10 && s.nlink == 1 | _ => false)
#guard (match (fs_stat img "/lib").2 with
        | .inr s => s.mode == 0o40755 && s.nlink == 3 | _ => false)
#guard (match (fs_stat img "/").2 with | .inr s => s.nlink == 4 | _ => false)
#guard (fs_stat img "/missing").2 matches .inl .enoent
-- fstat through Cerberus's libc path form
#guard (match (fs_stat img "/proc/self/fd/0").2 with | .inr s => s.mode == 0o20000 | _ => false)
#guard (fs_stat img "/proc/self/fd/9").2 matches .inl .ebadf

/-- Unlink while open: the name is gone, the descriptor still reads, fstat says nlink 0. -/
def unlinkOpen : Option (List Char × Int) := do
  let (st, .inr fd) := fs_open img "/prog.c" O_RDONLY none | none
  let (st, .inr 0) := fs_unlink st "/prog.c" | none
  let (st, .inl .enoent) := fs_stat st "/prog.c" | none
  let (st, .inr cs) := fs_read st fd 3 | none
  let (_, .inr s) := fs_stat st s!"/proc/self/fd/{fd}" | none
  pure (cs, s.nlink)

#guard unlinkOpen == some ("int".toList, 0)
#guard (fs_unlink img "/lib").2 == .inl .eisdir
#guard (fs_unlink img "/missing/").2 == .inl .enotdir
#guard (fs_rename img "/prog.c" "/q.c").2 == .inl .emlink

end Tcc.CSem.FsModel
