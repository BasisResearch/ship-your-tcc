# tcc -run for bare-metal RV64I (HTIF): TinyCC 43c7708 (vendor/tinycc) plus
# patches/*.patch, linked into one static image with newlib, an in-image
# read-only file system (headers, libc.a, libgcc.a, libtcc1.a) and the C
# program to compile and run, /prog.c.
#
#   make [PROG=tests/run/hello.c] [FLAGS=1] [ARGS='"-lm",']
#                     ./tcc-riscv-htif.elf running `tcc -run /prog.c`
#   make run          run it on the Sail Lean emulator (EMU=...)
#   make isa-check    no M/F/D/A/C instruction (nor ecall) in the image
#   make clean
#
# FLAGS is gen_embed.sh's embedded_flags (bit 0: print the generated code
# before running it, see src/main.c). ARGS are extra tcc arguments, as C
# string literals each followed by a comma; they go before -run.
# Everything that depends on PROG/FLAGS/ARGS is in the .embed section at a
# fixed address (src/link.ld), so the rest of the image is the same bytes
# for every program (tests/run.sh checks this).

# Cross toolchain: the xPack riscv-none-elf-gcc 15.2.0 (GCC 15.2.0 + newlib
# 4.5.0.20241231), the release that built ship-your-lua's and
# ship-your-ocaml's images.
XPACK_GCC := $(firstword $(wildcard $(HOME)/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin/riscv-none-elf-gcc))
RISCV_CC  ?= $(if $(XPACK_GCC),$(XPACK_GCC),riscv-none-elf-gcc)
XPREFIX   := $(patsubst %gcc,%,$(RISCV_CC))
RISCV_AR      := $(XPREFIX)ar
RISCV_OBJDUMP := $(XPREFIX)objdump
RISCV_OBJCOPY := $(XPREFIX)objcopy
RISCV_NM      := $(XPREFIX)nm
# The same ISA/ABI/code model as ship-your-lua's image: plain rv64i (the
# executable Sail model has no M/F/D/A/C; mul/div and floating point come
# from libgcc's soft routines), lp64, medany.
MULTI        := -march=rv64i -mabi=lp64
RISCV_CFLAGS ?= -std=gnu11 -O2 -Wall $(MULTI) -mcmodel=medany
TCC_CFLAGS   := -std=gnu11 -O2 $(MULTI) -mcmodel=medany -fno-strict-aliasing \
                -Wno-unused-result -Wno-format-truncation

# newlib and libgcc of the rv64i/lp64 multilib (what the image links)
LIBC        := $(shell $(RISCV_CC) $(MULTI) -print-file-name=libc.a)
LIBM        := $(shell $(RISCV_CC) $(MULTI) -print-file-name=libm.a)
LIBGCC      := $(shell $(RISCV_CC) $(MULTI) -print-libgcc-file-name)
NEWLIB_INC  := $(abspath $(shell $(RISCV_CC) $(MULTI) -print-sysroot)/include)

BUILD   ?= build
TCCSRC  := $(BUILD)/tcc-src
PROG    ?= tests/run/hello.c
FLAGS   ?= 1
ARGS    ?=
ELF     ?= tcc-riscv-htif.elf
EMU     ?= $(HOME)/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator

PATCHES := $(sort $(wildcard ../patches/*.patch))
# TCC_RV64I_SOFT (patches/0001, the codegen lane): defined once that patch
# is present.
SOFT    := $(if $(wildcard ../patches/0001-*.patch),-DTCC_RV64I_SOFT,)

all: $(ELF)

# --- tcc's source tree: vendor/tinycc + patches/ ---------------------------
# scripts/tcc_src.sh (the codegen lane's) when it exists, else the same by
# hand. vendor/tinycc is never edited in place.
$(TCCSRC)/.stamp: $(PATCHES) $(wildcard ../scripts/tcc_src.sh)
	rm -rf $(TCCSRC)
	@mkdir -p $(BUILD)
	if [ -x ../scripts/tcc_src.sh ]; then ../scripts/tcc_src.sh $(TCCSRC); \
	else cp -r ../vendor/tinycc $(TCCSRC) && \
	  for p in $(PATCHES); do echo "applying $$p"; patch -s -d $(TCCSRC) -p1 < $$p || exit 1; done; fi
	touch $@

# --- the image's runtime (everything but .embed) ----------------------------
# The .S files are assembled to named objects first: assembling them inline
# leaves the preprocessor's random temp-file names in the symbol table.
$(BUILD)/crt0.o: src/crt0.S
	@mkdir -p $(BUILD)
	$(RISCV_CC) $(RISCV_CFLAGS) -c -o $@ $<

$(BUILD)/htif.o: src/htif.c
	@mkdir -p $(BUILD)
	$(RISCV_CC) $(RISCV_CFLAGS) -DTCC_HTIF -c -o $@ $<

$(BUILD)/main.o: src/main.c
	@mkdir -p $(BUILD)
	$(RISCV_CC) $(RISCV_CFLAGS) -c -o $@ $<

# tcc as one translation unit (ONE_SOURCE), its main renamed tcc_main
# (src/main.c calls it). src/tcc-config/config.h is tcc's configuration.
$(BUILD)/tcc.o: $(TCCSRC)/.stamp src/tcc-config/config.h
	$(RISCV_CC) $(TCC_CFLAGS) $(SOFT) -Isrc/tcc-config -I$(TCCSRC) -Dmain=tcc_main \
	    -c -o $@ $(TCCSRC)/tcc.c

RUNTIME_O := $(BUILD)/crt0.o $(BUILD)/htif.o $(BUILD)/main.o $(BUILD)/tcc.o

# --- files of the in-image file system --------------------------------------
# /lib/tcc/runmain.o: src/runmain.c (tcc_run links it into every program)
$(BUILD)/fs/runmain.o: src/runmain.c
	@mkdir -p $(BUILD)/fs
	$(RISCV_CC) $(RISCV_CFLAGS) -c -o $@ $<

# /lib/tcc/libtcc1.a: tcc's lib/ helpers that apply to riscv64, built with
# the image's flags: alloca (alloca.S), the __builtin_clz/ctz/popcount/...
# fallbacks (builtin.c) and the 128-bit long double routines (lib-arm64.c;
# libgcc.a, searched first, has them too). Not atomic.S (A extension),
# stdatomic.c, armflush.c (fence.i) or the backtrace/bcheck objects.
LIBTCC1_SRC := alloca.S builtin.c lib-arm64.c
$(BUILD)/fs/libtcc1.a: $(TCCSRC)/.stamp
	@mkdir -p $(BUILD)/fs/libtcc1
	rm -f $@
	for f in $(LIBTCC1_SRC); do \
	  $(RISCV_CC) $(RISCV_CFLAGS) -w -I$(TCCSRC) -c -o $(BUILD)/fs/libtcc1/$${f%.*}.o $(TCCSRC)/lib/$$f || exit 1; done
	$(RISCV_AR) rcs $@ $(patsubst %,$(BUILD)/fs/libtcc1/%.o,$(basename $(LIBTCC1_SRC)))

# the list of files (/path/in/image=host/file), all but /prog.c
$(BUILD)/fs.list: $(TCCSRC)/.stamp $(BUILD)/fs/runmain.o $(BUILD)/fs/libtcc1.a Makefile
	( for f in $$(cd $(TCCSRC)/include && ls *.h); do echo "/lib/tcc/include/$$f=$(abspath $(TCCSRC))/include/$$f"; done; \
	  echo "/lib/tcc/libtcc1.a=$(abspath $(BUILD))/fs/libtcc1.a"; \
	  echo "/lib/tcc/runmain.o=$(abspath $(BUILD))/fs/runmain.o"; \
	  echo "/lib/libc.a=$(LIBC)"; \
	  echo "/lib/libm.a=$(LIBM)"; \
	  echo "/lib/libgcc.a=$(LIBGCC)"; \
	  cd $(NEWLIB_INC) && find . -path ./c++ -prune -o -type f -name '*.h' -print | LC_ALL=C sort | \
	    sed 's|^\./\(.*\)$$|/include/\1=$(NEWLIB_INC)/\1|' ) > $@

# --- the embedded program -----------------------------------------------------
$(BUILD)/embed.S: src/gen_embed.sh $(BUILD)/fs.list $(PROG) FORCE
	sh src/gen_embed.sh --cargs '$(ARGS)' --flags '$(FLAGS)' --list $(BUILD)/fs.list \
	    /prog.c=$(abspath $(PROG)) > $@.tmp
	cmp -s $@.tmp $@ || mv $@.tmp $@; rm -f $@.tmp

$(BUILD)/embed.o: $(BUILD)/embed.S
	$(RISCV_CC) $(RISCV_CFLAGS) -c -o $@ $<

# -lm: tcc calls ldexp & co.; gcc's defaults add -lgcc -lc -lgloss (htif.c
# defines the system calls first, so nothing comes from libgloss: isa-check).
$(ELF): $(RUNTIME_O) $(BUILD)/embed.o src/link.ld
	$(RISCV_CC) $(RISCV_CFLAGS) -nostartfiles -T src/link.ld -o $@ \
	    $(RUNTIME_O) $(BUILD)/embed.o -lm

run: $(ELF)
	$(EMU) $(ELF)

# The image and everything tcc links into programs from the file system:
# no instruction of the M, F, D, A or C extensions, no ecall.
isa-check: $(ELF) $(BUILD)/fs/libtcc1.a $(BUILD)/fs/runmain.o
	sh tests/isa_check.sh $(RISCV_OBJDUMP) -- $(ELF) $(BUILD)/fs/runmain.o $(BUILD)/fs/libtcc1.a $(LIBC) $(LIBM) $(LIBGCC)

clean:
	rm -rf build tcc-riscv-htif.elf

FORCE:
.PHONY: all run isa-check clean FORCE
