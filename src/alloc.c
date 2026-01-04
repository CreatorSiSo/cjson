#include "alloc.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <valgrind.h>

#define PAGE_SIZE (sysconf(_SC_PAGE_SIZE))

struct Region {
    struct Region* next;
    uint8_t* bytes;
    size_t remaining;
};

Region* Region_new(size_t size) {
    void* page = mmap(
        nullptr,
        size,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE,
        -1,
        0);
    VALGRIND_MALLOCLIKE_BLOCK(page, size, 0, 0);
    assert(page != MAP_FAILED);

    Region* region = (Region*)page;
    *region = (Region){
        .next = nullptr,
        .bytes = (uint8_t*)page + sizeof(Region),
        .remaining = size - sizeof(Region),
    };

    return region;
}

uint8_t* Region_try_alloc(Region* region, size_t size) {
    assert(region != nullptr);
    if (size > region->remaining) {
        return nullptr;
    }

    // TODO: Alignment
    uint8_t* bytes = region->bytes;
    region->bytes += size;
    region->remaining -= size;
    return bytes;
}

void Region_destroy(Region* region) {
    assert(region != nullptr);

    VALGRIND_FREELIKE_BLOCK(region, 0);
    int result = munmap((void*)region, PAGE_SIZE);
    assert(result == 0);
}

void Region_debug_print(Region* region) {
    assert(region != nullptr);

    fprintf(
        stderr,
        "Region(%p) {\n"
        "  .next = %p,\n"
        "  .bytes = %p,\n"
        "  .remaining = %zu,\n"
        "}\n",
        (void*)region,
        (void*)region->next,
        region->bytes,
        region->remaining);
}

Arena Arena_new(void) {
    Arena arena;
    arena.head = Region_new(PAGE_SIZE);
    arena.tail = arena.head;
    arena.regions = 1;
    return arena;
}

Region* Arena_new_region(Arena* arena, size_t size) {
    Region* region = Region_new(size);
    arena->tail->next = region;
    arena->tail = region;
    arena->regions += 1;
    return region;
}

uint8_t* Arena_alloc(Arena* arena, size_t size) {
    Region* current = arena->head;
    Region* next;
    while (current != nullptr) {
        next = current->next;
        uint8_t* bytes = Region_try_alloc(current, size);
        if (bytes != nullptr) {
            return bytes;
        }
        current = next;
    }

    // Did not find a large enough slot in the already allocated regions
    size_t region_size =
        size < (size_t)PAGE_SIZE ? (size_t)PAGE_SIZE : sizeof(Region) + size;
    Region* tail = Arena_new_region(arena, region_size);
    return Region_try_alloc(tail, size);
}

void Arena_destroy(Arena* arena) {
    Region* current = arena->head;
    Region* next;
    while (current != nullptr) {
        next = current->next;
        Region_destroy(current);
        current = next;
    }
    arena->head = nullptr;
    arena->tail = nullptr;
}

void Arena_debug_print(Arena* arena) {
    assert(arena != nullptr);

    Region* current = arena->head;
    Region* next;
    while (current != nullptr) {
        next = current->next;
        Region_debug_print(current);
        current = next;
    }
}

ByteSlice ByteSlice_new(uint8_t* data, size_t length) {
    return (ByteSlice){.data = data, .length = length};
}

// AnyBuffer Buffer_from_slice(ByteSlice slice, Arena* arena) {
//     AnyBuffer buf = (AnyBuffer){
//         .data = Arena_alloc(arena, slice.length),
//         .size = slice.length,
//         .capacity = slice.length,
//     };
//     assert(buf.data != nullptr);

//     memcpy(buf.data, slice.start, buf.size);

//     return buf;
// }

ByteSlice Buffer_as_bytes(AnyBuffer* self) {
    return (ByteSlice){.data = self->data, .length = self->length};
}

void Buffer_grow(AnyBuffer* buffer, Arena* arena, size_t by) {
    if (buffer->length + by <= buffer->capacity) {
        return;
    }
    if (buffer->capacity == 0) {
        constexpr size_t STARTING_CAPACITY = 128;
        buffer->capacity = STARTING_CAPACITY;

        buffer->data = Arena_alloc(arena, buffer->capacity);
        assert(buffer->data != nullptr);

        return;
    } else {
        buffer->capacity *= 2;
    }

    uint8_t* new_bytes = Arena_alloc(arena, buffer->capacity);
    assert(new_bytes != nullptr);

    memcpy(new_bytes, buffer->data, buffer->length);
    buffer->data = new_bytes;
}
