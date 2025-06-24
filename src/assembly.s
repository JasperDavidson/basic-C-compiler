	.globl _main
_main:
	mov	x0, #1
	str	x0, [sp, #-16]!
	mov	x0, #2
	ldr	x1, [sp], #16
	mul	x0, x1, x0
	str	x0, [sp, #-16]!
	mov	x0, #8
	str	x0, [sp, #-16]!
	mov	x0, #4
	ldr	x1, [sp], #16
	udiv	x0, x1, x0
	ldr	x1, [sp], #16
	add	x0, x1, x0
	str	x0, [sp, #-16]!
	mov	x0, #5
	str	x0, [sp, #-16]!
	mov	x0, #6
	ldr	x1, [sp], #16
	mul	x0, x1, x0
	ldr	x1, [sp], #16
	sub	x0, x1, x0
	ret
