extern printf, scanf, malloc, calloc, realloc, free, exit
global main

%define MAX_PHYSICAL_MEMORY 4096

%macro call_realloc 0

	inc qword [m_size]
    mov rdi, [m]
    mov rsi,[m_size]
    lea rsi,[rsi*8]
    xor rax, rax
    call realloc

    cmp rax, 0
    jnz %%reallocSuccess

    mov rdi, fs_realloc_failed
    call printf
    jmp end

%%reallocSuccess:
	mov [m],rax
%endmacro


section .data

	fs_realloc_failed: 	db "A call to realloc() failed", 10, 0
	scanf_long_format:  db "%ld", 0
	printf_format:		db "%ld ", 0
	print_newline:		db 10, 0
	m_size: 			dq 1
	index:				dq 0

section .bss

	input:	resq 1
	m:		resq 1
	temp:	resq 1

section .text
;----------------------------main--------------------------
main:
	
	call first_alloc
	call getinput
	call parse_sic
	call print_sic_program
	call do_free
end:
	call exit
;----------------------------------------------------------
getinput:;-------------------input-------------------------
	mov rdi, scanf_long_format
	mov rsi, input
	xor rax, rax
	call scanf
	cmp qword rax,1				;EOF
	jnz .finish

	mov rbx,[m_size]			
	dec rbx							
	mov rdx,[input]
	mov rdi, [m]
	lea rbx, [rdi+rbx*8]				
	mov [rbx],rdx	

	call_realloc
	jmp getinput
	
.finish:
	sub qword [m_size], 2
	call_realloc

	ret
;----------------------------------------------------------
first_alloc:
	enter 0,0
	mov rdi,1
	mov rsi,8
	xor rax,rax
	call calloc
	mov [m],rax
	leave
	ret
;----------------------------------------------------------
do_free:
	enter 0,0
	mov rdi, [m]
	xor rax, rax
	call free
	leave
	ret
;----------------------------------------------------------
parse_sic:
	enter 0,0
.whileLoop:
	mov rdi, [m]
	mov rdx, [index]
	cmp qword [rdi+rdx*8], 0
	jnz .Cont
	cmp qword [rdi+rdx*8+1*8], 0
	jnz .Cont
	cmp qword [rdi+rdx*8+2*8], 0
	jz .finish

.Cont:
	cmp [m_size], rdx
	jle .finish
	mov rbx, [rdi+rdx*8]
	mov rbx, [rdi+rbx*8]		;rbx = M[M[i]]

	mov rax, [rdi+rdx*8+1*8]
	mov rax, [rdi+rax*8]		;rax = M[M[i + 1]]

	sub rbx, rax
	mov rsi, [rdi+rdx*8]
	mov [rdi+rsi*8], rbx		; M[M[i]] = M[M[i]] - M[M[i + 1]]
	cmp rbx, 0
	jge .nextLine

	mov rbx, [rdi+rdx*8+2*8]	;rbx = M[i + 2]
	mov [index], rbx
	jmp .whileLoop
.nextLine:
	add qword [index], 3

	jmp .whileLoop
.finish:
	leave
	ret
;----------------------------------------------------------
print_sic_program:
	enter 0,0
	xor rcx, rcx
.loop:
	mov rdi, printf_format
	mov rsi, [m]
	mov rsi, [rsi+rcx*8]
	xor rax, rax
	push rcx
	call printf
	.test:
	pop rcx
	inc rcx
	cmp rcx, [m_size]
	jnz .loop
	mov rdi,print_newline
	xor rax, rax
	call printf
	leave
	ret