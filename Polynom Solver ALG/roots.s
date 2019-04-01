; %1 - order of polynomial
; purpose: malloc((order+1) *sizeof(double)*2);
%macro call_malloc 1
; @TODO check >=0
    mov rdi, %1
    lea rdi, [rdi*8]
    lea rdi, [rdi*2 + 16]
    push rdi
    xor rax, rax
    call malloc
    add rsp, 8
    cmp rax, 0
    jnz .MallocSuccess
    mov rdi, fs_malloc_failed
    call printf
    jmp end
.MallocSuccess:
%endmacro


;place initial in p
;answer stored at res
;%1 poly address
;%2 order
%macro computePoly 2
	enter 0,0
	mov rax, [a_real]
	push rax
	mov rax, [a_imag]
	push rax
	mov rax, [b_real]
	push rax
	mov rax, [b_imag]
	push rax
	mov qword [res_real], 0
	mov qword [res_imag], 0
	mov rcx, %2
%%loop:
	mov rax, [res_real]
	mov [a_real], rax
	mov rax, [res_imag]
	mov [a_imag], rax
	mov rax, [initial_real]
	mov [b_real], rax
	mov rax, [initial_imag]
	mov [b_imag], rax
	call multComplex

	mov rax, [res_real]
	mov [a_real], rax
	mov rax, [res_imag]
	mov [a_imag], rax
	mov rbx, %1
	mov rsi, rcx
	lea rsi, [rsi*8]
	lea rsi, [rsi*2]
	lea rsi, [rbx+rsi]
	mov rbx, [rsi]
	mov [b_real], rbx
	mov rbx, [rsi+8]
	mov [b_imag], rbx
	call addComplex

	dec rcx
	jnl %%loop

	pop rax
	mov [b_imag], rax
	pop rax
	mov [b_real], rax
	pop rax
	mov [a_imag], rax
	pop rax
	mov [a_real], rax
	leave
%endmacro

%macro norma2 1
	fld qword [%1]
	fld qword [%1]
	fmulp
	fld qword [%1+8]
	fld qword [%1+8]
	fmulp
	faddp
%endmacro

%macro printResult 0
	mov rdi, print_result
	movsd xmm0, [initial_real]
	movsd xmm1, [initial_imag]
	mov rax, 2
	call printf
%endmacro

%macro freeAll 0
	mov rdi, [p]
	call free
	mov rdi, [dp]
	call free
%endmacro

%macro OrderZero 0
	mov rdi, print_order_error
	xor rax, rax
	call printf
	jmp end
%endmacro

section .data
;Scans:
    scanf_epsilon_format:   db "epsilon = %lf", 0
    scanf_order_format:     db " order = %ld", 0
    scanf_coeff_idx_format: db " coeff %ld", 0
    scanf_coeff_val_format: db " = %lf %lf", 0
    scanf_initial_format:   db " initial = %lf %lf", 0

;Prints:
	fs_malloc_failed: db "A call to malloc() failed", 10, 0
	print_result: db "root = %.16e %.16e", 10, 0
	print_order_error: db "Order cannot be 0.", 10, 0

	inRange db 0

section .bss
	a_real: resq 1
	a_imag: resq 1
	b_real: resq 1
	b_imag: resq 1
	res_real: resq 1
	res_imag: resq 1
	epsilon: resq 1
	order: resq 1
	p: resq 1
	dp: resq 1
	initial_real: resq 1
	initial_imag: resq 1
	coeff_index: resq 1
	temp: resq 1


extern malloc, free, printf, scanf
global main
section .text

; gets complex a, complex b
addComplex:
	fld qword [a_real]
	fld qword [b_real]
	faddp
	fstp qword [res_real]
	fld qword [a_imag]
	fld qword [b_imag]
	faddp
	fstp qword [res_imag]
	ret

subComplex:
	fld qword [a_real]
	fld qword [b_real]
	fsubp
	fstp qword [res_real]
	fld qword [a_imag]
	fld qword [b_imag]
	fsubp
	fstp qword [res_imag]
	ret

multComplex:
	fld qword [a_real]
	fld qword [b_real]
	fmulp
	fld qword [a_imag]
	fld qword[b_imag]
	fmulp
	fsubp
	fstp qword [res_real]
	fld qword [a_real]
	fld qword[b_imag]
	fmulp
	fld qword [a_imag]
	fld qword [b_real]
	fmulp
	faddp
	fstp qword [res_imag]
	ret

divComplex:
;real
	fld qword [a_real]
	fld qword [b_real]
	fmulp
	fld qword [a_imag]
	fld qword [b_imag]
	fmulp
	faddp
	fld qword [b_real]
	fld qword [b_real]
	fmulp
	fld qword [b_imag]
	fld qword [b_imag]
	fmulp
	faddp
	fdivp
	fstp qword [res_real]
;imag
	fld qword [a_imag]
	fld qword [b_real]
	fmulp
	fld qword [a_real]
	fld qword [b_imag]
	fmulp
	fsubp
	fld qword [b_real]
	fld qword [b_real]
	fmulp
	fld qword [b_imag]
	fld qword [b_imag]
	fmulp
	faddp
	fdivp
	fstp qword [res_imag]
	ret

derive:
	mov rcx, [order]
	dec rcx
	push rcx
	call_malloc rcx
	pop rcx
	mov [dp], rax
.loop:
	mov rsi, rcx
    lea rsi, [rsi*8]
    lea rsi, [rsi*2]
    mov rbx, [dp]
    lea rdx, [rsi+16]
    lea rsi, [rbx+rsi]
    mov rbx, [p]
    lea rdx, [rbx+rdx]
    fld qword [rdx]
    inc rcx
	mov [temp], rcx
    fild qword [temp]
    dec rcx
    fmulp
    fstp qword [rsi]
    jmp .imag
.MidLoop:
	jmp .loop
.imag:    
    mov rsi, rcx
    lea rsi, [rsi*8]
    lea rsi, [rsi*2]
    mov rbx, [dp]
    lea rdx, [rsi+16]
    lea rsi, [rbx+rsi+8]
    mov rbx, [p]
    lea rdx, [rbx+rdx+8]
    fld qword [rdx]
    fild qword [temp]
    fmulp
    fstp qword [rsi]
    dec rcx
	jnl .MidLoop
	ret


isInRange:
	enter 0,0
	computePoly [p], [order]
	lea rdx,[res_real]
	norma2 rdx
	fld qword [epsilon]
	fld qword [epsilon]
	fmulp
	fcompp
	fstsw ax
	sahf 
	jb .Leave
	mov byte [inRange], 1 	
.Leave:
	leave
	ret

; Recursive..
findRoot:
	enter 0,0
	
	computePoly [p], [order]
	movsd xmm0, [res_real]
	movsd [a_real], xmm0
	movsd xmm0, [res_imag]
	movsd [a_imag], xmm0

	mov rbx, [order]
	dec rbx
	computePoly [dp], rbx

	movsd xmm0, [res_real]
	movsd [b_real], xmm0
	movsd xmm0, [res_imag]
	movsd [b_imag], xmm0

	call divComplex

	movsd xmm0, [res_real]
	movsd [b_real], xmm0
	movsd xmm0, [res_imag]
	movsd [b_imag], xmm0

	movsd xmm0, [initial_real]
	movsd [a_real], xmm0
	movsd xmm0, [initial_imag]
	movsd [a_imag], xmm0

	call subComplex

	movsd xmm0, [res_real]
	movsd [initial_real], xmm0
	movsd xmm0, [res_imag]
	movsd [initial_imag], xmm0
	call isInRange
	cmp byte [inRange], 1
	jz .FoundRoot
	call findRoot

.FoundRoot:
	leave
	ret


getinput:
	enter 0,0
	mov rdi, scanf_epsilon_format
	mov rsi, epsilon
	xor rax, rax
	call scanf
	mov rdi, scanf_order_format
	mov rsi, order
	xor rax, rax
	call scanf

	cmp qword [order], 0
	jnz .ContinueGetInput
	jmp .end

.ContinueGetInput:
	call_malloc [order]
	mov [p], rax
	
	mov rcx, [order]
	inc rcx
.loop:
	push rcx
    mov rdi, scanf_coeff_idx_format
    mov rsi, coeff_index
    xor rax, rax
    call scanf
    mov rdi, scanf_coeff_val_format
    mov rsi, [coeff_index]
    lea rsi, [rsi*8]
    lea rsi, [rsi*2]
    mov rbx, [p]
    lea rsi, [rbx+rsi]
    mov rdx, [coeff_index]
    lea rdx, [rdx*8]
    lea rdx, [rdx*2]
    mov rbx, [p]
    lea rdx, [rbx+rdx+8]
    xor rax, rax
    call scanf
    pop rcx
    dec rcx
    jz .Continue
	jmp .loop
.Continue:
    mov rdi, scanf_initial_format
    mov rsi, initial_real
    mov rdx, initial_imag
    xor rax, rax
    call scanf
.end:
    leave
    ret

main:
	enter 0,0
	finit

	call getinput
	cmp qword [order], 0
	jnz .ContinueMain
	OrderZero
.ContinueMain:
	call derive
	call findRoot
	printResult
	freeAll

end:
	leave
	ret