.text
.globl _start

_start:
	adr x9, myVar           // x9 = &myVar
	add x10, xzr, #10       // x10 = xzr + 10
	str x9, [x10, #0]       // Memory[x9+0][0:31] = x10[0:31]
	ldrsw x11, [x9, #0]     // x11 = Memory[x9+0][0:31]
	add x11, x11, #5        // x11 = x11 + 5
	str x9, [x11, #0]       // Memory[x9+0][0:31] = x11[0:31]
	add x8, xzr, #93        // x8 = xzr + 93
	add x0, xzr, #0         // x0 = xzr + 0
	svc #0                  // Transfer control to OS

.bss
	myVar .space 4


