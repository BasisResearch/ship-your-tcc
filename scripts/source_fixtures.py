"""Corpus files embedded beside /prog.c, independent of any compiler output."""
from pathlib import Path
import re


def fixture_files(path):
    path = Path(path)
    names = re.findall(rb'#\s*include\s*"([^"/]+)"', path.read_bytes())
    direct = [path.parent/n.decode() for n in names if (path.parent/n.decode()).is_file()]
    return sorted({path, *path.parent.glob('*.h'), *direct})


def fixture_entries(path):
    """Retain basename self-includes and ../<directory>/header fixture aliases.

    The caller installs /prog.c first and chooses its image-file precedence.
    The same explicit input files must be passed to every compared engine.
    """
    path = Path(path)
    return [(virtual, host) for host in fixture_files(path)
            for virtual in ['/'+host.name, '/'+path.parent.name+'/'+host.name]]
