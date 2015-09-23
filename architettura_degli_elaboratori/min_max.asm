org 100h

visualizza_stringa MACRO stringa
    lea dx, stringa
    mov ah, 9
    int 21h
endm

visualizza_cifra MACRO numero, carattere
    mov bl, numero
    add bl, '0'
    mov carattere[0], bl
    mov carattere[1], "$"
    visualizza_stringa carattere
endm

acapo MACRO
    mov al, 0Dh
    mov ah, 0Eh
    int 10h
    mov al, 0Ah
    mov ah, 0Eh
    int 10h
endm

mov cx, N
mov si, 1 ; possiamo inizializzarlo a 1
mov ah, a[0]
mov al, ah

ciclo: cmp a[si], al
       jb aggiorna_min
       cmp a[si], ah
       ja aggiorna_max
       loop ciclo
       mov max, ah
       mov min, al
       jmp visualizza_dati
       
              
aggiorna_min: mov al, a[si]
              inc si
              loop ciclo

aggiorna_max: mov ah, a[si]
              inc si
              loop ciclo

visualizza_dati: visualizza_stringa msg1
                 visualizza_cifra min, cifra
                 acapo
                 visualizza_stringa msg2
                 visualizza_cifra max, cifra
                 ; terminazione programma
mov ah, 4ch
int 21h
                 

; dichiarazione delle variabili
N equ 10 ; lunghezza di a
a db 1, 5, 6, 8, 5, 4, 1, 3, 7, 7
max db ?
min db ?
msg1 db "Il minimo e' $"
msg2 db "Il massimo e' $"
cifra db ?