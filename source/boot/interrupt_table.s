.section .vector_table

.global _v_table
b _reset_
b undefined_instruction_vector
b software_interrupt_vector
b prefetch_abort_vector
b _reset_
b _reset_
b interrupt_vector
b fast_interrupt_vector
