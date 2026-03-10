.text
.globl _start

_start:
	adr, w9, __global_pointer$// w9 = &__global_pointer$
	add w0, w9, #0          // w0 = w9 + 0
	add w8, wzr, #93        // w8 = wzr + 93
	svc #0                  // Transfer control to OS

.data
	__global_pointer$:	.word 71824


