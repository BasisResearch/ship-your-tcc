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
  *_TIMEOUT      a process exceeded --timeout (not a grammar rejection)
  *_CRASH        signal, execution failure, or unrecognised nonzero exit
  *_PROTOCOL     malformed output or inconsistent status/exit
  HARNESS_ERROR  unexpected per-file harness exception

Only AGREE, BOTH_REJECT, diagnosed CPP_FAIL and oracle-only census outcomes
exit successfully. Every mismatch, including LEAN_ONLY, and every unexpected
failure exits 1. Missing/empty named corpora are command errors (exit 2).
Artifacts retain stage stdout/stderr/status under a unique run directory and
an absolute-input-path hash; no result is reused from an earlier invocation.
"""
import argparse, concurrent.futures as cf, glob, hashlib, json, os, re, shlex, subprocess, sys, tempfile
from dataclasses import dataclass
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
    out = subprocess.run(["bash", "-c", f"source {shlex.quote(str(ROOT / 'scripts/csem_env.sh'))}; "
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


# The oracle driver exits 1 for a reported CPARSER error; other failures are
# not grammar refusals. Keep this list aligned with string_of_cparser_cause in
# the pinned ocaml_frontend/pprinters/pp_errors.ml (no desugar errors here).
PARSER_ERRORS = (
    "invalid symbol", "invalid string character", "invalid line directive:",
    "unexpected end of file", "unexpected token ",
    "unsupported non-standard concatenation of string literals",
    "found a semicolon after a function definition",
    "found an extra semicolon inside a struct definition",
    "found an extra semicolon inside a union definition",
    "found K&R-style declaration (unsupported)", "nested comment",
    "unterminated ", "magic comment syntax at open but not close",
    "experimental keyword '", "unimplemented keyword '", "deprecated keyword '",
)
CMP_EXIT = {"AGREE": 0, "BOTH_REJECT": 0, "LEAN_ONLY": 1,
            "ORACLE_ONLY": 1, "DIFF": 1, "ROUNDTRIP": 1}
PASS = {"AGREE", "BOTH_REJECT", "CPP_FAIL", "ORACLE_OK", "ORACLE_REJECT"}


@dataclass
class Run:
    rc: int | None
    out: bytes = b""
    err: bytes = b""
    timeout: bool = False


def run(cmd, timeout=600):
    try:
        p = subprocess.run(cmd, capture_output=True, timeout=timeout)
        return Run(p.returncode, p.stdout, p.stderr)
    except subprocess.TimeoutExpired as e:
        return Run(None, e.stdout or b"", e.stderr or b"", timeout=True)
    except OSError as e:
        return Run(None, err=str(e).encode())


def message(r):
    return f"rc={r.rc} " + r.err.decode(errors="replace").strip().replace("\n", " ")[:600].rstrip()


def oracle_status(r):
    if r.timeout:
        return "ORACLE_TIMEOUT"
    if r.rc == 0:
        try:
            doc = json.loads(r.out)
            if (not isinstance(doc, dict) or doc.get("tag") != "TUnit"
                    or not isinstance(doc.get("decls"), list)
                    or not re.fullmatch(r"[0-9a-f]{32}", str(doc.get("digest", "")))
                    or re.search(rb"error:|Fatal error|exception|Out of memory|Stack overflow", r.err, re.I)):
                return "ORACLE_PROTOCOL"
        except (ValueError, UnicodeError):
            return "ORACLE_PROTOCOL"
        return "ORACLE_OK"
    # Fatal OCaml exceptions (even exit 1), signals, missing executables and
    # timeouts must never manufacture BOTH_REJECT or LEAN_ONLY evidence.
    err = r.err.decode(errors="replace")
    reasons = re.findall(r"^.*?:[0-9]+:[0-9]+: error: (.*)$", err, re.M)
    if (r.rc == 1 and not r.out.strip() and reasons
            and all(reason.startswith(PARSER_ERRORS) for reason in reasons)
            and not re.search(r"Fatal error|exception|Out of memory|Stack overflow", err, re.I)):
        return "ORACLE_REJECT"
    return "ORACLE_CRASH"


def compare_status(r, oracle_ok):
    if r.timeout:
        return "LEAN_TIMEOUT", message(r)
    if r.rc is None or r.rc < 0 or r.rc >= 128:
        return "LEAN_CRASH", message(r)
    lines = r.out.decode(errors="replace").splitlines()
    if len(lines) != 1:
        return "LEAN_PROTOCOL", f"expected one status line; {message(r)}; stdout={r.out[:300]!r}"
    status, sep, detail = lines[0].partition("\t")
    allowed = {"AGREE", "ORACLE_ONLY", "DIFF", "ROUNDTRIP"} if oracle_ok else {"BOTH_REJECT", "LEAN_ONLY"}
    if not sep or status not in allowed or r.rc != CMP_EXIT.get(status) or r.err.strip():
        return "LEAN_PROTOCOL", f"inconsistent status/exit/oracle: {lines[0][:300]}; {message(r)}"
    return status, detail


def one(corpus, f, oracle, runtime, lean_bin, oracle_only, cache=CACHE, timeout=600):
    # Absolute-path identity plus a per-run directory protects equal basenames,
    # repeated corpus entries and simultaneous comparator invocations.
    key = hashlib.sha256(os.fsencode(os.path.abspath(f))).hexdigest()
    d = cache / corpus / key
    d.mkdir(parents=True, exist_ok=True)
    pp, oj = d / "input.i", d / "oracle.json"

    def invoke(stage, cmd):
        r = run(cmd, timeout)
        (d / (stage + ".stdout")).write_bytes(r.out)
        (d / (stage + ".stderr")).write_bytes(r.err)
        (d / (stage + ".status")).write_text(f"rc={r.rc} timeout={r.timeout}\n")
        return r

    def row(status, detail):
        return corpus, f, status, [detail.replace("\t", " ").replace("\n", " ").strip()]

    cpp = invoke("cpp", cpp_cmd(runtime) + [f])
    if cpp.timeout:
        return row("CPP_TIMEOUT", message(cpp))
    if cpp.rc != 0:
        # A normal diagnosed cc refusal is a preprocessing classification.
        # Unknown statuses, signals and execution failures are harness failures.
        status = "CPP_FAIL" if cpp.rc == 1 and b"error:" in cpp.err else "CPP_CRASH"
        return row(status, message(cpp))
    pp.write_bytes(cpp.out.removesuffix(b"\n"))
    o = invoke("oracle", [oracle, f"--runtime={runtime}", "--cabs-json", f])
    status = oracle_status(o)
    if status not in ("ORACLE_OK", "ORACLE_REJECT") or oracle_only:
        return row(status, message(o))
    oracle_ok = status == "ORACLE_OK"
    if oracle_ok:
        oj.write_bytes(o.out)
    l = invoke("lean", [lean_bin, "--cmp", str(pp), f, str(oj) if oracle_ok else "-"])
    status, detail = compare_status(l, oracle_ok)
    if status in ("BOTH_REJECT", "ORACLE_ONLY", "LEAN_ONLY"):
        detail += " | oracle: " + message(o)
    return row(status, detail)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--corpus", default=",".join(CORPORA))
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--timeout", type=float, default=600, help="seconds per process")
    ap.add_argument("--oracle-only", action="store_true")
    ap.add_argument("--report", help="TSV path (default: full corpus results/csem/f1a-{parser,oracle}.tsv; ad-hoc in run artifacts)")
    ap.add_argument("--lean-bin", default=str(ROOT / ".lake/build/bin/csem_cparse"))
    ap.add_argument("files", nargs="*")
    a = ap.parse_args()
    if a.jobs < 1 or a.timeout <= 0:
        ap.error("--jobs and --timeout must be positive")
    jobs = []
    if a.files:
        jobs = [("adhoc", f) for f in a.files]
    else:
        for c in a.corpus.split(","):
            if c not in CORPORA:
                ap.error(f"unknown corpus: {c}")
            fs = list(files_of(c))
            if not fs:
                ap.error(f"empty corpus: {c}")
            jobs += [(c, f) for f in fs]
    jobs = sorted({(c, os.path.abspath(f)) for c, f in jobs})
    if not jobs:
        ap.error("empty input corpus")
    if any(not Path(f).is_file() for _, f in jobs):
        ap.error("every input must be an existing file")
    oracle, runtime = env_paths()
    CACHE.mkdir(parents=True, exist_ok=True)
    cache = Path(tempfile.mkdtemp(prefix="run-", dir=CACHE))
    print(f"artifacts: {cache}", flush=True)
    rows = []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        futs = {ex.submit(one, c, f, oracle, runtime, a.lean_bin, a.oracle_only, cache, a.timeout)
                : (c, f) for c, f in jobs}
        for fu in cf.as_completed(futs):
            try:
                rows.append(fu.result())
            except Exception as e:
                c, f = futs[fu]
                rows.append((c, f, "HARNESS_ERROR", [str(e).replace("\n", " ")]))
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
    report = Path(a.report) if a.report else (
        cache / "report.tsv" if a.files else ROOT / "results/csem" / ("f1a-oracle.tsv" if a.oracle_only else "f1a-parser.tsv"))
    report.parent.mkdir(parents=True, exist_ok=True)
    with report.open("w") as fh:
        fh.write("corpus\tfile\tstatus\tdetail\n")
        for c, f, s, d in rows:
            fh.write(f"{c}\t{rel(f)}\t{s}\t{' '.join(d)}\n")
    for c, f, s, d in rows:
        if s not in ("AGREE", "BOTH_REJECT", "ORACLE_OK", "ORACLE_REJECT") or a.files:
            print(f"{c}\t{rel(f)}\t{s}\t{' '.join(d)[:300]}")
    for c in counts:
        print(c, " ".join(f"{k}={v}" for k, v in sorted(counts[c].items())), f"total={sum(counts[c].values())}")
    bad = sum(v for c in counts for k, v in counts[c].items()
              if k not in PASS)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
