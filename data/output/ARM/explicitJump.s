.text
.globl _start
.local L.diss.0

_start:
	add w9, wzr, #1         // w9 = wzr + 1
	b L.diss.0              // Branch to L.diss.0 
	add w9, w9, #10         // w9 = w9 + 10

L.diss.0:
	add w9, w9, #20         // w9 = w9 + 20
	add w9, w9, #100        // w9 = w9 + 100
	brk #0                  // Transfer control to debugger

