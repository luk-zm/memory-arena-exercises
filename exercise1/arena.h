#ifndef ARENA
#define ARENA

typedef unsigned long u64;
typedef unsigned char u8;


typedef struct Arena {
  void *data;
  u64 size;
  u64 pos;
  u64 align;
} Arena;

Arena *arena_alloc(u64 cap);
void arena_release(Arena *arena);
void arena_set_auto_align(Arena *arena, u64 align);

u64 arena_pos(Arena *arena);

void *arena_push_no_zero(Arena *arena, u64 size);
void *arena_push_aligner(Arena *Arena, u64 alignment);
void *arena_push(Arena *arena, u64 size);

void arena_pop_to(Arena *arena, u64 pos);
void arena_pop(Arena *arena, u64 size);
void arena_clear(Arena *arena);

#endif
