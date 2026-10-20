#!/usr/bin/env python3
"""csem_lane.py — the fork's exec differential lane, CSem (Lean 4.34) vs the
Cerberus OCaml oracle.

    scripts/csem_lane.py [--baseline csem/scripts/exec_baseline.txt] [DIR|FILE...]

Default corpus: csem/tests/minimal (the fork's `test_exec.sh` default), checked
against the fork's committed classification csem/scripts/exec_baseline.txt.

Per file, as csem/scripts/test_exec.sh does it (its header is the contract):
  * oracle: `cerberus --runtime=R --nolibc --exec --batch --mode=exhaustive f.c`;
    exit 124/134/137/139, no verdict line, or an oracle `Error` line -> CERB_SKIP;
  * CSem: `csem_run f.c` (cerberus --cabs-json, then the Lean pipeline, batch,
    exhaustive), LEAN_ABORT_ON_PANIC=1;
  * both outputs reduced to the ordered verdict-token sequence (VAL:<whole
    Defined line body>, UB:<whole Undefined line body>, ERR:<Error line>);
    equal sequences -> MATCH (no UB) / UB_MATCH; equal up to UB bodies at the
    same positions -> UB_DIFF; one-sided UB -> DIFF; otherwise MISMATCH; the
    fuel-exhaustion line -> FUEL; Lean exit >= 128 -> LEAN_CRASH; timeout ->
    TIMEOUT.
Not reproduced from test_exec.sh: the HANG CPU/wall split (a Lean timeout is
TIMEOUT here), the CERB_FLOOR stderr token and the exit-code consistency
check. The fork's own test_exec.sh needs the fork's in-tree OCaml build and
lean_frontend Lake package; it stays runnable in the pristine upstream
checkout (scripts/csem_toolchain.sh), not against the root project's build.

Exit 0 iff every file's status equals its baseline row (files absent from the
baseline must be MATCH/UB_MATCH/CERB_SKIP).
"""
import argparse, os, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def env_paths():
    out = subprocess.run(["bash", "-c", f"source {ROOT}/scripts/csem_env.sh; "
                          "echo $CSEM_ORACLE; echo $CSEM_RUNTIME; echo $CSEM_LEAN_BIN"],
                         capture_output=True, text=True, check=True).stdout.split("\n")
    return out[0], out[1], out[2]


def tokens(stdout):
    toks = []
    for line in stdout.splitlines():
        if line.startswith("Defined {"):
            toks.append("VAL:" + line[len("Defined "):])
        elif line.startswith("Undefined {"):
            toks.append("UB:" + line[len("Undefined "):])
        elif line.startswith("Error {"):
            toks.append("ERR:" + line[len("Error "):])
    return toks


def run(cmd, timeout, env=None):
    try:
        p = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout, env=env,
                           errors="replace")
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired:
        return 124, "", "timeout"


def classify(o_toks, l_toks):
    if o_toks == l_toks:
        return "UB_MATCH" if any(t.startswith("UB:") for t in o_toks) else "MATCH"
    o_ub = [t.startswith("UB:") for t in o_toks]
    l_ub = [t.startswith("UB:") for t in l_toks]
    if len(o_toks) == len(l_toks) and o_ub == l_ub and \
            all(a == b for a, b, u in zip(o_toks, l_toks, o_ub) if not u):
        return "UB_DIFF"
    if any(o_ub) != any(l_ub):
        return "DIFF"
    return "MISMATCH"


def one(f, oracle, runtime, lean, timeout, scratch):
    rc, out, err = run([oracle, f"--runtime={runtime}", "--nolibc", "--exec", "--batch",
                        "--mode=exhaustive", str(f)], timeout)
    o_toks = tokens(out)
    # an oracle `Error {...}` line is an oracle-side non-comparison in the
    # fork's taxonomy (csem/tests/minimal/112-*.c header): CERB_SKIP
    if rc in (124, 134, 137, 139) or not o_toks or any(t.startswith("ERR:") for t in o_toks):
        return "CERB_SKIP", f"oracle exit {rc}"
    env = dict(os.environ, LEAN_ABORT_ON_PANIC="1", CSEM_ORACLE=oracle, CSEM_RUNTIME=runtime,
               TMPDIR=scratch)
    rc, out, err = run([lean, str(f)], timeout, env)
    if rc == 124:
        return "TIMEOUT", ""
    if any(l.strip() in ('Error {msg: "lem: fuel exhausted"}', "lem: fuel exhausted")
           for l in (out + err).splitlines()):
        return "FUEL", ""
    if rc >= 128:
        return "LEAN_CRASH", f"exit {rc}"
    if rc == 3:
        return "FAIL", "front end refused"
    l_toks = tokens(out)
    if not l_toks:
        return "FAIL", f"exit {rc}: {err.strip()[:200]}"
    st = classify(o_toks, l_toks)
    return st, "" if st in ("MATCH", "UB_MATCH") else f"oracle {o_toks[:2]} lean {l_toks[:2]}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("paths", nargs="*", default=[str(ROOT / "csem/tests/minimal")])
    ap.add_argument("--baseline", default=str(ROOT / "csem/scripts/exec_baseline.txt"))
    ap.add_argument("--timeout", type=int, default=int(os.environ.get("TIMEOUT_SECS", "30")))
    a = ap.parse_args()
    oracle, runtime, lean = env_paths()
    for p, what in ((oracle, "oracle (scripts/csem_toolchain.sh oracle)"),
                    (lean, "csem_run (scripts/csem_build.sh csem_run)")):
        if not os.access(p, os.X_OK):
            sys.exit(f"csem_lane: {what} missing: {p}")
    files = []
    for p in map(Path, a.paths):
        files += sorted(p.glob("*.c")) if p.is_dir() else [p]
    base = {}
    if a.baseline and os.path.exists(a.baseline):
        for line in open(a.baseline):
            if line.strip() and not line.startswith("#"):
                k, v = line.split()
                base[k] = v
    counts, bad = {}, []
    with tempfile.TemporaryDirectory(prefix="csem-lane-") as scratch:
        for f in files:
            st, why = one(f, oracle, runtime, lean, a.timeout, scratch)
            counts[st] = counts.get(st, 0) + 1
            want = base.get(f.name)
            ok = (st == want) if want else st in ("MATCH", "UB_MATCH", "CERB_SKIP")
            print(f"{'ok ' if ok else 'BAD'} {f.name} {st}" + (f" (baseline {want})" if want and not ok else "")
                  + (f"  {why}" if why and not ok else ""))
            if not ok:
                bad.append(f.name)
    print("csem_lane: " + ", ".join(f"{k} {v}" for k, v in sorted(counts.items()))
          + f"; {len(files)} files, {len(files) - len(bad)} as baselined")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
