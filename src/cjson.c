#include "cjson.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

typedef enum : uint8_t {
    TOKEN_BRACE_OPEN,
    TOKEN_BRACE_CLOSE,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,
    TOKEN_COMMA,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_TRUE,
    TOKEN_FALSE,
} TokenKind;

const char* TokenKind_to_string(TokenKind kind) {
    switch (kind) {
        case TOKEN_BRACE_OPEN:
            return "BRACE_OPEN";
        case TOKEN_BRACE_CLOSE:
            return "BRACE_CLOSE";
        case TOKEN_BRACKET_OPEN:
            return "BRACKET_OPEN";
        case TOKEN_BRACKET_CLOSE:
            return "BRACKET_CLOSE";
        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_STRING:
            return "STRING";
        case TOKEN_TRUE:
            return "TRUE";
        case TOKEN_FALSE:
            return "FALSE";
    }
}

typedef struct {
    uint32_t start;
    uint32_t end;
} Span;

typedef struct {
    TokenKind* data;
    size_t length;
    size_t capacity;
} TokenKindBuffer;

typedef struct {
    Span* data;
    size_t length;
    size_t capacity;
} SpanBuffer;

typedef struct {
    TokenKindBuffer kinds;
    SpanBuffer spans;
} Tokens;

Tokens tokenize(ByteSlice input, Arena* arena) {
    Tokens tokens = (Tokens){
        .kinds = Buffer_new(),
        .spans = Buffer_new(),
    };

    for (uint32_t i = 0; i < input.length; ++i) {
        switch (input.data[i]) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            case '{':
                Buffer_push(&tokens.kinds, arena, TOKEN_BRACE_OPEN);
                break;
            case '}':
                Buffer_push(&tokens.kinds, arena, TOKEN_BRACE_CLOSE);
                break;
            case '[':
                Buffer_push(&tokens.kinds, arena, TOKEN_BRACKET_OPEN);
                break;
            case ']':
                Buffer_push(&tokens.kinds, arena, TOKEN_BRACKET_CLOSE);
                break;
            case ',':
                Buffer_push(&tokens.kinds, arena, TOKEN_COMMA);
                break;
            case 't':
                break;
            case 'f':
                break;
            case '"':
                break;
            default:
                break;
        }
    }

    return tokens;
}

Json Json_parse(ByteSlice input, Arena* arena) {
    Tokens tokens = tokenize(input, arena);
    printf("Tokens: ");
    for (size_t i = 0; i < tokens.kinds.length; ++i) {
        TokenKind kind = tokens.kinds.data[i];
        if (i != 0) {
            printf(", ");
        }
        printf("%s", TokenKind_to_string(kind));
    }
    puts("");

    return Json_null();
}

ByteBuffer Json_to_string(Json* value, bool pretty) {
    return (ByteBuffer)Buffer_new();
}
