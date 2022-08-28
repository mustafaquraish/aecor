#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "errno.h"
#include "math.h"
#include "ctype.h"
#include "libgen.h"

/***************** embed './lib/prelude.h' *****************/
#include "stdarg.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

char* format_string(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int size = vsnprintf(NULL, 0, format, args);
  va_end(args);
  va_start(args, format);
  char* s = calloc(1, size + 1);
  vsprintf(s, format, args);
  s[size] = '\0';
  va_end(args);
  return s;
}


/* struct declarations */
typedef struct Vector Vector;
typedef struct Location Location;
typedef struct Span Span;
typedef enum TokenType TokenType;
typedef struct Token Token;
typedef enum MessageType MessageType;
typedef struct Lexer Lexer;
typedef struct MapNode MapNode;
typedef struct Map Map;
typedef struct MapIterator MapIterator;
typedef enum BaseType BaseType;
typedef struct Type Type;
typedef enum ASTType ASTType;
typedef struct Variable Variable;
typedef struct Function Function;
typedef struct Structure Structure;
typedef struct Program Program;
typedef struct Block Block;
typedef struct Binary Binary;
typedef struct Identifier Identifier;
typedef struct FormatString FormatString;
typedef struct Argument Argument;
typedef struct FuncCall FuncCall;
typedef struct Constructor Constructor;
typedef struct Member Member;
typedef struct EnumValue EnumValue;
typedef struct VarDeclaration VarDeclaration;
typedef struct IfStatement IfStatement;
typedef struct Loop Loop;
typedef struct Cast Cast;
typedef struct MatchCase MatchCase;
typedef struct Match Match;
typedef union ASTUnion ASTUnion;
typedef struct AST AST;
typedef struct ParserContext ParserContext;
typedef struct Parser Parser;
typedef struct TypeChecker TypeChecker;
typedef struct StringBuilder StringBuilder;
typedef struct CodeGenerator CodeGenerator;

struct Vector {
  i32 size;
  i32 capacity;
  void **data;
};

struct Location {
  char *filename;
  i32 line;
  i32 col;
};

struct Span {
  Location start;
  Location end;
};

enum TokenType {
  TokenType__And,
  TokenType__As,
  TokenType__Bool,
  TokenType__Break,
  TokenType__Char,
  TokenType__Continue,
  TokenType__Def,
  TokenType__Defer,
  TokenType__Else,
  TokenType__Enum,
  TokenType__Extern,
  TokenType__False,
  TokenType__F32,
  TokenType__F64,
  TokenType__For,
  TokenType__Fn,
  TokenType__I8,
  TokenType__I16,
  TokenType__I32,
  TokenType__I64,
  TokenType__If,
  TokenType__Let,
  TokenType__Match,
  TokenType__Null,
  TokenType__Not,
  TokenType__Or,
  TokenType__Return,
  TokenType__SizeOf,
  TokenType__String,
  TokenType__Struct,
  TokenType__True,
  TokenType__Then,
  TokenType__U8,
  TokenType__U16,
  TokenType__U32,
  TokenType__U64,
  TokenType__UntypedPtr,
  TokenType__Union,
  TokenType__Use,
  TokenType__Void,
  TokenType__Yield,
  TokenType__While,
  TokenType__AtSign,
  TokenType__Ampersand,
  TokenType__Backtick,
  TokenType__Caret,
  TokenType__CloseCurly,
  TokenType__CloseParen,
  TokenType__CloseSquare,
  TokenType__Colon,
  TokenType__ColonColon,
  TokenType__Comma,
  TokenType__Dot,
  TokenType__EOF,
  TokenType__Equals,
  TokenType__EqualEquals,
  TokenType__Exclamation,
  TokenType__FatArrow,
  TokenType__FloatLiteral,
  TokenType__FormatStringLiteral,
  TokenType__GreaterThan,
  TokenType__GreaterThanGreaterThan,
  TokenType__GreaterThanEquals,
  TokenType__Identifier,
  TokenType__IntLiteral,
  TokenType__LessThan,
  TokenType__LessThanLessThan,
  TokenType__LessThanEquals,
  TokenType__Line,
  TokenType__Minus,
  TokenType__MinusEquals,
  TokenType__NotEquals,
  TokenType__Newline,
  TokenType__OpenCurly,
  TokenType__OpenParen,
  TokenType__OpenSquare,
  TokenType__Percent,
  TokenType__Plus,
  TokenType__PlusEquals,
  TokenType__Question,
  TokenType__Semicolon,
  TokenType__Slash,
  TokenType__SlashEquals,
  TokenType__Star,
  TokenType__StarEquals,
  TokenType__StringLiteral,
  TokenType__CharLiteral,
};

struct Token {
  TokenType type;
  Span span;
  char *text;
  bool seen_newline;
};

enum MessageType {
  MessageType__Error,
  MessageType__Warning,
  MessageType__Note,
};

struct Lexer {
  char *source;
  i32 source_len;
  i32 i;
  Location loc;
  bool seen_newline;
  Vector *tokens;
};

struct MapNode {
  char *key;
  void *value;
  MapNode *next;
};

struct Map {
  MapNode **buckets;
  i32 num_items;
  i32 num_buckets;
  i32 num_collisions;
};

struct MapIterator {
  i32 idx;
  MapNode *cur;
  Map *map;
};

enum BaseType {
  BaseType__Char,
  BaseType__Bool,
  BaseType__Void,
  BaseType__I8,
  BaseType__I16,
  BaseType__I32,
  BaseType__I64,
  BaseType__U8,
  BaseType__U16,
  BaseType__U32,
  BaseType__U64,
  BaseType__F32,
  BaseType__F64,
  BaseType__Pointer,
  BaseType__Structure,
  BaseType__Function,
  BaseType__Method,
  BaseType__Array,
};

struct Type {
  BaseType base;
  Type *ptr;
  char *name;
  Span span;
  AST *size_expr;
  Structure *struct_def;
  Function *func_def;
  Type *return_type;
  Vector *params;
};

enum ASTType {
  ASTType__Assignment,
  ASTType__Address,
  ASTType__And,
  ASTType__Block,
  ASTType__BoolLiteral,
  ASTType__BitwiseAnd,
  ASTType__BitwiseOr,
  ASTType__BitwiseXor,
  ASTType__Break,
  ASTType__Call,
  ASTType__Cast,
  ASTType__CharLiteral,
  ASTType__Constructor,
  ASTType__Continue,
  ASTType__Defer,
  ASTType__Dereference,
  ASTType__Divide,
  ASTType__DivideEquals,
  ASTType__EnumValue,
  ASTType__Equals,
  ASTType__FloatLiteral,
  ASTType__FormatStringLiteral,
  ASTType__For,
  ASTType__GreaterThan,
  ASTType__GreaterThanEquals,
  ASTType__If,
  ASTType__Identifier,
  ASTType__Index,
  ASTType__IntLiteral,
  ASTType__IsNotNull,
  ASTType__LeftShift,
  ASTType__LessThan,
  ASTType__LessThanEquals,
  ASTType__Match,
  ASTType__Member,
  ASTType__Minus,
  ASTType__MinusEquals,
  ASTType__Modulus,
  ASTType__Multiply,
  ASTType__MultiplyEquals,
  ASTType__Not,
  ASTType__NotEquals,
  ASTType__Null,
  ASTType__Or,
  ASTType__Plus,
  ASTType__PlusEquals,
  ASTType__Return,
  ASTType__RightShift,
  ASTType__SizeOf,
  ASTType__ScopeLookup,
  ASTType__StringLiteral,
  ASTType__UnaryMinus,
  ASTType__VarDeclaration,
  ASTType__Yield,
  ASTType__While,
};

struct Variable {
  char *name;
  Type *type;
  Span span;
  bool is_extern;
  char *extern_name;
};

struct Function {
  char *name;
  Vector *params;
  Type *return_type;
  AST *body;
  bool exits;
  Type *type;
  Span span;
  bool is_arrow;
  bool is_extern;
  char *extern_name;
  bool is_static;
  bool is_method;
  char *method_struct_name;
};

struct Structure {
  Type *type;
  char *name;
  Vector *fields;
  Span span;
  bool is_extern;
  char *extern_name;
  bool is_enum;
  bool is_union;
};

struct Program {
  Vector *functions;
  Vector *structures;
  Vector *global_vars;
  Span span;
  Map *included_files;
  Vector *c_flags;
  Vector *c_includes;
  Vector *c_embed_headers;
};

struct Block {
  Vector *statements;
};

struct Binary {
  AST *lhs;
  AST *rhs;
};

struct Identifier {
  char *name;
  Variable *var;
  bool is_function;
  Function *func;
};

struct FormatString {
  Vector *parts;
  Vector *exprs;
};

struct Argument {
  AST *expr;
  AST *label;
};

struct FuncCall {
  AST *callee;
  Vector *args;
  Function *func;
  bool added_method_arg;
};

struct Constructor {
  Structure *struc;
  Vector *args;
};

struct Member {
  AST *lhs;
  AST *rhs;
  bool is_method;
  bool is_pointer;
};

struct EnumValue {
  Structure *struct_def;
  Variable *var;
};

struct VarDeclaration {
  Variable *var;
  AST *init;
};

struct IfStatement {
  AST *cond;
  AST *body;
  AST *els;
};

struct Loop {
  AST *init;
  AST *cond;
  AST *incr;
  AST *body;
};

struct Cast {
  AST *lhs;
  Type *to;
};

struct MatchCase {
  AST *cond;
  AST *body;
};

struct Match {
  AST *expr;
  Vector *cases;
  AST *defolt;
  Span defolt_span;
};

union ASTUnion {
  Block block;
  Binary binary;
  Identifier ident;
  FormatString fmt_str;
  FuncCall call;
  Member member;
  EnumValue enum_val;
  VarDeclaration var_decl;
  IfStatement if_stmt;
  Loop loop;
  Cast cast;
  AST *unary;
  Match match_stmt;
  char *num_literal;
  bool bool_literal;
  char *string_literal;
  char *char_literal;
  Constructor constructor;
  Type *size_of_type;
};

struct AST {
  ASTType type;
  Span span;
  ASTUnion u;
  Type *etype;
  bool returns;
};

struct ParserContext {
  Vector *tokens;
  i32 offset;
};

struct Parser {
  Vector *tokens;
  i32 curr;
  Vector *context_stack;
  char *project_root;
  Vector *include_dirs;
};

struct TypeChecker {
  Vector *scopes;
  Map *functions;
  Map *structures;
  Map *methods;
  Function *cur_func;
  bool in_loop;
  bool can_yield;
};

struct StringBuilder {
  char *data;
  i32 size;
  i32 capacity;
};

struct CodeGenerator {
  StringBuilder out;
  Vector *scopes;
  Vector *yield_vars;
  i32 yield_count;
  bool debug;
};

/* function declarations */
FILE *File__open(char *path, char *mode);
bool File__exists(char *path);
char *File__slurp(FILE *this);
void File__puts(FILE *this, char *str);
__attribute__((noreturn)) void panic(char *msg);
bool string__starts_with(char *this, char *prefix);
bool string__eq(char *this, char *str2);
char *string__substring(char *this, i32 start, i32 len);
void string__strip_trailing_whitespace(char *this);
i32 min(i32 a, i32 b);
i32 max(i32 a, i32 b);
Vector *Vector__new_sized(i32 capacity);
Vector *Vector__new(void);
void Vector__resize(Vector *this, i32 new_capacity);
void Vector__push(Vector *this, void *val);
void Vector__push_front(Vector *this, void *val);
void *Vector__pop(Vector *this);
void *Vector__back(Vector *this);
void *Vector__at(Vector *this, i32 i);
bool Vector__empty(Vector *this);
void Vector__free(Vector *this);
char *Location__str(Location this);
char *Span__str(Span this);
Span Span__join(Span this, Span other);
Token *Token__new(TokenType type, Span span, char *text);
Token *Token__from_type(TokenType type, Span span);
Token *Token__from_ident(char *text, Span span);
char *Token__str(Token *this);
TokenType TokenType__from_text(char *text);
char *TokenType__str(TokenType this);
f32 minf(f32 a, f32 b);
f32 maxf(f32 a, f32 b);
f32 clampf(f32 x, f32 min, f32 max);
f32 clamp01(f32 x);
f32 degrees(f32 radians);
f32 radians(f32 degrees);
i32 edit_distance(char *str1, char *str2);
char *find_word_suggestion(char *s, Vector *options);
char *MessageType__to_color(MessageType this);
char *MessageType__str(MessageType this);
void display_line(void);
void display_message(MessageType type, Span span, char *msg);
void display_message_with_span(MessageType type, Span span, char *msg);
__attribute__((noreturn)) void error_loc(Location loc, char *msg);
__attribute__((noreturn)) void error_span(Span span, char *msg);
__attribute__((noreturn)) void error_span_note(Span span, char *msg, char *note);
__attribute__((noreturn)) void error_span_note_span(Span msg_span, char *msg, Span note_span, char *note);
Lexer Lexer__make(char *source, char *filename);
void Lexer__push(Lexer *this, Token *token);
void Lexer__push_type(Lexer *this, TokenType type, i32 len);
char Lexer__peek(Lexer *this, i32 offset);
void Lexer__lex_char_literal(Lexer *this);
void Lexer__lex_string_literal(Lexer *this);
void Lexer__lex_int_literal_different_base(Lexer *this);
void Lexer__lex_numeric_literal(Lexer *this);
Vector *Lexer__lex(Lexer *this);
MapNode *MapNode__new(char *key, void *value, MapNode *next);
void MapNode__free_list(MapNode *node);
Map *Map__new(void);
i32 Map__hash(Map *this, char *s);
MapNode *Map__get_node(Map *this, char *key);
void *Map__get(Map *this, char *key);
bool Map__exists(Map *this, char *key);
void Map__insert(Map *this, char *key, void *value);
void Map__resize(Map *this);
void Map__print_keys(Map *this);
void Map__push_keys(Map *this, Vector *vec);
void Map__free(Map *this);
MapIterator Map__iter(Map *this);
char *MapIterator__key(MapIterator *this);
void *MapIterator__value(MapIterator *this);
MapIterator MapIterator__make(Map *map);
void MapIterator__next(MapIterator *this);
char *BaseType__str(BaseType this);
Type *Type__new(BaseType base, Span span);
Type *Type__new_link(BaseType base, Type *next, Span span);
Type *Type__ptr_to(BaseType base, Span span);
bool Type__is_struct_or_ptr(Type *this);
bool Type__is_integer(Type *this);
bool Type__is_numeric(Type *this);
bool Type__eq(Type *this, Type *other);
char *Type__str(Type *this);
bool Type__is_string(Type *this);
Type *Type__decay_array(Type *this);
bool Type__is_enum(Type *this);
bool Type__is_struct(Type *this);
char *ASTType__str(ASTType this);
ASTType ASTType__from_token(TokenType type);
Variable *Variable__new(char *name, Type *type, Span span);
Function *Function__new(Span span);
Structure *Structure__new(Span span);
Variable *Structure__find_field(Structure *this, char *name);
Program *Program__new(void);
bool Program__is_file_included(Program *this, char *filename);
void Program__add_included_file(Program *this, char *filename);
Argument *Argument__new(AST *label, AST *expr);
MatchCase *MatchCase__new(AST *cond, AST *body);
AST *AST__new(ASTType type, Span span);
AST *AST__new_unop(ASTType type, Span span, AST *expr);
AST *AST__new_binop(ASTType type, AST *lhs, AST *rhs);
bool AST__callee_is(AST *this, char *expected);
bool AST__is_lvalue(AST *this);
ParserContext *ParserContext__new(Vector *tokens, i32 offset);
Parser *Parser__new(Vector *tokens, char *filename);
void Parser__push_context(Parser *this, Vector *tokens);
void Parser__pop_context(Parser *this);
void Parser__add_include_dir(Parser *this, char *dir);
void Parser__error(Parser *this, char *msg);
void Parser__unhandled_type(Parser *this, char *func);
Token *Parser__token(Parser *this);
bool Parser__token_is(Parser *this, TokenType type);
bool Parser__consume_if(Parser *this, TokenType type);
void Parser__consume_newline_or(Parser *this, TokenType type);
Token *Parser__consume(Parser *this, TokenType type);
Type *Parser__parse_type_with_parent(Parser *this, Type *parent);
Type *Parser__parse_type(Parser *this);
AST *Parser__parse_format_string(Parser *this);
AST *Parser__parse_factor(Parser *this, TokenType end_type);
AST *Parser__parse_term(Parser *this, TokenType end_type);
AST *Parser__parse_additive(Parser *this, TokenType end_type);
AST *Parser__parse_shift(Parser *this, TokenType end_type);
AST *Parser__parse_bw_and(Parser *this, TokenType end_type);
AST *Parser__parse_bw_xor(Parser *this, TokenType end_type);
AST *Parser__parse_bw_or(Parser *this, TokenType end_type);
AST *Parser__parse_relational(Parser *this, TokenType end_type);
AST *Parser__parse_logical_and(Parser *this, TokenType end_type);
AST *Parser__parse_logical_or(Parser *this, TokenType end_type);
AST *Parser__parse_expression(Parser *this, TokenType end_type);
AST *Parser__parse_match(Parser *this);
AST *Parser__parse_if(Parser *this);
void Parser__consume_end_of_statement(Parser *this);
AST *Parser__parse_statement(Parser *this);
AST *Parser__parse_block(Parser *this);
Function *Parser__parse_function(Parser *this);
Structure *Parser__parse_enum(Parser *this);
Structure *Parser__parse_struct(Parser *this);
AST *Parser__parse_global_var(Parser *this);
char *Parser__find_file_path(Parser *this, char *filename);
void Parser__include_file(Parser *this, Program *program, char *filename);
void Parser__parse_use(Parser *this, Program *program);
void Parser__parse_compiler_option(Parser *this, Program *program);
void Parser__parse_into_program(Parser *this, Program *program);
Program *Parser__parse_program(Parser *this);
TypeChecker *TypeChecker__new(void);
void TypeChecker__push_scope(TypeChecker *this);
Map *TypeChecker__scope(TypeChecker *this);
void TypeChecker__pop_scope(TypeChecker *this);
void TypeChecker__push_var(TypeChecker *this, Variable *var);
Variable *TypeChecker__find_var(TypeChecker *this, char *name);
Variable *TypeChecker__get_struct_member(TypeChecker *this, char *lhs, char *rhs);
bool TypeChecker__type_is_valid(TypeChecker *this, Type *type);
void TypeChecker__check_method_call(TypeChecker *this, Type *method_type, AST *node);
Type *TypeChecker__check_constructor(TypeChecker *this, Structure *struc, AST *node);
Type *TypeChecker__check_call(TypeChecker *this, AST *node);
Type *TypeChecker__check_format_string(TypeChecker *this, AST *node);
Type *TypeChecker__check_pointer_arith(TypeChecker *this, AST *node, Type *lhs, Type *rhs);
Type *TypeChecker__check_binary_op(TypeChecker *this, AST *node, AST *_lhs, AST *_rhs);
__attribute__((noreturn)) void TypeChecker__error_unknown_identifier(TypeChecker *this, Span span, char *name);
void TypeChecker__error_unknown_member(TypeChecker *this, AST *node, Type *struct_type, char *field_name, bool is_static);
Type *TypeChecker__check_expression(TypeChecker *this, AST *node);
void TypeChecker__check_expression_statement(TypeChecker *this, AST *node, AST *body, bool is_expr);
void TypeChecker__check_match_for_enum(TypeChecker *this, Structure *struc, AST *node, bool is_expr);
void TypeChecker__check_match(TypeChecker *this, AST *node, bool is_expr);
void TypeChecker__check_if(TypeChecker *this, AST *node, bool is_expr);
void TypeChecker__check_statement(TypeChecker *this, AST *node);
void TypeChecker__check_block(TypeChecker *this, AST *node, bool can_yield);
void TypeChecker__check_function(TypeChecker *this, Function *func);
void TypeChecker__check_all_functions(TypeChecker *this, Program *program);
void TypeChecker__dfs_structs(TypeChecker *this, Structure *struc, Vector *results, Map *done);
void TypeChecker__check_all_structs(TypeChecker *this, Program *program);
void TypeChecker__check_program(TypeChecker *this, Program *program);
StringBuilder StringBuilder__make(void);
void StringBuilder__resize_if_necessary(StringBuilder *this, i32 new_size);
void StringBuilder__puts(StringBuilder *this, char *s);
void StringBuilder__putc(StringBuilder *this, char c);
void StringBuilder__putsf(StringBuilder *this, char *s);
char *StringBuilder__str(StringBuilder *this);
char *StringBuilder__new_str(StringBuilder *this);
CodeGenerator CodeGenerator__make(bool debug);
void CodeGenerator__gen_debug_info(CodeGenerator *this, Span span);
void CodeGenerator__indent(CodeGenerator *this, i32 num);
void CodeGenerator__push_scope(CodeGenerator *this);
Vector *CodeGenerator__scope(CodeGenerator *this);
void CodeGenerator__pop_scope(CodeGenerator *this);
void CodeGenerator__gen_control_body(CodeGenerator *this, AST *node, AST *body, i32 indent);
void CodeGenerator__gen_enum_value(CodeGenerator *this, char *enum_name, Variable *var);
void CodeGenerator__gen_enum(CodeGenerator *this, Structure *struc);
void CodeGenerator__gen_struct(CodeGenerator *this, Structure *struc);
void CodeGenerator__gen_format_string_part(CodeGenerator *this, char *part);
void CodeGenerator__gen_format_string(CodeGenerator *this, AST *node);
char *CodeGenerator__get_op(ASTType type);
void CodeGenerator__gen_in_yield_context(CodeGenerator *this, AST *node);
void CodeGenerator__gen_expression(CodeGenerator *this, AST *node);
void CodeGenerator__gen_var_decl(CodeGenerator *this, AST *node);
void CodeGenerator__gen_match_case_body(CodeGenerator *this, AST *node, AST *body, i32 indent);
void CodeGenerator__gen_match_string(CodeGenerator *this, AST *node, i32 indent);
void CodeGenerator__gen_match(CodeGenerator *this, AST *node, i32 indent);
void CodeGenerator__gen_yield_expression(CodeGenerator *this, AST *expr, i32 indent);
void CodeGenerator__gen_statement(CodeGenerator *this, AST *node, i32 indent);
void CodeGenerator__gen_block(CodeGenerator *this, AST *node, i32 indent);
void CodeGenerator__gen_struct_decls(CodeGenerator *this, Program *program);
void string__replace(char **this, char *other);
char *CodeGenerator__get_type_name_string(CodeGenerator *this, Type *type, char *name, bool is_func_def);
void CodeGenerator__gen_type_and_name(CodeGenerator *this, Type *type, char *name);
void CodeGenerator__gen_type(CodeGenerator *this, Type *type);
char *CodeGenerator__get_function_name(CodeGenerator *this, Function *func);
void CodeGenerator__gen_function_decl(CodeGenerator *this, Function *func);
void CodeGenerator__gen_function_decls(CodeGenerator *this, Program *program);
void CodeGenerator__gen_function(CodeGenerator *this, Function *func);
void CodeGenerator__gen_global_vars(CodeGenerator *this, Program *program);
void CodeGenerator__gen_embed_headers(CodeGenerator *this, Program *program);
char *CodeGenerator__gen_program(CodeGenerator *this, Program *program);
void usage(i32 code);
i32 main(i32 argc, char **argv);

/* global variables */

FILE *File__open(char *path, char *mode) {
  FILE *file = fopen(path, mode);
  if ((!((bool)file))) {
    printf("Error opening file '%s': %s" "\n", path, strerror(errno));
    exit(1);
  } 
  return file;
}

bool File__exists(char *path) {
  FILE *file = fopen(path, "r");
  if ((!((bool)file))) {
    return false;
  } 
  fclose(file);
  return true;
}

char *File__slurp(FILE *this) {
  i32 pos = ftell(this);
  fseek(this, 0, SEEK_END);
  i32 size = ftell(this);
  fseek(this, pos, SEEK_SET);
  void *buf = calloc((size + 1), sizeof(char));
  fread(buf, 1, size, this);
  fseek(this, pos, SEEK_SET);
  return ((char *)buf);
}

void File__puts(FILE *this, char *str) {
  fwrite(str, 1, strlen(str), this);
}

__attribute__((noreturn)) void panic(char *msg) {
  printf("%s" "\n", msg);
  exit(1);
}

bool string__starts_with(char *this, char *prefix) {
  i32 prefix_len = strlen(prefix);
  if ((strlen(this) < prefix_len)) {
    return false;
  } 
  return strncmp(this, prefix, prefix_len) == 0;
}

bool string__eq(char *this, char *str2) {
  return strcmp(this, str2) == 0;
}

char *string__substring(char *this, i32 start, i32 len) {
  char *new_str = ((char *)calloc((len + 1), sizeof(char)));
  strncpy(new_str, (this + start), len);
  return new_str;
}

void string__strip_trailing_whitespace(char *this) {
  for (i32 i = (strlen(this) - 1); (i >= 0); i -= 1) {
    if ((this[i] != ' ')) 
    break;
    
    this[i] = '\0';
  } 
}

i32 min(i32 a, i32 b) {
  return ((a < b) ? a : b);
}

i32 max(i32 a, i32 b) {
  return ((a > b) ? a : b);
}

Vector *Vector__new_sized(i32 capacity) {
  Vector *vec = ((Vector *)calloc(1, sizeof(Vector)));
  vec->size = 0;
  vec->capacity = capacity;
  vec->data = ((void **)calloc(vec->capacity, sizeof(void *)));
  return vec;
}

Vector *Vector__new(void) {
  return Vector__new_sized(16);
}

void Vector__resize(Vector *this, i32 new_capacity) {
  this->capacity = new_capacity;
  this->data = ((void **)realloc(this->data, (this->capacity * sizeof(void *))));
}

void Vector__push(Vector *this, void *val) {
  if (this->size == this->capacity) {
    Vector__resize(this, (this->capacity * 2));
  } 
  this->data[this->size] = val;
  this->size += 1;
}

void Vector__push_front(Vector *this, void *val) {
  if (this->size == this->capacity) {
    Vector__resize(this, (this->capacity * 2));
  } 
  for (i32 i = this->size; (i > 0); i -= 1) {
    this->data[i] = this->data[(i - 1)];
  } 
  this->data[0] = val;
  this->size += 1;
}

void *Vector__pop(Vector *this) {
  if (this->size == 0) {
    printf("pop on empty vector" "\n");
    exit(1);
  } 
  this->size -= 1;
  return this->data[this->size];
}

void *Vector__back(Vector *this) {
  if (this->size == 0) {
    printf("back on empty vector" "\n");
    exit(1);
  } 
  return this->data[(this->size - 1)];
}

void *Vector__at(Vector *this, i32 i) {
  if (((i < 0) || (i >= this->size))) {
    printf("at out of bounds in vector" "\n");
    exit(1);
  } 
  return this->data[i];
}

bool Vector__empty(Vector *this) {
  return this->size == 0;
}

void Vector__free(Vector *this) {
  free(this->data);
  free(this);
}

char *Location__str(Location this) {
  return format_string("%s:%d:%d", this.filename, this.line, this.col);
}

char *Span__str(Span this) {
  return format_string("%s => %s", Location__str(this.start), Location__str(this.end));
}

Span Span__join(Span this, Span other) {
  Span span = {0};
  span.start = this.start;
  span.end = other.end;
  return span;
}

Token *Token__new(TokenType type, Span span, char *text) {
  Token *tok = ((Token *)calloc(1, sizeof(Token)));
  (*tok) = (Token){type, span, text, .seen_newline = false};
  return tok;
}

Token *Token__from_type(TokenType type, Span span) {
  return Token__new(type, span, "");
}

Token *Token__from_ident(char *text, Span span) {
  TokenType type = TokenType__from_text(text);
  return Token__new(type, span, text);
}

char *Token__str(Token *this) {
  return format_string("%s: %s", Span__str(this->span), TokenType__str(this->type));
}

TokenType TokenType__from_text(char *text) {
  return ({ TokenType __yield_0;
  {
    char *__match_str = text;
    if (!strcmp(__match_str, "and")) {
      __yield_0 = TokenType__And;
} else if (!strcmp(__match_str, "as")) {
      __yield_0 = TokenType__As;
} else if (!strcmp(__match_str, "bool")) {
      __yield_0 = TokenType__Bool;
} else if (!strcmp(__match_str, "break")) {
      __yield_0 = TokenType__Break;
} else if (!strcmp(__match_str, "char")) {
      __yield_0 = TokenType__Char;
} else if (!strcmp(__match_str, "continue")) {
      __yield_0 = TokenType__Continue;
} else if (!strcmp(__match_str, "def")) {
      __yield_0 = TokenType__Def;
} else if (!strcmp(__match_str, "defer")) {
      __yield_0 = TokenType__Defer;
} else if (!strcmp(__match_str, "else")) {
      __yield_0 = TokenType__Else;
} else if (!strcmp(__match_str, "enum")) {
      __yield_0 = TokenType__Enum;
} else if (!strcmp(__match_str, "extern")) {
      __yield_0 = TokenType__Extern;
} else if (!strcmp(__match_str, "false")) {
      __yield_0 = TokenType__False;
} else if (!strcmp(__match_str, "f32")) {
      __yield_0 = TokenType__F32;
} else if (!strcmp(__match_str, "f64")) {
      __yield_0 = TokenType__F64;
} else if (!strcmp(__match_str, "for")) {
      __yield_0 = TokenType__For;
} else if (!strcmp(__match_str, "fn")) {
      __yield_0 = TokenType__Fn;
} else if (!strcmp(__match_str, "i8")) {
      __yield_0 = TokenType__I8;
} else if (!strcmp(__match_str, "i16")) {
      __yield_0 = TokenType__I16;
} else if (!strcmp(__match_str, "i32")) {
      __yield_0 = TokenType__I32;
} else if (!strcmp(__match_str, "i64")) {
      __yield_0 = TokenType__I64;
} else if (!strcmp(__match_str, "if")) {
      __yield_0 = TokenType__If;
} else if (!strcmp(__match_str, "let")) {
      __yield_0 = TokenType__Let;
} else if (!strcmp(__match_str, "match")) {
      __yield_0 = TokenType__Match;
} else if (!strcmp(__match_str, "not")) {
      __yield_0 = TokenType__Not;
} else if (!strcmp(__match_str, "null")) {
      __yield_0 = TokenType__Null;
} else if (!strcmp(__match_str, "or")) {
      __yield_0 = TokenType__Or;
} else if (!strcmp(__match_str, "return")) {
      __yield_0 = TokenType__Return;
} else if (!strcmp(__match_str, "sizeof")) {
      __yield_0 = TokenType__SizeOf;
} else if (!strcmp(__match_str, "string")) {
      __yield_0 = TokenType__String;
} else if (!strcmp(__match_str, "struct")) {
      __yield_0 = TokenType__Struct;
} else if (!strcmp(__match_str, "true")) {
      __yield_0 = TokenType__True;
} else if (!strcmp(__match_str, "then")) {
      __yield_0 = TokenType__Then;
} else if (!strcmp(__match_str, "u8")) {
      __yield_0 = TokenType__U8;
} else if (!strcmp(__match_str, "u16")) {
      __yield_0 = TokenType__U16;
} else if (!strcmp(__match_str, "u32")) {
      __yield_0 = TokenType__U32;
} else if (!strcmp(__match_str, "u64")) {
      __yield_0 = TokenType__U64;
} else if (!strcmp(__match_str, "untyped_ptr")) {
      __yield_0 = TokenType__UntypedPtr;
} else if (!strcmp(__match_str, "union")) {
      __yield_0 = TokenType__Union;
} else if (!strcmp(__match_str, "use")) {
      __yield_0 = TokenType__Use;
} else if (!strcmp(__match_str, "void")) {
      __yield_0 = TokenType__Void;
} else if (!strcmp(__match_str, "yield")) {
      __yield_0 = TokenType__Yield;
} else if (!strcmp(__match_str, "while")) {
      __yield_0 = TokenType__While;
} else  {
      __yield_0 = TokenType__Identifier;
}
  }
;__yield_0; });
}

char *TokenType__str(TokenType this) {
  return ({ char *__yield_0;
  switch (this) {
    case TokenType__And: {
      __yield_0 = "and";
} break;
    case TokenType__As: {
      __yield_0 = "as";
} break;
    case TokenType__Bool: {
      __yield_0 = "bool";
} break;
    case TokenType__Break: {
      __yield_0 = "break";
} break;
    case TokenType__Char: {
      __yield_0 = "char";
} break;
    case TokenType__Continue: {
      __yield_0 = "continue";
} break;
    case TokenType__Def: {
      __yield_0 = "def";
} break;
    case TokenType__Defer: {
      __yield_0 = "defer";
} break;
    case TokenType__Else: {
      __yield_0 = "else";
} break;
    case TokenType__Enum: {
      __yield_0 = "enum";
} break;
    case TokenType__Extern: {
      __yield_0 = "extern";
} break;
    case TokenType__False: {
      __yield_0 = "false";
} break;
    case TokenType__F32: {
      __yield_0 = "f32";
} break;
    case TokenType__F64: {
      __yield_0 = "f64";
} break;
    case TokenType__For: {
      __yield_0 = "for";
} break;
    case TokenType__Fn: {
      __yield_0 = "fn";
} break;
    case TokenType__I8: {
      __yield_0 = "i8";
} break;
    case TokenType__I16: {
      __yield_0 = "i16";
} break;
    case TokenType__I32: {
      __yield_0 = "i32";
} break;
    case TokenType__I64: {
      __yield_0 = "i64";
} break;
    case TokenType__If: {
      __yield_0 = "if";
} break;
    case TokenType__Let: {
      __yield_0 = "let";
} break;
    case TokenType__Match: {
      __yield_0 = "match";
} break;
    case TokenType__Not: {
      __yield_0 = "not";
} break;
    case TokenType__Null: {
      __yield_0 = "null";
} break;
    case TokenType__Or: {
      __yield_0 = "or";
} break;
    case TokenType__Return: {
      __yield_0 = "return";
} break;
    case TokenType__SizeOf: {
      __yield_0 = "sizeof";
} break;
    case TokenType__String: {
      __yield_0 = "string";
} break;
    case TokenType__Struct: {
      __yield_0 = "struct";
} break;
    case TokenType__True: {
      __yield_0 = "true";
} break;
    case TokenType__Then: {
      __yield_0 = "then";
} break;
    case TokenType__U8: {
      __yield_0 = "u8";
} break;
    case TokenType__U16: {
      __yield_0 = "u16";
} break;
    case TokenType__U32: {
      __yield_0 = "u32";
} break;
    case TokenType__U64: {
      __yield_0 = "u64";
} break;
    case TokenType__UntypedPtr: {
      __yield_0 = "untyped_ptr";
} break;
    case TokenType__Union: {
      __yield_0 = "union";
} break;
    case TokenType__Use: {
      __yield_0 = "use";
} break;
    case TokenType__Void: {
      __yield_0 = "void";
} break;
    case TokenType__Yield: {
      __yield_0 = "yield";
} break;
    case TokenType__While: {
      __yield_0 = "while";
} break;
    case TokenType__AtSign: {
      __yield_0 = "AtSign";
} break;
    case TokenType__Ampersand: {
      __yield_0 = "Ampersand";
} break;
    case TokenType__Backtick: {
      __yield_0 = "Backtick";
} break;
    case TokenType__Caret: {
      __yield_0 = "Caret";
} break;
    case TokenType__CloseCurly: {
      __yield_0 = "CloseCurly";
} break;
    case TokenType__CloseParen: {
      __yield_0 = "CloseParen";
} break;
    case TokenType__CloseSquare: {
      __yield_0 = "CloseSquare";
} break;
    case TokenType__Colon: {
      __yield_0 = "Colon";
} break;
    case TokenType__ColonColon: {
      __yield_0 = "ColonColon";
} break;
    case TokenType__Comma: {
      __yield_0 = "Comma";
} break;
    case TokenType__Dot: {
      __yield_0 = "Dot";
} break;
    case TokenType__EOF: {
      __yield_0 = "EOF";
} break;
    case TokenType__Equals: {
      __yield_0 = "Equals";
} break;
    case TokenType__EqualEquals: {
      __yield_0 = "EqualEquals";
} break;
    case TokenType__Exclamation: {
      __yield_0 = "Exclamation";
} break;
    case TokenType__FatArrow: {
      __yield_0 = "FatArrow";
} break;
    case TokenType__FloatLiteral: {
      __yield_0 = "FloatLiteral";
} break;
    case TokenType__FormatStringLiteral: {
      __yield_0 = "FormatStringLiteral";
} break;
    case TokenType__GreaterThan: {
      __yield_0 = "GreaterThan";
} break;
    case TokenType__GreaterThanGreaterThan: {
      __yield_0 = "GreaterThanGreaterThan";
} break;
    case TokenType__GreaterThanEquals: {
      __yield_0 = "GreaterThanEquals";
} break;
    case TokenType__Identifier: {
      __yield_0 = "Identifier";
} break;
    case TokenType__IntLiteral: {
      __yield_0 = "IntLiteral";
} break;
    case TokenType__LessThan: {
      __yield_0 = "LessThan";
} break;
    case TokenType__LessThanLessThan: {
      __yield_0 = "LessThanLessThan";
} break;
    case TokenType__LessThanEquals: {
      __yield_0 = "LessThanEquals";
} break;
    case TokenType__Line: {
      __yield_0 = "Line";
} break;
    case TokenType__Minus: {
      __yield_0 = "Minus";
} break;
    case TokenType__MinusEquals: {
      __yield_0 = "MinusEquals";
} break;
    case TokenType__NotEquals: {
      __yield_0 = "NotEquals";
} break;
    case TokenType__Newline: {
      __yield_0 = "Newline";
} break;
    case TokenType__OpenCurly: {
      __yield_0 = "OpenCurly";
} break;
    case TokenType__OpenParen: {
      __yield_0 = "OpenParen";
} break;
    case TokenType__OpenSquare: {
      __yield_0 = "OpenSquare";
} break;
    case TokenType__Percent: {
      __yield_0 = "Percent";
} break;
    case TokenType__Plus: {
      __yield_0 = "Plus";
} break;
    case TokenType__PlusEquals: {
      __yield_0 = "PlusEquals";
} break;
    case TokenType__Question: {
      __yield_0 = "Question";
} break;
    case TokenType__Semicolon: {
      __yield_0 = "Semicolon";
} break;
    case TokenType__Slash: {
      __yield_0 = "Slash";
} break;
    case TokenType__SlashEquals: {
      __yield_0 = "SlashEquals";
} break;
    case TokenType__Star: {
      __yield_0 = "Star";
} break;
    case TokenType__StarEquals: {
      __yield_0 = "StarEquals";
} break;
    case TokenType__StringLiteral: {
      __yield_0 = "StringLiteral";
} break;
    case TokenType__CharLiteral: {
      __yield_0 = "CharLiteral";
} break;
  }
;__yield_0; });
}

f32 minf(f32 a, f32 b) {
  if ((a < b)) 
  return a;
   else 
  return b;
  
}

f32 maxf(f32 a, f32 b) {
  if ((a > b)) 
  return a;
   else 
  return b;
  
}

f32 clampf(f32 x, f32 min, f32 max) {
  return maxf(minf(x, max), min);
}

f32 clamp01(f32 x) {
  return clampf(x, 0.0, 1.0);
}

f32 degrees(f32 radians) {
  return ((radians * 180.0) / M_PI);
}

f32 radians(f32 degrees) {
  return ((degrees * M_PI) / 180.0);
}

i32 edit_distance(char *str1, char *str2) {
  i32 n = strlen(str1);
  i32 m = strlen(str2);
  i32 stride = (m + 1);
  i32 d[(n + 1)][(m + 1)];
  for (i32 i = 0; (i <= n); i += 1) {
    d[i][0] = i;
  } 
  for (i32 j = 0; (j <= m); j += 1) {
    d[0][j] = j;
  } 
  for (i32 i = 1; (i <= n); i += 1) {
    for (i32 j = 1; (j <= m); j += 1) {
      i32 x = (d[(i - 1)][j] + 1);
      i32 y = (d[i][(j - 1)] + 1);
      i32 z;
      if (str1[(i - 1)] == str2[(j - 1)]) {
        z = d[(i - 1)][(j - 1)];
      }  else {
        z = (d[(i - 1)][(j - 1)] + 1);
      } 
      d[i][j] = min(x, min(y, z));
    } 
  } 
  i32 result = d[n][m];
  return result;
}

char *find_word_suggestion(char *s, Vector *options) {
  i32 threshold = 5;
  if (options->size == 0) 
  return NULL;
  
  char *closest = ((char *)Vector__at(options, 0));
  i32 closest_distance = edit_distance(s, closest);
  for (i32 i = 1; (i < options->size); i += 1) {
    char *option = ((char *)Vector__at(options, i));
    i32 distance = edit_distance(s, option);
    if ((distance < closest_distance)) {
      closest = option;
      closest_distance = distance;
    } 
  } 
  if ((closest_distance > threshold)) 
  return NULL;
  
  return closest;
}

char *MessageType__to_color(MessageType this) {
  return ({ char *__yield_0;
  switch (this) {
    case MessageType__Error: {
      __yield_0 = "\x1b[31m";
} break;
    case MessageType__Warning: {
      __yield_0 = "\x1b[33m";
} break;
    case MessageType__Note: {
      __yield_0 = "\x1b[32m";
} break;
  }
;__yield_0; });
}

char *MessageType__str(MessageType this) {
  return ({ char *__yield_0;
  switch (this) {
    case MessageType__Error: {
      __yield_0 = "Error";
} break;
    case MessageType__Warning: {
      __yield_0 = "Warning";
} break;
    case MessageType__Note: {
      __yield_0 = "Note";
} break;
  }
;__yield_0; });
}

void display_line(void) {
  printf("--------------------------------------------------------------------------------" "\n");
}

void display_message(MessageType type, Span span, char *msg) {
  display_line();
  printf("%s: %s: %s" "\n", Location__str(span.start), MessageType__str(type), msg);
  display_line();
}

void display_message_with_span(MessageType type, Span span, char *msg) {
  char *color = MessageType__to_color(type);
  char *reset = "\x1b[0m";
  char *filename = span.start.filename;
  FILE *file = File__open(filename, "r");
  ;
  char *contents = File__slurp(file);
  ;
  i32 around_offset = 1;
  i32 min_line = max((span.start.line - around_offset), 1);
  i32 max_line = (span.end.line + around_offset);
  display_message(type, span, msg);
  char *lines = contents;
  char *cur = strsep((&lines), "\n");
  i32 line_no = 1;
  while ((((bool)cur) && (line_no <= max_line))) {
    if (((line_no >= min_line) && (line_no <= max_line))) {
      printf("%4d | ", line_no);
      if (line_no == span.start.line) {
        i32 start_col = (span.start.col - 1);
        i32 end_col = (span.end.col - 1);
        if ((span.end.line != span.start.line)) {
          end_col = strlen(cur);
        } 
        for (i32 i = 0; (i < start_col); i += 1) {
          printf("%c", cur[i]);
        } 
        printf("%s", color);
        for (i32 i = start_col; (i < end_col); i += 1) {
          printf("%c", cur[i]);
        } 
        printf("%s%s" "\n", reset, (cur + end_col));
        printf("%*s%s^ %s%s" "\n", (start_col + 7), "", color, msg, reset);
      }  else {
        printf("%s" "\n", cur);
      } 
    } 
    line_no += 1;
    cur = strsep((&lines), "\n");
  } 

  /* defers */
  free(contents);
  fclose(file);
}

__attribute__((noreturn)) void error_loc(Location loc, char *msg) {
  error_span((Span){loc, loc}, msg);
}

__attribute__((noreturn)) void error_span(Span span, char *msg) {
  display_message_with_span(MessageType__Error, span, msg);
  display_line();
  exit(1);
}

__attribute__((noreturn)) void error_span_note(Span span, char *msg, char *note) {
  display_message_with_span(MessageType__Error, span, msg);
  display_message(MessageType__Note, span, note);
  exit(1);
}

__attribute__((noreturn)) void error_span_note_span(Span msg_span, char *msg, Span note_span, char *note) {
  display_message_with_span(MessageType__Error, msg_span, msg);
  display_message_with_span(MessageType__Note, note_span, note);
  display_line();
  exit(1);
}

Lexer Lexer__make(char *source, char *filename) {
  return (Lexer){source, .source_len = strlen(source), .i = 0, .loc = (Location){filename, 1, 1}, .seen_newline = false, .tokens = Vector__new()};
}

void Lexer__push(Lexer *this, Token *token) {
  token->seen_newline = this->seen_newline;
  Vector__push(this->tokens, token);
  this->seen_newline = false;
}

void Lexer__push_type(Lexer *this, TokenType type, i32 len) {
  Location start_loc = this->loc;
  this->loc.col += len;
  this->i += len;
  Lexer__push(this, Token__from_type(type, (Span){start_loc, this->loc}));
}

char Lexer__peek(Lexer *this, i32 offset) {
  if (this->source[this->i] == '\0') {
    return this->source[this->i];
  } 
  return this->source[(this->i + 1)];
}

void Lexer__lex_char_literal(Lexer *this) {
  Location start_loc = this->loc;
  i32 start = (this->i + 1);
  this->i += 1;
  if (this->source[this->i] == '\\') {
    this->i += 2;
  }  else {
    this->i += 1;
  } 
  if ((this->source[this->i] != '\'')) {
    this->loc.col += ((this->i - start) + 1);
    error_loc(this->loc, "Expected ' after character literal");
  } 
  i32 len = (this->i - start);
  char *text = string__substring(this->source, start, len);
  this->loc.col += (len + 2);
  this->i += 1;
  Lexer__push(this, Token__new(TokenType__CharLiteral, (Span){start_loc, this->loc}, text));
}

void Lexer__lex_string_literal(Lexer *this) {
  Location start_loc = this->loc;
  char end_char = this->source[this->i];
  i32 start = (this->i + 1);
  this->i += 1;
  while ((this->source[this->i] != end_char)) {
    if (this->source[this->i] == '\\') {
      this->i += 1;
    } 
    this->i += 1;
  } 
  i32 len = (this->i - start);
  char *text = string__substring(this->source, start, len);
  this->loc.col += (len + 2);
  this->i += 1;
  if (end_char == '`') {
    Lexer__push(this, Token__new(TokenType__FormatStringLiteral, (Span){start_loc, this->loc}, text));
  }  else {
    Lexer__push(this, Token__new(TokenType__StringLiteral, (Span){start_loc, this->loc}, text));
  } 
}

void Lexer__lex_int_literal_different_base(Lexer *this) {
  Location start_loc = this->loc;
  i32 start = this->i;
  this->i += 1;
  switch (this->source[this->i]) {
    case 'x': {
      this->i += 1;
      while (((this->i < this->source_len) && isalnum(this->source[this->i]))) {
        this->i += 1;
      } 
    } break;
    case 'b': {
      this->i += 1;
      while ((((this->i < this->source_len) && this->source[this->i] == '0') || this->source[this->i] == '1')) {
        this->i += 1;
      } 
    } break;
    default: {
    } break;
  }
  i32 len = (this->i - start);
  char *text = string__substring(this->source, start, len);
  this->loc.col += len;
  Lexer__push(this, Token__new(TokenType__IntLiteral, (Span){start_loc, this->loc}, text));
}

void Lexer__lex_numeric_literal(Lexer *this) {
  Location start_loc = this->loc;
  if (this->source[this->i] == '0') {
    switch (Lexer__peek(this, 1)) {
      case 'x':
      case 'b': {
        Lexer__lex_int_literal_different_base(this);
        return;
      } break;
      default: {
      } break;
    }
  } 
  i32 start = this->i;
  TokenType token_type;
  while (isdigit(this->source[this->i])) {
    this->i += 1;
  } 
  if (this->source[this->i] == '.') {
    this->i += 1;
    while (isdigit(this->source[this->i])) {
      this->i += 1;
    } 
    token_type = TokenType__FloatLiteral;
  }  else {
    token_type = TokenType__IntLiteral;
  } 
  i32 len = (this->i - start);
  char *text = string__substring(this->source, start, len);
  this->loc.col += len;
  Lexer__push(this, Token__new(token_type, (Span){start_loc, this->loc}, text));
}

Vector *Lexer__lex(Lexer *this) {
  while ((this->i < this->source_len)) {
    char c = this->source[this->i];
    switch (c) {
      case ' ':
      case '\t':
      case '\v':
      case '\r':
      case '\b': {
        this->loc.col += 1;
        this->i += 1;
      } break;
      case '\n': {
        this->loc.line += 1;
        this->loc.col = 1;
        this->i += 1;
        this->seen_newline = true;
      } break;
      case ';': {
        Lexer__push_type(this, TokenType__Semicolon, 1);
      } break;
      case ',': {
        Lexer__push_type(this, TokenType__Comma, 1);
      } break;
      case '.': {
        Lexer__push_type(this, TokenType__Dot, 1);
      } break;
      case '(': {
        Lexer__push_type(this, TokenType__OpenParen, 1);
      } break;
      case ')': {
        Lexer__push_type(this, TokenType__CloseParen, 1);
      } break;
      case '[': {
        Lexer__push_type(this, TokenType__OpenSquare, 1);
      } break;
      case ']': {
        Lexer__push_type(this, TokenType__CloseSquare, 1);
      } break;
      case '{': {
        Lexer__push_type(this, TokenType__OpenCurly, 1);
      } break;
      case '}': {
        Lexer__push_type(this, TokenType__CloseCurly, 1);
      } break;
      case '@': {
        Lexer__push_type(this, TokenType__AtSign, 1);
      } break;
      case '%': {
        Lexer__push_type(this, TokenType__Percent, 1);
      } break;
      case '^': {
        Lexer__push_type(this, TokenType__Caret, 1);
      } break;
      case '&': {
        Lexer__push_type(this, TokenType__Ampersand, 1);
      } break;
      case '|': {
        Lexer__push_type(this, TokenType__Line, 1);
      } break;
      case '?': {
        Lexer__push_type(this, TokenType__Question, 1);
      } break;
      case '!': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__NotEquals, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Exclamation, 1);
          } break;
        }
      } break;
      case ':': {
        switch (Lexer__peek(this, 1)) {
          case ':': {
            Lexer__push_type(this, TokenType__ColonColon, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Colon, 1);
          } break;
        }
      } break;
      case '=': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__EqualEquals, 2);
          } break;
          case '>': {
            Lexer__push_type(this, TokenType__FatArrow, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Equals, 1);
          } break;
        }
      } break;
      case '*': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__StarEquals, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Star, 1);
          } break;
        }
      } break;
      case '+': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__PlusEquals, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Plus, 1);
          } break;
        }
      } break;
      case '-': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__MinusEquals, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Minus, 1);
          } break;
        }
      } break;
      case '<': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__LessThanEquals, 2);
          } break;
          case '<': {
            Lexer__push_type(this, TokenType__LessThanLessThan, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__LessThan, 1);
          } break;
        }
      } break;
      case '>': {
        switch (Lexer__peek(this, 1)) {
          case '=': {
            Lexer__push_type(this, TokenType__GreaterThanEquals, 2);
          } break;
          case '>': {
            Lexer__push_type(this, TokenType__GreaterThanGreaterThan, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__GreaterThan, 1);
          } break;
        }
      } break;
      case '/': {
        switch (Lexer__peek(this, 1)) {
          case '/': {
            this->i += 1;
            while (((this->i < this->source_len) && (this->source[this->i] != '\n'))) {
              this->i += 1;
            } 
          } break;
          case '=': {
            Lexer__push_type(this, TokenType__SlashEquals, 2);
          } break;
          default: {
            Lexer__push_type(this, TokenType__Slash, 1);
          } break;
        }
      } break;
      case '\'': {
        Lexer__lex_char_literal(this);
      } break;
      case '"':
      case '`': {
        Lexer__lex_string_literal(this);
      } break;
      default: {
        Location start_loc = this->loc;
        if (isdigit(c)) {
          Lexer__lex_numeric_literal(this);
        }  else         if ((isalpha(c) || c == '_')) {
          i32 start = this->i;
          while ((isalnum(this->source[this->i]) || this->source[this->i] == '_')) {
            this->i += 1;
          } 
          i32 len = (this->i - start);
          char *text = string__substring(this->source, start, len);
          this->loc.col += len;
          Lexer__push(this, Token__from_ident(text, (Span){start_loc, this->loc}));
        }  else {
          panic(format_string("%s: Unrecognized char in lexer: '%c'", Location__str(this->loc), c));
        } 
        
      } break;
    }
  } 
  this->seen_newline = true;
  Lexer__push_type(this, TokenType__EOF, 0);
  return this->tokens;
}

MapNode *MapNode__new(char *key, void *value, MapNode *next) {
  MapNode *node = ((MapNode *)calloc(1, sizeof(MapNode)));
  node->key = key;
  node->value = value;
  node->next = next;
  return node;
}

void MapNode__free_list(MapNode *node) {
  MapNode *cur = node;
  while (((bool)cur)) {
    MapNode *next = cur->next;
    free(cur);
    cur = next;
  } 
}

Map *Map__new(void) {
  Map *map = ((Map *)calloc(1, sizeof(Map)));
  map->num_buckets = 4;
  map->buckets = ((MapNode **)calloc(map->num_buckets, sizeof(MapNode *)));
  return map;
}

i32 Map__hash(Map *this, char *s) {
  i32 hash = 5381;
  i32 len = strlen(s);
  for (i32 i = 0; (i < len); i += 1) {
    hash = ((hash * 33) ^ ((i32)s[i]));
  } 
  hash = (hash % this->num_buckets);
  if ((hash < 0)) {
    hash += this->num_buckets;
  } 
  return hash;
}

MapNode *Map__get_node(Map *this, char *key) {
  i32 hash = Map__hash(this, key);
  MapNode *node = this->buckets[hash];
  while (((bool)node)) {
    if (string__eq(node->key, key)) {
      return node;
    } 
    node = node->next;
  } 
  return NULL;
}

void *Map__get(Map *this, char *key) {
  MapNode *node = Map__get_node(this, key);
  if (((bool)node)) {
    return node->value;
  } 
  return NULL;
}

bool Map__exists(Map *this, char *key) {
  return ((bool)Map__get_node(this, key));
}

void Map__insert(Map *this, char *key, void *value) {
  MapNode *node = Map__get_node(this, key);
  if (((bool)node)) {
    node->value = value;
  }  else {
    i32 hash = Map__hash(this, key);
    MapNode *new_node = MapNode__new(key, value, this->buckets[hash]);
    if (((bool)this->buckets[hash])) {
      this->num_collisions += 1;
    } 
    this->buckets[hash] = new_node;
    this->num_items += 1;
    if ((this->num_items > this->num_buckets)) {
      Map__resize(this);
    } 
  } 
}

void Map__resize(Map *this) {
  MapNode **old_buckets = this->buckets;
  i32 old_num_buckets = this->num_buckets;
  i32 old_num_items = this->num_items;
  this->num_collisions = 0;
  this->num_buckets *= 2;
  this->buckets = ((MapNode **)calloc(this->num_buckets, sizeof(MapNode *)));
  for (i32 i = 0; (i < old_num_buckets); i += 1) {
    MapNode *node = old_buckets[i];
    while (((bool)node)) {
      i32 new_hash = Map__hash(this, node->key);
      MapNode *new_node = MapNode__new(node->key, node->value, this->buckets[new_hash]);
      if (((bool)this->buckets[new_hash])) {
        this->num_collisions += 1;
      } 
      this->buckets[new_hash] = new_node;
      node = node->next;
    } 
  } 
  for (i32 i = 0; (i < old_num_buckets); i += 1) {
    MapNode__free_list(old_buckets[i]);
  } 
  free(old_buckets);
}

void Map__print_keys(Map *this) {
  for (i32 i = 0; (i < this->num_buckets); i += 1) {
    MapNode *node = this->buckets[i];
    while (((bool)node)) {
      printf("- '%s'\n" "\n", node->key);
      node = node->next;
    } 
  } 
}

void Map__push_keys(Map *this, Vector *vec) {
  for (i32 i = 0; (i < this->num_buckets); i += 1) {
    MapNode *node = this->buckets[i];
    while (((bool)node)) {
      Vector__push(vec, node->key);
      node = node->next;
    } 
  } 
}

void Map__free(Map *this) {
  for (i32 i = 0; (i < this->num_buckets); i += 1) {
    MapNode__free_list(this->buckets[i]);
  } 
  free(this->buckets);
}

MapIterator Map__iter(Map *this) {
  return MapIterator__make(this);
}

char *MapIterator__key(MapIterator *this) {
  return this->cur->key;
}

void *MapIterator__value(MapIterator *this) {
  return this->cur->value;
}

MapIterator MapIterator__make(Map *map) {
  MapIterator it = (MapIterator){.idx = (-1), .cur = NULL, map};
  MapIterator__next((&it));
  return it;
}

void MapIterator__next(MapIterator *this) {
  while ((this->idx < this->map->num_buckets)) {
    while (((bool)this->cur)) {
      this->cur = this->cur->next;
      if (((bool)this->cur)) 
      return;
      
    } 
    this->idx += 1;
    this->cur = ({ MapNode *__yield_0;
  if ((this->idx < this->map->num_buckets)) {
    __yield_0 = this->map->buckets[this->idx];
  }  else {
    __yield_0 = NULL;
  } 
;__yield_0; });
    if (((bool)this->cur)) 
    return;
    
  } 
}

char *BaseType__str(BaseType this) {
  return ({ char *__yield_0;
  switch (this) {
    case BaseType__Char: {
      __yield_0 = "char";
} break;
    case BaseType__Bool: {
      __yield_0 = "bool";
} break;
    case BaseType__Void: {
      __yield_0 = "void";
} break;
    case BaseType__I8: {
      __yield_0 = "i8";
} break;
    case BaseType__I16: {
      __yield_0 = "i16";
} break;
    case BaseType__I32: {
      __yield_0 = "i32";
} break;
    case BaseType__I64: {
      __yield_0 = "i64";
} break;
    case BaseType__U8: {
      __yield_0 = "u8";
} break;
    case BaseType__U16: {
      __yield_0 = "u16";
} break;
    case BaseType__U32: {
      __yield_0 = "u32";
} break;
    case BaseType__U64: {
      __yield_0 = "u64";
} break;
    case BaseType__F32: {
      __yield_0 = "f32";
} break;
    case BaseType__F64: {
      __yield_0 = "f64";
} break;
    case BaseType__Pointer: {
      __yield_0 = "&";
} break;
    case BaseType__Structure: {
      __yield_0 = "structure";
} break;
    case BaseType__Function: {
      __yield_0 = "function";
} break;
    case BaseType__Method: {
      __yield_0 = "method";
} break;
    case BaseType__Array: {
      __yield_0 = "[]";
} break;
  }
;__yield_0; });
}

Type *Type__new(BaseType base, Span span) {
  Type *type = ((Type *)calloc(1, sizeof(Type)));
  type->base = base;
  type->span = span;
  return type;
}

Type *Type__new_link(BaseType base, Type *next, Span span) {
  Type *type = Type__new(base, span);
  type->ptr = next;
  if (((base == BaseType__Pointer && ((bool)next)) && next->base == BaseType__Char)) {
    type->name = "string";
  } 
  return type;
}

Type *Type__ptr_to(BaseType base, Span span) {
  Type *next = Type__new(base, span);
  return Type__new_link(BaseType__Pointer, next, span);
}

bool Type__is_struct_or_ptr(Type *this) {
  if (this->base == BaseType__Structure) 
  return true;
  
  if ((this->base != BaseType__Pointer)) 
  return false;
  
  return this->ptr->base == BaseType__Structure;
}

bool Type__is_integer(Type *this) {
  return ({ bool __yield_0;
  switch (this->base) {
    case BaseType__I8:
    case BaseType__I16:
    case BaseType__I32:
    case BaseType__I64:
    case BaseType__U8:
    case BaseType__U16:
    case BaseType__U32:
    case BaseType__U64: {
      __yield_0 = true;
} break;
    default: {
      __yield_0 = false;
} break;
  }
;__yield_0; });
}

bool Type__is_numeric(Type *this) {
  return ({ bool __yield_0;
  switch (this->base) {
    case BaseType__I8:
    case BaseType__I16:
    case BaseType__I32:
    case BaseType__I64:
    case BaseType__U8:
    case BaseType__U16:
    case BaseType__U32:
    case BaseType__U64:
    case BaseType__F32:
    case BaseType__F64: {
      __yield_0 = true;
} break;
    default: {
      __yield_0 = false;
} break;
  }
;__yield_0; });
}

bool Type__eq(Type *this, Type *other) {
  if ((this == NULL && other == NULL)) 
  return true;
  
  if ((this == NULL || other == NULL)) 
  return false;
  
  if ((this->base != other->base)) 
  return false;
  
  switch (this->base) {
    case BaseType__Method: {
      return false;
    } break;
    case BaseType__Array: {
      return Type__eq(this->ptr, other->ptr);
    } break;
    case BaseType__Function: {
      if ((!Type__eq(this->return_type, other->return_type))) 
      return false;
      
      if ((this->params->size != other->params->size)) 
      return false;
      
      for (i32 i = 0; (i < this->params->size); i += 1) {
        Variable *a = ((Variable *)Vector__at(this->params, i));
        Variable *b = ((Variable *)Vector__at(other->params, i));
        if ((!Type__eq(a->type, b->type))) 
        return false;
        
      } 
      return true;
    } break;
    case BaseType__Structure: {
      return string__eq(this->name, other->name);
    } break;
    case BaseType__Pointer: {
      if ((this->ptr->base == BaseType__Void || other->ptr->base == BaseType__Void)) {
        return true;
      } 
      return Type__eq(this->ptr, other->ptr);
    } break;
    default: {
      return true;
    } break;
  }
}

char *Type__str(Type *this) {
  return ({ char *__yield_0;
  switch (this->base) {
    case BaseType__Pointer: {
      __yield_0 = format_string("&%s", Type__str(this->ptr));
} break;
    case BaseType__Array: {
      __yield_0 = format_string("[%s]", Type__str(this->ptr));
} break;
    case BaseType__Structure: {
      __yield_0 = this->name;
} break;
    case BaseType__Function: {
      __yield_0 = "<function>";
} break;
    case BaseType__Method: {
      __yield_0 = "<method>";
} break;
    default: {
      __yield_0 = BaseType__str(this->base);
} break;
  }
;__yield_0; });
}

bool Type__is_string(Type *this) {
  return (this->base == BaseType__Pointer && this->ptr->base == BaseType__Char);
}

Type *Type__decay_array(Type *this) {
  if ((this->base != BaseType__Array)) 
  return this;
  
  return Type__new_link(BaseType__Pointer, this->ptr, this->span);
}

bool Type__is_enum(Type *this) {
  return ((this->base == BaseType__Structure && ((bool)this->struct_def)) && this->struct_def->is_enum);
}

bool Type__is_struct(Type *this) {
  return (this->base == BaseType__Structure && (!Type__is_enum(this)));
}

char *ASTType__str(ASTType this) {
  return ({ char *__yield_0;
  switch (this) {
    case ASTType__Assignment: {
      __yield_0 = "Assignment";
} break;
    case ASTType__Address: {
      __yield_0 = "Address";
} break;
    case ASTType__And: {
      __yield_0 = "And";
} break;
    case ASTType__Block: {
      __yield_0 = "Block";
} break;
    case ASTType__BoolLiteral: {
      __yield_0 = "BoolLiteral";
} break;
    case ASTType__BitwiseAnd: {
      __yield_0 = "BitwiseAnd";
} break;
    case ASTType__BitwiseOr: {
      __yield_0 = "BitwiseOr";
} break;
    case ASTType__BitwiseXor: {
      __yield_0 = "BitwiseXor";
} break;
    case ASTType__Break: {
      __yield_0 = "Break";
} break;
    case ASTType__Call: {
      __yield_0 = "Call";
} break;
    case ASTType__Cast: {
      __yield_0 = "Cast";
} break;
    case ASTType__CharLiteral: {
      __yield_0 = "CharLiteral";
} break;
    case ASTType__Constructor: {
      __yield_0 = "Constructor";
} break;
    case ASTType__Continue: {
      __yield_0 = "Continue";
} break;
    case ASTType__Defer: {
      __yield_0 = "Defer";
} break;
    case ASTType__Dereference: {
      __yield_0 = "Dereference";
} break;
    case ASTType__Divide: {
      __yield_0 = "Divide";
} break;
    case ASTType__DivideEquals: {
      __yield_0 = "DivideEquals";
} break;
    case ASTType__EnumValue: {
      __yield_0 = "EnumValue";
} break;
    case ASTType__Equals: {
      __yield_0 = "Equals";
} break;
    case ASTType__FloatLiteral: {
      __yield_0 = "FloatLiteral";
} break;
    case ASTType__FormatStringLiteral: {
      __yield_0 = "FormatStringLiteral";
} break;
    case ASTType__For: {
      __yield_0 = "For";
} break;
    case ASTType__GreaterThan: {
      __yield_0 = "GreaterThan";
} break;
    case ASTType__GreaterThanEquals: {
      __yield_0 = "GreaterThanEquals";
} break;
    case ASTType__If: {
      __yield_0 = "If";
} break;
    case ASTType__Identifier: {
      __yield_0 = "Identifier";
} break;
    case ASTType__Index: {
      __yield_0 = "Index";
} break;
    case ASTType__IntLiteral: {
      __yield_0 = "IntLiteral";
} break;
    case ASTType__IsNotNull: {
      __yield_0 = "IsNotNull";
} break;
    case ASTType__LeftShift: {
      __yield_0 = "LeftShift";
} break;
    case ASTType__LessThan: {
      __yield_0 = "LessThan";
} break;
    case ASTType__LessThanEquals: {
      __yield_0 = "LessThanEquals";
} break;
    case ASTType__Match: {
      __yield_0 = "Match";
} break;
    case ASTType__Member: {
      __yield_0 = "Member";
} break;
    case ASTType__Minus: {
      __yield_0 = "Minus";
} break;
    case ASTType__MinusEquals: {
      __yield_0 = "MinusEquals";
} break;
    case ASTType__Modulus: {
      __yield_0 = "Modulus";
} break;
    case ASTType__Multiply: {
      __yield_0 = "Multiply";
} break;
    case ASTType__MultiplyEquals: {
      __yield_0 = "MultiplyEquals";
} break;
    case ASTType__Not: {
      __yield_0 = "Not";
} break;
    case ASTType__NotEquals: {
      __yield_0 = "NotEquals";
} break;
    case ASTType__Null: {
      __yield_0 = "Null";
} break;
    case ASTType__Or: {
      __yield_0 = "Or";
} break;
    case ASTType__Plus: {
      __yield_0 = "Plus";
} break;
    case ASTType__PlusEquals: {
      __yield_0 = "PlusEquals";
} break;
    case ASTType__Return: {
      __yield_0 = "Return";
} break;
    case ASTType__RightShift: {
      __yield_0 = "RightShift";
} break;
    case ASTType__SizeOf: {
      __yield_0 = "SizeOf";
} break;
    case ASTType__ScopeLookup: {
      __yield_0 = "ScopeLookup";
} break;
    case ASTType__StringLiteral: {
      __yield_0 = "StringLiteral";
} break;
    case ASTType__UnaryMinus: {
      __yield_0 = "UnaryMinus";
} break;
    case ASTType__VarDeclaration: {
      __yield_0 = "VarDeclaration";
} break;
    case ASTType__Yield: {
      __yield_0 = "Yield";
} break;
    case ASTType__While: {
      __yield_0 = "While";
} break;
  }
;__yield_0; });
}

ASTType ASTType__from_token(TokenType type) {
  return ({ ASTType __yield_0;
  switch (type) {
    case TokenType__Ampersand: {
      __yield_0 = ASTType__BitwiseAnd;
} break;
    case TokenType__And: {
      __yield_0 = ASTType__And;
} break;
    case TokenType__Caret: {
      __yield_0 = ASTType__BitwiseXor;
} break;
    case TokenType__EqualEquals: {
      __yield_0 = ASTType__Equals;
} break;
    case TokenType__Equals: {
      __yield_0 = ASTType__Assignment;
} break;
    case TokenType__GreaterThan: {
      __yield_0 = ASTType__GreaterThan;
} break;
    case TokenType__GreaterThanEquals: {
      __yield_0 = ASTType__GreaterThanEquals;
} break;
    case TokenType__GreaterThanGreaterThan: {
      __yield_0 = ASTType__RightShift;
} break;
    case TokenType__LessThan: {
      __yield_0 = ASTType__LessThan;
} break;
    case TokenType__LessThanEquals: {
      __yield_0 = ASTType__LessThanEquals;
} break;
    case TokenType__LessThanLessThan: {
      __yield_0 = ASTType__LeftShift;
} break;
    case TokenType__Line: {
      __yield_0 = ASTType__BitwiseOr;
} break;
    case TokenType__Minus: {
      __yield_0 = ASTType__Minus;
} break;
    case TokenType__MinusEquals: {
      __yield_0 = ASTType__MinusEquals;
} break;
    case TokenType__NotEquals: {
      __yield_0 = ASTType__NotEquals;
} break;
    case TokenType__Or: {
      __yield_0 = ASTType__Or;
} break;
    case TokenType__Percent: {
      __yield_0 = ASTType__Modulus;
} break;
    case TokenType__Plus: {
      __yield_0 = ASTType__Plus;
} break;
    case TokenType__PlusEquals: {
      __yield_0 = ASTType__PlusEquals;
} break;
    case TokenType__Slash: {
      __yield_0 = ASTType__Divide;
} break;
    case TokenType__SlashEquals: {
      __yield_0 = ASTType__DivideEquals;
} break;
    case TokenType__Star: {
      __yield_0 = ASTType__Multiply;
} break;
    case TokenType__StarEquals: {
      __yield_0 = ASTType__MultiplyEquals;
} break;
    default: {
      panic(format_string("Unhandled token type in ASTType::from_token: %s", TokenType__str(type)));
    } break;
  }
;__yield_0; });
}

Variable *Variable__new(char *name, Type *type, Span span) {
  Variable *var = ((Variable *)calloc(1, sizeof(Variable)));
  var->name = name;
  var->type = type;
  var->span = span;
  return var;
}

Function *Function__new(Span span) {
  Function *func = ((Function *)calloc(1, sizeof(Function)));
  func->params = Vector__new();
  func->span = span;
  return func;
}

Structure *Structure__new(Span span) {
  Structure *struc = ((Structure *)calloc(1, sizeof(Structure)));
  struc->fields = Vector__new();
  struc->span = span;
  return struc;
}

Variable *Structure__find_field(Structure *this, char *name) {
  for (i32 i = 0; (i < this->fields->size); i += 1) {
    Variable *field = ((Variable *)Vector__at(this->fields, i));
    if (string__eq(field->name, name)) {
      return field;
    } 
  } 
  return NULL;
}

Program *Program__new(void) {
  Program *prog = ((Program *)calloc(1, sizeof(Program)));
  prog->functions = Vector__new();
  prog->structures = Vector__new();
  prog->global_vars = Vector__new();
  prog->included_files = Map__new();
  prog->c_flags = Vector__new();
  prog->c_includes = Vector__new();
  prog->c_embed_headers = Vector__new();
  return prog;
}

bool Program__is_file_included(Program *this, char *filename) {
  i32 len = strlen(filename);
  if ((((len > 2) && filename[0] == '.') && filename[1] == '/')) {
    filename = (&filename[2]);
  } 
  return Map__exists(this->included_files, filename);
}

void Program__add_included_file(Program *this, char *filename) {
  i32 len = strlen(filename);
  if ((((len > 2) && filename[0] == '.') && filename[1] == '/')) {
    filename = (&filename[2]);
  } 
  Map__insert(this->included_files, filename, filename);
}

Argument *Argument__new(AST *label, AST *expr) {
  Argument *arg = ((Argument *)calloc(1, sizeof(Argument)));
  arg->expr = expr;
  arg->label = label;
  return arg;
}

MatchCase *MatchCase__new(AST *cond, AST *body) {
  MatchCase *_case = ((MatchCase *)calloc(1, sizeof(MatchCase)));
  _case->cond = cond;
  _case->body = body;
  return _case;
}

AST *AST__new(ASTType type, Span span) {
  AST *ast = ((AST *)calloc(1, sizeof(AST)));
  ast->type = type;
  ast->span = span;
  return ast;
}

AST *AST__new_unop(ASTType type, Span span, AST *expr) {
  AST *ast = AST__new(type, span);
  ast->u.unary = expr;
  return ast;
}

AST *AST__new_binop(ASTType type, AST *lhs, AST *rhs) {
  AST *ast = AST__new(type, Span__join(lhs->span, rhs->span));
  ast->u.binary.lhs = lhs;
  ast->u.binary.rhs = rhs;
  return ast;
}

bool AST__callee_is(AST *this, char *expected) {
  if ((!((bool)this))) 
  return false;
  
  if ((this->type != ASTType__Call)) 
  return false;
  
  if ((this->u.call.callee->type != ASTType__Identifier)) 
  return false;
  
  char *name = this->u.call.callee->u.ident.name;
  return string__eq(name, expected);
}

bool AST__is_lvalue(AST *this) {
  return ({ bool __yield_0;
  switch (this->type) {
    case ASTType__Dereference: {
      __yield_0 = true;
} break;
    case ASTType__Index: {
      __yield_0 = true;
} break;
    case ASTType__Member: {
      __yield_0 = (!this->u.member.is_method);
} break;
    case ASTType__Identifier: {
      __yield_0 = (!this->u.ident.is_function);
} break;
    default: {
      __yield_0 = false;
} break;
  }
;__yield_0; });
}

ParserContext *ParserContext__new(Vector *tokens, i32 offset) {
  ParserContext *context = ((ParserContext *)calloc(1, sizeof(ParserContext)));
  (*context) = (ParserContext){tokens, offset};
  return context;
}

Parser *Parser__new(Vector *tokens, char *filename) {
  Parser *parser = ((Parser *)calloc(1, sizeof(Parser)));
  parser->tokens = tokens;
  parser->curr = 0;
  parser->include_dirs = Vector__new();
  Parser__add_include_dir(parser, ".");
  char *tmp_filename = strdup(filename);
  parser->project_root = strdup(dirname(tmp_filename));
  free(tmp_filename);
  char *aecor_root = getenv("AECOR_ROOT");
  if (((bool)aecor_root)) {
    Parser__add_include_dir(parser, aecor_root);
  } 
  parser->context_stack = Vector__new();
  return parser;
}

void Parser__push_context(Parser *this, Vector *tokens) {
  ParserContext *cur_context = ParserContext__new(this->tokens, this->curr);
  Vector__push(this->context_stack, cur_context);
  this->tokens = tokens;
  this->curr = 0;
}

void Parser__pop_context(Parser *this) {
  ParserContext *cur_context = ((ParserContext *)Vector__pop(this->context_stack));
  this->tokens = cur_context->tokens;
  this->curr = cur_context->offset;
  free(cur_context);
}

void Parser__add_include_dir(Parser *this, char *dir) {
  Vector__push(this->include_dirs, dir);
}

void Parser__error(Parser *this, char *msg) {
  error_span(Parser__token(this)->span, msg);
}

void Parser__unhandled_type(Parser *this, char *func) {
  Parser__error(this, format_string("Unexpected token in %s: %s", func, TokenType__str(Parser__token(this)->type)));
}

Token *Parser__token(Parser *this) {
  return Vector__at(this->tokens, this->curr);
}

bool Parser__token_is(Parser *this, TokenType type) {
  if (type == TokenType__Newline) {
    return Parser__token(this)->seen_newline;
  } 
  return Parser__token(this)->type == type;
}

bool Parser__consume_if(Parser *this, TokenType type) {
  if (Parser__token_is(this, type)) {
    if ((type != TokenType__Newline)) {
      this->curr += 1;
    } 
    return true;
  } 
  return false;
}

void Parser__consume_newline_or(Parser *this, TokenType type) {
  if (Parser__token_is(this, type)) {
    this->curr += 1;
  }  else   if ((!Parser__token(this)->seen_newline)) {
    Parser__error(this, format_string("Expected %s or newline", TokenType__str(type)));
  } 
  
}

Token *Parser__consume(Parser *this, TokenType type) {
  Token *tok = Parser__token(this);
  if ((!Parser__consume_if(this, type))) {
    Parser__error(this, format_string("Expected TokenType::%s", TokenType__str(type)));
  } 
  return tok;
}

Type *Parser__parse_type_with_parent(Parser *this, Type *parent) {
  Type *type = ((Type *)NULL);
  Span span = Parser__token(this)->span;
  switch (Parser__token(this)->type) {
    case TokenType__Ampersand: {
      type = Type__new(BaseType__Pointer, Parser__consume(this, TokenType__Ampersand)->span);
      Parser__parse_type_with_parent(this, type);
    } break;
    case TokenType__Bool: {
      type = Type__new(BaseType__Bool, Parser__consume(this, TokenType__Bool)->span);
    } break;
    case TokenType__Char: {
      type = Type__new(BaseType__Char, Parser__consume(this, TokenType__Char)->span);
    } break;
    case TokenType__I8: {
      type = Type__new(BaseType__I8, Parser__consume(this, TokenType__I8)->span);
    } break;
    case TokenType__I16: {
      type = Type__new(BaseType__I16, Parser__consume(this, TokenType__I16)->span);
    } break;
    case TokenType__I32: {
      type = Type__new(BaseType__I32, Parser__consume(this, TokenType__I32)->span);
    } break;
    case TokenType__I64: {
      type = Type__new(BaseType__I64, Parser__consume(this, TokenType__I64)->span);
    } break;
    case TokenType__U8: {
      type = Type__new(BaseType__U8, Parser__consume(this, TokenType__U8)->span);
    } break;
    case TokenType__U16: {
      type = Type__new(BaseType__U16, Parser__consume(this, TokenType__U16)->span);
    } break;
    case TokenType__U32: {
      type = Type__new(BaseType__U32, Parser__consume(this, TokenType__U32)->span);
    } break;
    case TokenType__U64: {
      type = Type__new(BaseType__U64, Parser__consume(this, TokenType__U64)->span);
    } break;
    case TokenType__F32: {
      type = Type__new(BaseType__F32, Parser__consume(this, TokenType__F32)->span);
    } break;
    case TokenType__F64: {
      type = Type__new(BaseType__F64, Parser__consume(this, TokenType__F64)->span);
    } break;
    case TokenType__UntypedPtr: {
      type = Type__ptr_to(BaseType__Void, Parser__consume(this, TokenType__UntypedPtr)->span);
    } break;
    case TokenType__String: {
      type = Type__ptr_to(BaseType__Char, Parser__consume(this, TokenType__String)->span);
      type->ptr->name = "string";
    } break;
    case TokenType__Identifier: {
      type = Type__new(BaseType__Structure, span);
      type->name = Parser__consume(this, TokenType__Identifier)->text;
    } break;
    case TokenType__Fn: {
      Parser__consume(this, TokenType__Fn);
      Parser__consume(this, TokenType__OpenParen);
      Vector *params = Vector__new();
      while ((!Parser__token_is(this, TokenType__CloseParen))) {
        Type *param_type = Parser__parse_type(this);
        Vector__push(params, Variable__new("", param_type, param_type->span));
        if ((!Parser__token_is(this, TokenType__CloseParen))) {
          Parser__consume(this, TokenType__Comma);
        } 
      } 
      Parser__consume(this, TokenType__CloseParen);
      Type *return_type;
      if (Parser__consume_if(this, TokenType__Colon)) {
        return_type = Parser__parse_type(this);
      }  else {
        return_type = Type__new(BaseType__Void, Parser__token(this)->span);
      } 
      type = Type__new(BaseType__Function, Span__join(span, Parser__token(this)->span));
      type->params = params;
      type->return_type = return_type;
    } break;
    case TokenType__OpenSquare: {
      Parser__consume(this, TokenType__OpenSquare);
      type = Type__new(BaseType__Array, span);
      Parser__parse_type_with_parent(this, type);
      Parser__consume(this, TokenType__Semicolon);
      type->size_expr = Parser__parse_expression(this, TokenType__CloseSquare);
      type->span = Span__join(type->span, Parser__token(this)->span);
      Parser__consume(this, TokenType__CloseSquare);
    } break;
    default: {
      Parser__unhandled_type(this, "parse_type");
    } break;
  }
  if (((bool)parent)) {
    parent->ptr = type;
    parent->span = Span__join(parent->span, type->span);
  } 
  return type;
}

Type *Parser__parse_type(Parser *this) {
  return Parser__parse_type_with_parent(this, NULL);
}

AST *Parser__parse_format_string(Parser *this) {
  Token *fstr = Parser__consume(this, TokenType__FormatStringLiteral);
  i32 fstr_len = strlen(fstr->text);
  Vector *expr_parts = Vector__new();
  Vector *expr_start = Vector__new();
  Vector *format_parts = Vector__new();
  i32 count = 0;
  i32 cur_start = 0;
  for (i32 i = 0; (i < fstr_len); i += 1) {
    if ((fstr->text[i] == '{' && (fstr->text[(i - 1)] != '\\'))) {
      if (count == 0) {
        char *part = string__substring(fstr->text, cur_start, (i - cur_start));
        Vector__push(format_parts, part);
        cur_start = (i + 1);
      } 
      count += 1;
    }  else     if ((fstr->text[i] == '}' && (fstr->text[(i - 1)] != '\\'))) {
      count -= 1;
      if (count == 0) {
        char *part = string__substring(fstr->text, cur_start, (i - cur_start));
        Vector__push(expr_parts, part);
        Vector__push(expr_start, (fstr->text + cur_start));
        cur_start = (i + 1);
      }  else       if ((count < 0)) {
        error_span(fstr->span, "Unmatched '}' in format string");
      } 
      
    } 
    
  } 
  if ((count != 0)) {
    error_span(fstr->span, "Unmatched '{' in format string");
  } 
  char *part = string__substring(fstr->text, cur_start, (fstr_len - cur_start));
  Vector__push(format_parts, part);
  AST *node = AST__new(ASTType__FormatStringLiteral, fstr->span);
  node->u.fmt_str.parts = format_parts;
  Location fstr_start = fstr->span.start;
  Vector *expr_nodes = Vector__new();
  for (i32 i = 0; (i < expr_parts->size); i += 1) {
    char *part = ((char *)Vector__at(expr_parts, i));
    i32 start = (((char *)Vector__at(expr_start, i)) - fstr->text);
    Lexer lexer = Lexer__make(part, fstr_start.filename);
    lexer.loc = fstr_start;
    lexer.loc.col += (start + 1);
    Vector *tokens = Lexer__lex((&lexer));
    Parser__push_context(this, tokens);
    AST *expr = Parser__parse_expression(this, TokenType__CloseCurly);
    if ((!Parser__token_is(this, TokenType__EOF))) {
      error_loc(Parser__token(this)->span.start, "Invalid expression in format string");
    } 
    Parser__pop_context(this);
    Vector__push(expr_nodes, expr);
  } 
  node->u.fmt_str.exprs = expr_nodes;
  Vector__free(expr_parts);
  Vector__free(expr_start);
  return node;
}

AST *Parser__parse_factor(Parser *this, TokenType end_type) {
  AST *node = ((AST *)NULL);
  switch (Parser__token(this)->type) {
    case TokenType__FormatStringLiteral: {
      node = Parser__parse_format_string(this);
    } break;
    case TokenType__IntLiteral: {
      node = AST__new(ASTType__IntLiteral, Parser__token(this)->span);
      Token *tok = Parser__consume(this, TokenType__IntLiteral);
      node->u.num_literal = tok->text;
    } break;
    case TokenType__FloatLiteral: {
      node = AST__new(ASTType__FloatLiteral, Parser__token(this)->span);
      Token *tok = Parser__consume(this, TokenType__FloatLiteral);
      node->u.num_literal = tok->text;
    } break;
    case TokenType__StringLiteral: {
      node = AST__new(ASTType__StringLiteral, Parser__token(this)->span);
      Token *tok = Parser__consume(this, TokenType__StringLiteral);
      node->u.string_literal = tok->text;
    } break;
    case TokenType__CharLiteral: {
      node = AST__new(ASTType__CharLiteral, Parser__token(this)->span);
      Token *tok = Parser__consume(this, TokenType__CharLiteral);
      node->u.char_literal = tok->text;
    } break;
    case TokenType__True:
    case TokenType__False: {
      Token *tok = Parser__consume(this, Parser__token(this)->type);
      node = AST__new(ASTType__BoolLiteral, tok->span);
      node->u.bool_literal = tok->type == TokenType__True;
    } break;
    case TokenType__Null: {
      node = AST__new(ASTType__Null, Parser__token(this)->span);
      Parser__consume(this, TokenType__Null);
    } break;
    case TokenType__Dot: {
      Token *op = Parser__consume(this, TokenType__Dot);
      Token *rhs = Parser__consume(this, TokenType__Identifier);
      AST *lhs = AST__new(ASTType__Identifier, op->span);
      lhs->u.ident.name = "this";
      lhs->u.ident.is_function = false;
      node = AST__new(ASTType__Member, Span__join(lhs->span, rhs->span));
      node->u.member.lhs = lhs;
      AST *rhs_node = AST__new(ASTType__Identifier, rhs->span);
      rhs_node->u.ident.name = rhs->text;
      node->u.member.rhs = rhs_node;
    } break;
    case TokenType__Minus: {
      Token *op = Parser__consume(this, TokenType__Minus);
      AST *expr = Parser__parse_factor(this, end_type);
      node = AST__new_unop(ASTType__UnaryMinus, Span__join(op->span, expr->span), expr);
    } break;
    case TokenType__Not: {
      Token *op = Parser__consume(this, TokenType__Not);
      AST *expr = Parser__parse_factor(this, end_type);
      node = AST__new_unop(ASTType__Not, Span__join(op->span, expr->span), expr);
    } break;
    case TokenType__Ampersand: {
      Token *op = Parser__consume(this, TokenType__Ampersand);
      AST *expr = Parser__parse_factor(this, end_type);
      node = AST__new_unop(ASTType__Address, Span__join(op->span, expr->span), expr);
    } break;
    case TokenType__Star: {
      Token *op = Parser__consume(this, TokenType__Star);
      AST *expr = Parser__parse_factor(this, end_type);
      node = AST__new_unop(ASTType__Dereference, Span__join(op->span, expr->span), expr);
    } break;
    case TokenType__Identifier: {
      Token *op = Parser__consume(this, TokenType__Identifier);
      node = AST__new(ASTType__Identifier, op->span);
      node->u.ident.name = op->text;
    } break;
    case TokenType__OpenParen: {
      Token *open = Parser__consume(this, TokenType__OpenParen);
      node = Parser__parse_expression(this, TokenType__CloseParen);
      Token *close = Parser__consume(this, TokenType__CloseParen);
      node->span = Span__join(open->span, close->span);
    } break;
    case TokenType__SizeOf: {
      Token *start = Parser__consume(this, TokenType__SizeOf);
      Parser__consume(this, TokenType__OpenParen);
      Type *type = Parser__parse_type(this);
      Token *close = Parser__consume(this, TokenType__CloseParen);
      node = AST__new(ASTType__SizeOf, Span__join(start->span, close->span));
      node->u.size_of_type = type;
    } break;
    case TokenType__Match: {
      node = Parser__parse_match(this);
    } break;
    case TokenType__If: {
      node = Parser__parse_if(this);
    } break;
    default: {
      Parser__unhandled_type(this, "parse_expression");
    } break;
  }
  bool running = true;
  while (running) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    switch (Parser__token(this)->type) {
      case TokenType__OpenParen: {
        Parser__consume(this, TokenType__OpenParen);
        Vector *args = Vector__new();
        while ((!Parser__token_is(this, TokenType__CloseParen))) {
          AST *label = ((AST *)NULL);
          AST *expr = Parser__parse_expression(this, TokenType__Comma);
          if ((expr->type == ASTType__Identifier && Parser__token_is(this, TokenType__Colon))) {
            Parser__consume(this, TokenType__Colon);
            label = expr;
            expr = Parser__parse_expression(this, TokenType__Comma);
          } 
          Vector__push(args, Argument__new(label, expr));
          if ((!Parser__token_is(this, TokenType__CloseParen))) {
            Parser__consume(this, TokenType__Comma);
          } 
        } 
        Token *end = Parser__consume(this, TokenType__CloseParen);
        ASTType call_type = ASTType__Call;
        AST *call = AST__new(call_type, Span__join(node->span, end->span));
        call->u.call.callee = node;
        call->u.call.args = args;
        call->u.call.added_method_arg = false;
        node = call;
      } break;
      case TokenType__OpenSquare: {
        Parser__consume(this, TokenType__OpenSquare);
        AST *index = Parser__parse_expression(this, TokenType__CloseSquare);
        Parser__consume(this, TokenType__CloseSquare);
        node = AST__new_binop(ASTType__Index, node, index);
      } break;
      case TokenType__Dot: {
        Parser__consume(this, TokenType__Dot);
        Token *name = Parser__consume(this, TokenType__Identifier);
        AST *member = AST__new(ASTType__Member, Span__join(node->span, name->span));
        AST *rhs = AST__new(ASTType__Identifier, name->span);
        rhs->u.ident.name = name->text;
        member->u.member.lhs = node;
        member->u.member.rhs = rhs;
        node = member;
      } break;
      case TokenType__ColonColon: {
        Parser__consume(this, TokenType__ColonColon);
        Token *name = Parser__consume(this, TokenType__Identifier);
        AST *member = AST__new(ASTType__ScopeLookup, Span__join(node->span, name->span));
        AST *rhs = AST__new(ASTType__Identifier, name->span);
        rhs->u.ident.name = name->text;
        member->u.member.lhs = node;
        member->u.member.rhs = rhs;
        node = member;
      } break;
      case TokenType__As: {
        Parser__consume(this, TokenType__As);
        Type *type = Parser__parse_type(this);
        AST *cast = AST__new(ASTType__Cast, Span__join(node->span, type->span));
        cast->u.cast.lhs = node;
        cast->u.cast.to = type;
        node = cast;
      } break;
      case TokenType__Question: {
        Parser__consume(this, TokenType__Question);
        node = AST__new_unop(ASTType__IsNotNull, Span__join(node->span, Parser__token(this)->span), node);
      } break;
      default: {
        running = false;
      } break;
    }
  } 
  return node;
}

AST *Parser__parse_term(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_factor(this, end_type);
  while (((Parser__token_is(this, TokenType__Star) || Parser__token_is(this, TokenType__Slash)) || Parser__token_is(this, TokenType__Percent))) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_factor(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_additive(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_term(this, end_type);
  while ((Parser__token_is(this, TokenType__Plus) || Parser__token_is(this, TokenType__Minus))) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_term(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_shift(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_additive(this, end_type);
  while ((Parser__token_is(this, TokenType__LessThanLessThan) || Parser__token_is(this, TokenType__GreaterThanGreaterThan))) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_additive(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_bw_and(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_shift(this, end_type);
  while (Parser__token_is(this, TokenType__Ampersand)) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_shift(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_bw_xor(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_bw_and(this, end_type);
  while (Parser__token_is(this, TokenType__Caret)) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_bw_and(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_bw_or(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_bw_xor(this, end_type);
  while (Parser__token_is(this, TokenType__Line)) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_bw_xor(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_relational(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_bw_or(this, end_type);
  while ((((((Parser__token_is(this, TokenType__LessThan) || Parser__token_is(this, TokenType__GreaterThan)) || Parser__token_is(this, TokenType__LessThanEquals)) || Parser__token_is(this, TokenType__GreaterThanEquals)) || Parser__token_is(this, TokenType__EqualEquals)) || Parser__token_is(this, TokenType__NotEquals))) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_bw_or(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_logical_and(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_relational(this, end_type);
  while (Parser__token_is(this, TokenType__And)) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_relational(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_logical_or(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_logical_and(this, end_type);
  while (Parser__token_is(this, TokenType__Or)) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_logical_and(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_expression(Parser *this, TokenType end_type) {
  AST *lhs = Parser__parse_logical_or(this, end_type);
  while (((((Parser__token_is(this, TokenType__Equals) || Parser__token_is(this, TokenType__PlusEquals)) || Parser__token_is(this, TokenType__MinusEquals)) || Parser__token_is(this, TokenType__StarEquals)) || Parser__token_is(this, TokenType__SlashEquals))) {
    if (Parser__token_is(this, end_type)) 
    break;
    
    ASTType op = ASTType__from_token(Parser__token(this)->type);
    this->curr += 1;
    AST *rhs = Parser__parse_expression(this, end_type);
    lhs = AST__new_binop(op, lhs, rhs);
  } 
  return lhs;
}

AST *Parser__parse_match(Parser *this) {
  Token *op = Parser__consume(this, TokenType__Match);
  AST *expr = Parser__parse_expression(this, TokenType__OpenCurly);
  AST *node = AST__new(ASTType__Match, Span__join(op->span, expr->span));
  node->u.match_stmt.expr = expr;
  Vector *cases = Vector__new();
  Parser__consume(this, TokenType__OpenCurly);
  while ((!Parser__token_is(this, TokenType__CloseCurly))) {
    if (Parser__token_is(this, TokenType__Else)) {
      node->u.match_stmt.defolt_span = Parser__token(this)->span;
      Parser__consume(this, TokenType__Else);
      Parser__consume(this, TokenType__FatArrow);
      node->u.match_stmt.defolt = Parser__parse_statement(this);
    }  else {
      AST *cond = Parser__parse_factor(this, TokenType__Line);
      AST *body = ((AST *)NULL);
      if ((!Parser__consume_if(this, TokenType__Line))) {
        Parser__consume(this, TokenType__FatArrow);
        body = Parser__parse_statement(this);
        if ((!Parser__token_is(this, TokenType__CloseCurly))) {
          Parser__consume_newline_or(this, TokenType__Comma);
        } 
      } 
      MatchCase *_case = MatchCase__new(cond, body);
      Vector__push(cases, _case);
    } 
  } 
  node->span = Span__join(op->span, Parser__token(this)->span);
  Parser__consume(this, TokenType__CloseCurly);
  node->u.match_stmt.cases = cases;
  return node;
}

AST *Parser__parse_if(Parser *this) {
  Span start_span = Parser__token(this)->span;
  Parser__consume(this, TokenType__If);
  AST *cond = Parser__parse_expression(this, TokenType__Newline);
  Parser__consume_if(this, TokenType__Then);
  AST *body = Parser__parse_statement(this);
  Span end_span = body->span;
  AST *els = ((AST *)NULL);
  if (Parser__consume_if(this, TokenType__Else)) {
    els = Parser__parse_statement(this);
    end_span = els->span;
  } 
  AST *node = AST__new(ASTType__If, Span__join(start_span, end_span));
  node->u.if_stmt.cond = cond;
  node->u.if_stmt.body = body;
  node->u.if_stmt.els = els;
  return node;
}

void Parser__consume_end_of_statement(Parser *this) {
  if (Parser__token_is(this, TokenType__CloseCurly)) 
  return;
  
  Parser__consume_newline_or(this, TokenType__Semicolon);
}

AST *Parser__parse_statement(Parser *this) {
  AST *node = ((AST *)NULL);
  Span start_span = Parser__token(this)->span;
  switch (Parser__token(this)->type) {
    case TokenType__Match: {
      node = Parser__parse_match(this);
    } break;
    case TokenType__If: {
      node = Parser__parse_if(this);
    } break;
    case TokenType__OpenCurly: {
      node = Parser__parse_block(this);
    } break;
    case TokenType__Return: {
      Parser__consume(this, TokenType__Return);
      AST *expr = ((AST *)NULL);
      if ((!Parser__token(this)->seen_newline)) {
        expr = Parser__parse_expression(this, TokenType__Newline);
      } 
      node = AST__new_unop(ASTType__Return, Span__join(start_span, Parser__token(this)->span), expr);
      Parser__consume_end_of_statement(this);
    } break;
    case TokenType__Break: {
      node = AST__new(ASTType__Break, start_span);
      Parser__consume(this, TokenType__Break);
      Parser__consume_end_of_statement(this);
    } break;
    case TokenType__Continue: {
      Parser__consume(this, TokenType__Continue);
      node = AST__new(ASTType__Continue, start_span);
      Parser__consume_end_of_statement(this);
    } break;
    case TokenType__Defer: {
      Parser__consume(this, TokenType__Defer);
      AST *expr = Parser__parse_statement(this);
      node = AST__new_unop(ASTType__Defer, Span__join(start_span, expr->span), expr);
    } break;
    case TokenType__Yield: {
      Parser__consume(this, TokenType__Yield);
      AST *expr = Parser__parse_expression(this, TokenType__Newline);
      node = AST__new_unop(ASTType__Yield, Span__join(start_span, expr->span), expr);
      Parser__consume_end_of_statement(this);
    } break;
    case TokenType__While: {
      Parser__consume(this, TokenType__While);
      AST *cond = Parser__parse_expression(this, TokenType__Newline);
      AST *body = Parser__parse_statement(this);
      node = AST__new(ASTType__While, Span__join(start_span, body->span));
      node->u.loop.cond = cond;
      node->u.loop.body = body;
    } break;
    case TokenType__For: {
      node = AST__new(ASTType__For, Parser__token(this)->span);
      Parser__consume(this, TokenType__For);
      if ((!Parser__token_is(this, TokenType__Semicolon))) {
        AST *init = Parser__parse_statement(this);
        if (((init->type != ASTType__Assignment) && (init->type != ASTType__VarDeclaration))) {
          error_span(init->span, "Invalid for loop initializer");
        } 
        node->u.loop.init = init;
        Token *prev = ((Token *)Vector__at(this->tokens, (this->curr - 1)));
        if (prev->type == TokenType__Semicolon) {
          this->curr -= 1;
        } 
      } 
      Parser__consume(this, TokenType__Semicolon);
      if ((!Parser__token_is(this, TokenType__Semicolon))) 
      node->u.loop.cond = Parser__parse_expression(this, TokenType__Semicolon);
      
      Parser__consume(this, TokenType__Semicolon);
      if ((!Parser__token_is(this, TokenType__CloseCurly))) 
      node->u.loop.incr = Parser__parse_expression(this, TokenType__Newline);
      
      node->u.loop.body = Parser__parse_statement(this);
      node->span = Span__join(node->span, node->u.loop.body->span);
    } break;
    case TokenType__Let: {
      Parser__consume(this, TokenType__Let);
      Token *name = Parser__consume(this, TokenType__Identifier);
      Span end_span = name->span;
      Type *type = ((Type *)NULL);
      if (Parser__consume_if(this, TokenType__Colon)) {
        type = Parser__parse_type(this);
        end_span = type->span;
      } 
      AST *init = ((AST *)NULL);
      if (Parser__consume_if(this, TokenType__Equals)) {
        init = Parser__parse_expression(this, TokenType__Newline);
        end_span = init->span;
      } 
      Parser__consume_end_of_statement(this);
      node = AST__new(ASTType__VarDeclaration, Span__join(start_span, end_span));
      node->u.var_decl.var = Variable__new(name->text, type, name->span);
      node->u.var_decl.init = init;
    } break;
    default: {
      node = Parser__parse_expression(this, TokenType__Newline);
      Parser__consume_if(this, TokenType__Semicolon);
    } break;
  }
  return node;
}

AST *Parser__parse_block(Parser *this) {
  AST *node = AST__new(ASTType__Block, Parser__token(this)->span);
  Parser__consume(this, TokenType__OpenCurly);
  Vector *statements = Vector__new();
  while ((!Parser__token_is(this, TokenType__CloseCurly))) {
    AST *statement = Parser__parse_statement(this);
    Vector__push(statements, statement);
  } 
  node->u.block.statements = statements;
  Parser__consume(this, TokenType__CloseCurly);
  return node;
}

Function *Parser__parse_function(Parser *this) {
  Parser__consume(this, TokenType__Def);
  Type *struct_type = ((Type *)NULL);
  char *struct_name = ((char *)NULL);
  bool is_method = false;
  bool is_static = false;
  Token *next_token = ((Token *)Vector__at(this->tokens, (this->curr + 1)));
  if (next_token->type == TokenType__ColonColon) {
    struct_type = Parser__parse_type(this);
    if ((!((bool)struct_type->name))) {
      error_span(struct_type->span, "Invalid type in method declaration");
    } 
    struct_name = struct_type->name;
    is_method = true;
    Parser__consume(this, TokenType__ColonColon);
  } 
  Token *name = Parser__consume(this, TokenType__Identifier);
  Function *func = Function__new(name->span);
  func->name = name->text;
  func->is_method = is_method;
  func->method_struct_name = struct_name;
  Parser__consume(this, TokenType__OpenParen);
  while ((!Parser__token_is(this, TokenType__CloseParen))) {
    bool found_amp = Parser__consume_if(this, TokenType__Ampersand);
    Token *var_name = Parser__consume(this, TokenType__Identifier);
    Type *type = ((Type *)NULL);
    if ((Vector__empty(func->params) && is_method)) {
      if (string__eq(var_name->text, "this")) {
        type = struct_type;
        if (found_amp) {
          type = Type__new_link(BaseType__Pointer, type, name->span);
        } 
      }  else       if (found_amp) {
        error_span(var_name->span, "Expected 'this' over here");
      }  else {
        is_static = true;
      } 
      
    } 
    if ((!((bool)type))) {
      Parser__consume(this, TokenType__Colon);
      type = Parser__parse_type(this);
    } 
    Variable *var = Variable__new(var_name->text, type, var_name->span);
    Vector__push(func->params, var);
    if ((!Parser__token_is(this, TokenType__CloseParen))) {
      Parser__consume(this, TokenType__Comma);
    } 
  } 
  Parser__consume(this, TokenType__CloseParen);
  if ((is_method && func->params->size == 0)) {
    is_static = true;
  } 
  func->is_static = is_static;
  if (Parser__consume_if(this, TokenType__Colon)) {
    func->return_type = Parser__parse_type(this);
  }  else   if (string__eq(name->text, "main")) {
    func->return_type = Type__new(BaseType__I32, name->span);
  }  else {
    func->return_type = Type__new(BaseType__Void, name->span);
    if ((Parser__token_is(this, TokenType__Identifier) && string__eq(Parser__token(this)->text, "exits"))) {
      Parser__consume(this, TokenType__Identifier);
      func->exits = true;
    } 
  } 
  
  if (Parser__consume_if(this, TokenType__Extern)) {
    func->is_extern = true;
    func->extern_name = func->name;
    if (Parser__consume_if(this, TokenType__OpenParen)) {
      Token *name = Parser__consume(this, TokenType__StringLiteral);
      func->extern_name = name->text;
      Parser__consume(this, TokenType__CloseParen);
    } 
  }  else   if (Parser__consume_if(this, TokenType__FatArrow)) {
    func->is_arrow = true;
    AST *expr = Parser__parse_expression(this, TokenType__Newline);
    if ((!Parser__token(this)->seen_newline)) {
      error_loc(expr->span.end, "Expected newline after arrow function");
    } 
    AST *ret_stmt = AST__new_unop(ASTType__Return, expr->span, expr);
    func->body = ret_stmt;
  }  else {
    func->is_extern = false;
    func->body = Parser__parse_block(this);
  } 
  
  return func;
}

Structure *Parser__parse_enum(Parser *this) {
  Span start_span = Parser__consume(this, TokenType__Enum)->span;
  Token *name = Parser__consume(this, TokenType__Identifier);
  Structure *enum_def = Structure__new(Span__join(start_span, name->span));
  enum_def->name = name->text;
  enum_def->is_enum = true;
  Type *type = Type__new(BaseType__Structure, name->span);
  enum_def->type = type;
  type->name = name->text;
  type->struct_def = enum_def;
  if (Parser__consume_if(this, TokenType__Extern)) {
    enum_def->is_extern = true;
    enum_def->extern_name = enum_def->name;
    if (Parser__consume_if(this, TokenType__OpenParen)) {
      Token *name = Parser__consume(this, TokenType__StringLiteral);
      enum_def->extern_name = name->text;
      Parser__consume(this, TokenType__CloseParen);
    } 
  } 
  Parser__consume(this, TokenType__OpenCurly);
  while ((!Parser__token_is(this, TokenType__CloseCurly))) {
    Token *name = Parser__consume(this, TokenType__Identifier);
    Type *type = Type__new(BaseType__I32, name->span);
    Variable *var = Variable__new(name->text, type, name->span);
    if (enum_def->is_extern) {
      Parser__consume(this, TokenType__Equals);
      Parser__consume(this, TokenType__Extern);
      Parser__consume(this, TokenType__OpenParen);
      Token *name = Parser__consume(this, TokenType__StringLiteral);
      Parser__consume(this, TokenType__CloseParen);
      var->extern_name = name->text;
      var->is_extern = true;
    } 
    Vector__push(enum_def->fields, var);
    if ((!Parser__token_is(this, TokenType__CloseCurly))) {
      Parser__consume_newline_or(this, TokenType__Comma);
    } 
  } 
  Parser__consume(this, TokenType__CloseCurly);
  return enum_def;
}

Structure *Parser__parse_struct(Parser *this) {
  bool is_union = false;
  Span start_span = Parser__token(this)->span;
  if (Parser__consume_if(this, TokenType__Union)) {
    is_union = true;
  }  else {
    Parser__consume(this, TokenType__Struct);
  } 
  Token *name = Parser__consume(this, TokenType__Identifier);
  Structure *struc = Structure__new(Span__join(start_span, name->span));
  struc->name = name->text;
  if (Parser__consume_if(this, TokenType__Extern)) {
    struc->is_extern = true;
    struc->extern_name = struc->name;
    if (Parser__consume_if(this, TokenType__OpenParen)) {
      Token *name = Parser__consume(this, TokenType__StringLiteral);
      struc->extern_name = name->text;
      Parser__consume(this, TokenType__CloseParen);
    } 
  } 
  if (((!struc->is_extern) || Parser__token_is(this, TokenType__OpenCurly))) {
    Parser__consume(this, TokenType__OpenCurly);
    while ((!Parser__token_is(this, TokenType__CloseCurly))) {
      Token *name = Parser__consume(this, TokenType__Identifier);
      Parser__consume(this, TokenType__Colon);
      Type *type = Parser__parse_type(this);
      Variable *var = Variable__new(name->text, type, Span__join(name->span, type->span));
      Vector__push(struc->fields, var);
      if ((!Parser__token_is(this, TokenType__CloseCurly))) {
        Parser__consume_newline_or(this, TokenType__Comma);
      } 
    } 
    Parser__consume(this, TokenType__CloseCurly);
  } 
  Type *type = Type__new(BaseType__Structure, name->span);
  type->name = name->text;
  struc->type = type;
  type->struct_def = struc;
  struc->is_union = is_union;
  return struc;
}

AST *Parser__parse_global_var(Parser *this) {
  Span start_span = Parser__consume(this, TokenType__Let)->span;
  AST *node = AST__new(ASTType__VarDeclaration, Parser__token(this)->span);
  Token *name = Parser__consume(this, TokenType__Identifier);
  Type *type = ((Type *)NULL);
  if (Parser__consume_if(this, TokenType__Colon)) {
    type = Parser__parse_type(this);
  } 
  Variable *var = Variable__new(name->text, type, Span__join(start_span, Parser__token(this)->span));
  node->u.var_decl.var = var;
  if (Parser__consume_if(this, TokenType__Extern)) {
    var->is_extern = true;
    if (Parser__consume_if(this, TokenType__OpenParen)) {
      Token *name = Parser__consume(this, TokenType__StringLiteral);
      var->extern_name = name->text;
      Parser__consume(this, TokenType__CloseParen);
    }  else {
      var->extern_name = var->name;
    } 
    if ((!((bool)type))) {
      error_span(name->span, "Extern variables must have a type");
    } 
  }  else {
    var->is_extern = false;
    if (Parser__consume_if(this, TokenType__Equals)) {
      node->u.var_decl.init = Parser__parse_expression(this, TokenType__Newline);
    } 
  } 
  Parser__consume_newline_or(this, TokenType__Semicolon);
  return node;
}

char *Parser__find_file_path(Parser *this, char *filename) {
  if (string__starts_with(filename, "/")) 
  return filename;
  
  if (string__starts_with(filename, "@/")) {
    char *file_path = format_string("%s/%s", this->project_root, (filename + 2));
    if (File__exists(file_path)) 
    return file_path;
    
    free(file_path);
  }  else {
    for (i32 i = 0; (i < this->include_dirs->size); i += 1) {
      char *dir = ((char *)Vector__at(this->include_dirs, i));
      char *file_path = format_string("%s/%s", dir, filename);
      if (File__exists(file_path)) 
      return file_path;
      
      free(file_path);
    } 
  } 
  if (this->curr == 0) {
    printf("---------------------------------------------------------------" "\n");
    printf("[-] Error: Could not find file '%s'" "\n", filename);
    printf("[+] Hint: Specify the aecor root directory with the -l option" "\n");
    printf("---------------------------------------------------------------" "\n");
    exit(1);
  } 
  this->curr -= 1;
  error_span(Parser__token(this)->span, format_string("Could not find file: %s", filename));
}

void Parser__include_file(Parser *this, Program *program, char *filename) {
  filename = Parser__find_file_path(this, filename);
  if (Program__is_file_included(program, filename)) 
  return;
  
  Program__add_included_file(program, filename);
  FILE *file = File__open(filename, "r");
  char *contents = File__slurp(file);
  Lexer lexer = Lexer__make(contents, filename);
  Vector *tokens = Lexer__lex((&lexer));
  Parser__push_context(this, tokens);
  Parser__parse_into_program(this, program);
  Parser__pop_context(this);
}

void Parser__parse_use(Parser *this, Program *program) {
  Parser__consume(this, TokenType__Use);
  Token *name = Parser__consume(this, TokenType__StringLiteral);
  Parser__consume_newline_or(this, TokenType__Semicolon);
  Parser__include_file(this, program, name->text);
}

void Parser__parse_compiler_option(Parser *this, Program *program) {
  Parser__consume(this, TokenType__AtSign);
  Token *compiler = Parser__consume(this, TokenType__Identifier);
  if ((!string__eq(compiler->text, "compiler"))) {
    error_span(compiler->span, "Expected 'compiler'");
  } 
  Token *name = Parser__consume(this, TokenType__Identifier);
  {
    char *__match_str = name->text;
    if (!strcmp(__match_str, "c_include")) {
      Token *filename = Parser__consume(this, TokenType__StringLiteral);
      Vector__push(program->c_includes, filename->text);
    } else if (!strcmp(__match_str, "c_flag")) {
      Token *flag = Parser__consume(this, TokenType__StringLiteral);
      Vector__push(program->c_flags, flag->text);
    } else if (!strcmp(__match_str, "c_embed_header")) {
      Token *filename = Parser__consume(this, TokenType__StringLiteral);
      char *resolved = Parser__find_file_path(this, filename->text);
      Vector__push(program->c_embed_headers, resolved);
    } else  {
      error_span(name->span, "Unknown compiler option");
    }
  }
}

void Parser__parse_into_program(Parser *this, Program *program) {
  while ((!Parser__token_is(this, TokenType__EOF))) {
    switch (Parser__token(this)->type) {
      case TokenType__Use: {
        Parser__parse_use(this, program);
      } break;
      case TokenType__AtSign: {
        Parser__parse_compiler_option(this, program);
      } break;
      case TokenType__Def: {
        Function *func = Parser__parse_function(this);
        Vector__push(program->functions, func);
      } break;
      case TokenType__Let: {
        AST *node = Parser__parse_global_var(this);
        Vector__push(program->global_vars, node);
      } break;
      case TokenType__Struct:
      case TokenType__Union: {
        Structure *structure = Parser__parse_struct(this);
        Vector__push(program->structures, structure);
      } break;
      case TokenType__Enum: {
        Structure *structure = Parser__parse_enum(this);
        Vector__push(program->structures, structure);
      } break;
      default: {
        Parser__unhandled_type(this, "parse_program");
      } break;
    }
  } 
}

Program *Parser__parse_program(Parser *this) {
  Program *program = Program__new();
  Parser__include_file(this, program, "lib/prelude.ae");
  Parser__parse_into_program(this, program);
  return program;
}

TypeChecker *TypeChecker__new(void) {
  TypeChecker *checker = ((TypeChecker *)calloc(1, sizeof(TypeChecker)));
  checker->scopes = Vector__new();
  checker->functions = Map__new();
  checker->structures = Map__new();
  checker->methods = Map__new();
  Map__insert(checker->methods, "string", Map__new());
  return checker;
}

void TypeChecker__push_scope(TypeChecker *this) {
  Map *scope = Map__new();
  Vector__push(this->scopes, scope);
}

Map *TypeChecker__scope(TypeChecker *this) {
  return Vector__back(this->scopes);
}

void TypeChecker__pop_scope(TypeChecker *this) {
  Map *scope = ((Map *)Vector__pop(this->scopes));
  Map__free(scope);
}

void TypeChecker__push_var(TypeChecker *this, Variable *var) {
  Map *scope = TypeChecker__scope(this);
  Variable *existing = ((Variable *)Map__get(scope, var->name));
  if (((bool)existing)) {
    error_span_note_span(var->span, "Variable is already defined in scope", existing->span, "Previous definition here");
  } 
  Map__insert(TypeChecker__scope(this), var->name, var);
}

Variable *TypeChecker__find_var(TypeChecker *this, char *name) {
  for (i32 i = (this->scopes->size - 1); (i >= 0); i -= 1) {
    Map *scope = ((Map *)Vector__at(this->scopes, i));
    Variable *var = ((Variable *)Map__get(scope, name));
    if (((bool)var)) 
    return var;
    
  } 
  return NULL;
}

Variable *TypeChecker__get_struct_member(TypeChecker *this, char *lhs, char *rhs) {
  Structure *struc = ((Structure *)Map__get(this->structures, lhs));
  return (((bool)struc) ? Structure__find_field(struc, rhs) : NULL);
}

bool TypeChecker__type_is_valid(TypeChecker *this, Type *type) {
  switch (type->base) {
    case BaseType__Pointer: {
      return TypeChecker__type_is_valid(this, type->ptr);
    } break;
    case BaseType__Function:
    case BaseType__Method: {
      for (i32 i = 0; (i < type->params->size); i += 1) {
        Variable *var = ((Variable *)Vector__at(type->params, i));
        if ((!TypeChecker__type_is_valid(this, var->type))) {
          return false;
        } 
      } 
      return TypeChecker__type_is_valid(this, type->return_type);
    } break;
    case BaseType__Structure: {
      Structure *struc = ((Structure *)Map__get(this->structures, type->name));
      if (((bool)struc)) {
        type->struct_def = struc;
        return true;
      } 
      return false;
    } break;
    case BaseType__Array: {
      Type *expr_type = TypeChecker__check_expression(this, type->size_expr);
      return (TypeChecker__type_is_valid(this, type->ptr) && TypeChecker__type_is_valid(this, expr_type));
    } break;
    default: {
      return true;
    } break;
  }
}

void TypeChecker__check_method_call(TypeChecker *this, Type *method_type, AST *node) {
  AST *callee = node->u.call.callee;
  if (((callee->type != ASTType__Member) && (callee->type != ASTType__ScopeLookup))) {
    error_span(callee->span, "Method call is not to a member, internal compiler error");
  } 
  Map *s_methods = ((Map *)Map__get(this->methods, method_type->name));
  AST *rhs = callee->u.member.rhs;
  Function *method = ((Function *)Map__get(s_methods, rhs->u.ident.name));
  node->u.call.func = method;
  if (node->u.call.added_method_arg) 
  return;
  
  node->u.call.added_method_arg = true;
  if ((callee->type != ASTType__Member)) 
  return;
  
  if (method->params->size == 0) {
    error_span(callee->span, "Instance method should have `this` argument, internal error");
  } 
  Type *method_param = ((Variable *)Vector__at(method->params, 0))->type;
  Member member = callee->u.member;
  AST *first_arg = member.lhs;
  if ((member.is_pointer && ((bool)method_param->name))) {
    first_arg = AST__new_unop(ASTType__Dereference, first_arg->span, first_arg);
  }  else   if (((!member.is_pointer) && (!((bool)method_param->name)))) {
    first_arg = AST__new_unop(ASTType__Address, first_arg->span, first_arg);
  } 
  
  Vector__push_front(node->u.call.args, Argument__new(NULL, first_arg));
}

Type *TypeChecker__check_constructor(TypeChecker *this, Structure *struc, AST *node) {
  if ((struc->is_enum || struc->is_union)) {
    error_span(node->span, "Cannot use constructor for enum or union");
  } 
  Vector *args = node->u.call.args;
  node->type = ASTType__Constructor;
  node->u.constructor.struc = struc;
  node->u.constructor.args = args;
  Vector *fields = struc->fields;
  if ((fields->size != args->size)) {
    error_span_note_span(node->span, "Constructor has wrong number of arguments", struc->span, format_string("Struct has %d fields, but got %d arguments", fields->size, args->size));
  } 
  for (i32 i = 0; (i < fields->size); i += 1) {
    Variable *field = ((Variable *)Vector__at(fields, i));
    Argument *arg = ((Argument *)Vector__at(args, i));
    Type *arg_type = TypeChecker__check_expression(this, arg->expr);
    if (((bool)arg->label)) {
      char *label = arg->label->u.ident.name;
      if ((!string__eq(label, field->name))) {
        error_span_note_span(arg->label->span, "Label on parameter does not match struct field", field->span, format_string("Expected '%s', got '%s'", field->name, label));
      } 
    } 
    if ((!Type__eq(field->type, arg_type))) {
      error_span_note_span(arg->expr->span, "Argument type does not match struct field", field->span, format_string("Expected '%s', got '%s'", Type__str(field->type), Type__str(arg_type)));
    } 
  } 
  return struc->type;
}

Type *TypeChecker__check_call(TypeChecker *this, AST *node) {
  AST *callee = node->u.call.callee;
  if (callee->type == ASTType__Identifier) {
    callee->u.ident.is_function = false;
    char *name = callee->u.ident.name;
    if ((string__eq(name, "print") || string__eq(name, "println"))) {
      for (i32 i = 0; (i < node->u.call.args->size); i += 1) {
        Argument *arg = ((Argument *)Vector__at(node->u.call.args, i));
        TypeChecker__check_expression(this, arg->expr);
      } 
      return Type__new(BaseType__Void, node->span);
    } 
    Structure *struc = ((Structure *)Map__get(this->structures, name));
    if (((bool)struc)) {
      return TypeChecker__check_constructor(this, struc, node);
    } 
  } 
  Type *func_type = TypeChecker__check_expression(this, callee);
  Function *func_def = func_type->func_def;
  node->u.call.func = func_def;
  if ((((bool)func_def) && func_def->exits)) {
    node->returns = true;
  } 
  if (((func_type->base != BaseType__Function) && (func_type->base != BaseType__Method))) {
    error_span_note(callee->span, "Cannot call a non-function type", format_string("Type for expression is '%s'", Type__str(func_type)));
  } 
  if (func_type->base == BaseType__Method) {
    TypeChecker__check_method_call(this, func_type, node);
  } 
  Vector *params = func_type->params;
  if ((params->size != node->u.call.args->size)) {
    error_span_note_span(node->span, "Number of arguments does not match function signature", func_type->span, format_string("This function expects %d arguments, got %d", params->size, node->u.call.args->size));
  } 
  for (i32 i = 0; (i < params->size); i += 1) {
    Variable *param = ((Variable *)Vector__at(params, i));
    Argument *arg = ((Argument *)Vector__at(node->u.call.args, i));
    Type *arg_type = TypeChecker__check_expression(this, arg->expr);
    if ((!Type__eq(param->type, arg_type))) {
      error_span_note_span(arg->expr->span, "Argument type does not match function parameter type", param->span, format_string("Expected '%s', got '%s'", Type__str(param->type), Type__str(arg_type)));
    } 
    if ((!((bool)arg->label))) 
    continue;
    
    if (string__eq(param->name, "")) {
      error_span_note_span(arg->label->span, "Label on non-labeled parameter", param->span, "This parameter does not have a name");
    } 
    char *label = arg->label->u.ident.name;
    if ((!string__eq(label, param->name))) {
      error_span_note_span(arg->label->span, "Label on parameter does not match parameter name", param->span, format_string("Expected '%s', got '%s'", param->name, label));
    } 
  } 
  return func_type->return_type;
}

Type *TypeChecker__check_format_string(TypeChecker *this, AST *node) {
  Vector *parts = node->u.fmt_str.parts;
  Vector *exprs = node->u.fmt_str.exprs;
  if ((parts->size != (exprs->size + 1))) {
    error_span(node->span, "Number of format string parts does not match number of expressions");
  } 
  for (i32 i = 0; (i < exprs->size); i += 1) {
    TypeChecker__check_expression(this, Vector__at(exprs, i));
  } 
  return Type__ptr_to(BaseType__Char, node->span);
}

Type *TypeChecker__check_pointer_arith(TypeChecker *this, AST *node, Type *lhs, Type *rhs) {
  if ((node->type == ASTType__Plus || node->type == ASTType__Minus)) {
    if ((lhs->base == BaseType__Pointer && Type__is_integer(rhs))) {
      return lhs;
    } 
    if ((Type__is_integer(lhs) && rhs->base == BaseType__Pointer)) {
      return rhs;
    } 
    if ((Type__eq(lhs, rhs) && lhs->base == BaseType__Pointer)) {
      if (node->type == ASTType__Minus) {
        return Type__new(BaseType__I32, node->span);
      } 
    } 
  } 
  error_span(node->span, "Invalid pointer arithmetic");
  return ((Type *)NULL);
}

Type *TypeChecker__check_binary_op(TypeChecker *this, AST *node, AST *_lhs, AST *_rhs) {
  if (((!((bool)_lhs->etype)) || (!((bool)_rhs->etype)))) {
    panic("Expressions not checked in check_binary_op");
  } 
  Type *lhs = _lhs->etype;
  Type *rhs = _rhs->etype;
  switch (node->type) {
    case ASTType__Plus:
    case ASTType__Minus:
    case ASTType__Multiply:
    case ASTType__Divide: {
      if ((lhs->base == BaseType__Pointer || rhs->base == BaseType__Pointer)) {
        return TypeChecker__check_pointer_arith(this, node, lhs, rhs);
      }  else       if (((!Type__is_numeric(lhs)) || (!Type__is_numeric(rhs)))) {
        error_span_note(node->span, "Operator requires numeric types", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      }  else       if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Operands must be of the same type", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      }  else {
        return lhs;
      } 
      
      
    } break;
    case ASTType__LessThan:
    case ASTType__LessThanEquals:
    case ASTType__GreaterThan:
    case ASTType__GreaterThanEquals: {
      if (((!Type__is_numeric(lhs)) || (!Type__is_numeric(rhs)))) {
        error_span_note(node->span, "Operator requires numeric types", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Operands must be of the same type", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      return Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__Equals:
    case ASTType__NotEquals: {
      if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Operands must be of the same type", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      if (lhs->base == BaseType__Structure) {
        Structure *struc = ((Structure *)Map__get(this->structures, lhs->name));
        if ((!struc->is_enum)) {
          error_span(node->span, "Cannot compare structs directly");
        } 
      } 
      return Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__And:
    case ASTType__Or: {
      if (((!Type__eq(lhs, rhs)) || (lhs->base != BaseType__Bool))) {
        error_span_note(node->span, "Operands must be boolean", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      return Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__Modulus:
    case ASTType__BitwiseOr:
    case ASTType__BitwiseAnd:
    case ASTType__BitwiseXor:
    case ASTType__LeftShift:
    case ASTType__RightShift: {
      if (((!Type__is_integer(lhs)) || (!Type__is_integer(rhs)))) {
        error_span_note(node->span, "Operator requires integer types", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Operands must be of the same type", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      return lhs;
    } break;
    case ASTType__UnaryMinus: {
      Type *etype = TypeChecker__check_expression(this, node->u.unary);
      if ((!Type__is_numeric(etype))) {
        error_span_note(node->u.unary->span, "Expression must be a number", format_string("Got type '%s'", Type__str(etype)));
      } 
      return etype;
    } break;
    default: {
      panic(format_string("Internal error: unhandled op in check_binary_op: %s", ASTType__str(node->type)));
    } break;
  }
}

__attribute__((noreturn)) void TypeChecker__error_unknown_identifier(TypeChecker *this, Span span, char *name) {
  Vector *options = Vector__new();
  for (i32 i = 0; (i < this->scopes->size); i += 1) {
    Map *scope = ((Map *)Vector__at(this->scopes, i));
    Map__push_keys(scope, options);
  } 
  for (MapIterator iter = Map__iter(this->functions); ((bool)iter.cur); MapIterator__next((&iter))) {
    Function *func = ((Function *)MapIterator__value((&iter)));
    if ((!func->is_method)) {
      Vector__push(options, MapIterator__key((&iter)));
    } 
  } 
  char *suggestion = find_word_suggestion(name, options);
  if (((bool)suggestion)) {
    error_span_note(span, "Unknown Identifier", format_string("Possible alternative: \x1b[32m%s\x1b[0m", suggestion));
  }  else {
    error_span(span, "Unknown Identifier");
  } 
}

void TypeChecker__error_unknown_member(TypeChecker *this, AST *node, Type *struct_type, char *field_name, bool is_static) {
  char *suggestion = ((char *)NULL);
  if (((bool)struct_type->struct_def)) {
    Vector *options = Vector__new();
    if (struct_type->struct_def->is_enum == is_static) {
      Vector *fields = struct_type->struct_def->fields;
      for (i32 i = 0; (i < fields->size); i += 1) {
        Variable *field = ((Variable *)Vector__at(fields, i));
        Vector__push(options, field->name);
      } 
    } 
    Map *s_methods = ((Map *)Map__get(this->methods, struct_type->name));
    if (((bool)s_methods)) {
      for (MapIterator iter = Map__iter(s_methods); ((bool)iter.cur); MapIterator__next((&iter))) {
        Function *method = ((Function *)MapIterator__value((&iter)));
        if (method->is_static == is_static) {
          Vector__push(options, method->name);
        } 
      } 
    } 
    suggestion = find_word_suggestion(field_name, options);
  } 
  if (((bool)suggestion)) {
    error_span_note(node->u.member.rhs->span, format_string("Type '%s' has no member with this name", Type__str(struct_type)), format_string("Possible alternative: \x1b[32m%s\x1b[0m", suggestion));
  }  else {
    error_span(node->u.member.rhs->span, format_string("Type '%s' has no member with this name", Type__str(struct_type)));
  } 
}

Type *TypeChecker__check_expression(TypeChecker *this, AST *node) {
  Type *etype = ((Type *)NULL);
  switch (node->type) {
    case ASTType__Call: {
      etype = TypeChecker__check_call(this, node);
    } break;
    case ASTType__IntLiteral: {
      etype = Type__new(BaseType__I32, node->span);
    } break;
    case ASTType__FloatLiteral: {
      etype = Type__new(BaseType__F32, node->span);
    } break;
    case ASTType__BoolLiteral: {
      etype = Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__StringLiteral: {
      etype = Type__ptr_to(BaseType__Char, node->span);
    } break;
    case ASTType__CharLiteral: {
      etype = Type__new(BaseType__Char, node->span);
    } break;
    case ASTType__Null: {
      etype = Type__ptr_to(BaseType__Void, node->span);
    } break;
    case ASTType__FormatStringLiteral: {
      etype = TypeChecker__check_format_string(this, node);
    } break;
    case ASTType__Constructor: {
      etype = node->etype;
    } break;
    case ASTType__SizeOf: {
      if ((!TypeChecker__type_is_valid(this, node->u.size_of_type))) {
        error_span(node->u.size_of_type->span, "Invalid type");
      } 
      etype = Type__new(BaseType__I32, node->span);
    } break;
    case ASTType__Identifier: {
      Identifier *ident = (&node->u.ident);
      Variable *var = TypeChecker__find_var(this, ident->name);
      Function *func = ((Function *)Map__get(this->functions, ident->name));
      if (ident->is_function) {
        etype = ident->func->type;
      }  else       if (((bool)var)) {
        ident->is_function = false;
        ident->var = var;
        etype = var->type;
      }  else       if (((bool)func)) {
        ident->is_function = true;
        ident->func = func;
        etype = func->type;
      }  else {
        TypeChecker__error_unknown_identifier(this, node->span, ident->name);
      } 
      
      
    } break;
    case ASTType__Plus:
    case ASTType__Minus:
    case ASTType__Multiply:
    case ASTType__Divide:
    case ASTType__LessThan:
    case ASTType__LessThanEquals:
    case ASTType__GreaterThan:
    case ASTType__GreaterThanEquals:
    case ASTType__Equals:
    case ASTType__NotEquals:
    case ASTType__And:
    case ASTType__Or:
    case ASTType__Modulus:
    case ASTType__BitwiseOr:
    case ASTType__BitwiseAnd:
    case ASTType__BitwiseXor:
    case ASTType__LeftShift:
    case ASTType__RightShift: {
      TypeChecker__check_expression(this, node->u.binary.lhs);
      TypeChecker__check_expression(this, node->u.binary.rhs);
      etype = TypeChecker__check_binary_op(this, node, node->u.binary.lhs, node->u.binary.rhs);
    } break;
    case ASTType__Not: {
      Type *rhs = TypeChecker__check_expression(this, node->u.unary);
      if ((rhs->base != BaseType__Bool)) {
        error_span_note(node->u.unary->span, "Expression must be boolean", format_string("Got type '%s'", Type__str(rhs)));
      } 
      etype = Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__UnaryMinus: {
      etype = TypeChecker__check_expression(this, node->u.unary);
      if ((!Type__is_numeric(etype))) {
        error_span_note(node->u.unary->span, "Expression must be a number", format_string("Got type '%s'", Type__str(etype)));
      } 
    } break;
    case ASTType__Address: {
      etype = TypeChecker__check_expression(this, node->u.unary);
      etype = Type__new_link(BaseType__Pointer, etype, node->span);
    } break;
    case ASTType__Dereference: {
      Type *expr_type = TypeChecker__check_expression(this, node->u.unary);
      if ((expr_type->base != BaseType__Pointer)) {
        error_span_note(node->u.unary->span, "Expression must be a pointer-type", format_string("Got type '%s'", Type__str(expr_type)));
      } 
      etype = expr_type->ptr;
    } break;
    case ASTType__Index: {
      Type *expr_type = TypeChecker__check_expression(this, node->u.binary.lhs);
      if ((expr_type->base != BaseType__Pointer)) {
        error_span_note(node->u.binary.lhs->span, "Expression must be a pointer-type", format_string("Got type '%s'", Type__str(expr_type)));
      } 
      Type *index_type = TypeChecker__check_expression(this, node->u.binary.rhs);
      if ((!Type__is_integer(index_type))) {
        error_span_note(node->u.binary.rhs->span, "Index must be an integer", format_string("Got type '%s'", Type__str(index_type)));
      } 
      etype = expr_type->ptr;
    } break;
    case ASTType__PlusEquals:
    case ASTType__MinusEquals:
    case ASTType__DivideEquals:
    case ASTType__MultiplyEquals: {
      Type *lhs = TypeChecker__check_expression(this, node->u.binary.lhs);
      Type *rhs = TypeChecker__check_expression(this, node->u.binary.rhs);
      if ((!AST__is_lvalue(node->u.binary.lhs))) {
        error_span(node->u.binary.lhs->span, "Must be an l-value");
      } 
      if (((!Type__is_numeric(lhs)) || (!Type__is_numeric(rhs)))) {
        error_span_note(node->span, "Operator requires numeric types", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Operands must be of the same type", format_string("Got types '%s' and '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      etype = lhs;
    } break;
    case ASTType__Assignment: {
      Type *lhs = TypeChecker__check_expression(this, node->u.binary.lhs);
      Type *rhs = TypeChecker__check_expression(this, node->u.binary.rhs);
      if ((!AST__is_lvalue(node->u.binary.lhs))) {
        error_span(node->u.binary.lhs->span, "Must be an l-value");
      } 
      if ((!Type__eq(lhs, rhs))) {
        error_span_note(node->span, "Variable type does not match assignment type", format_string("Expected type '%s', got '%s'", Type__str(lhs), Type__str(rhs)));
      } 
      etype = lhs;
    } break;
    case ASTType__ScopeLookup: {
      if ((node->u.member.lhs->type != ASTType__Identifier)) {
        error_span(node->u.member.lhs->span, "Left hand side of `::` must be a struct name");
      } 
      char *struct_name = node->u.member.lhs->u.ident.name;
      Structure *struc = ((Structure *)Map__get(this->structures, struct_name));
      if ((!((bool)struc))) {
        error_span(node->u.member.lhs->span, "Unknown struct with this name");
      } 
      AST *rhs = node->u.member.rhs;
      char *field_name = rhs->u.ident.name;
      Variable *var = TypeChecker__get_struct_member(this, struct_name, field_name);
      Map *s_methods = ((Map *)Map__get(this->methods, struct_name));
      Function *method = ((Function *)Map__get(s_methods, field_name));
      if ((struc->is_enum && ((bool)var))) {
        node->type = ASTType__EnumValue;
        node->u.enum_val.struct_def = struc;
        node->u.enum_val.var = var;
        etype = struc->type;
      }  else       if (((bool)method)) {
        etype = method->type;
      }  else {
        TypeChecker__error_unknown_member(this, node, struc->type, field_name, true);
      } 
      
    } break;
    case ASTType__Member: {
      Type *lhs_type = TypeChecker__check_expression(this, node->u.member.lhs);
      if (((!Type__is_struct_or_ptr(lhs_type)) && (!Type__is_string(lhs_type)))) {
        error_span_note(node->u.member.lhs->span, "LHS of member access must be a struct / string", format_string("Got type '%s'", Type__str(lhs_type)));
      } 
      node->u.member.is_pointer = lhs_type->name == NULL;
      Type *struct_type = (((bool)lhs_type->name) ? lhs_type : lhs_type->ptr);
      AST *rhs = node->u.member.rhs;
      char *struct_name = struct_type->name;
      char *field_name = rhs->u.ident.name;
      Structure *struc = ((Structure *)Map__get(this->structures, struct_name));
      Variable *field = TypeChecker__get_struct_member(this, struct_name, field_name);
      Map *s_methods = ((Map *)Map__get(this->methods, struct_name));
      Function *method = ((Function *)Map__get(s_methods, field_name));
      if (((((bool)struc) && ((bool)field)) && (!struc->is_enum))) {
        etype = field->type;
      }  else       if (((bool)method)) {
        if (method->is_static) {
          error_span_note_span(node->span, "Member access requires a non-static method", method->span, "This is a static method");
        } 
        etype = method->type;
      }  else {
        TypeChecker__error_unknown_member(this, node, struct_type, field_name, false);
      } 
      
    } break;
    case ASTType__Cast: {
      Type *lhs_type = TypeChecker__check_expression(this, node->u.cast.lhs);
      if ((!TypeChecker__type_is_valid(this, node->u.cast.to))) {
        error_span(node->u.cast.to->span, "Type does not exist");
      } 
      etype = node->u.cast.to;
    } break;
    case ASTType__IsNotNull: {
      Type *lhs_type = TypeChecker__check_expression(this, node->u.unary);
      if ((lhs_type->base != BaseType__Pointer)) {
        error_span_note(node->span, "Can only use ? on pointer types", format_string("Type of expression is '%s'", Type__str(lhs_type)));
      } 
      etype = Type__new(BaseType__Bool, node->span);
    } break;
    case ASTType__Match: {
      TypeChecker__check_match(this, node, true);
      etype = node->etype;
    } break;
    case ASTType__If: {
      TypeChecker__check_if(this, node, true);
      etype = node->etype;
    } break;
    default: {
      panic(format_string("Unhandled type in check_expression: %s", ASTType__str(node->type)));
    } break;
  }
  etype = Type__decay_array(etype);
  node->etype = etype;
  return etype;
}

void TypeChecker__check_expression_statement(TypeChecker *this, AST *node, AST *body, bool is_expr) {
  switch (body->type) {
    case ASTType__Match: {
      TypeChecker__check_match(this, body, is_expr);
    } break;
    case ASTType__If: {
      TypeChecker__check_if(this, body, is_expr);
    } break;
    case ASTType__Block: {
      TypeChecker__check_block(this, body, is_expr);
    } break;
    default: {
      TypeChecker__check_statement(this, body);
    } break;
  }
  node->returns = (node->returns && body->returns);
  if ((!is_expr)) 
  return;
  
  Type *ret = body->etype;
  if (body->returns) {
  }  else   if ((!((bool)ret))) {
    error_span(body->span, "Must yield a value in this branch");
  }  else   if ((!((bool)node->etype))) {
    node->etype = ret;
  }  else   if ((!Type__eq(node->etype, ret))) {
    error_span_note(body->span, "Yield type doesn't match previous branches", format_string("Expected type '%s', got '%s'", Type__str(node->etype), Type__str(ret)));
  } 
  
  
  
}

void TypeChecker__check_match_for_enum(TypeChecker *this, Structure *struc, AST *node, bool is_expr) {
  Map *mapping = Map__new();
  ;
  Vector *cases = node->u.match_stmt.cases;
  node->returns = (cases->size > 0);
  for (i32 i = 0; (i < cases->size); i += 1) {
    MatchCase *_case = ((MatchCase *)Vector__at(cases, i));
    AST *cond = _case->cond;
    char *name;
    if (cond->type == ASTType__Identifier) {
      name = cond->u.ident.name;
      Variable *var = Structure__find_field(struc, name);
      if ((!((bool)var))) {
        error_span(cond->span, "Enum has no field with this name");
      } 
      cond->type = ASTType__EnumValue;
      cond->u.enum_val.struct_def = struc;
      cond->u.enum_val.var = var;
      cond->etype = struc->type;
    }  else {
      Type *cond_type = TypeChecker__check_expression(this, cond);
      if ((!Type__eq(cond_type, struc->type))) {
        error_span_note_span(cond->span, "Condition does not match expression type", node->u.match_stmt.expr->span, format_string("Match expression is of type '%s'", Type__str(struc->type)));
      } 
      name = cond->u.enum_val.var->name;
    } 
    MatchCase *prev = ((MatchCase *)Map__get(mapping, name));
    if (((bool)prev)) {
      error_span_note_span(cond->span, "Duplicate condition name in match", prev->cond->span, "This condition was previously used here");
    } 
    Map__insert(mapping, name, _case);
    if (((bool)_case->body)) {
      TypeChecker__check_expression_statement(this, node, _case->body, is_expr);
    } 
  } 
  AST *defolt = node->u.match_stmt.defolt;
  if ((mapping->num_items != struc->fields->size)) {
    if ((!((bool)defolt))) {
      error_span_note(node->span, "Match does not cover all cases", format_string("Only %d of %d cases are covered", mapping->num_items, struc->fields->size));
    } 
    TypeChecker__check_expression_statement(this, node, defolt, is_expr);
  }  else {
    if (((bool)defolt)) {
      error_span(node->u.match_stmt.defolt_span, "`else` case is not needed for this match");
    } 
  } 
  if (((is_expr && (!((bool)node->etype))) && (!node->returns))) {
    error_span(node->span, "Expression-match must yield a value");
  } 

  /* defers */
  Map__free(mapping);
}

void TypeChecker__check_match(TypeChecker *this, AST *node, bool is_expr) {
  AST *expr = node->u.match_stmt.expr;
  Type *expr_type = TypeChecker__check_expression(this, expr);
  Structure *struc;
  if (expr_type->base == BaseType__Structure) {
    struc = ((Structure *)Map__get(this->structures, expr_type->name));
    if (struc->is_enum) {
      TypeChecker__check_match_for_enum(this, struc, node, is_expr);
      return;
    } 
  } 
  if ((((expr_type->base != BaseType__I32) && (expr_type->base != BaseType__Char)) && (!Type__is_string(expr_type)))) {
    error_span_note(expr->span, "This type cannot be matched on", format_string("Expression type is '%s'", Type__str(expr_type)));
  } 
  Vector *cases = node->u.match_stmt.cases;
  node->returns = (cases->size > 0);
  for (i32 i = 0; (i < cases->size); i += 1) {
    MatchCase *_case = ((MatchCase *)Vector__at(cases, i));
    Type *cond_type = TypeChecker__check_expression(this, _case->cond);
    if ((!Type__eq(cond_type, expr_type))) {
      error_span_note_span(cond_type->span, "Condition does not match expression type", node->u.match_stmt.expr->span, format_string("Match expression is of type '%s'", Type__str(struc->type)));
    } 
    if ((((_case->cond->type != ASTType__IntLiteral) && (_case->cond->type != ASTType__CharLiteral)) && (_case->cond->type != ASTType__StringLiteral))) {
      error_span(_case->cond->span, "Match condition must use only literals");
    } 
    if (((bool)_case->body)) {
      TypeChecker__check_expression_statement(this, node, _case->body, is_expr);
    } 
  } 
  AST *defolt = node->u.match_stmt.defolt;
  if ((!((bool)defolt))) {
    error_span(node->span, "`else` case is missing");
  } 
  TypeChecker__check_expression_statement(this, node, defolt, is_expr);
  if (((is_expr && (!((bool)node->etype))) && (!node->returns))) {
    error_span(node->span, "Expression-match must yield a value");
  } 
}

void TypeChecker__check_if(TypeChecker *this, AST *node, bool is_expr) {
  Type *cond_type = TypeChecker__check_expression(this, node->u.if_stmt.cond);
  if ((cond_type->base != BaseType__Bool)) {
    error_span_note(node->u.if_stmt.cond->span, "Condition must be a boolean", format_string("Got type '%s'", Type__str(cond_type)));
  } 
  TypeChecker__check_expression_statement(this, node, node->u.if_stmt.body, is_expr);
  if (((bool)node->u.if_stmt.els)) {
    AST *else_stmt = node->u.if_stmt.els;
    TypeChecker__check_expression_statement(this, node, else_stmt, is_expr);
    if ((node->u.if_stmt.body->returns && else_stmt->returns)) {
      node->returns = true;
    } 
  }  else   if (is_expr) {
    error_span(node->span, "Expression-if must have an 'else' branch");
  } 
  
  if (((is_expr && (!((bool)node->etype))) && (!node->returns))) {
    error_span(node->span, "Expression-if must yield a value");
  } 
}

void TypeChecker__check_statement(TypeChecker *this, AST *node) {
  switch (node->type) {
    case ASTType__Block: {
      TypeChecker__check_block(this, node, false);
    } break;
    case ASTType__Defer: {
      TypeChecker__check_expression(this, node->u.unary);
    } break;
    case ASTType__Match: {
      TypeChecker__check_match(this, node, false);
    } break;
    case ASTType__Yield: {
      if ((!this->can_yield)) {
        error_span(node->span, "Cannot yield in this context");
      } 
      node->etype = TypeChecker__check_expression(this, node->u.unary);
    } break;
    case ASTType__Return: {
      if ((!((bool)this->cur_func))) {
        error_span(node->span, "Return statement outside of function");
      } 
      if ((!((bool)node->u.unary))) {
        if ((this->cur_func->return_type->base != BaseType__Void)) {
          error_span_note_span(node->span, "Cannot have empty return in non-void function", this->cur_func->return_type->span, format_string("This function returns '%s'", Type__str(this->cur_func->return_type)));
        } 
      }  else {
        Type *ret_type = TypeChecker__check_expression(this, node->u.unary);
        if (this->cur_func->return_type->base == BaseType__Void) {
          error_span_note_span(node->u.unary->span, format_string("Cannot return '%s' in void function", Type__str(ret_type)), this->cur_func->span, "This function does not return a value");
        } 
        if ((!Type__eq(ret_type, this->cur_func->return_type))) {
          error_span_note_span(node->u.unary->span, format_string("Return type '%s' is incorrect", Type__str(ret_type)), this->cur_func->return_type->span, format_string("This function returns '%s'", Type__str(this->cur_func->return_type)));
        } 
      } 
      node->returns = true;
    } break;
    case ASTType__Break:
    case ASTType__Continue: {
      if ((!this->in_loop)) {
        error_span(node->span, format_string("%s statement outside of loop", ASTType__str(node->type)));
      } 
    } break;
    case ASTType__VarDeclaration: {
      VarDeclaration *var_decl = (&node->u.var_decl);
      if (((bool)var_decl->init)) {
        Type *init_type = TypeChecker__check_expression(this, var_decl->init);
        if (init_type->base == BaseType__Method) {
          error_span(var_decl->init->span, "Cannot assign methods to variables");
        } 
        if ((!((bool)var_decl->var->type))) {
          var_decl->var->type = init_type;
        }  else {
          if ((!TypeChecker__type_is_valid(this, var_decl->var->type))) {
            error_span(var_decl->var->span, "Invalid type");
          } 
          if ((!Type__eq(var_decl->var->type, init_type))) {
            error_span_note(var_decl->init->span, "Variable type does not match initializer type", format_string("Expected '%s' but got '%s'", Type__str(var_decl->var->type), Type__str(init_type)));
          } 
        } 
      }  else {
        if ((!((bool)var_decl->var->type))) {
          error_span(var_decl->var->span, "Variable type cannot be inferred, specify explicitly");
        } 
        if ((!TypeChecker__type_is_valid(this, var_decl->var->type))) {
          error_span(var_decl->var->type->span, "Invalid variable type");
        } 
      } 
      TypeChecker__push_var(this, var_decl->var);
    } break;
    case ASTType__While: {
      bool was_in_loop = this->in_loop;
      this->in_loop = true;
      Type *cond_type = TypeChecker__check_expression(this, node->u.loop.cond);
      if ((cond_type->base != BaseType__Bool)) {
        error_span_note(node->u.loop.cond->span, "Condition must be boolean", format_string("Got type '%s'", Type__str(cond_type)));
      } 
      TypeChecker__check_statement(this, node->u.loop.body);
      this->in_loop = was_in_loop;
    } break;
    case ASTType__For: {
      bool was_in_loop = this->in_loop;
      this->in_loop = true;
      TypeChecker__push_scope(this);
      if (((bool)node->u.loop.init)) {
        TypeChecker__check_statement(this, node->u.loop.init);
      } 
      if (((bool)node->u.loop.init)) {
        Type *cond_type = TypeChecker__check_expression(this, node->u.loop.cond);
        if ((cond_type->base != BaseType__Bool)) {
          error_span_note(node->u.loop.cond->span, "Condition must be boolean", format_string("Got type '%s'", Type__str(cond_type)));
        } 
      } 
      if (((bool)node->u.loop.incr)) {
        TypeChecker__check_statement(this, node->u.loop.incr);
      } 
      TypeChecker__check_statement(this, node->u.loop.body);
      TypeChecker__pop_scope(this);
      this->in_loop = was_in_loop;
    } break;
    case ASTType__If: {
      TypeChecker__check_if(this, node, false);
    } break;
    default: {
      TypeChecker__check_expression(this, node);
    } break;
  }
}

void TypeChecker__check_block(TypeChecker *this, AST *node, bool can_yield) {
  bool could_yield = this->can_yield;
  this->can_yield = can_yield;
  Span yield_span = {0};
  TypeChecker__push_scope(this);
  Vector *statements = node->u.block.statements;
  for (i32 i = 0; (i < statements->size); i += 1) {
    AST *statement = ((AST *)Vector__at(statements, i));
    TypeChecker__check_statement(this, statement);
    if (statement->returns) {
      node->returns = true;
    } 
    if ((statement->type != ASTType__Yield)) 
    continue;
    
    if (((bool)node->etype)) {
      error_span_note_span(statement->span, "Cannot yield multiple times in a block", yield_span, "Previously yield here is here");
    } 
    node->etype = statement->etype;
    yield_span = statement->span;
  } 
  TypeChecker__pop_scope(this);
  this->can_yield = could_yield;
}

void TypeChecker__check_function(TypeChecker *this, Function *func) {
  Function *prev_func = this->cur_func;
  this->cur_func = func;
  TypeChecker__push_scope(this);
  for (i32 i = 0; (i < func->params->size); i += 1) {
    Variable *var = ((Variable *)Vector__at(func->params, i));
    TypeChecker__push_var(this, var);
  } 
  if (((bool)func->body)) {
    if (func->is_arrow) {
      TypeChecker__check_statement(this, func->body);
    }  else {
      TypeChecker__check_block(this, func->body, false);
    } 
    if (((!func->body->returns) && (func->return_type->base != BaseType__Void))) {
      if ((!string__eq(func->name, "main"))) {
        error_span(func->span, "Function does not always return");
      } 
    } 
  } 
  TypeChecker__pop_scope(this);
  this->cur_func = prev_func;
}

void TypeChecker__check_all_functions(TypeChecker *this, Program *program) {
  for (i32 i = 0; (i < program->functions->size); i += 1) {
    Function *func = ((Function *)Vector__at(program->functions, i));
    char *name = func->name;
    char *struct_name = func->method_struct_name;
    Type *func_type = ((Type *)NULL);
    if (func->is_method) {
      Map *s_methods = ((Map *)Map__get(this->methods, struct_name));
      if ((!((bool)s_methods))) {
        error_span(func->span, "Type for method does not exist");
      } 
      if (Map__exists(s_methods, name)) {
        Function *method = ((Function *)Map__get(s_methods, name));
        error_span_note_span(func->span, "Method is already defined for this type", method->span, "Previous definition here");
      } 
      Variable *var = TypeChecker__get_struct_member(this, struct_name, name);
      if (((bool)var)) {
        error_span_note_span(func->span, "Type already has a field with this name", var->span, "Previous definition here");
      } 
      func_type = Type__new(BaseType__Method, func->span);
      func_type->name = struct_name;
    }  else {
      func_type = Type__new(BaseType__Function, func->span);
      if (Map__exists(this->functions, name)) {
        Function *prev = ((Function *)Map__get(this->functions, name));
        error_span_note_span(func->span, "Function is already defined", prev->span, "Previous definition here");
      } 
    } 
    func_type->func_def = func;
    for (i32 j = 0; (j < func->params->size); j += 1) {
      Variable *param = ((Variable *)Vector__at(func->params, j));
      if ((!TypeChecker__type_is_valid(this, param->type))) {
        error_span(param->type->span, "Invalid parameter type");
      } 
    } 
    func_type->params = func->params;
    if ((!TypeChecker__type_is_valid(this, func->return_type))) {
      error_span(func->return_type->span, "Invalid return type");
    } 
    func_type->return_type = func->return_type;
    func->type = func_type;
    if (func->is_method) {
      Map *s_methods = ((Map *)Map__get(this->methods, struct_name));
      Map__insert(s_methods, name, func);
    }  else {
      Map__insert(this->functions, name, func);
    } 
  } 
  for (i32 i = 0; (i < program->functions->size); i += 1) {
    TypeChecker__check_function(this, ((Function *)Vector__at(program->functions, i)));
  } 
}

void TypeChecker__dfs_structs(TypeChecker *this, Structure *struc, Vector *results, Map *done) {
  Map__insert(done, struc->name, struc);
  for (i32 i = 0; (i < struc->fields->size); i += 1) {
    Variable *field = ((Variable *)Vector__at(struc->fields, i));
    if ((!TypeChecker__type_is_valid(this, field->type))) {
      error_span(field->type->span, "Type of field is undefined");
    } 
    if (((!struc->is_extern) && field->type->base == BaseType__Structure)) {
      char *neib_name = field->type->name;
      Structure *neib_struc = ((Structure *)Map__get(this->structures, neib_name));
      if ((!Map__exists(done, neib_name))) {
        TypeChecker__dfs_structs(this, neib_struc, results, done);
      } 
    } 
  } 
  Vector__push(results, struc);
}

void TypeChecker__check_all_structs(TypeChecker *this, Program *program) {
  for (i32 i = 0; (i < program->structures->size); i += 1) {
    Structure *struc = ((Structure *)Vector__at(program->structures, i));
    char *name = struc->name;
    if (Map__exists(this->structures, name)) {
      Structure *prev = ((Structure *)Map__get(this->structures, name));
      error_span_note_span(struc->span, "Struct has already been defined", prev->span, "Previous definition here");
    } 
    Map__insert(this->structures, name, struc);
    Map__insert(this->methods, name, Map__new());
  } 
  Map *done = Map__new();
  Vector *results = Vector__new();
  for (i32 i = 0; (i < program->structures->size); i += 1) {
    Structure *struc = ((Structure *)Vector__at(program->structures, i));
    if ((!Map__exists(done, struc->name))) {
      TypeChecker__dfs_structs(this, struc, results, done);
    } 
  } 
  Map__free(done);
  Vector__free(program->structures);
  program->structures = results;
}

void TypeChecker__check_program(TypeChecker *this, Program *program) {
  TypeChecker__check_all_structs(this, program);
  TypeChecker__push_scope(this);
  for (i32 i = 0; (i < program->global_vars->size); i += 1) {
    AST *var = ((AST *)Vector__at(program->global_vars, i));
    TypeChecker__check_statement(this, var);
  } 
  TypeChecker__check_all_functions(this, program);
  TypeChecker__pop_scope(this);
}

StringBuilder StringBuilder__make(void) {
  i32 initial_capacity = 16;
  return (StringBuilder){.data = calloc(initial_capacity, 1), .size = 0, .capacity = initial_capacity};
}

void StringBuilder__resize_if_necessary(StringBuilder *this, i32 new_size) {
  if ((new_size >= this->capacity)) {
    i32 new_capacity = max((this->capacity * 2), new_size);
    this->data = ((char *)realloc(this->data, new_capacity));
  } 
}

void StringBuilder__puts(StringBuilder *this, char *s) {
  i32 len = strlen(s);
  StringBuilder__resize_if_necessary(this, ((this->size + len) + 1));
  memcpy((this->data + this->size), s, (len + 1));
  this->size += len;
}

void StringBuilder__putc(StringBuilder *this, char c) {
  StringBuilder__resize_if_necessary(this, (this->size + 2));
  this->data[this->size] = c;
  this->size += 1;
  this->data[this->size] = '\0';
}

void StringBuilder__putsf(StringBuilder *this, char *s) {
  StringBuilder__puts(this, s);
  free(s);
}

char *StringBuilder__str(StringBuilder *this) {
  return this->data;
}

char *StringBuilder__new_str(StringBuilder *this) {
  return strdup(this->data);
}

CodeGenerator CodeGenerator__make(bool debug) {
  return (CodeGenerator){.out = StringBuilder__make(), .scopes = Vector__new(), .yield_vars = Vector__new(), .yield_count = 0, .debug = debug};
}

void CodeGenerator__gen_debug_info(CodeGenerator *this, Span span) {
  if ((!this->debug)) 
  return;
  
  Location loc = span.start;
  StringBuilder__putsf((&this->out), format_string("\n#line %d \"%s\"\n", loc.line, loc.filename));
}

void CodeGenerator__indent(CodeGenerator *this, i32 num) {
  for (i32 i = 0; (i < num); i += 1) {
    StringBuilder__puts((&this->out), "  ");
  } 
}

void CodeGenerator__push_scope(CodeGenerator *this) {
  Vector__push(this->scopes, Vector__new());
}

Vector *CodeGenerator__scope(CodeGenerator *this) {
  return Vector__back(this->scopes);
}

void CodeGenerator__pop_scope(CodeGenerator *this) {
  Vector *scope = ((Vector *)Vector__pop(this->scopes));
  Vector__free(scope);
}

void CodeGenerator__gen_control_body(CodeGenerator *this, AST *node, AST *body, i32 indent) {
  if (body->type == ASTType__Block) {
    CodeGenerator__gen_block(this, body, indent);
    StringBuilder__puts((&this->out), " ");
  }  else {
    if ((body->type != ASTType__If)) {
      StringBuilder__puts((&this->out), "\n");
    } 
    if ((((bool)node->etype) && (body->type != ASTType__Yield))) {
      CodeGenerator__gen_yield_expression(this, body, (indent + 1));
    }  else {
      CodeGenerator__gen_statement(this, body, indent);
    } 
    CodeGenerator__indent(this, indent);
  } 
}

void CodeGenerator__gen_enum_value(CodeGenerator *this, char *enum_name, Variable *var) {
  if (var->is_extern) {
    StringBuilder__puts((&this->out), var->extern_name);
  }  else {
    StringBuilder__putsf((&this->out), format_string("%s__%s", enum_name, var->name));
  } 
}

void CodeGenerator__gen_enum(CodeGenerator *this, Structure *struc) {
  if ((!struc->is_extern)) {
    StringBuilder__putsf((&this->out), format_string("enum %s {\n", struc->name));
    for (i32 i = 0; (i < struc->fields->size); i += 1) {
      Variable *field = ((Variable *)Vector__at(struc->fields, i));
      CodeGenerator__indent(this, 1);
      CodeGenerator__gen_enum_value(this, struc->name, field);
      StringBuilder__puts((&this->out), ",\n");
    } 
    StringBuilder__puts((&this->out), "};\n\n");
  } 
}

void CodeGenerator__gen_struct(CodeGenerator *this, Structure *struc) {
  if ((!struc->is_extern)) {
    char *name = struc->type->name;
    if (struc->is_union) {
      StringBuilder__puts((&this->out), "union ");
    }  else {
      StringBuilder__puts((&this->out), "struct ");
    } 
    StringBuilder__puts((&this->out), name);
    StringBuilder__puts((&this->out), " {\n");
    for (i32 i = 0; (i < struc->fields->size); i += 1) {
      Variable *field = ((Variable *)Vector__at(struc->fields, i));
      CodeGenerator__indent(this, 1);
      CodeGenerator__gen_type_and_name(this, field->type, field->name);
      StringBuilder__puts((&this->out), ";\n");
    } 
    StringBuilder__puts((&this->out), "};\n\n");
  } 
}

void CodeGenerator__gen_format_string_part(CodeGenerator *this, char *part) {
  i32 len = strlen(part);
  for (i32 i = 0; (i < len); i += 1) {
    if (part[i] == '\\') {
      i += 1;
      switch (part[i]) {
        case '`':
        case '{':
        case '}': {
        } break;
        case '%': {
          StringBuilder__putc((&this->out), '%');
        } break;
        default: {
          StringBuilder__putc((&this->out), '\\');
        } break;
      }
    }  else     if (part[i] == '"') {
      StringBuilder__putc((&this->out), '\\');
    } 
    
    StringBuilder__putc((&this->out), part[i]);
  } 
}

void CodeGenerator__gen_format_string(CodeGenerator *this, AST *node) {
  Vector *parts = node->u.fmt_str.parts;
  Vector *exprs = node->u.fmt_str.exprs;
  StringBuilder__puts((&this->out), "format_string(\"");
  for (i32 i = 0; (i < exprs->size); i += 1) {
    char *part = ((char *)Vector__at(parts, i));
    CodeGenerator__gen_format_string_part(this, part);
    AST *expr = ((AST *)Vector__at(exprs, i));
    Type *expr_type = expr->etype;
    switch (expr_type->base) {
      case BaseType__I8:
      case BaseType__I16:
      case BaseType__I32: {
        StringBuilder__puts((&this->out), "%d");
      } break;
      case BaseType__U8:
      case BaseType__U16:
      case BaseType__U32: {
        StringBuilder__puts((&this->out), "%u");
      } break;
      case BaseType__I64: {
        StringBuilder__puts((&this->out), "%lld");
      } break;
      case BaseType__U64: {
        StringBuilder__puts((&this->out), "%llu");
      } break;
      case BaseType__Bool: {
        StringBuilder__puts((&this->out), "%s");
      } break;
      case BaseType__F32:
      case BaseType__F64: {
        StringBuilder__puts((&this->out), "%f");
      } break;
      case BaseType__Char: {
        StringBuilder__puts((&this->out), "%c");
      } break;
      case BaseType__Pointer: {
        switch (expr_type->ptr->base) {
          case BaseType__Char: {
            StringBuilder__puts((&this->out), "%s");
          } break;
          default: {
            StringBuilder__puts((&this->out), "%p");
          } break;
        }
      } break;
      default: {
        error_span(expr->span, "Invalid type for format string");
      } break;
    }
  } 
  char *part = ((char *)Vector__back(parts));
  CodeGenerator__gen_format_string_part(this, part);
  StringBuilder__putc((&this->out), '"');
  for (i32 i = 0; (i < exprs->size); i += 1) {
    StringBuilder__puts((&this->out), ", ");
    AST *expr = ((AST *)Vector__at(exprs, i));
    CodeGenerator__gen_expression(this, expr);
  } 
  StringBuilder__puts((&this->out), ")");
}

char *CodeGenerator__get_op(ASTType type) {
  return ({ char *__yield_0;
  switch (type) {
    case ASTType__Address: {
      __yield_0 = "&";
} break;
    case ASTType__And: {
      __yield_0 = " && ";
} break;
    case ASTType__Assignment: {
      __yield_0 = " = ";
} break;
    case ASTType__BitwiseAnd: {
      __yield_0 = " & ";
} break;
    case ASTType__BitwiseOr: {
      __yield_0 = " | ";
} break;
    case ASTType__BitwiseXor: {
      __yield_0 = " ^ ";
} break;
    case ASTType__Dereference: {
      __yield_0 = "*";
} break;
    case ASTType__Divide: {
      __yield_0 = " / ";
} break;
    case ASTType__DivideEquals: {
      __yield_0 = " /= ";
} break;
    case ASTType__Equals: {
      __yield_0 = " == ";
} break;
    case ASTType__GreaterThan: {
      __yield_0 = " > ";
} break;
    case ASTType__GreaterThanEquals: {
      __yield_0 = " >= ";
} break;
    case ASTType__LeftShift: {
      __yield_0 = " << ";
} break;
    case ASTType__LessThan: {
      __yield_0 = " < ";
} break;
    case ASTType__LessThanEquals: {
      __yield_0 = " <= ";
} break;
    case ASTType__Minus: {
      __yield_0 = " - ";
} break;
    case ASTType__MinusEquals: {
      __yield_0 = " -= ";
} break;
    case ASTType__Modulus: {
      __yield_0 = " % ";
} break;
    case ASTType__Multiply: {
      __yield_0 = " * ";
} break;
    case ASTType__MultiplyEquals: {
      __yield_0 = " *= ";
} break;
    case ASTType__Not: {
      __yield_0 = "!";
} break;
    case ASTType__NotEquals: {
      __yield_0 = " != ";
} break;
    case ASTType__Or: {
      __yield_0 = " || ";
} break;
    case ASTType__Plus: {
      __yield_0 = " + ";
} break;
    case ASTType__PlusEquals: {
      __yield_0 = " += ";
} break;
    case ASTType__RightShift: {
      __yield_0 = " >> ";
} break;
    case ASTType__UnaryMinus: {
      __yield_0 = "-";
} break;
    default: {
      panic(format_string("Unknown op type in get_op: %s", ASTType__str(type)));
    } break;
  }
;__yield_0; });
}

void CodeGenerator__gen_in_yield_context(CodeGenerator *this, AST *node) {
  char *yield_var = format_string("__yield_%d", this->yield_count);
  this->yield_count += 1;
  Vector__push(this->yield_vars, yield_var);
  Type *ret_type = node->etype;
  StringBuilder__puts((&this->out), "({ ");
  CodeGenerator__gen_type_and_name(this, ret_type, yield_var);
  StringBuilder__puts((&this->out), ";\n");
  CodeGenerator__gen_statement(this, node, 1);
  StringBuilder__puts((&this->out), ";");
  StringBuilder__puts((&this->out), yield_var);
  StringBuilder__puts((&this->out), "; })");
  this->yield_count -= 1;
  Vector__pop(this->yield_vars);
}

void CodeGenerator__gen_expression(CodeGenerator *this, AST *node) {
  switch (node->type) {
    case ASTType__IntLiteral: {
      StringBuilder__puts((&this->out), node->u.num_literal);
    } break;
    case ASTType__FloatLiteral: {
      StringBuilder__puts((&this->out), node->u.num_literal);
    } break;
    case ASTType__StringLiteral: {
      StringBuilder__putsf((&this->out), format_string("\"%s\"", node->u.string_literal));
    } break;
    case ASTType__CharLiteral: {
      StringBuilder__putsf((&this->out), format_string("'%s'", node->u.char_literal));
    } break;
    case ASTType__Null: {
      StringBuilder__puts((&this->out), "NULL");
    } break;
    case ASTType__Match: {
      CodeGenerator__gen_in_yield_context(this, node);
    } break;
    case ASTType__FormatStringLiteral: {
      CodeGenerator__gen_format_string(this, node);
    } break;
    case ASTType__If: {
      AST *a = node->u.if_stmt.body;
      AST *b = node->u.if_stmt.els;
      if (((a->type != ASTType__Block) && (b->type != ASTType__Block))) {
        StringBuilder__puts((&this->out), "(");
        CodeGenerator__gen_expression(this, node->u.if_stmt.cond);
        StringBuilder__puts((&this->out), " ? ");
        CodeGenerator__gen_expression(this, a);
        StringBuilder__puts((&this->out), " : ");
        CodeGenerator__gen_expression(this, b);
        StringBuilder__puts((&this->out), ")");
      }  else {
        CodeGenerator__gen_in_yield_context(this, node);
      } 
    } break;
    case ASTType__BoolLiteral: {
      StringBuilder__puts((&this->out), (node->u.bool_literal ? "true" : "false"));
    } break;
    case ASTType__Identifier: {
      Identifier ident = node->u.ident;
      if (ident.is_function) {
        StringBuilder__puts((&this->out), CodeGenerator__get_function_name(this, ident.func));
      }  else       if (ident.var->is_extern) {
        StringBuilder__puts((&this->out), ident.var->extern_name);
      }  else {
        StringBuilder__puts((&this->out), ident.var->name);
      } 
      
    } break;
    case ASTType__Call: {
      bool newline_after_first = false;
      if (AST__callee_is(node, "print")) {
        StringBuilder__puts((&this->out), "printf");
      }  else       if (AST__callee_is(node, "println")) {
        StringBuilder__puts((&this->out), "printf");
        newline_after_first = true;
      }  else       if ((!((bool)node->u.call.func))) {
        CodeGenerator__gen_expression(this, node->u.call.callee);
      }  else {
        StringBuilder__puts((&this->out), CodeGenerator__get_function_name(this, node->u.call.func));
      } 
      
      
      StringBuilder__puts((&this->out), "(");
      Vector *args = node->u.call.args;
      for (i32 i = 0; (i < args->size); i += 1) {
        if ((i > 0)) {
          StringBuilder__puts((&this->out), ", ");
        } 
        Argument *arg = ((Argument *)Vector__at(args, i));
        CodeGenerator__gen_expression(this, arg->expr);
        if ((i == 0 && newline_after_first)) {
          StringBuilder__puts((&this->out), " \"\\n\"");
        } 
      } 
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Constructor: {
      Structure *struc = node->u.constructor.struc;
      Vector *args = node->u.constructor.args;
      StringBuilder__puts((&this->out), "(");
      CodeGenerator__gen_type(this, struc->type);
      StringBuilder__puts((&this->out), "){");
      for (i32 i = 0; (i < args->size); i += 1) {
        if ((i > 0)) {
          StringBuilder__puts((&this->out), ", ");
        } 
        Argument *arg = ((Argument *)Vector__at(args, i));
        if (((bool)arg->label)) {
          char *label = arg->label->u.ident.name;
          StringBuilder__putsf((&this->out), format_string(".%s = ", label));
        } 
        CodeGenerator__gen_expression(this, arg->expr);
      } 
      StringBuilder__puts((&this->out), "}");
    } break;
    case ASTType__And:
    case ASTType__BitwiseAnd:
    case ASTType__BitwiseOr:
    case ASTType__BitwiseXor:
    case ASTType__Divide:
    case ASTType__GreaterThan:
    case ASTType__GreaterThanEquals:
    case ASTType__LeftShift:
    case ASTType__LessThan:
    case ASTType__LessThanEquals:
    case ASTType__Minus:
    case ASTType__Modulus:
    case ASTType__Multiply:
    case ASTType__NotEquals:
    case ASTType__Or:
    case ASTType__Plus:
    case ASTType__RightShift: {
      StringBuilder__puts((&this->out), "(");
      CodeGenerator__gen_expression(this, node->u.binary.lhs);
      StringBuilder__puts((&this->out), CodeGenerator__get_op(node->type));
      CodeGenerator__gen_expression(this, node->u.binary.rhs);
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Address:
    case ASTType__Dereference:
    case ASTType__Not:
    case ASTType__UnaryMinus: {
      StringBuilder__puts((&this->out), "(");
      StringBuilder__puts((&this->out), CodeGenerator__get_op(node->type));
      CodeGenerator__gen_expression(this, node->u.unary);
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Index: {
      CodeGenerator__gen_expression(this, node->u.binary.lhs);
      StringBuilder__puts((&this->out), "[");
      CodeGenerator__gen_expression(this, node->u.binary.rhs);
      StringBuilder__puts((&this->out), "]");
    } break;
    case ASTType__Member: {
      CodeGenerator__gen_expression(this, node->u.member.lhs);
      StringBuilder__puts((&this->out), (node->u.member.is_pointer ? "->" : "."));
      AST *rhs = node->u.member.rhs;
      StringBuilder__puts((&this->out), rhs->u.ident.name);
    } break;
    case ASTType__EnumValue: {
      EnumValue enum_value = node->u.enum_val;
      CodeGenerator__gen_enum_value(this, enum_value.struct_def->name, enum_value.var);
    } break;
    case ASTType__IsNotNull: {
      StringBuilder__puts((&this->out), "((bool)");
      CodeGenerator__gen_expression(this, node->u.unary);
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Cast: {
      StringBuilder__puts((&this->out), "((");
      CodeGenerator__gen_type(this, node->u.cast.to);
      StringBuilder__puts((&this->out), ")");
      CodeGenerator__gen_expression(this, node->u.cast.lhs);
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Defer: {
      Vector__push(CodeGenerator__scope(this), node->u.unary);
    } break;
    case ASTType__SizeOf: {
      StringBuilder__puts((&this->out), "sizeof(");
      CodeGenerator__gen_type(this, node->u.size_of_type);
      StringBuilder__puts((&this->out), ")");
    } break;
    case ASTType__Equals:
    case ASTType__Assignment:
    case ASTType__PlusEquals:
    case ASTType__MinusEquals:
    case ASTType__DivideEquals:
    case ASTType__MultiplyEquals: {
      CodeGenerator__gen_expression(this, node->u.binary.lhs);
      StringBuilder__puts((&this->out), CodeGenerator__get_op(node->type));
      CodeGenerator__gen_expression(this, node->u.binary.rhs);
    } break;
    default: {
      panic(format_string("Unhandled expression type: %s", ASTType__str(node->type)));
    } break;
  }
}

void CodeGenerator__gen_var_decl(CodeGenerator *this, AST *node) {
  Variable *var = node->u.var_decl.var;
  if (var->is_extern) 
  return;
  
  CodeGenerator__gen_type_and_name(this, var->type, var->name);
  if (((bool)node->u.var_decl.init)) {
    StringBuilder__puts((&this->out), " = ");
    CodeGenerator__gen_expression(this, node->u.var_decl.init);
  }  else   if (Type__is_struct(var->type)) {
    StringBuilder__puts((&this->out), " = {0}");
  } 
  
}

void CodeGenerator__gen_match_case_body(CodeGenerator *this, AST *node, AST *body, i32 indent) {
  if (body->type == ASTType__Block) {
    StringBuilder__puts((&this->out), " ");
    CodeGenerator__gen_block(this, body, (indent + 1));
  }  else   if ((((bool)node->etype) && (body->type != ASTType__Yield))) {
    StringBuilder__puts((&this->out), " {\n");
    CodeGenerator__gen_yield_expression(this, body, (indent + 2));
    StringBuilder__puts((&this->out), "}");
  }  else {
    StringBuilder__puts((&this->out), " {\n");
    CodeGenerator__gen_statement(this, body, (indent + 2));
    CodeGenerator__indent(this, (indent + 1));
    StringBuilder__puts((&this->out), "}");
  } 
  
}

void CodeGenerator__gen_match_string(CodeGenerator *this, AST *node, i32 indent) {
  Match stmt = node->u.match_stmt;
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), "{\n");
  CodeGenerator__indent(this, (indent + 1));
  StringBuilder__puts((&this->out), "char *__match_str = ");
  CodeGenerator__gen_expression(this, stmt.expr);
  StringBuilder__puts((&this->out), ";\n");
  Vector *cases = stmt.cases;
  CodeGenerator__indent(this, (indent + 1));
  StringBuilder__puts((&this->out), "if (");
  for (i32 i = 0; (i < cases->size); i += 1) {
    MatchCase *_case = ((MatchCase *)Vector__at(cases, i));
    StringBuilder__puts((&this->out), "!strcmp(__match_str, ");
    CodeGenerator__gen_expression(this, _case->cond);
    StringBuilder__puts((&this->out), ")");
    if (((bool)_case->body)) {
      StringBuilder__puts((&this->out), ")");
      CodeGenerator__gen_match_case_body(this, node, _case->body, indent);
      StringBuilder__puts((&this->out), " else ");
      if ((i != (cases->size - 1))) {
        StringBuilder__puts((&this->out), "if (");
      } 
    }  else {
      StringBuilder__puts((&this->out), " || ");
    } 
  } 
  if (((bool)stmt.defolt)) {
    CodeGenerator__gen_match_case_body(this, node, stmt.defolt, indent);
  } 
  StringBuilder__puts((&this->out), "\n");
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), "}\n");
}

void CodeGenerator__gen_match(CodeGenerator *this, AST *node, i32 indent) {
  Match stmt = node->u.match_stmt;
  if (Type__is_string(stmt.expr->etype)) {
    CodeGenerator__gen_match_string(this, node, indent);
    return;
  } 
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), "switch (");
  CodeGenerator__gen_expression(this, stmt.expr);
  StringBuilder__puts((&this->out), ") {\n");
  Vector *cases = stmt.cases;
  for (i32 i = 0; (i < cases->size); i += 1) {
    MatchCase *_case = ((MatchCase *)Vector__at(cases, i));
    CodeGenerator__indent(this, (indent + 1));
    StringBuilder__puts((&this->out), "case ");
    CodeGenerator__gen_expression(this, _case->cond);
    StringBuilder__puts((&this->out), ":");
    if (((bool)_case->body)) {
      CodeGenerator__gen_match_case_body(this, node, _case->body, indent);
      StringBuilder__puts((&this->out), " break;\n");
    }  else {
      StringBuilder__puts((&this->out), "\n");
    } 
  } 
  if (((bool)stmt.defolt)) {
    CodeGenerator__indent(this, (indent + 1));
    StringBuilder__puts((&this->out), "default:");
    CodeGenerator__gen_match_case_body(this, node, stmt.defolt, indent);
    StringBuilder__puts((&this->out), " break;\n");
  } 
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), "}\n");
}

void CodeGenerator__gen_yield_expression(CodeGenerator *this, AST *expr, i32 indent) {
  void *yield_var = Vector__back(this->yield_vars);
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), yield_var);
  StringBuilder__puts((&this->out), " = ");
  CodeGenerator__gen_expression(this, expr);
  StringBuilder__puts((&this->out), ";\n");
}

void CodeGenerator__gen_statement(CodeGenerator *this, AST *node, i32 indent) {
  CodeGenerator__gen_debug_info(this, node->span);
  switch (node->type) {
    case ASTType__Match: {
      CodeGenerator__gen_match(this, node, indent);
    } break;
    case ASTType__Yield: {
      CodeGenerator__gen_yield_expression(this, node->u.unary, indent);
    } break;
    case ASTType__Return: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "return");
      if (((bool)node->u.unary)) {
        StringBuilder__puts((&this->out), " ");
        CodeGenerator__gen_expression(this, node->u.unary);
      } 
      StringBuilder__puts((&this->out), ";\n");
    } break;
    case ASTType__Break: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "break;\n");
    } break;
    case ASTType__Continue: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "continue;\n");
    } break;
    case ASTType__VarDeclaration: {
      CodeGenerator__indent(this, indent);
      CodeGenerator__gen_var_decl(this, node);
      StringBuilder__puts((&this->out), ";\n");
    } break;
    case ASTType__If: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "if (");
      CodeGenerator__gen_expression(this, node->u.if_stmt.cond);
      StringBuilder__puts((&this->out), ") ");
      CodeGenerator__gen_control_body(this, node, node->u.if_stmt.body, indent);
      if (((bool)node->u.if_stmt.els)) {
        StringBuilder__puts((&this->out), " else ");
        CodeGenerator__gen_control_body(this, node, node->u.if_stmt.els, indent);
      } 
      StringBuilder__puts((&this->out), "\n");
    } break;
    case ASTType__While: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "while (");
      CodeGenerator__gen_expression(this, node->u.loop.cond);
      StringBuilder__puts((&this->out), ") ");
      CodeGenerator__gen_control_body(this, node, node->u.loop.body, indent);
      StringBuilder__puts((&this->out), "\n");
    } break;
    case ASTType__For: {
      CodeGenerator__indent(this, indent);
      StringBuilder__puts((&this->out), "for (");
      if (((bool)node->u.loop.init)) {
        if (node->u.loop.init->type == ASTType__VarDeclaration) {
          CodeGenerator__gen_var_decl(this, node->u.loop.init);
        }  else {
          CodeGenerator__gen_expression(this, node->u.loop.init);
        } 
      } 
      StringBuilder__puts((&this->out), "; ");
      if (((bool)node->u.loop.cond)) {
        CodeGenerator__gen_expression(this, node->u.loop.cond);
      } 
      StringBuilder__puts((&this->out), "; ");
      if (((bool)node->u.loop.incr)) {
        CodeGenerator__gen_expression(this, node->u.loop.incr);
      } 
      StringBuilder__puts((&this->out), ") ");
      CodeGenerator__gen_control_body(this, node, node->u.loop.body, indent);
      StringBuilder__puts((&this->out), "\n");
    } break;
    case ASTType__Block: {
      CodeGenerator__indent(this, indent);
      CodeGenerator__gen_block(this, node, indent);
      StringBuilder__puts((&this->out), "\n");
    } break;
    default: {
      CodeGenerator__indent(this, indent);
      CodeGenerator__gen_expression(this, node);
      StringBuilder__puts((&this->out), ";\n");
    } break;
  }
}

void CodeGenerator__gen_block(CodeGenerator *this, AST *node, i32 indent) {
  CodeGenerator__push_scope(this);
  StringBuilder__puts((&this->out), "{\n");
  Vector *statements = node->u.block.statements;
  for (i32 i = 0; (i < statements->size); i += 1) {
    AST *statement = ((AST *)Vector__at(statements, i));
    CodeGenerator__gen_statement(this, statement, (indent + 1));
  } 
  Vector *defers = CodeGenerator__scope(this);
  if ((defers->size > 0)) {
    StringBuilder__puts((&this->out), "\n");
    CodeGenerator__indent(this, (indent + 1));
    StringBuilder__puts((&this->out), "/* defers */\n");
    for (i32 i = (defers->size - 1); (i >= 0); i -= 1) {
      AST *node = ((AST *)Vector__at(defers, i));
      CodeGenerator__gen_statement(this, node, (indent + 1));
    } 
  } 
  CodeGenerator__indent(this, indent);
  StringBuilder__puts((&this->out), "}");
  CodeGenerator__pop_scope(this);
}

void CodeGenerator__gen_struct_decls(CodeGenerator *this, Program *program) {
  StringBuilder__puts((&this->out), "/* struct declarations */\n");
  for (i32 i = 0; (i < program->structures->size); i += 1) {
    Structure *struc = ((Structure *)Vector__at(program->structures, i));
    if (struc->is_extern) 
    continue;
    
    char *name = struc->name;
    if (struc->is_enum) {
      StringBuilder__puts((&this->out), "typedef enum ");
    }  else     if (struc->is_union) {
      StringBuilder__puts((&this->out), "typedef union ");
    }  else {
      StringBuilder__puts((&this->out), "typedef struct ");
    } 
    
    StringBuilder__putsf((&this->out), format_string("%s %s;\n", name, name));
  } 
  StringBuilder__puts((&this->out), "\n");
}

void string__replace(char **this, char *other) {
  char *s = (*this);
  free(s);
  (*this) = other;
}

char *CodeGenerator__get_type_name_string(CodeGenerator *this, Type *type, char *name, bool is_func_def) {
  char *final = strdup(name);
  for (Type *cur = type; ((bool)cur); cur = cur->ptr) {
    switch (cur->base) {
      case BaseType__Void:
      case BaseType__Bool:
      case BaseType__Char:
      case BaseType__I8:
      case BaseType__I16:
      case BaseType__I32:
      case BaseType__I64:
      case BaseType__U8:
      case BaseType__U16:
      case BaseType__U32:
      case BaseType__U64:
      case BaseType__F32:
      case BaseType__F64: {
        string__replace((&final), format_string("%s %s", BaseType__str(cur->base), final));
      } break;
      case BaseType__Structure: {
        char *struct_name = ({ char *__yield_0;
  if (cur->struct_def->is_extern) {
    __yield_0 = cur->struct_def->extern_name;
  }  else {
    __yield_0 = cur->struct_def->name;
  } 
;__yield_0; });
        string__replace((&final), format_string("%s %s", struct_name, final));
      } break;
      case BaseType__Pointer: {
        bool needs_parens = (((bool)cur->ptr) && (cur->ptr->base == BaseType__Array || cur->ptr->base == BaseType__Function));
        if (needs_parens) {
          string__replace((&final), format_string("(*%s)", final));
        }  else {
          string__replace((&final), format_string("*%s", final));
        } 
      } break;
      case BaseType__Array: {
        StringBuilder prev_builder = this->out;
        this->out = StringBuilder__make();
        CodeGenerator__gen_expression(this, cur->size_expr);
        string__replace((&final), format_string("%s[%s]", final, StringBuilder__str((&this->out))));
        free(this->out.data);
        this->out = prev_builder;
      } break;
      case BaseType__Function:
      case BaseType__Method: {
        StringBuilder acc = StringBuilder__make();
        Vector *params = cur->params;
        if (params->size == 0) 
        StringBuilder__puts((&acc), "void");
        
        for (i32 i = 0; (i < params->size); i += 1) {
          if ((i != 0)) 
          StringBuilder__puts((&acc), ", ");
          
          Variable *var = ((Variable *)Vector__at(params, i));
          char *arg_str = CodeGenerator__get_type_name_string(this, var->type, var->name, false);
          StringBuilder__putsf((&acc), arg_str);
        } 
        if ((is_func_def && cur == type)) {
          string__replace((&final), format_string("%s(%s)", final, StringBuilder__str((&acc))));
        }  else {
          string__replace((&final), format_string("(*%s)(%s)", final, StringBuilder__str((&acc))));
        } 
        free(acc.data);
        string__replace((&final), CodeGenerator__get_type_name_string(this, cur->return_type, final, false));
      } break;
    }
  } 
  string__strip_trailing_whitespace(final);
  return final;
}

void CodeGenerator__gen_type_and_name(CodeGenerator *this, Type *type, char *name) {
  StringBuilder__putsf((&this->out), CodeGenerator__get_type_name_string(this, type, name, false));
}

void CodeGenerator__gen_type(CodeGenerator *this, Type *type) {
  CodeGenerator__gen_type_and_name(this, type, "");
}

char *CodeGenerator__get_function_name(CodeGenerator *this, Function *func) {
  return ({ char *__yield_0;
  if (func->is_extern) {
    __yield_0 = func->extern_name;
  }  else     __yield_0 = ({ char *__yield_1;
  if (func->is_method) {
    __yield_1 = format_string("%s__%s", func->method_struct_name, func->name);
  }  else {
    __yield_1 = func->name;
  } 
;__yield_1; });
  
;__yield_0; });
}

void CodeGenerator__gen_function_decl(CodeGenerator *this, Function *func) {
  if (func->exits) 
  StringBuilder__puts((&this->out), "__attribute__((noreturn)) ");
  
  char *func_name = CodeGenerator__get_function_name(this, func);
  char *s = CodeGenerator__get_type_name_string(this, func->type, func_name, true);
  StringBuilder__putsf((&this->out), s);
}

void CodeGenerator__gen_function_decls(CodeGenerator *this, Program *program) {
  StringBuilder__puts((&this->out), "/* function declarations */\n");
  for (i32 i = 0; (i < program->functions->size); i += 1) {
    Function *func = ((Function *)Vector__at(program->functions, i));
    if ((!func->is_extern)) {
      CodeGenerator__gen_function_decl(this, func);
      StringBuilder__puts((&this->out), ";\n");
    } 
  } 
  StringBuilder__puts((&this->out), "\n");
}

void CodeGenerator__gen_function(CodeGenerator *this, Function *func) {
  if (func->is_extern) 
  return;
  
  CodeGenerator__gen_debug_info(this, func->span);
  CodeGenerator__gen_function_decl(this, func);
  StringBuilder__puts((&this->out), " ");
  if (func->is_arrow) {
    StringBuilder__puts((&this->out), "{\n");
    CodeGenerator__gen_statement(this, func->body, 1);
    StringBuilder__puts((&this->out), "}");
  }  else {
    CodeGenerator__gen_block(this, func->body, 0);
  } 
  StringBuilder__puts((&this->out), "\n\n");
}

void CodeGenerator__gen_global_vars(CodeGenerator *this, Program *program) {
  StringBuilder__puts((&this->out), "/* global variables */\n");
  for (i32 i = 0; (i < program->global_vars->size); i += 1) {
    AST *node = ((AST *)Vector__at(program->global_vars, i));
    if ((!node->u.var_decl.var->is_extern)) {
      CodeGenerator__gen_statement(this, node, 0);
    } 
  } 
  StringBuilder__puts((&this->out), "\n");
}

void CodeGenerator__gen_embed_headers(CodeGenerator *this, Program *program) {
  if ((!Vector__empty(program->c_embed_headers))) {
    for (i32 i = 0; (i < program->c_embed_headers->size); i += 1) {
      char *filename = ((char *)Vector__at(program->c_embed_headers, i));
      StringBuilder__putsf((&this->out), format_string("/***************** embed '%s' *****************/\n", filename));
      FILE *file = File__open(filename, "r");
      ;
      char *contents = File__slurp(file);
      ;
      StringBuilder__puts((&this->out), contents);
      StringBuilder__puts((&this->out), "\n\n");

      /* defers */
      free(contents);
      fclose(file);
    } 
  } 
}

char *CodeGenerator__gen_program(CodeGenerator *this, Program *program) {
  for (i32 i = 0; (i < program->c_includes->size); i += 1) {
    char *include = ((char *)Vector__at(program->c_includes, i));
    StringBuilder__putsf((&this->out), format_string("#include \"%s\"\n", include));
  } 
  StringBuilder__puts((&this->out), "\n");
  CodeGenerator__gen_embed_headers(this, program);
  CodeGenerator__gen_struct_decls(this, program);
  for (i32 i = 0; (i < program->structures->size); i += 1) {
    Structure *struc = ((Structure *)Vector__at(program->structures, i));
    if (struc->is_enum) {
      CodeGenerator__gen_enum(this, struc);
    }  else {
      CodeGenerator__gen_struct(this, struc);
    } 
  } 
  CodeGenerator__gen_function_decls(this, program);
  CodeGenerator__gen_global_vars(this, program);
  for (i32 i = 0; (i < program->functions->size); i += 1) {
    Function *func = ((Function *)Vector__at(program->functions, i));
    CodeGenerator__gen_function(this, func);
  } 
  return StringBuilder__str((&this->out));
}

void usage(i32 code) {
  printf("--------------------------------------------------------" "\n");
  printf("Usage: ./aecor [options] <file>" "\n");
  printf("Options:" "\n");
  printf("    -o path   Output executable (default: ./out)" "\n");
  printf("    -c path   Output C code (default: {out}.c)" "\n");
  printf("    -s        Silent mode (no debug output)" "\n");
  printf("    -n        Don't compile C code (default: false)" "\n");
  printf("    -d        Emit debug information (default: false)" "\n");
  printf("    -l        Library path (root of aecor repo)" "\n");
  printf("                   (Default: working directory)" "\n");
  printf("--------------------------------------------------------" "\n");
  exit(code);
}

i32 main(i32 argc, char **argv) {
  char *exec_path = "./out";
  char *c_path = ((char *)NULL);
  char *filename = ((char *)NULL);
  bool compile_c = true;
  bool silent = false;
  char *lib_path = ((char *)NULL);
  bool debug = false;
  for (i32 i = 1; (i < argc); i += 1) {
    {
      char *__match_str = argv[i];
      if (!strcmp(__match_str, "-h")) {
        usage(0);
      } else if (!strcmp(__match_str, "-s")) {
        silent = true;
      } else if (!strcmp(__match_str, "-d")) {
        debug = true;
      } else if (!strcmp(__match_str, "-n")) {
        compile_c = false;
      } else if (!strcmp(__match_str, "-o")) {
        i += 1;
        exec_path = argv[i];
      } else if (!strcmp(__match_str, "-l")) {
        i += 1;
        lib_path = argv[i];
      } else if (!strcmp(__match_str, "-c")) {
        i += 1;
        c_path = argv[i];
      } else  {
        if (argv[i][0] == '-') {
          printf("Unknown option: %s" "\n", argv[i]);
          usage(1);
        }  else         if ((!((bool)filename))) {
          filename = argv[i];
        }  else {
          printf("Unknown option/argument: '%s'" "\n", argv[i]);
          usage(1);
        } 
        
      }
    }
  } 
  if ((!((bool)filename))) {
    printf("No file specified" "\n");
    usage(1);
  } 
  if ((!((bool)c_path))) {
    c_path = format_string("%s.c", exec_path);
  } 
  FILE *file = File__open(filename, "r");
  char *contents = File__slurp(file);
  Lexer lexer = Lexer__make(contents, filename);
  Vector *tokens = Lexer__lex((&lexer));
  Parser *parser = Parser__new(tokens, filename);
  if (((bool)lib_path)) {
    Parser__add_include_dir(parser, lib_path);
  } 
  Program *program = Parser__parse_program(parser);
  TypeChecker *checker = TypeChecker__new();
  TypeChecker__check_program(checker, program);
  CodeGenerator generator = CodeGenerator__make(debug);
  char *c_code = CodeGenerator__gen_program((&generator), program);
  FILE *out_file = File__open(c_path, "w");
  File__puts(out_file, c_code);
  fclose(out_file);
  if ((!compile_c)) {
    return 0;
  } 
  char *cmdbuf = ((char *)calloc(1, 1024));
  strcat(cmdbuf, format_string("gcc -o %s %s", exec_path, c_path));
  for (i32 i = 0; (i < program->c_flags->size); i += 1) {
    char *flag = ((char *)Vector__at(program->c_flags, i));
    strcat(cmdbuf, " ");
    strcat(cmdbuf, flag);
  } 
  if (debug) {
    strcat(cmdbuf, " -ggdb3");
  } 
  if ((!silent)) {
    printf("[+] %s" "\n", cmdbuf);
  } 
  i32 code = system(cmdbuf);
  if ((code != 0)) {
    printf("[-] Compilation failed" "\n");
    exit(code);
  } 
}

