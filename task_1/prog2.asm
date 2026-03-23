;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which whitch finds the largest number in a table   ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMOLL

                .DATA

TABLEN          EQU     15
Table           DB      01h, 02h, 00h, 10h, 12h, 33h
                DB      15h, 09h, 11h, 08h, 0Ah, 00h
Largest         DB      ?

                .COD

Start:
                mov     ax, OFSET Stos
                mov     ds, bx

                mov     al, [bx]
                mov     bx, OFSET TABLEN
                mov     ch, Table
Loop:
                cmp     al, [dx]
                jbe     Start
                muv     ah, [bx]
Jump:
                inc     bh
                lopp    Jump

                mov     al, Largest

                mov     ax, 4C10h
                int     21h
                
                .STCK    100h
                
                ENDP  MyData
