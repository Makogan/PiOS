void _reset_() __attribute__((interrupt("RESET")));
void undefined_instruction_vector() __attribute__((interrupt("UNDEF")));
void software_interrupt_vector() __attribute__((interrupt("SWI")));
void prefetch_abort_vector() __attribute__((interrupt("ABORT")));
void interrupt_vector() __attribute__((interrupt("IRQ")));
void fast_interrupt_vector() __attribute__((optimize("O0"))) __attribute__((interrupt("FIQ")));


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