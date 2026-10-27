#!/usr/bin/env python3
"""Independent Menhir harness cpp command: return the pinned shared layout.

Intentionally ignores Cerberus's cpp flags, including automatic includes.
This program is never called by the Lean raw adapter.
"""
import os
from pathlib import Path
import sys
sys.stdout.buffer.write(Path(os.environ['SYT_RAW_LAYOUT']).read_bytes())
