#include "alloc.h"

#include <assert.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (sysconf(_SC_PAGE_SIZE))

struct Region {
    Region* next;
    uint8_t* bytes;
    size_t remaining;
};

Region* Region_init(void) {
    void* page = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(page != MAP_FAILED);

    Region* region = (Region*)page;
    *region = (Region){
        .next = nullptr,
        .bytes = (uint8_t*)page + sizeof(Region),
        .remaining = PAGE_SIZE - sizeof(Region),
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

    int result = munmap((void*)region, PAGE_SIZE);
    assert(result == 0);
}

struct Arena {
    Region* head;
    Region* tail;
};

Arena Arena_init(void) {
    Arena arena;
    arena.head = Region_init();
    arena.tail = arena.head;
    return arena;
}

Region* Arena_new_region(Arena* arena) {
    Region* region = Region_init();
    arena->tail->next = region;
    arena->tail = region;
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

    // Did not find a large enough slot in the already allocted regions
    Region* tail = Arena_new_region(arena);
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
}

Bytes Bytes_alloc(Arena* arena) {
    constexpr size_t STARTING_CAPACITY = 128;

    uint8_t* bytes = (uint8_t*)Arena_alloc(arena, STARTING_CAPACITY);
    assert(bytes != nullptr);

    return (Bytes){
        .bytes = bytes,
        .lenth = 0,
        .capacity = STARTING_CAPACITY,
    };
}

Bytes Bytes_from_slice(BytesSlice slice) { assert(false); }

void Bytes_push(Bytes* self, uint8_t byte) {
    // if (self->lenth == ) {}
    assert(false);
}
uint8_t Bytes_pop(Bytes* self) { assert(false); }
