import Tcc.Boot.Startup.ConfigPma
import Vsa.Meta.SimpNF

namespace Tcc.Boot.Startup
open LeanRV64DExecutable LeanRV64DExecutable.Functions

-- Normalize the closed integer conversions before state-level composition.
#simp_nf configMemoryProgram : check_mem_layout () using [check_mem_layout,
  clint_address_bits, clint_size_bits, signal_address_bits, pure_bind]

end Tcc.Boot.Startup
