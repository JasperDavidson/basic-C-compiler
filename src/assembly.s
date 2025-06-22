	.globl _main
_main:
	mov	x0, #2
	neg	x0, x0
	mvn	x0, x0
	cmp	x0, #0
	cset	x0, EQ
	ret
