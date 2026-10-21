/-
Unit tests: the examples of C11 §6.10 verbatim (§6.10.3.3p4, §6.10.3.4p4,
§6.10.3.5 EXAMPLES 1–7, §6.10.9p2), each checked under the plain ISO
configuration `cfgIso` and under tcc's `Tcc.cfgTcc` (tcc's predefined text
contributes its own tokens first; they are compared as a prefix).

A test states: the token sequence `phases14` produces for the example equals
the decomposition into preprocessing tokens (phase 3) of the result the
standard prints. `#guard` evaluates each check when the module is built.
-/
import Spec.Pre.Main
import Spec.Pre.Iso
import Spec.Pre.Tcc.Cfg
import Spec.Pre.Tcc.ImageFS

namespace CSem.Pre.Test

open CSem.Pre

/-- Phase-3 tokens of a text (the standard's printed result). -/
def toksOf (cfg : PreCfg) (s : String) : List PPToken :=
  match phases123 cfg [] (bs s) with
  | .ok items => items.filterMap (fun i => match i with | .tok t => some ⟨t.kind, t.spell⟩ | .nl _ => none)
  | .error _ => []

def runPP (cfg : PreCfg) (fs : FileSys) (s : String) : Option (List PPToken) :=
  match phases14 cfg fs (bs s) with
  | .ok ts => some ts
  | .error _ => none

/-- `src` preprocesses to the predefined text's tokens followed by `expected`'s. -/
def ppIs (cfg : PreCfg) (fs : FileSys) (src expected : String) : Bool :=
  match runPP cfg fs "", runPP cfg fs src with
  | some pre, some out => out == pre ++ toksOf cfg expected
  | _, _ => false

/-- `src` is rejected (a constraint violation). -/
def ppFails (cfg : PreCfg) (fs : FileSys) (src : String) : Bool := (runPP cfg fs src).isNone

/-- The file system of the tests: the image's headers and `/vers2.h`
(EXAMPLE 4 includes it). -/
def testFS : FileSys := ⟨Tcc.imageFS.files ++ [(bs "/vers2.h", bs "vers2_h_contents\n")]⟩

def both (src expected : String) : Bool :=
  ppIs cfgIso testFS src expected && ppIs Tcc.cfgTcc testFS src expected

/-! ### §6.10.3.5 EXAMPLE 1 -/
#guard both "#define TABSIZE 100
int table[TABSIZE];
" "int table[100];"

/-! ### §6.10.3.5 EXAMPLE 2 -/
#guard both "#define max(a, b) ((a) > (b) ? (a) : (b))
max(x, y)
" "((x) > (y) ? (x) : (y))"

/-! ### §6.10.3.5 EXAMPLE 3 -/
def ex3 : String := "#define x 3
#define f(a) f(x * (a))
#undef x
#define x 2
#define g f
#define z z[0]
#define h g(~
#define m(a) a(w)
#define w 0,1
#define t(a) a
#define p() int
#define q(x) x
#define r(x,y) x ## y
#define str(x) # x
f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
g(x+(3,4)-w) | h 5) & m
(f)^m(m);
p() i[q()] = { q(1), r(2,3), r(4,), r(,5), r(,) };
char c[2][6] = { str(hello), str() };
"
def ex3out : String := "f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);
f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);
int i[] = { 1, 23, 4, 5, };
char c[2][6] = { \"hello\", \"\" };
"
#guard both ex3 ex3out

/-! ### §6.10.3.5 EXAMPLE 4 (the `#include` names "vers2.h"; its contents
stand where the standard prints `#include "vers2.h"`) -/
def ex4 : String := "#define str(s) # s
#define xstr(s) str(s)
#define debug(s, t) printf(\"x\" # s \"= %d, x\" # t \"= %s\", \\
 x ## s, x ## t)
#define INCFILE(n) vers ## n
#define glue(a, b) a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW \"hello\"
#define LOW LOW \", world\"
debug(1, 2);
fputs(str(strncmp(\"abc\\0d\", \"abc\", '\\4') // this goes away
 == 0) str(: @\\n), s);
#include xstr(INCFILE(2).h)
glue(HIGH, LOW);
xglue(HIGH, LOW)
"
def ex4out : String := "printf(\"x\" \"1\" \"= %d, x\" \"2\" \"= %s\", x1, x2);
fputs(\"strncmp(\\\"abc\\\\0d\\\", \\\"abc\\\", '\\\\4') == 0\" \": @\\n\", s);
vers2_h_contents
\"hello\";
\"hello\" \", world\"
"
#guard both ex4 ex4out

/-! ### §6.10.3.5 EXAMPLE 5 -/
#guard both "#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,),
 t(10,,), t(,11,), t(,,12), t(,,) };
" "int j[] = { 123, 45, 67, 89,
 10, 11, 12, };"

/-! ### §6.10.3.5 EXAMPLE 6: valid and invalid redefinitions -/
def ex6valid : String := "#define OBJ_LIKE (1-1)
#define OBJ_LIKE /* white space */ (1-1) /* other */
#define FUNC_LIKE(a) ( a )
#define FUNC_LIKE( a )( /* note the white space */ \\
 a /* other stuff on this line
 */ )
"
#guard both ex6valid ""
#guard ppFails cfgIso testFS (ex6valid ++ "#define OBJ_LIKE (0) // different token sequence\n")
#guard ppFails cfgIso testFS (ex6valid ++ "#define OBJ_LIKE (1 - 1) // different white space\n")
#guard ppFails cfgIso testFS (ex6valid ++ "#define FUNC_LIKE(b) ( a ) // different parameter usage\n")
#guard ppFails cfgIso testFS (ex6valid ++ "#define FUNC_LIKE(b) ( b ) // different parameter spelling\n")

/-! ### §6.10.3.5 EXAMPLE 7 -/
#guard both "#define debug(...) fprintf(stderr, __VA_ARGS__)
#define showlist(...) puts(#__VA_ARGS__)
#define report(test, ...) ((test)?puts(#test):\\
 printf(__VA_ARGS__))
debug(\"Flag\");
debug(\"X = %d\\n\", x);
showlist(The first, second, and third items.);
report(x>y, \"x is %d but y is %d\", x, y);
" "fprintf(stderr, \"Flag\" );
fprintf(stderr, \"X = %d\\n\", x );
puts( \"The first, second, and third items.\" );
((x>y)?puts(\"x>y\"): printf(\"x is %d but y is %d\", x, y));
"

/-! ### §6.10.3.3p4 EXAMPLE (`##` producing `##`) -/
#guard both "#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)
char p[] = join(x, y);
" "char p[] = \"x ## y\";"

/-! ### §6.10.3.4p4 EXAMPLE: `f(2)(9)` is `2*9*g` or `2*f(9)` (unspecified);
Prosser's hide sets give `2*9*g`. -/
#guard both "#define f(a) a*g
#define g(a) f(a)
f(2)(9)
" "2*9*g"

/-! ### §6.10.9p2 EXAMPLE (`_Pragma`): the pragma's tokens, bracketed. -/
#guard (match runPP cfgIso testFS "#define LISTING(x) PRAGMA(listing on #x)
#define PRAGMA(x) _Pragma(#x)
LISTING ( ..\\listing.dir )
" with
  | some out => out == [⟨.pragmaBegin, []⟩] ++ toksOf cfgIso "listing on \"..\\listing.dir\"" ++ [⟨.pragmaEnd, []⟩]
  | none => false)

/-! ### Trigraphs (§5.2.1.1): on in `cfgIso`, off in tcc's configuration. -/
#guard ppIs cfgIso testFS "??=define X a??/
b
X ??( ??)
" "ab [ ]"
#guard ppIs Tcc.cfgTcc testFS "??( ??)\n" "? ? ( ? ? )"

/-! ### `#if` arithmetic is in intmax_t / uintmax_t (§6.10.1p4). -/
#guard both "#if 0xFFFFFFFF + 1 == 0x100000000 && -1 < 0 && -1 > 0u && (0 ? 1u : -1) > 0
yes
#endif
" "yes"
#guard ppFails cfgIso testFS "#if 9223372036854775807 + 1\n#endif\n"

/-! ### Digraphs (§6.4.6p3) are punctuators, spelled as written. -/
#guard both "%:define DG(a) %:a
int x<:2:> = <% 1 %>; DG(y)
" "int x<:2:> = <% 1 %>; \"y\""

/-! ### C11's `u8`/`u`/`U` prefixes: one token in C11 (`cfgIso`), an
identifier and a literal in C99 (tcc's default mode). -/
#guard ppIs cfgIso testFS "#define u8 X\nu8\"a\"\n" "u8\"a\""
#guard ppIs Tcc.cfgTcc testFS "#define u8 X\nu8\"a\"\n" "X \"a\""

end CSem.Pre.Test
