#include "arena.h"

#include <assert.h>
#include <stdlib.h>

void pyc_arena_init(Pyc_Arena *arena, size_t size)
{
  arena->ptr = malloc(size);
  assert(arena->ptr != NULL);

  arena->index = 0;
  arena->size = size;
}

void *pyc_arena_alloc(Pyc_Arena *arena, size_t size)
{
  assert(size < (arena->size - arena->index));
  void *ptr = arena->ptr + arena->index;
  arena->index += size;
  return ptr;
}

void pyc_arena_free(Pyc_Arena *arena)
{
  assert(arena != NULL);
  free(arena->ptr);
}
