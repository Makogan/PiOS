#include <memory_management.h>
#include <string.h>

#define LAST_ADDRESS 0x30000000
#define NULL nullptr
#define WORD_ALIGN(val) (val + val % sizeof(void*))
extern uint32_t Kernel_End;

typedef enum
{
    FREE,
    RESERVED,
    LOCKED,
}Block_state;

struct Memory_Block
{
    Memory_Block *next_block;
    uint32_t size;
    Block_state state;
};

void* heap_start;
void* heap_end;

void* stack_start;
void* stack_end;

Memory_Block* first_block;
extern "C" void init_memory_manager()
{
    heap_start = (void*)LAST_ADDRESS;
    heap_end = (void*)(LAST_ADDRESS - sizeof(Memory_Block));

    stack_start = 0x0;
    stack_end = &Kernel_End;

    first_block = 
        (Memory_Block*)
        ((char*)(LAST_ADDRESS-sizeof(Memory_Block)-sizeof(uint32_t)));
    
    first_block->next_block = nullptr;
    first_block->size = 0;
    first_block->state = LOCKED;
}

void increment_heap(size_t size)
{
    if( (char*)heap_end - WORD_ALIGN(size)  > (char*)stack_end)
    {
        heap_end = (void*)((char*)heap_end - WORD_ALIGN(size));
    }

    else
    {
        //TODO: add error handling here
    }
}

extern "C" void* memory_alloc(size_t size)
{
    Memory_Block *current_block = first_block;
    bool finished = false;
    do
    {
        if(current_block->state == FREE && current_block->size >= size)
            return (void*)((char*)current_block + sizeof(Memory_Block));
        
        if(current_block->next_block != nullptr)
            current_block = current_block->next_block;
        
        else if((char*)current_block - (size + sizeof(Memory_Block)) < heap_end)
        {
            increment_heap((size + sizeof(Memory_Block))-
                ((uint32_t)current_block-(uint32_t)heap_end));
            
            Memory_Block *newBlock = (Memory_Block*)heap_end;

            newBlock->next_block = nullptr;
            newBlock->size = size;
            newBlock->state = LOCKED;

            current_block->next_block = newBlock;

            return (void*)((char*)newBlock+sizeof(Memory_Block));
        }

        else 
            finished = true;

    }
    while(!finished); 

    return nullptr;
}

extern "C" void* memory_re_alloc(void* ptr, size_t size)
{
    Memory_Block *block = (Memory_Block*)((char*)ptr - sizeof(Memory_Block));

    if(block->size >= size)
        return ptr;
    
    block->state = FREE;

    return memory_alloc(size);
}

extern "C" void* memory_calloc(size_t size)
{
    size = WORD_ALIGN(size);
    void* ptr = memory_alloc(size);

    for(uint32_t i=0; i<size; i++)
    {
        *((uint32_t*)ptr + i) = 0x0;
    }

    return ptr;
}

extern "C" void memory_free(void* location)
{
    Memory_Block *block = (Memory_Block*)((char*)location - sizeof(Memory_Block));
    block->state = FREE;
}