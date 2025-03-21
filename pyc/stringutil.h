#ifndef PYC_STRINGUTIL_H
#define PYC_STRINGUTIL_H

#include <stddef.h>
#include <stdint.h>

#include "arena.h"

typedef struct
{
  size_t size;
  const char *data;
} Pyc_CharSpan;

Pyc_CharSpan pyc_charspan_from_cstr(const char *cstr);

Pyc_CharSpan pyc_charspan_subspan(Pyc_CharSpan span, size_t index, size_t size);

int pyc_charspan_compare(Pyc_CharSpan x, Pyc_CharSpan y);

int pyc_charspan_compare_cstr(Pyc_CharSpan x, const char *cstr);

int64_t pyc_charspan_to_int(Pyc_CharSpan span);

const char *pyc_charspan_to_cstr(Pyc_Arena *arena, Pyc_CharSpan span);

#endif // PYC_STRINGUTIL_H
