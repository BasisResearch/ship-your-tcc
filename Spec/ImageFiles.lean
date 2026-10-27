import CerbFS

/-!
Read-only views of the image filesystem. Reuse CerbFS's ordered initializer
and component traversal, shared with the C semantics; do not normalize away
missing directories or invent a different duplicate-file policy.

Paths here are bytes, as in htif.c. The one-byte-to-Char injection is only
an adapter to CerbFS's component representation, not UTF-8 decoding.
-/
namespace ImageFiles

abbrev Entry := List UInt8 × Array UInt8

def pathChars (p : List UInt8) : List Char := p.map (fun b => Char.ofNat b.toNat)

/-- Initial embedded table, in order, with the image's node capacity. -/
def initial (entries : List Entry) : CerbFS.FsState :=
  entries.foldl (fun st (p, data) =>
    CerbFS.embed data st 0 (CerbFS.splitSlash (pathChars p))) CerbFS.emptyState

/-- Read an existing regular file, preserving failed intermediate traversal. -/
def lookup (st : CerbFS.FsState) (path : List UInt8) : Option (Array UInt8) :=
  match (CerbFS.resolve st (String.ofList (pathChars path))).target with
  | .file i => (st.node? i).map (·.data)
  | _ => none

/-- Flat immutable views used by the compiler host model. They are derived
from the initialized tree, so file/directory collisions and capacity agree. -/
structure Views where
  files : Array (ByteArray × ByteArray) := #[]
  dirs : Array ByteArray := #[]

/-- The initializer allocates each parent before its children and never
unlinks nodes. Export only this initial tree, not an arbitrary mutable FS. -/
def views (entries : List Entry) : Views := Id.run do
  let st := initial entries
  let mut paths := Array.replicate st.nodes.size ByteArray.empty
  let mut out : Views := {}
  for i in [1:st.nodes.size] do
    if let some node := st.nodes[i]! then
      let name := ByteArray.mk (node.name.toArray.map (fun c => c.toNat.toUInt8))
      let path := (paths[node.parent]!.push 47) ++ name
      paths := paths.set! i path
      if node.dir then out := { out with dirs := out.dirs.push path }
      else out := { out with files := out.files.push (path, ByteArray.mk node.data) }
  return out

end ImageFiles
