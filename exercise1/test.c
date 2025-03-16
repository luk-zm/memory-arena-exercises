#include "arena.h"
#include <stdio.h>

int main() {
  Arena *int_arena = arena_alloc(1024 * 1024);
  int arr_size = 24;
  int *arr = (int *)arena_push(int_arena, sizeof(int) * arr_size);
  for (int i = 0; i < arr_size; ++i) {
    arr[i] = (i + 1) * (i + 1);
  }

  for (int i = 0; i < arr_size; ++i) {
    printf("%d, ", arr[i]);
  }
  printf("\n");
}
