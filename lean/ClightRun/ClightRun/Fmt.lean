/-
  FFI to the host libc's snprintf for one printf conversion at a time
  (see c/fmt.c).  Specs are NUL-terminated ByteArrays.
-/
namespace ClightRun

@[extern "clightrun_fmt_sll"] opaque fmtSll (spec : @& ByteArray) (v : UInt64) : ByteArray
@[extern "clightrun_fmt_ull"] opaque fmtUll (spec : @& ByteArray) (v : UInt64) : ByteArray
@[extern "clightrun_fmt_chr"] opaque fmtChr (spec : @& ByteArray) (v : UInt32) : ByteArray
@[extern "clightrun_fmt_dbl"] opaque fmtDbl (spec : @& ByteArray) (v : Float) : ByteArray
@[extern "clightrun_fmt_str"] opaque fmtStr (spec : @& ByteArray) (s : @& ByteArray) : ByteArray

end ClightRun
