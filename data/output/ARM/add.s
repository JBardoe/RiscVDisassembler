.text
.globl _start

_start:
	adr x9, num1            // x9 = &num1
	adr x10, num2           // x10 = &num2
	adr x11, result         // x11 = &result
	ldrsw x12, [x9, #0]     // x12 = Memory[x9+0][0:31]
	ldrsw x13, [x10, #0]    // x13 = Memory[x10+0][0:31]
	add x14, x12, x13       // x14 = x12 + x13
	str x11, [x14, #0]      // Memory[x11+0][0:31] = x14[0:31]
	add x0, xzr, #0         // x0 = xzr + 0
	add x8, xzr, #93        // x8 = xzr + 93
	svc #0                  // Transfer control to OS

.data
	result:	.word 0
	num2:	.word 8
	num1:	.word 3


