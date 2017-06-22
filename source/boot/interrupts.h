#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "mailbox.h"
#include "stdint.h"
#include "string.h"

#ifdef __cpluplus
    extern "C"{
#endif

void enable_select_irq(unsigned char irq);

void interrupt_enable_IF();
void interrupt_enable_I();
void interrupt_enable_F();

uint32_t interrupt_disable_IF();
uint32_t interrupt_disable_I();
uint32_t interrupt_disable_F();

void restore();

#ifdef __cpluplus
    }
#endif

#endif