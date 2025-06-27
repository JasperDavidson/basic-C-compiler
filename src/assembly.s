	.globl _main
_main:
	mov	x0, #1
	str	x0, [sp, #-16]!
	mov	x0, #1
	ldr	x1, [sp], #16
	sub	x0, x1, x0
	cmp	x0, #0
	b.eq	_label_0
	mov	x0, #1
	b	_label_1
_label_0:
	mov	x0, #0
	cmp	x0, #0
	b.ne	_label_2
	mov	x0, #0
	b	_label_3
_label_2:
	mov	x0, #1
	cmp	x0, #0
	cset	x0, ne
_label_3:
	cmp	x0, #0
	cset	x0, ne
_label_1:
	cmp	x0, #0
	b.eq	_label_4
	mov	x0, #1
	b	_label_5
_label_4:
	mov	x0, #1
	cmp	x0, #0
	cset	x0, ne
_label_5:
	cmp	x0, #0
	b.ne	_label_6
	mov	x0, #0
	b	_label_7
_label_6:
	mov	x0, #0
	cmp	x0, #0
	b.eq	_label_8
	mov	x0, #1
	b	_label_9
_label_8:
	mov	x0, #1
	cmp	x0, #0
	cset	x0, ne
_label_9:
	cmp	x0, #0
	cset	x0, ne
_label_7:
	ret
