#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct vec_header {
    size_t len;
    size_t capacity;
};

#define vec_header(v) \
    ((struct vec_header*)(((uint8_t*)v) - sizeof(struct vec_header)))

#define vec_new() \
    (calloc(sizeof(struct vec_header), 1) + sizeof(struct vec_header))

#define vec_clone(v) \
    (memcpy(malloc(sizeof(struct vec_header) + sizeof(typeof(*v))*vec_capacity(v)), vec_header(v), \
            sizeof(struct vec_header) + sizeof(typeof(*v))*vec_capacity(v)) + sizeof(struct vec_header))

#define vec_free(v) \
    free(vec_header(v))

#define vec_len(v) \
    (vec_header(v)->len)

#define vec_capacity(v) \
    (vec_header(v)->capacity)

#define vec_reserve(v, n) \
    do{ \
        struct vec_header* h = vec_header(v); \
        if (h->capacity < n) { \
            h->capacity = n; \
            h = realloc(h, sizeof(struct vec_header) + sizeof(typeof(*v)) * h->capacity); \
            v = (typeof(*v) *)(((uint8_t*)h) + sizeof(struct vec_header)); \
        } \
    }while(0)

#define vec_push(v, val) \
    do{ \
        struct vec_header* h = vec_header(v); \
        if (h->len == h->capacity) { \
            h->capacity *= 2; h->capacity++; \
            h = realloc(h, sizeof(struct vec_header) + sizeof(typeof(*v)) * h->capacity); \
            v = (typeof(*v) *)(((uint8_t*)h) + sizeof(struct vec_header)); \
        } \
        v[h->len++] = val; \
    }while(0)

#define vec_pop(v) \
    do{ \
        assert(vec_header(v)->len > 0); \
        vec_header(v)->len--; \
    }while(0)

#define vec_from(X, ...) \
    memcpy(memcpy(malloc(sizeof(struct vec_header) \
                    + sizeof((typeof(X)[]){X,__VA_ARGS__})), \
                    (size_t[]){ \
                    sizeof((typeof(X)[]){X,__VA_ARGS__})/sizeof(typeof(X)), \
                    sizeof((typeof(X)[]){X,__VA_ARGS__})/sizeof(typeof(X)), \
                    }, \
                    2 * sizeof(size_t)) + sizeof(struct vec_header), \
                    (typeof(X)[]){X,__VA_ARGS__}, \
                    sizeof((typeof(X)[]){X,__VA_ARGS__}))

#define vec_foreach(v, elem, ...) \
    for (typeof(*v) elem, *_elem = v; \
            _elem != &v[vec_len(v)] \
            && ((elem = *_elem),true);\
            *_elem = elem, ++_elem) \
            __VA_ARGS__

#define vec_retain(v, elem, pred, ...) \
    for (typeof(*v) elem, *_elem = v, *_v = v; \
            _elem != &v[vec_len(v) + _v - v] \
            && ((elem = *_elem),true) \
            && (((pred) ? --vec_len(v), *(_v++) = *_elem, 0 : 0 ),true) \
            ; \
            *(_elem++) = elem) \
            __VA_ARGS__

