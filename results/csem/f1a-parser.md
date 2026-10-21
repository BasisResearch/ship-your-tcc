# F1a recovered parser: build and first differential

2026-10-21. Recovered the previous agent’s uncommitted F1 parser from
`agent-a31b97b8f1c090141`. It had not compiled. Fixed reserved identifiers,
a monadic bind, record layout, module comments inside a mutual block, and
Lean 4.34 JSON map API calls. `lake build csem_cparse` now passes (112 jobs),
under `systemd-run --user --scope -p MemoryMax=12G`, without budget changes.

The first `scripts/csem_parse_cmp.py --jobs 6` run attempted 664 files:
375 AGREE, 239 ORACLE_ONLY, 40 BOTH_REJECT, 9 CPP_FAIL, 1 LEAN_CRASH.
See `f1a-parser.tsv`. The sole LEAN_CRASH is an explicitly interrupted
`tests2/101_cleanup.c` process when worker ownership changed; it had not
completed after roughly a minute. This run does not close F1.

Review findings to address next:

- `Parser.isTok` omits ELSE, although grammar uses `accept .ELSE`; this
  causes accepted if/else programs to fail at else. Audit all token predicate
  uses rather than patching only the observed case.
- GNU keyword aliases are exposed by `--gnu`, but grammar support for
  ATTRIBUTE, EXTENSION, LABEL and BUILTIN_VA_LIST is unfinished. The
  referenced `csem/GRAMMAR.md` has not been written.
- The comparison harness should fail on LEAN_ONLY, reject an empty corpus,
  distinguish oracle crashes/timeouts from syntax rejection, and avoid
  basename collisions for ad-hoc inputs.
- No connection to CSem’s raw-source front end or F2 token conversion has
  been made yet. The total lexer/parser accepts preprocessed bytes.

The parser core is total and fuel bounded. `Main.firstDiff` is a partial
diagnostic printer, outside the semantic entry point. No new proof axioms or
forbidden proof terms were added.
