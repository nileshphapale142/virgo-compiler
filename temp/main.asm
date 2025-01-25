section .bss
	print_str resb 20
section .text
	global _start
_start:
	mov rax, 0
	mov rbx, 1
	mov rcx, 12
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 0
	imul rbx, rcx
	add rax, rbx
	cmp rax, 0
	je if_end_1
	mov rax, 0
	mov rbx, 1
	mov rcx, 12
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	sub rax, rbx
	cmp rax, 0
	je if_end_2
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	mov rbx, 1
	mov rcx, 5
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
	inc rdx
	mov byte [r8 + 1], 10
	inc rdx
	mov rax, 1
	mov rsi, rdi
	mov rdi, 1
	syscall
	pop rax
jmp cond_end_2
if_end_2:
	mov rax, 0
	mov rbx, 1
	mov rcx, 0
	imul rbx, rcx
	add rax, rbx
	cmp rax, 0
	je elif_end_2
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
	inc rdx
	mov byte [r8 + 1], 10
	inc rdx
	mov rax, 1
	mov rsi, rdi
	mov rdi, 1
	syscall
jmp cond_end_2
elif_end_2:
	mov rax, 0
	mov rbx, 1
	mov rcx, 32
	imul rbx, rcx
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
	inc rdx
	mov byte [r8 + 1], 10
	inc rdx
	mov rax, 1
	mov rsi, rdi
	mov rdi, 1
	syscall
cond_end_2:
	pop rax
jmp cond_end_1
if_end_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, 0
	imul rbx, rcx
	add rax, rbx
	cmp rax, 0
	je elif_end_1
	mov rax, 0
	mov rbx, 1
	mov rcx, 123
	imul rbx, rcx
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
	inc rdx
	mov byte [r8 + 1], 10
	inc rdx
	mov rax, 1
	mov rsi, rdi
	mov rdi, 1
	syscall
jmp cond_end_1
elif_end_1:
cond_end_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, 213
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
	inc rdx
	mov byte [r8 + 1], 10
	inc rdx
	mov rax, 1
	mov rsi, rdi
	mov rdi, 1
	syscall
	mov rax, 60
	xor rdi, rdi
	syscall
itoa:
	xor rbx, rbx
	mov rcx, 10
itoa_loop:
	xor rdx, rdx
	div rcx
	add dl, '0'
	dec rdi
 	mov [rdi], dl
	test rax, rax
	jnz itoa_loop
	ret
