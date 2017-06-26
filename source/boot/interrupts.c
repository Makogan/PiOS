//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C
*
* This file contains all the Interrupt related function definitions.
* It defines an interrupt controller structure used to manage interrupt
* enabling, disabling and related activities. 
* 
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <interrupts.h>

#define BIT(bit) (1 << bit)

//Taken from: http://www.valvers.com/open-software/raspberry-pi/step04-bare-metal-programming-in-c-pt4/
#define RPI_INTERRUPT_CONTROLLER_BASE   (IO_BASE + 0xB200)

//########################################################################################

//========================================================================================
/*
* Global symbol deffinitions:
*/
//========================================================================================

/*
* The interrupt controller struct, each field represents a register. The first three 
* registers indicate the source of an interrupt. The following three after FIQ control 
* are used to enable a specific interrupt source. The final three are used to disable a 
* specific interupt source. For details about the function of each bit please see the 
* attached pdf manual.
*/
struct rpi_irq_controller_t
{
    // Inicate source of triggered interrupts
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    // Used to enable an interrupt source
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    // Used to disable an interrupt source
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
};

// Attach the controller structure to the actual controller's base address
rpi_irq_controller_t *irq_controller = (rpi_irq_controller_t*) (RPI_INTERRUPT_CONTROLLER_BASE);


// Define the vector table exception handlers
extern "C" {

void _reset_() __attribute__((interrupt("RESET")));
void undefined_instruction_vector() __attribute__((interrupt("UNDEF")));
void software_interrupt_vector() __attribute__((interrupt("SWI")));
void prefetch_abort_vector() __attribute__((interrupt("ABORT")));
void interrupt_vector() __attribute__((interrupt("IRQ")));
void fast_interrupt_vector() __attribute__((interrupt("FIQ")));

}

//########################################################################################

//========================================================================================
/*
* Function Definitions:
*/
//========================================================================================

/*
* Function used to select a single interrupt source to be enabled.
*
* Parameters:
*   uinsigned char irq: the number of the irq to enable. Our convention is:
*   0-31: the 32 bits in basic enable irq's register. 
*   32-63: the 32 bits in enable irq 1 register.
*   64+: the 32 bits in enable irq 2 register.
*
* For more information about the bit values see the pdf documentation 
*/
void enable_select_irq(unsigned char irq)
{
  if(irq < 32)
  {
    // Select an enable the chosen irq source
    irq_controller->Enable_Basic_IRQs = BIT(irq);
  }

  else if(irq < 64)
  {
    // Select an enable the chosen irq source
    irq_controller->Enable_IRQs_1 = BIT((irq-32));
  }

  else
  {
    // Select an enable the chosen irq source
    irq_controller->Enable_IRQs_2 = BIT((irq-64));
  }
}

/*
* Function used to select a single interrupt source to be disabled.
*
* Parameters:
*   uinsigned char irq: the number of the irq to disable. Our convention is:
*   0-31: the 32 bits in basic disable irq's register. 
*   32-63: the 32 bits in disable irq 1 register.
*   64+: the 32 bits in disable irq 2 register.
*
* For more information about the bit values see the pdf documentation 
*/
void disable_select_irq(unsigned char irq)
{
  if(irq < 32)
  {
    // Select an disabel the chosen irq source
    irq_controller->Disable_Basic_IRQs = BIT(irq);
  }

  else if(irq < 64)
  {
    // Select an disabel the chosen irq source
    irq_controller->Disable_IRQs_1 = BIT((irq-32));
  }

  else
  {
    // Select an disabel the chosen irq source
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
  while(1){}
}

void software_interrupt_vector()
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
  while(1){}
}

void prefetch_abort_vector()
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
  while(1){}
}

//interrupts get cleared after disabling them
void interrupt_vector()
{
  uint32_t pending_1_status = irq_controller->IRQ_pending_1;
  irq_controller->Disable_IRQs_1 = irq_controller->IRQ_pending_1;
  if(pending_1_status & 0x2)
  {
    if(system_timer->compare_1 <= system_timer->counter_low)
    {
      system_timer->control_status = 0b10;
      print("\ntimer c1:");
      print(pending_1_status);
      system_timer->compare_1 = system_timer->counter_low + 3000000;
    }

    irq_controller->Enable_IRQs_1 = 0x2;
  }
}

void fast_interrupt_vector(void)
{
    __asm__
  (
    "cpsid if\n"
  );
  set_LED(ON);
  while(1){}
}

//########################################################################################