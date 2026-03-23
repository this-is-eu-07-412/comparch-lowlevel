;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which which finds the largest number in a table    ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALL

                .DATA
TABLEN          DB     12
Array           DB     01h, 02h, 00h, 10h, 12h, 33h
                DB     15h, 09h, 11h, 08h, 0Ah, 00h
Largest         DB     12

                .CODE
Start:			mov     ax, @DATA
                mov     ds, ax
				mov 	bx, OFFSET Array
		
;Loop:			mov     cl, TABLEN
;
;				cmp     
				
;				dec 	cl

                mov     al, Largest
                mov     ah, 4Ch
                int     21h
                
                .STACK    100h
                END  Start


;                mov     bx, OFFSET TABLEN
;                mov     al, [bx]
;
;                mov     ch, Table
;Loop:
;                cmp     al, [dx]
;                jbe     Start
;                muv     ah, [bx]
;Jump:
;                inc     bh
;                jmp    Jump