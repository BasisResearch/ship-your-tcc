/* ship-your-tcc: TinyCC's config.h for the bare-metal image, in place of the
 * one ./configure writes. tcc is a native riscv64 compiler here, linked
 * statically into the image (c/Makefile), running tcc -run only. */
#define TCC_VERSION "0.9.28rc"
#define CC_NAME CC_gcc
#define GCC_MAJOR 15
#define GCC_MINOR 2
#define TCC_TARGET_RISCV64 1
#define ONE_SOURCE 1

/* the image (patches/0002-tcc-baremetal-run.patch) */
#define TCC_BAREMETAL 1
/* no dlopen/dlsym (tccrun.c's static table, empty under TCC_BAREMETAL) */
#define CONFIG_TCC_STATIC 1
#define CONFIG_TCC_BACKTRACE 0
#define CONFIG_TCC_BCHECK 0
#define CONFIG_TCC_SEMLOCK 0
/* include/tccdefs.h is read from the image's file system */
#define CONFIG_TCC_PREDEFS 0

/* paths in the image's file system (htif.c, gen_embed.sh) */
#define CONFIG_TCCDIR "/lib/tcc"
#define CONFIG_TCC_SYSINCLUDEPATHS "{B}/include:/include"
#define CONFIG_TCC_LIBPATHS "{B}:/lib"   /* {B}: runmain.o, libtcc1.a */
#define CONFIG_TCC_CRTPREFIX "/lib"
#define CONFIG_TCC_ELFINTERP "-"
/* tcc_add_runtime adds it after -lc: newlib's libc.a calls libgcc's
   soft-float and mul/div routines, and so does the generated code */
#define TCC_LIBGCC "/lib/libgcc.a"
