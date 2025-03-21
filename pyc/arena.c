#include "arena.h"

#include <assert.h>
#include <stdlib.h>

void Pyc_Arena_init(Pyc_Arena *arena, size_t size)
{
  arena->ptr = malloc(size);
  assert(arena->ptr != NULL);

  arena->index = 0;
  arena->size = size;
}

void *Pyc_Arena_alloc(Pyc_Arena *arena, size_t size)
{
  assert(size < (arena->size - arena->index));
  void *ptr = arena->ptr;
  arena->index += size;
  return ptr;
}

void Pyc_Arena_free(Pyc_Arena *arena)
{
  assert(arena != NULL);
  free(arena->ptr);
}
