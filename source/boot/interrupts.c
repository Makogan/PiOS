#include <stdint.h>

#define RPI_INTERRUPT_CONTROLLER_BASE   (IO_BASE + 0xB200)

typedef struct rpi_irq_controller_t
{
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
};

void _reset_() __attribute__((interrupt("RESET")));
void undefined_instruction_vector() __attribute__((interrupt("UNDEF")));
void software_interrupt_vector() __attribute__((interrupt("SWI")));
void prefetch_abort_vector() __attribute__((interrupt("ABORT")));
void interrupt_vector() __attribute__((interrupt("IRQ")));
void fast_interrupt_vector(); //__attribute__((optimize("O0"))) __attribute__((interrupt("FIQ")));


void _reset_()
{

}

void undefined_instruction_vector()
{

}

void software_interrupt_vector()
{

}

void prefetch_abort_vector()
{

}

void interrupt_vector()
{

}

void fast_interrupt_vector(void)
{

}
