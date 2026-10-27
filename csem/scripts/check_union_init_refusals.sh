#!/usr/bin/env bash
# Compatibility entry point. The item-U guard is retired by the C11 semantic
# repair; these are positive specified-outcome checks, not refusal witnesses.
exec python3 "$(dirname "$0")/check_union_initializers.py" "$@"
