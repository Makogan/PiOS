#include <stdint.h>

//Taken from: http://www.valvers.com/open-software/raspberry-pi/step04-bare-metal-programming-in-c-pt4/
#define RPI_INTERRUPT_CONTROLLER_BASE   (IO_BASE + 0xB200)

struct rpi_irq_controller_t
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

__asm__
(
  ".section .text\n"
  ".global backup_table\n"
  "b_table:"
  "b _reset_\n"
  "b undefined_instruction_vector\n"
  "b software_interrupt_vector\n"
  "b prefetch_abort_vector\n"
  "b _reset_\n"
  "b _reset_\n"
  "b interrupt_vector\n"
  "b fast_interrupt_vector\n"
);

void _reset_()
{
  __asm__ volatile
  (
    "ldr r0, =b_table\n"
    "mov r1, #0x0\n"
    "ldmia r1!, {r2-r9}\n"
    "stmia r0!, {r2-r9}\n"
    "b  _start"
  );
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
