# Attribution

This repository reuses the machinery of ship-your-interpreter
(BasisResearch/ship-your-interpreter) through its most advanced copies,
ship-your-lua and ship-your-ocaml (all Basis Research). Third-party
components keep their own licences.

## Third party
| here | origin | licence |
|---|---|---|
| `vendor/tinycc/` | tinycc-mirror-repository, mob, commit 43c7708 (`vendor/TINYCC_REV`) | LGPL-2.1 (`vendor/tinycc/COPYING`) |

## Copied from the sister projects
| here | there | changes |
|---|---|---|
| `c/src/crt0.S`, `c/src/link.ld` | ship-your-ocaml `c/src/` | none (copied by the earlier Codex agent) |
| `abstractions/gate.py` | ship-your-lua `abstractions/gate.py` | header line only |
| `abstractions/census.py` | ship-your-lua | scan root `Tcc` instead of `Lua` |
| `scripts/check_discipline.py` | ship-your-lua (from ship-your-interpreter) | scan root `Tcc` |
| `scripts/discipline_rules.tsv` | ship-your-lua | the language-agnostic rules only (Lua-specific R16–R18 dropped) |
| `scripts/check.sh` | after ship-your-lua `scripts/check.sh` | rewritten for this repository's stages |
