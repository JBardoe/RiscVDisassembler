# Translator

## Register Mapping for Translation (RV32I -> AArch64)

0. zero -> wzr
1. ra -> lr
2. sp -> sp
3. gp -> ELIMINATED
4. tp -> ELMINATED
5. t0 -> w9
6. t1 -> w10
7. t2 -> w11
8. s0 / fp -> fp
9. s1 -> w19
10. a0 -> w0
11. a1 -> w1
12. a2 -> w2
13. a3 -> w3
14. a4 -> w4
15. a5 -> w5
16. a6 -> w6
17. a7 -> w8
18. s2 -> w20
19. s3 -> w21
20. s4 -> w22
21. s5 -> w23
22. s6 -> w24
23. s7 -> w25
24. s8 -> w26
25. s9 -> w27
26. s10 -> w28
27. s11 -> w7
28. t3 -> w12
29. t4 -> w13
30. t5 -> w14
31. t6 -> w15&emsp;-&emsp;(Also sometimes used as a scratch registers by jalr)

## Instruction Mapping for Translation (RV32I -> AArch64)

1. add\
   &emsp; add Wd, Wn, Wm
2. sub\
   &emsp; sub Wd, Wn, Wm
3. xor\
   &emsp; eor Wd, Wn, Wm
4. or\
   &emsp; orr Wd, Wn, Wm
5. and\
   &emsp; and Wd, Wn, Wm
6. sll\
   &emsp; lsl Wd, Wn, Wm
7. srl\
   &emsp; lsr Wd, Wn, Wm
8. sra\
   &emsp; asr Wd, Wn, Wm
9. slt\
   &emsp; cmp Wn, Wm\
   &emsp; cset Wd, lt
10. sltu\
    &emsp; cmp Wn, Wm\
    &emsp; cset Wd, lo
11. addi\
    &emsp; add Wd, Wn, #imm
12. xori\
    &emsp; eor Wd, Wn, #imm
13. ori\
    &emsp; orr Wd, Wn, #imm
14. andi\
    &emsp; and Wd, Wn, #imm
15. slli\
    &emsp; lsl Wd, Wn, #imm
16. srli\
    &emsp; lsr Wd, Wn, #imm
17. srai\
    &emsp; asr Wd, Wn, #imm
18. slti\
    &emsp; cmp Wn, #imm\
    &emsp; cset Wd, lt
19. sltiu\
    &emsp; cmp Wn, Wm\
    &emsp; cset Wd, lo
20. l\{b|h|w\}\
    &emsp; ldrs\{b|h|w\} Wd, [Wn, #imm]
21. l\{b|h\}u\
    &emsp; ldr\{b|h\} Wd, [Wn, #imm]
22. s\{b|h|w\}\
    &emsp; str\{b|h|w\} Wm, [Wn, #imm]
23. beq\
    &emsp; cmp Wn, Wm\
    &emsp; b.eq \{label|#imm\}
24. bne\
    &emsp; cmp Wn, Wm\
    &emsp; b.ne \{label|#imm\}
25. blt\
    &emsp; cmp Wn, Wm\
    &emsp; b.lt \{label|#imm\}
26. bge\
    &emsp; cmp Wn, Wm\
    &emsp; b.ge \{label|#imm\}
27. bltu\
    &emsp; cmp Wn, Wm\
    &emsp; b.lo \{label|#imm\}
28. bgeu\
    &emsp; cmp Wn, Wm\
    &emsp; b.hs \{label|#imm\}
29. jal\
    &emsp; if(rd == ra)\
    &emsp; &emsp; bl \{label|#imm\}\
    &emsp; else if(rd == zero)\
    &emsp; &emsp; b \{label|#imm\}\
    &emsp; else\
    &emsp; &emsp; adr Wd .+8\
    &emsp; &emsp; b \{label|#imm\}
30. jalr\
    &emsp; if(rd == ra)\
    &emsp; &emsp; add w15, Wn, #imm\
    &emsp; &emsp; blr w15\
    &emsp; else if(rd == zero)\
    &emsp; &emsp; add w15, Wn, #imm\
    &emsp; &emsp; br w15\
    &emsp; else\
    &emsp; &emsp; add w15, Wn, #imm\
    &emsp; &emsp; adr Wd .+8\
    &emsp; &emsp; br w15
31. lui\
    &emsp; movz Wd, #0\
    &emsp; add Wd, Wd, #imm, lsl #12
32. auipc\
    &emsp; adr Wd, .\
    &emsp; add Wd, Wd, #imm, lsl #12
33. ecall\
    &emsp; svc #0
34. ebreak\
    &emsp; brk #0
35. la\
    &emsp; adr Wd, symbol
36. l\{b|h|w\} symbol\
    &emsp; ldrs\{b|h|w\} Wd, \[symbol\]
37. s\{b|h\|w\} symbol\
    &emsp; str\{b|h\| \} Wt, symbol
