#pragma once

#include <stddef.h>
#include <stdint.h>

#include "alloc.h"

typedef struct Json Json;

typedef struct {
    ByteSlice* keys;
    Json* values;
    size_t length;
} JsonObject;

typedef struct {
    Json* values;
    size_t length;
} JsonArray;

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL,
} JsonKind;

typedef union {
    JsonObject object;
    JsonArray array;
    double number;
    AnyBuffer string;
    nullptr_t none;
} JsonData;

struct Json {
    JsonKind kind;
    JsonData data;
};

static inline Json Json_array(JsonArray value) {
    return (Json){.kind = JSON_ARRAY, .data = {.array = value}};
}

static inline Json Json_string(AnyBuffer value) {
    return (Json){.kind = JSON_STRING, .data = {.string = value}};
}

static inline Json Json_number(double value) {
    return (Json){.kind = JSON_NUMBER, .data = {.number = value}};
}

static inline Json Json_bool(bool value) {
    return (Json){.kind = value ? JSON_TRUE : JSON_FALSE,
                  .data = {.none = nullptr}};
}

static inline Json Json_null() {
    return (Json){.kind = JSON_NULL, .data = {.none = nullptr}};
}

ByteBuffer Json_to_string(Json* value, bool pretty);

Json Json_parse(ByteSlice input, Arena* arena);
