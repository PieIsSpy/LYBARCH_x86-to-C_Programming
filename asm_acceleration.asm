section .text

default rel
bits 64

global asm_acceleration
asm_acceleration:

	subsd   xmm1, xmm0        
    divsd   xmm1, xmm2         

    cvttsd2si eax, xmm1
	
	ret