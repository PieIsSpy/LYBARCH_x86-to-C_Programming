section .data
    align 8
    c1000: dq 1000.0
    c3600: dq 3600.0

section .text

default rel
bits64

global asm_acceleration
asm_acceleration:
    movsd   xmm3, [c1000]
    movsd   xmm4, [c3600]

    ; convert vi to m/s
    mulsd   xmm0, xmm3
    divsd   xmm0, xmm4

    ; convert vf to m/s
    mulsd   xmm1, xmm3
    divsd   xmm1, xmm4

    ; calculate acceleration
    subsd   xmm1, xmm0
    divsd   xmm1, xmm2

    ; convert to int
    cvtsd2si eax, xmm1

    ret