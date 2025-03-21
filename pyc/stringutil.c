#include "stringutil.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

Pyc_CharSpan pyc_charspan_from_cstr(const char *cstr)
{
  size_t size = strlen(cstr);
  Pyc_CharSpan span = {0};
  span.size = size;
  span.data = cstr;
  return span;
}

Pyc_CharSpan pyc_charspan_subspan(
    Pyc_CharSpan span,
    size_t index,
    size_t size)
{
  Pyc_CharSpan sub_span = {0};
  sub_span.data = span.data + index;
  sub_span.size = size;
  return sub_span;
}

int Pyc_CharSpan_equals(Pyc_CharSpan x, Pyc_CharSpan y)
{
  if (x.size != y.size)
    return -1;

  return strncmp(x.data, y.data, y.size) == 0;
}

int Pyc_CharSpan_equals_cstr(Pyc_CharSpan x, const char *cstr)
{
  size_t cstr_size = strlen(cstr);

  if (cstr_size != x.size)
    return -1;

  size_t min_size = x.size < cstr_size ? x.size : cstr_size;
  return strncmp(x.data, cstr, min_size);
}

int64_t pyc_charspan_to_int(Pyc_CharSpan span)
{
  int64_t value = 0;
  size_t index = span.size - 1;
  size_t power = 0;

  while (index != 0)
  {
    value += pow(10, power) * (span.data[index] - '0');
    index--;
    power++;
  }

  if (span.data[index] == '-')
    value *= -1;
  else
    value += pow(10, power) * (span.data[index] - '0');

  return value;
}

const char *pyc_charspan_to_cstr(Pyc_Arena *arena, Pyc_CharSpan span)
{
  char *cstr = pyc_arena_alloc(arena, span.size + 1);
  strncpy(cstr, span.data, span.size);
  cstr[span.size] = '\0';
  return cstr;
}
