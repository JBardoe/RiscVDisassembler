.text
.globl _start
.globl entry_a
.globl entry_b

_start:
	adr, w9, var_byte       // w9 = &var_byte
	ldrsb w10, [w9, #0]     // w10 = Memory[w9+0][0:7] (sign-extends)
	adr, w9, var_half       // w9 = &var_half
	ldrsh w11, [w9, #0]     // w11 = Memory[w9+0][0:15] (sign-extends)
	adr, w9, var_word       // w9 = &var_word
	ldr w12, [w9, #0]       // w12 = Memory[w9+0][0:31]
	add w13, w10, w11       // w13 = w10 + w11
	add w13, w13, w12       // w13 = w13 + w12
	b entry_a               // Branch to entry_a 

entry_a:
	adr, w0, var_word       // w0 = &var_word
	ldr w1, [w0, #0]        // w1 = Memory[w0+0][0:31]
	add w1, w1, #1          // w1 = w1 + 1
	str w0, [w1, #0]        // Memory[w0+0][0:31] = w1[0:31]
	b entry_b               // Branch to entry_b 

entry_b:
	adr, w0, var_byte       // w0 = &var_byte
	ldrsb w1, [w0, #0]      // w1 = Memory[w0+0][0:7] (sign-extends)
	add w1, w1, #2          // w1 = w1 + 2
	strb w0, [w1, #0]       // Memory[w0+0][0:7] = w1[0:7]
	b entry_b               // Branch to entry_b 

.data
	var_word:	.word 100000
	var_half:	.half 300
	var_byte:	.byte 5


