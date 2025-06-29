	.globl _main
_main:
	mov	x0, #5
	str	x0, [sp, #-16]!
	mov	x0, #3
	ldr	x1, [sp], #16
	lsl	x0, x1, x0
	str	x0, [sp, #-16]!
	mov	x0, #3
	str	x0, [sp, #-16]!
	mov	x0, #8
	ldr	x1, [sp], #16
	asr	x0, x1, x0
	ldr	x1, [sp], #16
	orr	x0, x1, x0
	ret
