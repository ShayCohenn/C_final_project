MAIN: 	mov #1, r3
	add r2, STR
X:	mcro a_mc
	mov #2, r4
	sub r1, r2
	inc B
	prn #-1
	mcroend
	
STR:	.data 6, -9, 15
	a_mc
	.string "afdfg"
	stop
