.text
.globl _start
.local L.diss.0

_start:
	add x9, xzr, #1         // x9 = xzr + 1
	b L.diss.0              // Branch to L.diss.0 
	add x9, x9, #10         // x9 = x9 + 10

L.diss.0:
	add x9, x9, #20         // x9 = x9 + 20
	add x9, x9, #100        // x9 = x9 + 100
	brk #0                  // Transfer control to debugger

