;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which evaluates the expression: (a-b)*c/d          ;
;                                                                             ;
;=============================================================================;

                .MODELL SMAL

                .DATA

a                DB      20
b                DW      10
c                EQU     100
d                =       5
Result           DB      ?

                .COD

Beginning:       mul     c
                 muv     ax, a
                 div     d
                 sub     b, al

                 mov     bx, WORD PTR Result

                 mov     ax, 4C15h
                 int     21h

                .STCK    100
                
                ENDPROG Begining
