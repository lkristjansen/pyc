#ifndef PYC_LEXER_H
#define PYC_LEXER_H

#include "stringutil.h"

#include <stddef.h>
#include <stdint.h>

enum
{
  Pyc_Lexeme_Eof,
  Pyc_Lexeme_Fixnum,
  Pyc_Lexeme_Left_Paren,
  Pyc_Lexeme_Right_Paren,
  Pyc_Lexeme_Operator,
  Pyc_Lexeme_Name
};

typedef struct
{
  int type;

  union
  {
    int64_t fixnum;
    Pyc_CharSpan name;
    Pyc_CharSpan op;
  };

} Pyc_Lexeme;

typedef struct
{
  size_t index;
  Pyc_CharSpan text;
} Pyc_Lexer;

Pyc_Lexer Pyc_Lexer_init(const char *text);

Pyc_Lexeme Pyc_Lexer_next(Pyc_Lexer *lexer);

Pyc_Lexeme Pyc_Lexer_peek(Pyc_Lexer lexer);

#endif // PYC_LEXER_H
