#include "arena.h"
#include <stdlib.h>

/* realistically alignment should be a multiple of 2, bit shifts would be a lot
 * faster than division 
 * it's a problem when we want to have a struct array for exmaple
*/

typedef struct Arena {
  void *data;
  u64 size;
  u64 pos;
  u64 align;
} Arena;


Arena *arena_alloc(u64 cap) {
  if (cap < sizeof(Arena)) {
    return NULL;
  }
  void *data = (void *)malloc(cap);
  Arena *arena = (Arena *)data;
  arena->size = cap;
  arena->pos = sizeof(Arena);
  arena->data = data;
  return arena;
}

void arena_release(Arena *arena) {
  free(arena->data);
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
  arena->pos += size;
  if (arena->pos >= arena->size) {
    *(int *)(0); // panic
  }
  for (u64 i = 0; i < size; ++i) {
    ((u8 *)res)[i] = 0;
  }
  return res;
}

void arena_pop_to(Arena *arena, u64 pos) {
  if (pos >= arena->pos) {
    return;
  }
  if (arena->align) {
    pos = ((pos / arena->align) + 1) * arena->align;
  }
  arena->pos = pos;
}

void arena_pop(Arena *arena, u64 size) {
  if (size >= arena->pos) {
    arena->pos = 0;
    return;
  }
  arena->pos -= size;
  if (arena->align) {
    arena->pos = ((arena->pos / arena->align) + 1) * arena->align;
  }
}

void arena_clear(Arena *arena) { arena->pos = 0; }
