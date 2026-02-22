#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//macros - I will set the alignment to the size of a void pointer instead of the user sepcifying
#define ALIGNMENT sizeof(void*)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {

  u64 capacity;
  u64 position;

} Arena;

//create the arena
Arena *arena_create(u64 capacity);

//push - returns a void pointer
void *arena_push(Arena *arena, u64 size);

//arena pop
void arena_pop(Arena *arena, u64 size);

//pop to
void arena_pop_to(Arena *arena, u64 position);

//arena clear
void arena_clear(Arena *arena);

//destory arena
void arena_destroy(Arena *arena);

int main(void){




  return 0;
}


Arena *arena_create(u64 capacity)

{

  Arena *arena = malloc(sizeof(*arena) + capacity);
  arena->capacity = capacity;
  arena->position = sizeof(*arena);

  return arena;

}

void *arena_push(Arena *arena, u64 size)

{

  u64 aligned_size = (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

  if (arena->position + aligned_size > sizeof(*arena) + arena->capacity)
  {

    return NULL;
  }

void *result = (u8*)arena + arena->position;
  arena->position += aligned_size;

  return result;

  }

void arena_pop(Arena *arena, u64 size)
{

  u64 aligned_size = (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
  arena->position -= aligned_size;
  if (arena->position < sizeof(*arena)){
  
    arena->position = sizeof(*arena);
    
}


}

void arena_pop_to(Arena *arena, u64 position)
{

  if (position < sizeof(*arena)) {

    position = sizeof(*arena);

}
  if (position > arena->position){

    position = arena->position;

}

  arena->position = position;

}

void arena_clear(Arena *arena)
{
  arena->position = sizeof(*arena);
}

void arena_destroy(Arena *arena){

  free(arena);
}
