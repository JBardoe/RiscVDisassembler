.text
.globl _start

_start:
	adr x9, __global_pointer$// x9 = &__global_pointer$
	add x0, x9, #0          // x0 = x9 + 0
	add x8, xzr, #93        // x8 = xzr + 93
	svc #0                  // Transfer control to OS

.data
	__global_pointer$:	.word 71824


