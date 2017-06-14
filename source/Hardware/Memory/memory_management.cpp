
#include <memory_management.h>

typedef enum
{
    FREE,
    RESERVED,
    LOCKED,
}Block_state;

struct MemoryManager
{
    uint32_t start;
    uint32_t end;
};

struct memory_block
{
    memory_block *next_block;
    uint32_t size;
    Block_state state;
};

extern "C" void* memory_alloc(size_t size)
{
    return 0;
}