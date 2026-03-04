.bss
	myVar .space 4


.text
.globl _start

_start:
	adr, w9, myVar          // w9 = &myVar
	add w10, wzr, #10       // w10 = wzr + 10
	str w9, [w10, #0]       // Memory[w9+0][0:31] = w10[0:31]
	ldrsw w11, [w9, #0]     // w11 = Memory[w9+0][0:31] (sign-extends)
	add w11, w11, #5        // w11 = w11 + 5
	str w9, [w11, #0]       // Memory[w9+0][0:31] = w11[0:31]
	add w8, wzr, #93        // w8 = wzr + 93
	add w0, wzr, #0         // w0 = wzr + 0
	svc #0                  // Transfer control to OS

