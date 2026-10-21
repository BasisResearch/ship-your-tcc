import Tcc.Model.State

/-!
# TccModel: the image's host layer as tcc sees it

tcc runs on newlib with the image's system-call layer (c/src/htif.c): an
in-memory, read-only file system built from the embedded files, file
descriptors (0–2 the console, files from 3 up, the lowest free first),
and a console shared by stdout and stderr. This module models what tcc
observes of it: `_open` (with htif.c's path resolution, `resolve`),
`_read`, `_lseek`, `_close`, and console output.
-/

namespace Tcc.Model
namespace Host

/-- split a path at '/' (keeping empty components, as htif.c's walk does) -/
private def splitSlash (p : ByteArray) : List ByteArray := Id.run do
  let mut out : Array ByteArray := #[]
  let mut cur := ByteArray.empty
  for c in p.data do
    if c == 47 then out := out.push cur; cur := ByteArray.empty
    else cur := cur.push c
  out := out.push cur
  return out.toList

inductive Res where
  | dir (path : ByteArray)
  | file (path : ByteArray)
  | none
  | err
  deriving Inhabited

private def parentOf (d : ByteArray) : ByteArray := Id.run do
  -- d is "" (root) or "/a/b"
  let mut last := 0
  for i in [0:d.size] do
    if d.get! i == 47 then last := i
  return d.extract 0 last

/-- htif.c's `resolve` (the working directory is the root) -/
def resolve (fsFiles : Std.HashMap ByteArray ByteArray) (fsDirs : Std.HashMap ByteArray Unit)
    (path : ByteArray) : Res := Id.run do
  if path.size == 0 then return .err
  let comps := splitSlash path
  let n := comps.length
  let mut d : ByteArray := ByteArray.empty
  let mut idx := 0
  for k in comps do
    let isLastSeg := idx + 1 == n
    -- `last`: no more components, or only slashes remain
    let restAllEmpty := (comps.drop (idx + 1)).all (·.size == 0)
    if k.size == 0 || (k.size == 1 && k.get! 0 == 46) then
      pure ()
    else if k.size == 2 && k.get! 0 == 46 && k.get! 1 == 46 then
      d := parentOf d
    else
      let cp := (d.push 47) ++ k
      if fsDirs.contains cp then
        d := cp
      else if fsFiles.contains cp then
        if isLastSeg then return .file cp
        else return .err
      else
        if isLastSeg || restAllEmpty then return .none
        else return .err
    idx := idx + 1
  return .dir d

/-- `_open(path, O_RDONLY)`: the descriptor, or -1 -/
def openRd (path : ByteArray) : M Int32 := do
  let s ← get
  match resolve s.fsFiles s.fsDirs path with
  | .err | .none => return -1
  | r =>
    let e : FdEntry := match r with
      | .file p => { data := s.fsFiles.getD p ByteArray.empty, pos := 0, isDir := false }
      | _ => { isDir := true }
    -- the lowest free descriptor (0..2 are the console)
    let mut fd := 3
    for i in [3:32] do
      if (s.fds[i]?.join).isNone then fd := i; break
      fd := i + 1
    if fd ≥ 32 then return -1
    let fds := if s.fds.size ≤ fd then s.fds ++ Array.replicate (fd + 1 - s.fds.size) Option.none else s.fds
    set { s with fds := fds.set! fd (some e) }
    return Int32.ofNat fd

/-- `_read(fd, buf, len)`: the bytes read (none: -1) -/
def read (fd : Int32) (len : Nat) : M (Option ByteArray) := do
  let s ← get
  match s.fds[fd.toNat']?.join with
  | some e =>
    if e.isDir then return none
    let avail := e.data.size - e.pos
    let n := min len avail
    let out := e.data.extract e.pos (e.pos + n)
    modify fun s => { s with fds := s.fds.set! fd.toNat' (some { e with pos := e.pos + n }) }
    return some out
  | none => if fd == 0 then return some ByteArray.empty else return none

/-- `_lseek(fd, off, SEEK_SET)` -/
def lseekSet (fd : Int32) (off : Nat) : M Int64 := do
  let s ← get
  match s.fds[fd.toNat']?.join with
  | some e =>
    modify fun s => { s with fds := s.fds.set! fd.toNat' (some { e with pos := off }) }
    return Int64.ofNat off
  | none => return -1

def close (fd : Int32) : M Unit := do
  modify fun s => if fd.toNat' < s.fds.size then { s with fds := s.fds.set! fd.toNat' none } else s

end Host
end Tcc.Model
