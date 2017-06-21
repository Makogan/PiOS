#include <interrupts.h>

#define BIT(bit) (1 << bit)

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

rpi_irq_controller_t *irq_controller = (rpi_irq_controller_t*) (RPI_INTERRUPT_CONTROLLER_BASE);

extern "C" {

void _reset_() __attribute__((interrupt("RESET")));
void undefined_instruction_vector() __attribute__((interrupt("UNDEF")));
void software_interrupt_vector() __attribute__((interrupt("SWI")));
void prefetch_abort_vector() __attribute__((interrupt("ABORT")));
void interrupt_vector() __attribute__((interrupt("IRQ")));
void fast_interrupt_vector() __attribute__((interrupt("FIQ")));
}

void enable_select_irq(unsigned char irq)
{
  if(irq < 32)
  {
    irq_controller->Enable_Basic_IRQs = BIT(irq);
  }

  else if(irq < 64)
  {
    irq_controller->Enable_IRQs_1 = BIT((irq-32));
  }

  else
  {
    irq_controller->Enable_IRQs_2 = BIT((irq-64));
  }
}

void disable_select_irq(unsigned char irq)
{
  if(irq < 32)
  {
    irq_controller->Disable_Basic_IRQs = BIT(irq);
  }

  else if(irq < 64)
  {
    irq_controller->Disable_IRQs_1 = BIT((irq-32));
  }

  else
  {
    irq_controller->Disable_IRQs_2 = BIT((irq-64));
  }
}

void interrupt_enable_IF()
{
  __asm__ volatile
  (
    "cpsie if\n"
  );
}

 void interrupt_enable_I()
{
  __asm__ volatile
  (
    "cpsie i\n"
  );
}

void interrupt_enable_F()
{
  __asm__ volatile
  (
    "cpsie f\n"
  );
}

uint32_t interrupt_disable_IF()
{
  uint32_t state;
  __asm__ volatile
  (
    "mrs r0, cpsr\n"
    "cpsid if\n"
    "mov %0, r0\n"
    :"=r" (state)
  );

  return state;
}

uint32_t interrupt_disable_I()
{
  uint32_t state;
  __asm__ volatile
  (
    "mrs r0, cpsr\n"
    "cpsid i\n"
    "mov %0, r0\n"
    :"=r" (state)
  );

  return state;
}

uint32_t interrupt_disable_F()
{
  uint32_t state;
  __asm__ volatile
  (
    "mrs r0, cpsr\n"
    "cpsid f\n"
    "mov %0, r0\n"
    :"=r" (state)
  );

  return state;
}

void restore()
{
  __asm__ volatile
  (
    "msr cpsr_c, r0\n"
  );
}

void undefined_instruction_vector()
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
  //while(1){}
}

void software_interrupt_vector()
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
  //while(1){}
}

void prefetch_abort_vector()
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
 // while(1){}
}

void interrupt_vector()
{
  
  irq_controller->Disable_IRQs_1 = 0x1;
  interrupt_disable_IF();
  void clear_time_irq();

  set_LED(ON);
  //if(irq_controller->IRQ_basic_pending & 0x1)
  {
    print("OMG it works!");
  }

  set_LED(ON);

 // while(1){}
}

void fast_interrupt_vector(void)
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
 // while(1){}
}