import Cabs

/-! # Tokens of Cerberus's C lexer (csem/parsers/c/tokens.ml, c_lexer.mll)

The token set the C grammar (`c_parser.mly`) reads, minus the CN tokens, which
the C lexer produces only inside CN magic comments (`inside_cn`, off for C) and
the `CERB_MAGIC` token (only under the `at_magic_comments` switch, off by
default). The one CN token the C lexer can produce outside CN mode is
`CN_CONSTANT` (`1u8`, `2i32`: c_lexer.mll's `integer_constant 'u'
cn_integer_width` rules); no C production accepts it, so it is kept only to be
refused.

`UNAME`/`LNAME` are one constructor `NAME`: the grammar uses them only through
`%inline NAME` (c_parser.mly:437-441). The `TYPE`/`VARIABLE` pseudo-tokens the
lexer feeds back after each name (`create_lexer`, `Lexer_feedback.is_typedefname`)
are not materialised: the parser classifies a `NAME` against its typedef context
at the point menhir asks for the next token, which is when the name is shifted
(see `Parser.lean`, "typedef-name feedback"). -/

namespace CSemCParser

inductive Tok where
  -- §6.4.1 keywords (c_lexer.mll `keywords`)
  | AUTO | BREAK | CASE | CHAR | CONST | CONTINUE | DEFAULT | DO | DOUBLE | ELSE | ENUM | EXTERN
  | FLOAT | FOR | GOTO | IF | INLINE | INT | LONG | REGISTER | RESTRICT | RETURN | SHORT | SIGNED
  | SIZEOF | STATIC | STRUCT | SWITCH | TYPEDEF | TYPEOF | UNION | UNSIGNED | VOID | VOLATILE | WHILE
  | ALIGNAS | ALIGNOF | ATOMIC | BOOL | COMPLEX | GENERIC | NORETURN | STATIC_ASSERT | THREAD_LOCAL
  -- non-standard keywords of the same table
  | ASSERT | OFFSETOF | VA_START | VA_COPY | VA_ARG | VA_END | PRINT_TYPE | BMC_ASSUME
  | ASM | ASM_VOLATILE | BUILTIN_TYPES_COMPATIBLE_P | BUILTIN_CHOOSE_EXPR
  -- GNU keywords added by the grammar extensions (csem/GRAMMAR.md); never
  -- produced in the faithful mode
  | ATTRIBUTE | EXTENSION | LABEL | BUILTIN_VA_LIST
  -- identifiers and literals
  | NAME (s : String)
  | CONSTANT (c : cabs_constant)
  | STRING_LITERAL (pref : Option cabs_encoding_prefix) (strs : List String)
  | CN_CONSTANT
  -- §6.4.6 punctuators
  | LBRACK | RBRACK | LPAREN | RPAREN | LBRACE | RBRACE | DOT | MINUS_GT | PLUS_PLUS | MINUS_MINUS
  | AMPERSAND | STAR | PLUS | MINUS | TILDE | BANG | SLASH | PERCENT | LT_LT | GT_GT | LT | GT
  | LT_EQ | GT_EQ | EQ_EQ | BANG_EQ | CARET | PIPE | AMPERSAND_AMPERSAND | PIPE_PIPE | QUESTION
  | COLON | SEMICOLON | ELLIPSIS | EQ | STAR_EQ | SLASH_EQ | PERCENT_EQ | COLON_COLON | PLUS_EQ
  | MINUS_EQ | LT_LT_EQ | GT_GT_EQ | AMPERSAND_EQ | CARET_EQ | PIPE_EQ | COMMA | LBRACK_LBRACK
  | QUESTION_COLON | LBRACES | PIPES | RBRACES
  | EOF
  deriving Inhabited, BEq

/-- A lexing position, as ocamllex keeps it (`Lexing.position` without the
file name, which is constant for a translation unit): line number, offset of
the line's first byte, byte offset. -/
structure LPos where
  lnum : Nat
  bol : Nat
  cnum : Nat
  deriving Inhabited, BEq, Repr

/-- A token with its `lex_start_p`/`lex_curr_p` (menhir's `$startpos`/`$endpos`
of the terminal). -/
structure RawTok where
  tok : Tok
  sp : LPos
  ep : LPos
  deriving Inhabited

/-- One `#` line marker the lexer saw (`LineMap.add`, c_lexer.mll
`offset_location`): from preprocessed line `key + 1` on, the source is `file`
at line `line` (`Cerb_position.LineMap.lookup`). -/
structure LineMark where
  key : Nat
  file : String
  line : Nat
  deriving Inhabited, Repr

/-- The token's spelling, for messages. -/
def Tok.describe : Tok → String
  | .NAME s => s!"identifier '{s}'"
  | .CONSTANT _ => "constant"
  | .STRING_LITERAL _ _ => "string literal"
  | .CN_CONSTANT => "CN constant"
  | .EOF => "end of file"
  | t => (keywordSpelling t).getD "punctuator"
where
  keywordSpelling : Tok → Option String
  | .AUTO => "auto" | .BREAK => "break" | .CASE => "case" | .CHAR => "char" | .CONST => "const"
  | .CONTINUE => "continue" | .DEFAULT => "default" | .DO => "do" | .DOUBLE => "double"
  | .ELSE => "else" | .ENUM => "enum" | .EXTERN => "extern" | .FLOAT => "float" | .FOR => "for"
  | .GOTO => "goto" | .IF => "if" | .INLINE => "inline" | .INT => "int" | .LONG => "long"
  | .REGISTER => "register" | .RESTRICT => "restrict" | .RETURN => "return" | .SHORT => "short"
  | .SIGNED => "signed" | .SIZEOF => "sizeof" | .STATIC => "static" | .STRUCT => "struct"
  | .SWITCH => "switch" | .TYPEDEF => "typedef" | .TYPEOF => "typeof" | .UNION => "union"
  | .UNSIGNED => "unsigned" | .VOID => "void" | .VOLATILE => "volatile" | .WHILE => "while"
  | .LPAREN => "(" | .RPAREN => ")" | .LBRACE => "{" | .RBRACE => "}" | .LBRACK => "["
  | .RBRACK => "]" | .SEMICOLON => ";" | .COMMA => "," | .COLON => ":" | .EQ => "="
  | .STAR => "*" | .DOT => "." | _ => none

end CSemCParser
