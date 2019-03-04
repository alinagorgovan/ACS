%include "io.inc"

section .data
    %include "input.inc"
    string db "Baza incorecta", 0





section .text
global CMAIN
CMAIN:
   mov ebp, esp
    xor ecx, ecx
    mov edx, 0
      
Bucla_for:
    mov edi,0
    mov eax, [nums_array + 4*ecx]
    mov ebx, [base_array + 4 * ecx]
    cmp ebx, 2 ;if baza < 2 sau > 16   
    jb mesaj
    cmp ebx, 16
    ja mesaj ;endif
    
    
    while:
        mov edx, 0
        div ebx
        push EDX
        inc Edi
        cmp eax,0
        jne while
    
    afisare:
        dec Edi
        pop EDX
        cmp EDX, 10
        jb print_number
        jae print_character
        
        print_number:
            ;sub EDX, 10
            add EDX, 48
            PRINT_CHAR EDX
            
        print_character:
            ;sub EDX, 10
            add EDX, 87
            PRINT_CHAR EDX
        cmp edi, 0
        jne afisare
        ;NEWLINE
    cont:
    ;NEWLINE
    inc ecx
    cmp ecx, [nums]
    jne print_newline
    jmp exit
  mesaj: 
        PRINT_STRING string
        jmp cont
  print_newline:
    NEWLINE
    jmp Bucla_for
  exit:  
    xor eax, eax
    ret

