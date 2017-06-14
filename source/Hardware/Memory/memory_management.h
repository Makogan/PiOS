#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stdint.h>

typedef uint32_t size_t;

extern "C" void* memory_alloc(size_t size);

#endif