	.globl _main
_main:
	str	fp, [sp, #-16]!
	mov	fp, sp
	mov	x0, #2
	str	x0, [sp, #-16]!
	mov	x0, #3
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-16]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-32]
	ldr	x1, [sp], #16
	mul	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-48]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-16]
	ldr	x1, [sp], #16
	mul	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-32]
	ldr	x1, [sp], #16
	add	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-48]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-16]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-64]
	ldr	x1, [sp], #16
	sdiv	x0, x1, x0
	ldr	x1, [sp], #16
	add	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-16]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-32]
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-48]
	ldr	x1, [sp], #16
	mul	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-64]
	ldr	x1, [sp], #16
	sdiv	x0, x1, x0
	ldr	x1, [sp], #16
	add	x0, x1, x0
	str	x0, [sp, #-16]!
	ldr	x0, [fp, #-80]
	ldr	x1, [sp], #16
	sub	x0, x1, x0
	mov	sp, fp
	ldr	fp, [sp], #16
	ret