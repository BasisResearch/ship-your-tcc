#!/usr/bin/env python3
"""csem_parse_cmp.py — F1a validation: the Lean C grammar against Cerberus's
menhir grammar, file by file, compared as Cabs.

    scripts/csem_parse_cmp.py [--corpus NAME,...] [--jobs N] [--oracle-only]
                              [--report results/csem/f1a-parser.tsv] [FILE.c ...]

Per file:
  1. cpp: the exact command `cerberus --cabs-json` runs (backend/driver/main.ml
     create_cpp_cmd: `cc -std=c11 -E -CC -Werror -Wno-builtin-macro-redefined
     -nostdinc -undef -D__cerb__ -DCERB_WITH_LIB -I$RT/libc/include
     -I$RT/libc/include/posix -include $RT/libc/include/builtins.h FILE`), its
     output joined as Cerberus joins it (backend/common/pipeline.ml: lines read
     with input_line and concatenated with "\n", so no final newline);
  2. oracle: `cerberus --runtime=R --cabs-json FILE` (the menhir parser; the JSON
     printer backend/lean_export/cabs_json.ml);
  3. Lean: `csem_cparse --cmp PP FILE ORACLE.json` (csem/cparser: the Lean lexer
     and parser on the same preprocessed text, the Cabs printed through
     CSemCParser.CabsExport, compared with the oracle's JSON after the oracle's
     JSON has been read back by CabsImport and printed by the same printer, and
     the round trip itself checked against the oracle's text).

Status per file (one TSV row: corpus, file, status, detail):
  AGREE          both accept, the Cabs are equal (locations included)
  DIFF           both accept, the Cabs differ (detail: first differing path)
  BOTH_REJECT    both refuse the token stream
  LEAN_ONLY      Lean accepts what menhir refuses (an extension, csem/GRAMMAR.md, or a bug)
  ORACLE_ONLY    menhir accepts what Lean refuses
  CPP_FAIL       the preprocessor refuses (identically for both: same command)
  ROUNDTRIP      the oracle JSON does not survive import + export (printer bug)
"""
import argparse, concurrent.futures as cf, glob, json, os, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MAIN = Path(subprocess.run(["git", "-C", str(ROOT), "rev-parse", "--path-format=absolute",
                            "--git-common-dir"], capture_output=True, text=True).stdout.strip()).parent
CORPORA = {
    "minimal": [ROOT / "csem/tests/minimal"],
    "fork-corpus": [ROOT / "csem/lean_frontend/corpus"],
    "csmith": [ROOT / "corpus/csmith"],
    # copied at run time by scripts/collect_corpus.sh (gitignored); fall back to
    # the main checkout's copies
    "compcert": [ROOT / "corpus/compcert", MAIN / "corpus/compcert"],
    "tcc": [ROOT / "corpus/tcc", MAIN / "corpus/tcc"],
    "c-tests": [ROOT / "c/tests", ROOT / "c/tests/p3"],
    "c-tests-tcc": [ROOT / "c/tests/tcc"],
    "tests2": [ROOT / "vendor/tinycc/tests/tests2"],
}
CACHE = ROOT / "build" / "f1a"


def env_paths():
    out = subprocess.run(["bash", "-c", f"source {ROOT}/scripts/csem_env.sh; "
                          "echo $CSEM_ORACLE; echo $CSEM_RUNTIME"],
                         capture_output=True, text=True, check=True).stdout.split("\n")
    return out[0], out[1]


def cpp_cmd(runtime):
    inc = f"{runtime}/lib/cerberus-lib/runtime/libc/include"
    return ["cc", "-std=c11", "-E", "-CC", "-Werror", "-Wno-builtin-macro-redefined",
            "-nostdinc", "-undef", "-D__cerb__", "-DCERB_WITH_LIB", f"-I{inc}",
            f"-I{inc}/posix", "-include", f"{inc}/builtins.h"]


def files_of(corpus):
    for d in CORPORA[corpus]:
        fs = sorted(glob.glob(str(d / "*.c")))
        if fs:
            if corpus == "c-tests":
                yield from fs
                continue
            yield from fs
            return


def run(cmd, timeout=600, stdin=None):
    try:
        p = subprocess.run(cmd, capture_output=True, timeout=timeout, input=stdin)
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired:
        return 124, b"", b"timeout"


def one(corpus, f, oracle, runtime, lean_bin, oracle_only):
    key = f"{corpus}__{Path(f).stem}"
    d = CACHE / corpus
    d.mkdir(parents=True, exist_ok=True)
    pp, oj = d / f"{Path(f).stem}.i", d / f"{Path(f).stem}.json"
    rc, out, err = run(cpp_cmd(runtime) + [f])
    if rc != 0:
        return corpus, f, "CPP_FAIL", err.decode(errors="replace").strip().splitlines()[0:1]
    if out.endswith(b"\n"):
        out = out[:-1]
    pp.write_bytes(out)
    orc, oout, oerr = run([oracle, f"--runtime={runtime}", "--cabs-json", f])
    oracle_ok = orc == 0
    if oracle_ok:
        oj.write_bytes(oout)
    elif oj.exists():
        oj.unlink()
    oracle_msg = oerr.decode(errors="replace").strip().replace("\n", " ")[:200]
    if oracle_only:
        return corpus, f, "ORACLE_OK" if oracle_ok else "ORACLE_REJECT", [oracle_msg]
    lrc, lout, lerr = run([lean_bin, "--cmp", str(pp), f, str(oj) if oracle_ok else "-"])
    line = (lout.decode(errors="replace").strip().splitlines() or ["?"])[-1]
    status, _, detail = line.partition("\t")
    if lrc >= 128 or status not in ("AGREE", "DIFF", "BOTH_REJECT", "LEAN_ONLY", "ORACLE_ONLY", "ROUNDTRIP"):
        status, detail = "LEAN_CRASH", (lerr.decode(errors="replace").strip()[-300:] or line)
    if status in ("BOTH_REJECT", "ORACLE_ONLY", "LEAN_ONLY"):
        detail = f"{detail} | oracle: {oracle_msg}"
    return corpus, f, status, [detail.replace("\t", " ").replace("\n", " ")]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--corpus", default=",".join(CORPORA))
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--oracle-only", action="store_true")
    ap.add_argument("--report", default=str(ROOT / "results/csem/f1a-parser.tsv"))
    ap.add_argument("--lean-bin", default=str(ROOT / ".lake/build/bin/csem_cparse"))
    ap.add_argument("files", nargs="*")
    a = ap.parse_args()
    oracle, runtime = env_paths()
    jobs = []
    if a.files:
        jobs = [("adhoc", f) for f in a.files]
    else:
        for c in a.corpus.split(","):
            jobs += [(c, f) for f in files_of(c)]
    rows = []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        futs = [ex.submit(one, c, os.path.abspath(f), oracle, runtime, a.lean_bin, a.oracle_only)
                for c, f in jobs]
        for fu in cf.as_completed(futs):
            rows.append(fu.result())
    rows.sort(key=lambda r: (r[0], r[1]))
    counts = {}
    for c, f, s, d in rows:
        counts.setdefault(c, {}).setdefault(s, 0)
        counts[c][s] += 1
    def rel(f):
        for base in (ROOT, MAIN):
            try:
                return str(Path(f).relative_to(base))
            except ValueError:
                pass
        return f
    if not a.files:
        Path(a.report).parent.mkdir(parents=True, exist_ok=True)
        with open(a.report, "w") as fh:
            fh.write("corpus\tfile\tstatus\tdetail\n")
            for c, f, s, d in rows:
                fh.write(f"{c}\t{rel(f)}\t{s}\t{' '.join(d)}\n")
    for c, f, s, d in rows:
        if s not in ("AGREE", "BOTH_REJECT", "ORACLE_OK", "ORACLE_REJECT") or a.files:
            print(f"{c}\t{rel(f)}\t{s}\t{' '.join(d)[:300]}")
    for c in counts:
        print(c, " ".join(f"{k}={v}" for k, v in sorted(counts[c].items())), f"total={sum(counts[c].values())}")
    bad = sum(v for c in counts for k, v in counts[c].items()
              if k in ("DIFF", "ORACLE_ONLY", "ROUNDTRIP", "LEAN_CRASH"))
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
