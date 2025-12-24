#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct Region Region;

typedef struct Arena {
    Region* head;
    Region* tail;
    size_t regions;
    // TODO Thread safety
} Arena;

Arena Arena_new(void);
uint8_t* Arena_alloc(Arena* arena, size_t bytes);
void Arena_destroy(Arena* arena);
void Arena_debug_print(Arena* arena);

typedef struct {
    uint8_t* start;
    size_t length;
} BytesSlice;

typedef struct {
    uint8_t* bytes;
    size_t lenth;
    size_t capacity;
} Bytes;

Bytes Bytes_init(void);
Bytes Bytes_from_slice(BytesSlice slice);

void Bytes_push(Bytes* self, uint8_t byte);
uint8_t Bytes_pop(Bytes* self);
