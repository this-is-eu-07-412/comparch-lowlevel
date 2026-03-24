;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which evaluates the expression: (a-b)*c/d          ;
;                                                                             ;
;=============================================================================;

                .MODEL SMALL

                .DATA
a                DB      20
b                DB      10
c                DB     100
d                DB       5
result 			 DB       ?

                .CODE
Beginning:      mov  	ax, @DATA
				mov     ds, ax
				mov 	al, a
				mov 	bl, b
				sub 	al, bl			; al = al-bl
				mov 	bl, c
				mul     bl				; al = al*bl
				mov 	bl, d
				div     bl				; al = al/bl
				mov     result, al      ; Moves the results to the memory
				mov     ah, 4Ch
				int     21h

                .STACK    100h
				END		  Beginning	
			