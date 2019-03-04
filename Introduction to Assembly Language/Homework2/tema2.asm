extern puts
extern printf

section .data
filename: db "./input.dat",0
inputlen: dd 2263
fmtstr: db "Key: %d",0xa,0

section .text
global main

; TODO: define functions and helper functions

xor_strings:
    push ebp
    mov ebp, esp
    
    ;ia adresele parametrilor de pe stiva
    mov eax, [ebp + 8]
    mov ebx, [ebp + 12]
    xor edx, edx
  
    mov ecx,0
    ;bucla realizeaza xor pe tot sirului
    parcurge_sir:
    mov dh, byte [eax + ecx]    ;ia byte din string
    mov dl, byte [ebx + ecx]    ;ia byte din cheie
    
    xor dh, dl      ;realizeaza xor intre ele
    mov byte [eax + ecx], dh    ;pune inapoi in sir
    inc ecx     ;muta indexul in sir
    
    cmp byte [eax + ecx], 0x00  ;repeta cat din nu se intalneste null
    jne parcurge_sir
        
    leave
    ret   

rolling_xor:
	push ebp
	mov ebp, esp

	mov eax, [ebp+8] ; pointer la sir
	mov ecx, 0

	mov bl, byte [eax+ecx] ; iau primul caracter din sir

repeta_string2_stiva:
	sub esp, 1 ; aloc pe stiva
	mov [esp], bl ; pun caracter cu caracter pe stiva

	inc ecx
	mov bl, byte [eax+ecx] ; iau urmatorul caracter
	cmp bl, 0 ; daca este null ma opresc
	jne repeta_string2_stiva

	mov esi, 1 ; contor pentru parcurgere string stiva
	sub ecx, 1 ; contor pentru parcurgere string original
roll:
	mov dl, byte [esp+ecx] ; caracter criptat la n+1
	mov dh, byte [eax+esi] ; caracter decriptat la n
	xor dh, dl ; xor intre n si n+1
	mov [eax+esi], dh ; il pun in stringul original
	dec ecx ; stiva se parcurge invers
	inc esi
	cmp ecx, 0 ; daca am terminat
	jg roll

	leave
	ret

xor_hex_strings:
    push ebp
    mov ebp, esp
    
    
    mov eax, [ebp + 8]
    mov ebx, [ebp + 12]
    
    xor ecx,ecx
    xor edx,edx
    
    mov esi, 0
    char_hex_string:
    ;transforma tot sirul din caracter ascii in hexa byte cu byte
    mov ch, byte [eax + esi]
    cmp ch, '9'     ;determina dace litera sau cifra pentru transformarea din ascii
    ja letter0
    jle digit0
    letter0:
    sub ch, 87
    jmp next0
    digit0:
    sub ch, 48
    next0:
    mov byte [eax + esi], ch
    inc esi
    cmp byte [eax + esi], 0x00
    jne char_hex_string
    
    mov esi, 0
    char_hex_key:
    ;transforma cheia din caracter ascii in hexa byte cu byte
    mov ch, byte [ebx + esi]
    cmp ch, '9'
    ja letter1
    jle digit1
    letter1:
    sub ch, 87
    jmp next1
    digit1:
    sub ch, 48
    next1:
    mov byte [ebx + esi], ch
    inc esi     ;se va afla la sfarsitul buclei lungimea sirului
    cmp byte [ebx + esi], 0x00
    jne char_hex_key
    
   
    xor ecx,ecx
    xor edx, edx
    mov edx, esi       ;salveaza valoarea lui esi in edx
    mov esi, 0
    
    bucla_decr:
    mov ch, byte [eax + esi]        ;citeste byte din sir
    shl ch,4        ;shiftam la stanga cu 4 pentru a avea bitii pe primele 4 pozitii din byte
    add ch, byte [eax + esi + 1]    ;adauga la byte-ul respectiv bitii din urmatorul caracter din sir
    mov cl, byte [ebx + esi]        ;citeste byte din cheie
    shl cl,4        ;se repeta acelasi proces ca pentru byte-ul din sir
    add cl,byte [ebx + esi + 1]
    xor ch, cl      ;se realizeaza xor intre cele doua caractere nemodificate
    mov byte [eax + esi], ch
    add esi,2   ;urmatoarele doua caractere
    cmp esi, edx    ;se verifica daca s-a ajuns la sfarsitul sirului
    jl bucla_decr

    
    mov ecx, 0
    mov esi, 0
    
    bucla_mutare:
    ;muta caracterele de pe pozitia i pe pozitia i/2
    mov dl, byte [eax + ecx]
    mov byte [eax + esi], dl
    inc esi
    add ecx,2
    cmp byte [eax + ecx], 0x00
    jne bucla_mutare
    
    bucla_zero:
    ;adauga zerouri pe jumatatea de string nefolosita
    mov byte [eax + esi], 0x00
    inc esi
    cmp esi, ecx
    jl bucla_zero

	xor esi,esi

	bucla_add_one:
        ;adauga un 1 la fiecare element din cheie pentru a nu mai avea valori de tipul 0x00
	add byte[ebx + esi], 1
	inc esi
	cmp esi, ecx
	jne bucla_add_one
    
    leave 
    ret
base32decode:
	push ebp
	mov ebp, esp

	mov eax, [ebp+8]
	mov ecx, 0
	mov bl, byte [eax]

repeta_conversie:
	sub esp, 1 ; aloc e stiva
	cmp bl, 60 ; iau 60 ca referinta in tabelul ASCII
	jg litera
	jb cifra

	litera:
	sub bl, 65 ; conversie la tabel pentru litera
	jmp schimbat

	cifra:
	sub bl, 24 ; conversie la tabel pentru cifta
	jmp schimbat
	
	schimbat:
	mov [esp], bl ; o pun pe stiva

	inc ecx ; trec la urmatoarea
	mov bl, byte[eax+ecx] ; conditie pentru null
	cmp bl, 0
	jne repeta_conversie

	mov edi, ecx
	mov esi, 0

shifteaza: ; incep seria lunga de shiftari pe stiva
	sub ecx, 2
	mov bx, word [esp+ecx] ; iau perechi de cate doi bytes (high:low)
	shl bh, 3 
	shr bl, 2
	add bl, bh ; adun in bl valorile celor doua parti
	mov [eax+esi], bl ; scriu valoarea in sirul original

	dec ecx ; ecx scade pentru ca sunt pe stiva
	mov bx, word [esp+ecx]
	shl bh, 6
	shl bl, 1
	add bh, bl
	dec ecx
	mov bl, byte [esp+ecx] ; aici este cazul in care dupa primele doua shiftari mai am loc in byte
	shr bl, 4
	xor bh, bl
	mov [eax+esi+1], bh

	dec ecx
	mov bx, word [esp+ecx]
	shr bl, 1
	shl bh, 4
	add bh, bl
	mov [eax+esi+2], bh

	dec ecx
	mov bx, word [esp+ecx]
	shl bl, 2
	shl bh, 7
	add bl, bh
	dec ecx
	mov bh, byte [esp+ecx]
	shr bh, 3
	add bl, bh
	mov [eax+esi+3], bl

	dec ecx
	mov bx, word [esp+ecx]
	shl bh, 5
	add bh, bl
	mov [eax+esi+4], bh

	add esi, 5 ; increment din 5 in 5 pentru ca procesez 5 bytes per bucla
	cmp ecx, 0
	jg shifteaza

	sub esi, 3
	mov byte [eax+esi], 0 ; setez null terminator

	add esp, edi
	
	leave
	ret

main:
    mov ebp, esp; for correct debugging
    push ebp
    mov ebp, esp
    sub esp, 2300
    
    ; fd = open("./input.dat", O_RDONLY);
    mov eax, 5
    mov ebx, filename
    xor ecx, ecx
    xor edx, edx
    int 0x80
    
	; read(fd, ebp-2300, inputlen);
	mov ebx, eax
	mov eax, 3
	lea ecx, [ebp-2300]
	mov edx, [inputlen]
	int 0x80

	; close(fd);
	mov eax, 6
	int 0x80

	; all input.dat contents are now in ecx (address on stack)

	; TASK 1: Simple XOR between two byte streams
	; TODO: compute addresses on stack for str1 and str2
	; TODO: XOR them byte by byte
        mov al, 0x00
        mov edi, ecx
        mov ebx, ecx
        repne scasb     ;calculeaza adresa cheii
        
	push edi
	push ebx
	call xor_strings
	add esp, 8
        
        ; Print the first resulting string
	push eax
	call puts
	add esp, 4

	; TASK 2: Rolling XOR
	mov al, 0x00
        repne scasb     ;calculeaza adresa sirului
        
        push edi
	call rolling_xor
	add esp, 4

	; Print the second resulting string
	push eax
	call puts
	add esp, 4

	
	; TASK 3: XORing strings represented as hex strings
	; TODO: compute addresses on stack for strings 4 and 5
	; TODO: implement and apply xor_hex_strings
		
	mov al, 0x00
        repne scasb
        mov ebx, edi        ;adresa string
       
        mov al, 0x00
        repne scasb         ;adresa cheii

        push edi
        push ebx
        call xor_hex_strings
        add esp, 8

	; Print the third string
	push eax
	call puts
	add esp, 4
	
	; TASK 4: decoding a base32-encoded string
	mov al, 0x00
        repne scasb     ;adresa string
        push edi
        call base32decode
        add esp, 4

	; Print the fourth string
	push eax
	call puts
	add esp, 4

	; TASK 5: Find the single-byte key used in a XOR encoding
	; TODO: determine address on stack for string 7
	; TODO: implement and apply bruteforce_singlebyte_xor
	;push key_addr
	;push addr_str7
	;call bruteforce_singlebyte_xor
	;add esp, 8

	; Print the fifth string and the found key value
	;push addr_str7
	;call puts
	;add esp, 4

	;push keyvalue
	;push fmtstr
	;call printf
	;add esp, 8

	; TASK 6: Break substitution cipher
	; TODO: determine address on stack for string 8
	; TODO: implement break_substitution
	;push substitution_table_addr
	;push addr_str8
	;call break_substitution
	;add esp, 8

	; Print final solution (after some trial and error)
	;push addr_str8
	;call puts
	;add esp, 4

	; Print substitution table
	;push substitution_table_addr
	;call puts
	;add esp, 4

	; Phew, finally done
    xor eax, eax
    leave
    ret
