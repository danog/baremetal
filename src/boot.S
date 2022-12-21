#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	mrs	x0,	mpidr_el1 // Copy processor affinity register into x0
	and	x0, x0,	#0xFF // Affinity 0 @ 0:7 is the core ID
	cbnz	x0, halt  // Halt if core != 0

	adr	x0, bss_begin // Argument 1: bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0    // Argument 2: bss_end-bss_begin
	bl 	memzero       // memzero(bss_begin, bss_end-bss_begin)

	mov	sp, #LOW_MEMORY // Initialize the kernel stack pointer to +4mb, growing downwards
	bl	kernel_main     // kernel_main()
	
halt: // Doesn't actually shut down the other cores for now, just makes them spin endlessly
	b halt