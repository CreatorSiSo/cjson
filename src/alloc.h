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
    void* data;
    size_t length;
} AnySlice;

typedef struct {
    uint8_t* data;
    size_t length;
} ByteSlice;

ByteSlice ByteSlice_new(uint8_t* data, size_t length);

typedef struct {
    void* data;
    size_t length;
    size_t capacity;
} AnyBuffer;

typedef struct {
    uint8_t* data;
    size_t length;
    size_t capacity;
} ByteBuffer;

// AnyBuffer Buffer_from_slice(ByteSlice slice, Arena* arena);

#define Buffer_new() {.data = nullptr, .length = 0, .capacity = 0}
#define Buffer_cast(buffer, buffer_type) (*(buffer_type*)&(buffer))

ByteSlice Buffer_as_bytes(AnyBuffer* self);

void Buffer_grow(AnyBuffer* data, Arena* arena, size_t by);

#define Buffer_push(buffer, arena, element)                      \
    Buffer_grow((AnyBuffer*)(buffer), (arena), sizeof(element)); \
    (buffer)->data[(buffer)->length] = element;                  \
    (buffer)->length += 1;

// #define Buffer_pop(buffer)      \
//     assert((buffer)->size > 0); \
//     (buffer)->size -= 1;        \
//     (buffer)->data[buffer->size];
