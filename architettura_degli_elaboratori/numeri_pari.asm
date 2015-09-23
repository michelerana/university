org 100h

; MACRO

; 4) stampa di una stringa su schermo
STAMPA MACRO stringa
    LEA DX, stringa
    MOV AH, 09h
    INT 21h
ENDM

fine MACRO
    MOV AH, 4Ch
    INT 21h
ENDM

da_ascii_a_decimale MACRO carattere
    MOV BH, carattere
    SUB BH, '0'
    MOV carattere, BH
ENDM

endLine MACRO
    mov al, 0Dh
    mov ah, 0Eh
    int 10h
    mov al, 0Ah
    mov ah, 0Eh
    int 10h
ENDM

;PROGRAMMA

STAMPA msg1
;1) legge la stringa
LEA DX, numero
MOV AH, 0Ah    
INT 21h
endLine

;2) ciclo conversione
MOV BL, numero[1]
INC BL
MOV SI, BX
condizione_ciclo: CMP SI, 01h
                  JE uscita_ciclo
                  da_ascii_a_decimale numero[SI] 
                  MOV AX, 0
                  MOV AL, numero[SI]
                  MOV BL, 2
                  DIV BL
                  CMP AH, 0
                  JNE fine_ciclo
                  INC contPari
                  MOV BL, numero[si]
                  ADD numPari, BL
                  fine_ciclo: DEC SI
                              JMP condizione_ciclo
uscita_ciclo:

;5) Converte il contatore in una stringa di caratteri 

MOV BL, contPari
CALL Converti
MOV stringa_due[0], DL
MOV stringa_due[1], DH

;6) Stampa contatore
STAMPA msg2_1
STAMPA stringa_due
STAMPA msg2_2
endLine

;7) Converte somma in stringa

MOV BL, numPari
CALL Converti
MOV stringa_due[0], DL
MOV stringa_due[1], DH

;8) Stampa somma

STAMPA msg3
STAMPA stringa_due

fine

; PROCEDURE

;3) Procedura che converte un numero di 2 cifre in stringa
Converti PROC
    MOV AX, 0
    CMP BL, 100
    JAE fine_converti
    MOV AL, BL
    MOV CL, 10
    DIV CL
    MOV DL, AL
    MOV DH, AH
    ADD DL, '0'
    ADD DH, '0'
    fine_converti: ret
ENDP    

;dichiarazione delle variabili
DIM EQU 10
numero DB DIM+1, ?, DIM+1 DUP ('$')
msg1 DB "Inserisci un numero (max 10 cifre): $"
msg2_1 DB "Il numero inserito ha $"
msg2_2 DB " cifre pari$"
msg3 DB "La somma delle cifre pari e' $"
contPari DB 0
numPari DB 0
stringa_due DB 3 DUP('$')