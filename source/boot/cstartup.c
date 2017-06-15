#include <stdint.h>

extern int __bss_start__;
extern int __bss_end__;
extern uint32_t Kernel_End;

extern void kernel_main();
extern void init_memory_manager();

void _cstartup()  __attribute__((optimize("-O0")));
void _cstartup()
{
    volatile int* current_address = &__bss_end__;
    int* end = &__bss_end__;
    while(current_address < end)
    {
        *current_address = 0;
        current_address++;
    }

    Kernel_End = 0xFFFFFFFF;

    init_memory_manager();

    kernel_main();

    while(1){}
}