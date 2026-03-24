;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which finds the largest number in a table          ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALL
                .STACK  100h            ; Fixed typo .STCK

                .DATA
TABLEN          EQU     12              ; Constant: number of elements in table
Table           DB      01h, 02h, 00h, 10h, 12h, 33h
                DB      15h, 09h, 11h, 08h, 0Ah, 00h
Largest         DB      0               ; Variable to store the result

                .CODE
Start:
                ; Initialize Data Segment
                mov     ax, @DATA
                mov     ds, ax

                ; Prepare for the search loop
                mov     cx, TABLEN      ; Load loop counter (12 elements)
                mov     bx, OFFSET Table ; Point BX to the start of the table
                mov     al, 0           ; AL will hold the current maximum value

FindMax:
                cmp     al, [bx]        ; Compare current max (AL) with table element
                jae     NextElement     ; If AL >= [bx], skip updating max
                mov     al, [bx]        ; If AL < [bx], update AL with new max

NextElement:
                inc     bx              ; Move to the next byte in the table
                loop    FindMax         ; Decrement CX and jump to FindMax if CX > 0

                ; Store final result in memory
                mov     Largest, al     ; Move the largest found value to Largest variable

                ; Terminate program
                mov     ax, 4C00h
                int     21h
                
                END     Start           ; Point to the entry label
