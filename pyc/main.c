#include "lexer.h"
#include "arena.h"
#include "parser.h"

#include <stdio.h>

void print_lexeme(Pyc_Arena *arena, Pyc_Lexeme lexeme)
{
  switch (lexeme.type)
  {
  case Pyc_Lexeme_Fixnum:
    printf("FIXNUM(%ld)\n", lexeme.fixnum);
    break;
  case Pyc_Lexeme_Operator:
    printf("OPERATOR(%c)\n", lexeme.op.data[0]);
    break;
  case Pyc_Lexeme_Name:
    printf("NAME(%s)\n", pyc_charspan_to_cstr(arena, lexeme.name));
    break;
  case Pyc_Lexeme_Eof:
    printf("EOF\n");
    break;
  }
}

int main()
{
  Pyc_Arena arena = {0};
  pyc_arena_init(&arena, 1024 * 1024);

  Pyc_Ast *ast = pyc_parse(&arena, "print(10)");
  
  pyc_ast_dump(&arena, stdout, ast);

  pyc_arena_free(&arena);

  return 0;
}
