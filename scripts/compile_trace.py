#!/usr/bin/env python3
"""Profile tcc's compile phase for the reflective-checker pilot (CompileCut).

    scripts/compile_trace.py --elf c/build/p3/sum-flags0.elf --out build/ccp/trace

Streams the emulator's `--trace-all` rows (LeanRiscv.traceLoop) from reset to
the first step at `tcc_baremetal_enter` (the captured cut) and keeps only what
the pilot needs:

* `pcs.tsv`: every executed PC with its dynamic count and instruction word
  (read from the ELF image; a fetch from a stored byte is an error, so the
  compile phase never executes code it wrote);
* `ops.tsv`: dynamic count and static count per mnemonic;
* `ckpt.tsv`: every `--every` steps, the step, PC and x1..x31 before it;
* `stores.bin`: the compile phase's stores, `<IQBQ` = step, addr, width, data;
* `meta.json`: step counts, per-chunk footprint statistics (distinct 256-byte
  pages written / read per chunk of 1k, 10k, 100k steps).

Every load is checked against the ELF image overlaid with every store.
"""
import argparse
import json
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from tcc_cut import EMU, elf_image, symbols  # noqa: E402

PAGE = 256


def mnemonic(w):
    op = w & 0x7f
    f3 = (w >> 12) & 7
    f7 = w >> 25
    rd = (w >> 7) & 31
    if op == 0x13:
        if f3 == 5:
            return 'srai' if (w >> 26) == 0x10 else 'srli'
        return ['addi', 'slli', 'slti', 'sltiu', 'xori', '?', 'ori', 'andi'][f3]
    if op == 0x1b:
        if f3 == 5:
            return 'sraiw' if f7 == 0x20 else 'srliw'
        return {0: 'addiw', 1: 'slliw'}.get(f3, '?op-imm-32')
    if op == 0x33:
        if f7 == 1:
            return 'M-ext'
        if f3 == 0:
            return 'sub' if f7 == 0x20 else 'add'
        if f3 == 5:
            return 'sra' if f7 == 0x20 else 'srl'
        return ['?', 'sll', 'slt', 'sltu', 'xor', '?', 'or', 'and'][f3]
    if op == 0x3b:
        if f3 == 0:
            return 'subw' if f7 == 0x20 else 'addw'
        if f3 == 5:
            return 'sraw' if f7 == 0x20 else 'srlw'
        return {1: 'sllw'}.get(f3, '?op-32')
    if op == 0x03:
        return ['lb', 'lh', 'lw', 'ld', 'lbu', 'lhu', 'lwu', '?'][f3]
    if op == 0x23:
        return ['sb', 'sh', 'sw', 'sd', '?', '?', '?', '?'][f3]
    if op == 0x63:
        return ['beq', 'bne', '?', '?', 'blt', 'bge', 'bltu', 'bgeu'][f3]
    if op == 0x6f:
        return 'jal' if rd else 'j'
    if op == 0x67:
        return 'jalr' if rd else 'jr'
    if op == 0x37:
        return 'lui'
    if op == 0x17:
        return 'auipc'
    if op == 0x0f:
        return 'fence'
    if op == 0x73:
        return 'system'
    return f'?{op:#x}'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--elf', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--every', type=int, default=1000)
    ap.add_argument('--max-steps', type=int, default=6_632_342)
    a = ap.parse_args()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    elf = Path(a.elf).resolve()
    syms = symbols(elf)
    enter = syms['tcc_baremetal_enter']
    segs = elf_image(elf)

    def img(addr):
        for base, body in segs:
            if base <= addr < base + len(body):
                return body[addr - base]
        return None

    written = {}
    pcs = Counter()
    words = {}
    chunks = (1000, 10000, 100000)
    cur_w = {k: set() for k in chunks}
    cur_r = {k: set() for k in chunks}
    hist_w = {k: [] for k in chunks}
    hist_r = {k: [] for k in chunks}
    ck = open(out / 'ckpt.tsv', 'w')
    st = open(out / 'stores.bin', 'wb')
    nst = 0
    nld = 0
    cut_step = None
    proc = subprocess.Popen([str(EMU), str(elf), '--trace-all', '--max-steps', str(a.max_steps + 1)],
                            stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, bufsize=1 << 20)
    for raw in proc.stderr:
        if not raw.startswith(b'T\t'):
            continue
        f = raw.split(b'\t')
        step = int(f[1])
        pc = int(f[2], 16)
        if pc == enter:
            cut_step = step
            ck.write(f'{step}\t{pc:x}\t' + '\t'.join(x.decode() for x in f[4:35]) + '\n')
            break
        if step % a.every == 0:
            ck.write(f'{step}\t{pc:x}\t' + '\t'.join(x.decode() for x in f[4:35]) + '\n')
        pcs[pc] += 1
        if pc not in words:
            bs = []
            for j in range(4):
                if pc + j in written:
                    raise SystemExit(f'fetch from stored byte {pc + j:#x} at step {step}')
                b = img(pc + j)
                if b is None:
                    raise SystemExit(f'fetch from absent byte {pc + j:#x}')
                bs.append(b)
            words[pc] = int.from_bytes(bytes(bs), 'little')
        pg = pc // PAGE
        for k in chunks:
            cur_r[k].add(pg)
        if len(f) > 35 and f[35][:1] in (b'L', b'S'):
            kind = f[35][:1]
            w = int(f[35][1:])
            addr = int(f[36], 16)
            if kind == b'L':
                nld += 1
                pre = int(f[37], 16)
                for j in range(w):
                    b = (pre >> (8 * j)) & 0xff
                    cb = written.get(addr + j)
                    if cb is None:
                        cb = img(addr + j) or 0
                    if cb != b:
                        raise SystemExit(f'load mismatch {addr + j:#x} step {step}')
                for k in chunks:
                    cur_r[k].add(addr // PAGE)
                    cur_r[k].add((addr + w - 1) // PAGE)
            else:
                nst += 1
                post = int(f[38], 16)
                data = post & ((1 << (8 * w)) - 1)
                for j in range(w):
                    written[addr + j] = (data >> (8 * j)) & 0xff
                st.write(struct.pack('<IQBQ', step, addr, w, data))
                for k in chunks:
                    cur_w[k].add(addr // PAGE)
                    cur_w[k].add((addr + w - 1) // PAGE)
        for k in chunks:
            if (step + 1) % k == 0:
                hist_w[k].append(len(cur_w[k]))
                hist_r[k].append(len(cur_r[k]))
                cur_w[k] = set()
                cur_r[k] = set()
    proc.kill()
    proc.wait()
    ck.close()
    st.close()
    if cut_step is None:
        print('warning: did not reach tcc_baremetal_enter (partial run)', file=sys.stderr)
    ops = Counter()
    sops = Counter()
    with open(out / 'pcs.tsv', 'w') as fh:
        for pc in sorted(pcs):
            m = mnemonic(words[pc])
            ops[m] += pcs[pc]
            sops[m] += 1
            fh.write(f'{pc:x}\t{words[pc]:08x}\t{pcs[pc]}\t{m}\n')
    with open(out / 'ops.tsv', 'w') as fh:
        for m, n in ops.most_common():
            fh.write(f'{m}\t{n}\t{sops[m]}\n')

    def stats(xs):
        xs = sorted(xs)
        if not xs:
            return {}
        return {'n': len(xs), 'mean': sum(xs) / len(xs), 'median': xs[len(xs) // 2],
                'p90': xs[int(len(xs) * 0.9)], 'max': xs[-1], 'sum': sum(xs)}
    meta = {
        'cut_step': cut_step, 'static_pcs': len(pcs), 'stores': nst, 'loads': nld,
        'written_bytes': len(written),
        'written_pages': len({x // PAGE for x in written}),
        'page_bytes': PAGE,
        'pages_written_per_chunk': {k: stats(hist_w[k]) for k in chunks},
        'pages_touched_per_chunk': {k: stats(hist_r[k]) for k in chunks},
    }
    (out / 'meta.json').write_text(json.dumps(meta, indent=1) + '\n')
    print(json.dumps(meta, indent=1))


if __name__ == '__main__':
    main()
