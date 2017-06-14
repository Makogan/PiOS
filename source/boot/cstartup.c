extern int __bss_start__;
extern int __bss_end__;

extern void kernel_main();

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

    kernel_main();

    while(1){}
}