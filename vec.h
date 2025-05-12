#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct _vec_header {
    size_t len;
    size_t capacity;
};

#define _vec_header(v) \
    ((struct _vec_header*)(((uint8_t*)v) - sizeof(struct _vec_header)))

#define vec_new() \
    (calloc(sizeof(struct _vec_header), 1) + sizeof(struct _vec_header))

#define vec_free(v) \
    (free(_vec_header(v)))

#define vec_len(v) \
    (_vec_header(v)->len)

#define vec_capacity(v) \
    (_vec_header(v)->capacity)

#define vec_reserve(v, n) \
do { \
    struct _vec_header* h = _vec_header(v); \
    if (h->capacity < n) { \
        h->capacity = n; \
        h = realloc(h, sizeof(struct _vec_header) + sizeof(typeof(*v)) * h->capacity); \
        v = (typeof(*v) *)(((uint8_t*)h) + sizeof(struct _vec_header)); \
    } \
} while(0)

#define vec_push(v, val) \
do { \
    struct _vec_header* h = _vec_header(v); \
    if (h->len == h->capacity) { \
        h->capacity *= 2; h->capacity++; \
        h = realloc(h, sizeof(struct _vec_header) + sizeof(typeof(*v)) * h->capacity); \
        v = (typeof(*v) *)(((uint8_t*)h) + sizeof(struct _vec_header)); \
    } \
    v[h->len++] = val; \
} while(0)

#define vec_pop(v) \
do { \
    assert(_vec_header(v)->len > 0); \
    _vec_header(v)->len--; \
} while(0)

#define vec_foreach(elem, v) \
    for (typeof(*v) elem, *_elem = v; \
            _elem != &v[vec_len(v)] && ((elem = *_elem) || true); \
            ++_elem) 

#define vec_from(X, ...) \
    memcpy(memcpy(malloc(sizeof(struct _vec_header) \
                    + sizeof((typeof(X)[]){X,__VA_ARGS__})), \
                    (size_t[]){ \
                    sizeof((typeof(X)[]){X,__VA_ARGS__})/sizeof(typeof(X)), \
                    sizeof((typeof(X)[]){X,__VA_ARGS__})/sizeof(typeof(X)), \
                    }, \
                    2 * sizeof(size_t)) + sizeof(struct _vec_header), \
                    (typeof(X)[]){X,__VA_ARGS__}, \
                    sizeof((typeof(X)[]){X,__VA_ARGS__}))

