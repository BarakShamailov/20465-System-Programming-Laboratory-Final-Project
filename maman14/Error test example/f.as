;file ps.as
;sample source code
	.extern K23G
	.entry END
    .entry
    .extern   wNumber
	.entry wNumber
	.extern LOOP
LOOP: addi $9,-45,$8
END:  ori $5,-45,$7
END: subi $2,-45,$9
;new check *****************************************
STR:	.asciz	aBcd"
	.asciz	aBcd
;new check *****************************************
MAIN:	add	$3 $5,$9
	add	,$3 ,$5,$9
	add	$3 ,$5 $9
	add	$3 ,$5,,$9
	add	$3 ,$5,$9,
	add
	add	$3 ,$5,$9, $45
	add	 %32 ,$45,$9
;new check *****************************************
 ori $12,-54,$15,
	ori $12,$5,$15
ori %6,#4,675
ori
ori $12,$5,,$15
ori $12,$5 $15
ori $12,$5,$15,
;new check *****************************************
	blt $23,$22,tree
blt LOOP,2,$5
blt  $23,$22,LOOP
blt  $23,$22 LOOP
blt  $23 $22,LOOP
blt  $23,$22,LOOP,
	blt  $23,$22,,LOOP
;new check *****************************************
lh  $9,%4,$2
sw
sb  $9 4,$2
lb  $9,4,$56
lh  ere,%4,$2
lh  $9 %4,$2
;new check *****************************************
	jmp 323
jmp K
jmp !@#4
jmp
la
la $32
la @#@3
la K
call K
call
call $%#
call 12123
stop 12121
;new check *****************************************
	.fggh 45,65
	.dh 0,%667,2
	.db 0,667 2
	.dh 0,,667,2
	.db 0, 667,2
	.db 0, 667,2,
.db 0,$5,asas
