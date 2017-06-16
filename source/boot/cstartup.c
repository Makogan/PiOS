#include <stdint.h>

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t Kernel_End;

extern "C" void kernel_main();
extern "C" void init_memory_manager();
extern "C" void init_main_cursor();

extern "C" void _cstartup()  __attribute__((optimize("-O0")));
void _cstartup()
{
    volatile uint32_t* current_address = &__bss_start__;
    uint32_t* end = &__bss_end__;
    while(current_address < end)
    {
        *current_address = 0;
        current_address++;
    }

    Kernel_End = 0xFFFFFFFF;

    //init_main_cursor();
    init_memory_manager();

    kernel_main();

    while(1){}
}