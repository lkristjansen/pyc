#ifndef PYC_ARENA_H
#define PYC_ARENA_H

#include <stddef.h>

typedef struct {
  void *ptr;
  size_t index;
  size_t size;
} Pyc_Arena;

void Pyc_Arena_init(Pyc_Arena *arena, size_t size);

void *Pyc_Arena_alloc(Pyc_Arena *arena, size_t size);

void Pyc_Arena_free(Pyc_Arena *arena);
  
#endif // PYC_ARENA_H
