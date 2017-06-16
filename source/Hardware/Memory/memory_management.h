#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdint.h>

typedef uint32_t size_t;

#ifdef __cplusplus
    extern "C" {
#endif

void* memory_alloc(size_t size);
void* memory_re_alloc(void* ptr, size_t size);
void* memory_calloc(size_t size);
void memory_free(void* location);

#ifdef __cplusplus
    }
#endif


#endif