import Lake
open Lake DSL System

/-! The fork's native C support as a Lake `extern_lib` (ours; upstream links a
`leanc`-compiled `native/md5.o` through `moreLinkArgs`, Makefile target
`lean-native-obj`). The source stays at its upstream path,
`csem/lean_frontend/native/md5.c`: the MD5 digest `cerb_md5_hex` and the
per-TU digest cell `cerb_digest_get`/`cerb_digest_set` behind
`CerberusFresh`'s `@[extern]` declarations. Lake links the extern libraries
of every package an executable's package depends on, so the root project's
`csem_run` and the fork's test executables get it. A native seam in the
semantic cone (DESIGN-COMPILER.md §3.4): F1 replaces it by its pure meaning. -/

package CSemNative

extern_lib cerbnative pkg := do
  let src := pkg.dir / ".." / "lean_frontend" / "native" / "md5.c"
  let o ← buildLeanO (pkg.buildDir / "native" / "md5.o") (← inputTextFile src) #[] #["-O2", "-fPIC"]
  buildStaticLib (pkg.staticLibDir / nameToStaticLib "cerbnative") #[o]
