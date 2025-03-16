#include "arena.h"
#include <stdlib.h>

/* realistically alignment should be a multiple of 2, bit shifts would be a lot
 * faster than division 
 * it's a problem when we want to have a struct array for exmaple
*/

Arena *arena_alloc(u64 cap) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  arena->size = cap;
  arena->data = (void *)malloc(cap);
  return arena;
}

void arena_release(Arena *arena) {
  free(arena->data);
  free(arena);
}

void arena_set_auto_align(Arena *arena, u64 align) { arena->align = align; }

u64 arena_pos(Arena *arena) { return arena->pos; }

void *arena_push_no_zero(Arena *arena, u64 size) {
  if (arena->align && size % arena->align != 0) {
    size = ((size / arena->align) + 1) * arena->align;
  }
  void *res = arena->data + arena->pos;
  arena->pos += size;
  if (arena->pos >= arena->size) {
    *(int *)(0); // panic
  }
  return res;
}

void *arena_push_aligner(Arena *arena, u64 alignment) {
  if (arena->pos % alignment == 0)
    return arena->data + arena->pos;
  arena->pos = ((arena->pos / alignment) + 1) * alignment;
  return arena->data + arena->pos;
}

void *arena_push(Arena *arena, u64 size) {
  if (arena->align && size % arena->align != 0) {
    size = ((size / arena->align) + 1) * arena->align;
  }
  void *res = arena->data + arena->pos;
  for (u64 i = 0; i < size; ++i) {
    ((u8 *)res)[i] = 0;
  }
  arena->pos += size;
  if (arena->pos >= arena->size) {
    *(int *)(0); // panic
  }
  return res;
}

void arena_pop_to(Arena *arena, u64 pos) {
  if (arena->align) {
    pos = ((pos / arena->align) + 1) * arena->align;
  }
  arena->pos = pos;
}

void arena_pop(Arena *arena, u64 size) {
  arena->pos -= size;
  if (arena->align) {
    arena->pos = ((arena->pos / arena->align) + 1) * arena->align;
  }
}

void arena_clear(Arena *arena) { arena->pos = 0; }
