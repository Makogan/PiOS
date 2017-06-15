#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdint.h>

typedef uint32_t size_t;

extern "C" void* memory_alloc(size_t size);
extern "C" void* memory_re_alloc(size_t size);
extern "C" void* memory_calloc(size_t size);
extern "C" void memory_free(void* location);

#endif