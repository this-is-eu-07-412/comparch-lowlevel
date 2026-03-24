;=============================================================================;
;                                                                             ;
; Exercise       : Compilation, linking and debugging of assembly programs    ;
; Authors        : First Name Surname, group number                           ;
; Date           : DD.MM.YY                                                   ;
; Notes          : Program which finds the largest number in a table          ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALL
                
                .DATA
TABLENGTH       EQU     12              	; Constant: number of elements in table
Table1          DB      01h, 02h, 00h, 10h, 12h, 33h
                DB      15h, 09h, 11h, 08h, 0Ah, 00h
Largest         DB      ?               	; Variable to store the result

                .CODE
Start:          mov     ax, @DATA			; Initialize Data Segment
                mov     ds, ax
                mov     cx, TABLENGTH    	; Load loop counter (12 elements)
                mov     bx, OFFSET Table1	; Point BX to the start of the table
                mov     al, [bx]           	; AL will hold the current maximum value, starting from first element in the table
;				dec 	cx					; These two (dec and inc) just make sure we don't waste time on comparing the first element with itself
;				inc 	bx					; But the program will work as intended without them (uncomment if needed)
				
FindMax:        cmp     al, [bx]        	; Compare current max (AL) with table element
                jae     NextElement     	; If AL >= [bx], skip updating max
                mov     al, [bx]       		; If AL < [bx], update AL with new max

NextElement:    inc     bx              	; Move to the next byte in the table
                loop    FindMax         	; Decrement CX and jump to FindMax if CX > 0
				
                mov     Largest, al     	; Store final result in memory (not required)
                mov     ah, 4Ch				; Terminate program returning the largest value as return code
                int     21h					
                
				.STACK  100h            
                END     Start           	; Point to the entry label