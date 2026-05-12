.text
.globl _start
.local init
.local copy_outer
.local copy_inner

_start:
	adr x9, src             // x9 = &src
	movz x10, #0            // x10 = 0 (zero-extended)
	add x10, x10, #4, lsl #12// x10 = x10 + 4
	add x11, xzr, #0        // x11 = xzr + 0

init:
	str x9, [x11, #0]       // Memory[x9+0][0:31] = x11[0:31]
	add x11, x11, #1        // x11 = x11 + 1
	add x9, x9, #4          // x9 = x9 + 4
	add x10, x10, #-1       // x10 = x10 + -1
	cmp x10, xzr            // set flags for x10 - xzr
	b.ne init               // Branch to init if most_recent_cmp was not equal
	add fp, xzr, #1000      // fp = xzr + 1000

copy_outer:
	adr x9, src             // x9 = &src
	adr x10, dst            // x10 = &dst
	movz x11, #0            // x11 = 0 (zero-extended)
	add x11, x11, #4, lsl #12// x11 = x11 + 4

copy_inner:
	ldrsw x12, [x9, #0]     // x12 = Memory[x9+0][0:31]
	str x10, [x12, #0]      // Memory[x10+0][0:31] = x12[0:31]
	add x9, x9, #4          // x9 = x9 + 4
	add x10, x10, #4        // x10 = x10 + 4
	add x11, x11, #-1       // x11 = x11 + -1
	cmp x11, xzr            // set flags for x11 - xzr
	b.ne copy_inner         // Branch to copy_inner if most_recent_cmp was not equal
	add fp, fp, #-1         // fp = fp + -1
	cmp fp, xzr             // set flags for fp - xzr
	b.ne copy_outer         // Branch to copy_outer if most_recent_cmp was not equal
	add x8, xzr, #93        // x8 = xzr + 93
	add x0, xzr, #0         // x0 = xzr + 0
	svc #0                  // Transfer control to OS

.bss
	dst .space 4
	src .space 4


