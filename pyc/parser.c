
#include "arena.h"
#include "lexer.h"
#include "stringutil.h"
#include "parser.h"

#include <stddef.h>
#include <stdlib.h>

static Pyc_Ast *parse_expr(Pyc_Arena *arena, Pyc_Lexer* lexer);

static void match(Pyc_Lexer *lexer, int token_type)
{
  Pyc_Lexeme lexeme = pyc_lexer_next(lexer);
  if (lexeme.type != token_type)
  {
    fprintf(stderr, "error: unexpected token type, expected: '%d', got: '%d'\n", token_type, lexeme.type);
    exit(EXIT_FAILURE);
  }
}

static Pyc_Ast *parse_arguments(Pyc_Arena *arena, Pyc_Lexer *lexer)
{
  if (pyc_lexer_peek(*lexer).type == Pyc_Lexeme_Right_Paren)
    return NULL;

  // TODO(lkr): support multiple arguments for call nodes.
  return parse_expr(arena, lexer);
}

static Pyc_Ast *parse_call(Pyc_Arena *arena, Pyc_Lexer *lexer, Pyc_CharSpan name)
{
  Pyc_Ast* call = pyc_arena_alloc(arena, sizeof(Pyc_Ast));
  call->type = Pyc_Ast_Call;
  call->call.name = pyc_arena_alloc(arena, sizeof(Pyc_Ast));
  call->call.name->type = Pyc_Ast_Name;
  call->call.name->name = name;

  match(lexer, Pyc_Lexeme_Left_Paren);

  call->call.exp = parse_arguments(arena, lexer);

  match(lexer, Pyc_Lexeme_Right_Paren);

  return call;
}

static Pyc_Ast *parse_expr(Pyc_Arena *arena, Pyc_Lexer* lexer)
{
  Pyc_Lexeme lexeme = pyc_lexer_next(lexer);

  if (lexeme.type == Pyc_Lexeme_Fixnum) {
    Pyc_Ast *constant = pyc_arena_alloc(arena, sizeof(Pyc_Ast));
    constant->type = Pyc_Ast_Constant;
    constant->constant = lexeme.fixnum;
    return constant;
  } else if (lexeme.type == Pyc_Lexeme_Name) {
    return parse_call(arena, lexer, lexeme.name);
  }

  fprintf(stderr, "error: unexpected token type.\n");
  exit(EXIT_FAILURE);

}

static Pyc_Ast *parse_stmt(Pyc_Arena *arena, Pyc_Lexer* lexer)
{
  Pyc_Ast *stmt = pyc_arena_alloc(arena, sizeof(Pyc_Ast));
  stmt->type = Pyc_Ast_Stmt;
  stmt->stmt = parse_expr(arena, lexer);
  return stmt;
}

Pyc_Ast *pyc_parse(Pyc_Arena *arena, const char *text)
{
  Pyc_Lexer lexer = pyc_lexer_init(text);
  Pyc_Ast *module = pyc_arena_alloc(arena, sizeof(Pyc_Ast));
  module->type = Pyc_Ast_Module;
  module->module = parse_stmt(arena, &lexer);
  return module;
}

void pyc_ast_dump(Pyc_Arena *arena, FILE *file, Pyc_Ast *ast)
{
  switch (ast->type)
  {
    case Pyc_Ast_Constant:
      fprintf(file, "Constant(%ld)", ast->constant);
      break;
    case Pyc_Ast_Name:
      fprintf(file, "Name(%s)", pyc_charspan_to_cstr(arena, ast->name));
      break;
    case Pyc_Ast_Call:
      fprintf(file, "Call(");
      pyc_ast_dump(arena, file, ast->call.name);
      fprintf(file, ", [");
      
      if (ast->call.exp != NULL)
        pyc_ast_dump(arena, file, ast->call.exp);
      
      fprintf(file, "]");
      fprintf(file, ")");
      break;
    case Pyc_Ast_BinaryOp:
      fprintf(file, "BinaryOp(");
      pyc_ast_dump(arena, file, ast->binary_op.left);
      fprintf(file, ", %c, ", ast->binary_op.op.data[0]);
      pyc_ast_dump(arena, file, ast->binary_op.right);
      fprintf(file, ")");
      break;
    case Pyc_Ast_UnaryOp:
      fprintf(file, "UnaryOp('%c', ", ast->unary_op.op.data[0]);
      pyc_ast_dump(arena, file, ast->unary_op.expr);
      fprintf(file, ")");
      break;
    case Pyc_Ast_Stmt:
      fprintf(file, "Statement(");
      pyc_ast_dump(arena, file, ast->stmt);
      fprintf(file, ")");
      break;
    case Pyc_Ast_Module:
    {
      fprintf(file, "Module(");
      pyc_ast_dump(arena, file, ast->module);
      fprintf(file, ")\n");
    } break;
    default:
      fprintf(stderr, "error: unrecognized ast type.");
      exit(EXIT_FAILURE);
  }
}
