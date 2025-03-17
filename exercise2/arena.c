#include "arena.h"
#include <stdlib.h>

/* realistically alignment should be a multiple of 2, bit shifts would be a lot
 * faster than division 
*/

typedef struct ArenaChunk {
  void *data;
  u64 local_pos;
  struct ArenaChunk *next;
} ArenaChunk;

typedef struct Arena {
  ArenaChunk *chunks;
  u64 cap;
  u64 size;
  u64 global_pos;
  u64 align;
  u64 chunk_count;
} Arena;

Arena *arena_alloc(u64 cap) {
  if (cap < (sizeof(Arena) + sizeof(ArenaChunk))) {
    return NULL;
  }
  void *data = (void *)malloc(cap);
  Arena *arena = (Arena *)data;
  arena->cap = cap;
  arena->size = cap;
  arena->align = 0;
  arena->chunk_count = 1;

  ArenaChunk *arena_chunk = (ArenaChunk *)(data + sizeof(Arena));
  arena_chunk->local_pos = sizeof(Arena) + sizeof(ArenaChunk);
  arena_chunk->data = data;
  arena_chunk->next = NULL;

  arena->global_pos = arena_chunk->local_pos;
  arena->chunks = arena_chunk;
  return arena;
}

void arena_release(Arena *arena) {
  ArenaChunk *current_chunk = arena->chunks;
  while (current_chunk != NULL) {
    ArenaChunk *next_chunk = current_chunk->next;
    free(current_chunk->data);
    current_chunk = next_chunk;
  }
}

// if set after some pushes should this fix alignment or
// is it supposed to be called only right after an alloc?
void arena_set_auto_align(Arena *arena, u64 align) { arena->align = align; }

u64 arena_pos(Arena *arena) { return arena->global_pos; }

void *arena_push_no_zero(Arena *arena, u64 size) {
  if (size > arena->cap) {
    return NULL;
  }
  if (arena->align && size % arena->align != 0) {
    size = ((size / arena->align) + 1) * arena->align;
  }
  void *res = NULL;
  arena->global_pos += size;
  u64 new_pos = arena->chunks->local_pos + size;
  if (new_pos >= arena->cap) {
    void *new_data = malloc(arena->cap);

    ArenaChunk *new_chunk = (ArenaChunk *)new_data;
    new_chunk->data = new_data;
    new_chunk->local_pos = sizeof(ArenaChunk);
    new_chunk->next = arena->chunks;
    new_chunk->local_pos += size;

    arena->size += arena->cap;
    arena->chunks = new_chunk;
    arena->chunk_count++;

    res = new_data + new_chunk->local_pos;
  } 
  else {
    res = arena->chunks->data + arena->chunks->local_pos;
    arena->chunks->local_pos = new_pos;
  }
  return res;
}

void *arena_push_aligner(Arena *arena, u64 alignment) {
  if (arena->chunks->local_pos % alignment == 0)
    return arena->chunks->data + arena->chunks->local_pos;
  arena->chunks->local_pos = ((arena->chunks->local_pos / alignment) + 1) * alignment;
  return arena->chunks->data + arena->chunks->local_pos;
}

void *arena_push(Arena *arena, u64 size) {
  void *res = arena_push_no_zero(arena, size);
  for (int i = 0; i < size; ++i) {
    ((u8 *)res)[i] = 0;
  }
  return res;
}

void pop_chunk(Arena *arena) {
  ArenaChunk *chunk_to_pop = arena->chunks;
  if (chunk_to_pop) {
    arena->chunks = arena->chunks->next;
    free(chunk_to_pop);
  }
}

void arena_pop_to(Arena *arena, u64 pos) {
  if (pos >= arena->global_pos)
    return;
  if (arena->align) {
    pos = ((pos / arena->align) + 1) * arena->align;
  }
  int chunks_to_pop = (arena->global_pos - pos) / arena->cap;
  for (int i = 0; i < chunks_to_pop; ++i) {
    pop_chunk(arena);
  }
  arena->global_pos = pos;
}

void arena_pop(Arena *arena, u64 size) {
  arena_pop_to(arena, arena->global_pos - size);
}

void arena_clear(Arena *arena) {
  while (arena->chunk_count != 1) {
    pop_chunk(arena);
    arena->chunk_count--;
  }
  arena->global_pos = 0;
  arena->chunks->local_pos = 0;
}
