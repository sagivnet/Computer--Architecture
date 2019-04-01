section .data
a:  dq 0
b:  dq 0
res: dq 0



section .text
global add1, sub1, mult2, div2

mult2:
    push rbp
    mov rbp, rsp
    sub rsp, 1
    mov byte [rbp-1], 0
    mov rcx, [rdi+8]
    mov rdi, [rdi+16]
.loop:
    mov rbx, rcx
    dec rbx
    mov al, [rdi+rbx]
    sub al, '0' ;al is 0-9
    shl al, 1
    cmp byte [rbp-1], 1
    jnz .ContinueNormally
    inc al
.ContinueNormally:
    mov byte [rbp-1], 0
    cmp al, 10
    jge .CarryHandling
.ContinuePushing:
    add al, '0'
    mov [rdi+rbx], al
    dec rcx
    cmp rcx, 1
    jnz .loop
    cmp byte [rbp-1], 1
    jz .PutOne
    jmp .EndFunc

.CarryHandling:
    mov byte [rbp-1], 1
    sub al, 10
    jmp .ContinuePushing

.PutOne:
    mov byte [rdi], '1'


.EndFunc:
    mov rsp, rbp
    pop rbp
    ret



div2:
    mov rcx, [rdi+8]
    mov [a], rdi
    mov rdi, [rdi+16]
.loop:
    mov rbx, rcx
    dec rbx
    mov al, [rdi+rbx]
    sub al, '0' ;al is 0-9
    mov dl, al
    and dl, 1b
    cmp dl, 1
    jz .CarryHandling
.ContinuePushing:
    shr al, 1
    add al, '0'
    mov [rdi+rbx], al
    loop .loop
    jmp .EndFunc

.CarryHandling:
    mov rdi, [a]
    mov rdi, [rdi+8]
    cmp rcx, rdi
    mov rdi, [a]
    mov rdi, [rdi+16]
    jz .DoneHandling
    add byte [rdi+rbx+1], 5
.DoneHandling:
    jmp .ContinuePushing
.EndFunc:
    ret

; a >= b
add1:
    push rbp
    mov rbp, rsp
    sub rsp, 17
    mov byte [rbp-17], 0
    mov [a], rdi
    mov [b], rsi
    mov [res], rdx
    mov rbx, [res]
    mov rcx, [rbx+8] ; res number of digs
    mov rbx, [a]
    mov rax, [rbx+8]
    mov [rbp-8], rax ; rbp-8 is a local variable and its value is num of digits of a
    mov rbx, [b]
    mov rax, [rbx+8]
    mov [rbp-16], rax ; b num of digs
.loop:
    mov rbx, [rbp-8]
    dec rbx
    dec qword [rbp-8]
    cmp rbx, -1
    jz .AddZeros
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    mov rbx, [rbp-16]
    dec rbx
    dec qword [rbp-16]
    cmp rbx, -1
    jz .CopyADigits
    mov rdi, [b]
    mov rdi, [rdi+16]
    mov dl, [rdi+rbx]
    sub al, '0'
    sub dl, '0'
    cmp byte [rbp-17], 1
    clc
    jnz .KeepAdding
    stc
    jmp .KeepAdding
.MidLoop:
    jmp .loop
.KeepAdding:
    adc al, dl
    mov byte [rbp-17], 0
    cmp al, 10
    jge .CarryHandling
.ContinuePushing:
    add al, '0'
    mov rbx, rcx
    dec rbx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov [rdi+rbx], al
    loop .MidLoop
    jmp .EndFunc

.CarryHandling:
    mov byte [rbp-17], 1
    sub al, 10
    jmp .ContinuePushing

.AddZeros:
    cmp byte [rbp-17], 1
    jnz .ContinueAddZeros
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov byte [rdi], '1'
    jmp .KeepLooping
.ContinueAddZeros:
    mov rbx, rcx
    dec rbx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov byte [rdi+rbx], '0'
.KeepLooping:
    loop .ContinueAddZeros
    jmp .EndFunc

.CopyADigits:
    mov rbx, [rbp-8]
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    mov rbx, rcx
    dec rbx
    mov rdi, [res]
    mov rdi, [rdi+16]
    cmp byte [rbp-17], 1
    jnz .Dont
    jmp .AddWithCarry
.Ok:
    mov byte [rbp-17], 0
.Dont:
    mov [rdi+rbx], al
    dec qword [rbp-8]
    dec rcx
    cmp qword [rbp-8], -1
    jz .AddZeros
    inc rcx
    loop .CopyADigits
    jmp .EndFunc

.AddWithCarry:
    mov rbx, [rbp-8]
    cmp rbx, -1
    jz .AddZeros
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    inc al
    dec qword [rbp-8]
    cmp al, 10 + '0'
    jnz .ContinueAddCarry
    mov rbx, rcx
    dec rbx
    dec rcx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov byte [rdi+rbx], '0'
    jmp .AddWithCarry
.ContinueAddCarry:
    mov byte [rbp-17], 0
    mov rbx, rcx
    dec rbx
    dec rcx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov [rdi+rbx], al
    cmp qword [rbp-8], -1
    jz .AddZeros
    jmp .CopyADigits


.EndFunc:
    mov rsp, rbp
    pop rbp
    ret

; a >= b
sub1:
    push rbp
    mov rbp, rsp
    sub rsp, 17
    mov byte [rbp-17], 0
    mov [a], rdi
    mov [b], rsi
    mov [res], rdx
    mov rbx, [res]
    mov rcx, [rbx+8] ; res number of digs
    mov rbx, [a]
    mov rax, [rbx+8]
    mov [rbp-8], rax ; rbp-8 is a local variable and its value is num of digits of a
    mov rbx, [b]
    mov rax, [rbx+8]
    mov [rbp-16], rax ; b num of digs
.loop:
    mov rbx, [rbp-8]
    dec rbx
    dec qword [rbp-8]
    cmp rbx, -1
    jz .EndFunc
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    mov rbx, [rbp-16]
    dec rbx
    dec qword [rbp-16]
    cmp rbx, -1
    jz .CopyADigits
    mov rdi, [b]
    mov rdi, [rdi+16]
    mov dl, [rdi+rbx]
    sub al, '0'
    sub dl, '0'
    cmp byte [rbp-17], 1
    clc
    jnz .KeepSubbing
    stc
    jmp .KeepSubbing
.MidLoop:
    jmp .loop
.KeepSubbing:
    sbb al, dl
    mov byte [rbp-17], 0
    cmp al, 0
    jl .CarryHandling
.ContinuePushing:
    add al, '0'
    mov rbx, rcx
    dec rbx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov [rdi+rbx], al
    loop .MidLoop
    jmp .EndFunc

.CarryHandling:
    mov byte [rbp-17], 1
    add al, 10
    jmp .ContinuePushing

.CopyADigits:
    mov rbx, [rbp-8]
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    mov rbx, rcx
    dec rbx
    mov rdi, [res]
    mov rdi, [rdi+16]
    cmp byte [rbp-17], 1
    jnz .Dont
    jmp .SubWithCarry
.Ok:
    mov byte [rbp-17], 0
.Dont:
    mov [rdi+rbx], al
    dec qword [rbp-8]
    dec rcx
    inc rcx
    loop .CopyADigits
    jmp .EndFunc

.SubWithCarry:
    mov rbx, [rbp-8]
    cmp rbx, -1
    jz .EndFunc
    mov rdi, [a]
    mov rdi, [rdi+16]
    mov al, [rdi+rbx]
    dec al
    dec qword [rbp-8]
    cmp al, '0' - 1
    jnz .ContinueSubCarry
    mov rbx, rcx
    dec rbx
    dec rcx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov byte [rdi+rbx], '9'
    jmp .SubWithCarry
.ContinueSubCarry:
    mov byte [rbp-17], 0
    mov rbx, rcx
    dec rbx
    dec rcx
    mov rdi, [res]
    mov rdi, [rdi+16]
    mov [rdi+rbx], al
    cmp qword [rbp-8], -1
    jz .EndFunc
    jmp .CopyADigits


.EndFunc:
    mov rsp, rbp
    pop rbp
    ret