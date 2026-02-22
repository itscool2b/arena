# Arena Allocator

A linear allocator for fast, contiguous memory allocations.

## Include

```c
#include "src/arena.h"
```

## Functions

### arena_create

```c
Arena *arena_create(u64 capacity)
```

Creates an arena with the specified capacity in bytes.

**Parameters:**
- `capacity`: Size of the arena in bytes

**Returns:** Pointer to the created arena, or NULL if allocation fails.

**Example:**
```c
Arena *arena = arena_create(1024 * 1024);  // 1MB arena
```

---

### arena_push

```c
void *arena_push(Arena *arena, u64 size)
```

Allocates memory from the arena. Alignment is handled automatically.

**Parameters:**
- `arena`: The arena to allocate from
- `size`: Number of bytes to allocate

**Returns:** Pointer to allocated memory, or NULL if arena is full.

**Example:**
```c
int *numbers = arena_push(arena, 100 * sizeof(int));
char *buffer = arena_push(arena, 256);
```

---

### arena_pop

```c
void arena_pop(Arena *arena, u64 size)
```

Frees memory back to the arena (LIFO order).

**Parameters:**
- `arena`: The arena to pop from
- `size`: Number of bytes to free

**Example:**
```c
arena_pop(arena, 256);        // Free the buffer
arena_pop(arena, 100 * sizeof(int));  // Free the numbers
```

---

### arena_pop_to

```c
void arena_pop_to(Arena *arena, u64 position)
```

Resets arena position to a specific point.

**Parameters:**
- `arena`: The arena to modify
- `position`: Position to reset to (obtained from arena->position)

**Example:**
```c
u64 saved = arena->position;
void *temp = arena_push(arena, 1024);
// ... use temp ...
arena_pop_to(arena, saved);  // Reset to saved position
```

---

### arena_clear

```c
void arena_clear(Arena *arena)
```

Frees all allocations in the arena.

**Parameters:**
- `arena`: The arena to clear

**Example:**
```c
arena_clear(arena);  // Reset to empty state
```

---

### arena_destroy

```c
void arena_destroy(Arena *arena)
```

Frees the arena and all its memory.

**Parameters:**
- `arena`: The arena to destroy

**Example:**
```c
arena_destroy(arena);
```

---

## Complete Example

```c
#include "src/arena.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    float x, y, z;
} Vec3;

int main() {
    // Create 1MB arena
    Arena *arena = arena_create(1024 * 1024);
    if (!arena) return 1;
    
    // Allocate some vectors
    Vec3 *v1 = arena_push(arena, sizeof(Vec3));
    v1->x = 1.0f; v1->y = 2.0f; v1->z = 3.0f;
    
    Vec3 *v2 = arena_push(arena, sizeof(Vec3));
    v2->x = 4.0f; v2->y = 5.0f; v2->z = 6.0f;
    
    // Allocate array
    int *nums = arena_push(arena, 100 * sizeof(int));
    for (int i = 0; i < 100; i++) nums[i] = i;
    
    // Save position for temporary data
    u64 temp_start = arena->position;
    char *temp = arena_push(arena, 4096);
    sprintf(temp, "Temporary buffer");
    
    // Discard temporary data
    arena_pop_to(arena, temp_start);
    
    // Or clear everything at once
    arena_clear(arena);
    
    // Cleanup
    arena_destroy(arena);
    return 0;
}
```

---

## Notes

- All allocations are aligned to `sizeof(void*)`
- Memory is not zero-initialized
- Arena uses a single malloc/free for the entire lifetime
- LIFO deallocation only (cannot free arbitrary allocations)
