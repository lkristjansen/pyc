#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define is_empty(lexer) (lexer->index >= lexer->text.size)

#define next(lexer) (lexer->index++)

#define current(lexer) (lexer->text.data[lexer->index])

static Pyc_Lexeme make_eof()
{
  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Eof;
  return lexeme;
}

static Pyc_Lexeme make_fixnum(int64_t value)
{
  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Fixnum;
  lexeme.fixnum = value;
  return lexeme;
}

static Pyc_Lexeme make_name(Pyc_CharSpan name)
{
  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Name;
  lexeme.name = name;
  return lexeme;
}

static Pyc_Lexeme lex_fixnum(Pyc_Lexer *lexer)
{
  size_t start_index = lexer->index;
  next(lexer);

  while (!is_empty(lexer) && isdigit(current(lexer)))
    next(lexer);

  size_t size = lexer->index - start_index;
  Pyc_CharSpan fixnum = Pyc_CharSpan_subspan(lexer->text, start_index, size);
  return make_fixnum(Pyc_CharSpan_to_int(fixnum));
}

Pyc_Lexeme lex_operator(Pyc_Lexer *lexer)
{
  Pyc_CharSpan op = {0};
  op.data = lexer->text.data + lexer->index;
  op.size = 1;

  next(lexer);

  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Operator;
  lexeme.op = op;
  return lexeme;
}

Pyc_Lexeme lex_name(Pyc_Lexer *lexer)
{
  size_t start_index = lexer->index;
  next(lexer);

  while (!is_empty(lexer) && isalnum(current(lexer)))
    next(lexer);

  size_t size = lexer->index - start_index;
  Pyc_CharSpan name = Pyc_CharSpan_subspan(lexer->text, start_index, size);
  return make_name(name);
}

static Pyc_Lexeme lex_left_paren(Pyc_Lexer *lexer)
{
  next(lexer);
  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Left_Paren;
  return lexeme;
}

static Pyc_Lexeme lex_right_paren(Pyc_Lexer *lexer)
{
  next(lexer);
  Pyc_Lexeme lexeme = {0};
  lexeme.type = Pyc_Lexeme_Right_Paren;
  return lexeme;
}

Pyc_Lexeme Pyc_Lexer_next(Pyc_Lexer *lexer)
{
  if (is_empty(lexer))
    return make_eof();

  while (isspace(current(lexer)))
    next(lexer);

  const char ch = current(lexer);

  if (isdigit(ch))
    return lex_fixnum(lexer);
  else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
    return lex_operator(lexer);
  else if (isalpha(ch))
    return lex_name(lexer);
  else if (ch == '(')
    return lex_left_paren(lexer);
  else if (ch == ')')
    return lex_right_paren(lexer);

  fprintf(stderr, "unexpected token '%c'.\n", ch);
  exit(EXIT_FAILURE);
}

Pyc_Lexer Pyc_Lexer_init(const char *text)
{
  Pyc_Lexer lexer = {0};
  lexer.index = 0;
  lexer.text = Pyc_CharSpan_from_cstr(text);
  return lexer;
}

Pyc_Lexeme Pyc_Lexer_peek(Pyc_Lexer lexer)
{
  return Pyc_Lexer_next(&lexer);
}
