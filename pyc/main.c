#include "lexer.h"
#include "arena.h"

#include <stdio.h>
#include <string.h>

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
    printf("NAME(%s)\n", Pyc_CharSpan_to_cstr(arena, lexeme.name));
    break;
  case Pyc_Lexeme_Eof:
    printf("EOF\n");
    break;
  }
}

int main()
{
  Pyc_Arena arena = {0};
  Pyc_Arena_init(&arena, 1024 * 1024);

  Pyc_Lexer lexer = Pyc_Lexer_init("name(30*40+2)");

  Pyc_Lexeme lexeme;
  do
  {
    lexeme = Pyc_Lexer_next(&lexer);
    print_lexeme(&arena, lexeme);
  } while (lexeme.type != Pyc_Lexeme_Eof);

  Pyc_Arena_free(&arena);

  return 0;
}
