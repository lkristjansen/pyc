#ifndef PYC_PARSER_H
#define PYC_PARSER_H

#include "arena.h"
#include "stringutil.h"

#include <stdint.h>
#include <stdio.h>

enum
{
  Pyc_Ast_Constant=0,
  Pyc_Ast_Name,
  Pyc_Ast_Call,
  Pyc_Ast_BinaryOp,
  Pyc_Ast_UnaryOp,
  Pyc_Ast_Stmt,
  Pyc_Ast_Module
};

typedef struct Pyc_Ast
{
  int type;

  union
  {
    int64_t constant;

    Pyc_CharSpan name;
    
    struct {
      struct Pyc_Ast *name;
      struct Pyc_Ast *exp;
    } call;

    struct {
      Pyc_CharSpan op;
      struct Pyc_Ast *left;
      struct Pyc_Ast *right;
    } binary_op;

    struct {
      Pyc_CharSpan op;
      struct Pyc_Ast *expr;
    } unary_op;

    struct Pyc_Ast *stmt; // print(expr) is the only supported stmt currently. 
    
    struct Pyc_Ast *module; // in first version modules only has one stmt. 
  };

} Pyc_Ast;

Pyc_Ast *pyc_parse(Pyc_Arena *arena, const char *text);

void pyc_ast_dump(Pyc_Arena *arena, FILE *file, Pyc_Ast *ast);

#endif // PYC_PARSER_H