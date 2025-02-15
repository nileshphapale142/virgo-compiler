section .bss
	print_str resb 20
section .text
	global _start
_start:
	mov rax, 0
	mov rbx, 1
	mov rcx, 0
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
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
while_start_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 5
	imul rbx, rcx
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	jge while_end_1
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	add rax, rbx
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	add rax, rbx
	mov [rsp + 16], rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	mov [rsp + 8], rax
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 3
	imul rbx, rcx
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	jne if_end_1
	mov rax, 0
	mov rbx, 1
	mov rcx, 5
	imul rbx, rcx
	add rax, rbx
	mov [rsp + 0], rax
	jmp cond_end_1
if_end_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
	sub rax, rbx
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
cond_end_1:
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
	jmp while_start_1
while_end_1:
	mov rax, 60
	xor rdi, rdi
	syscall
itoa:
	xor rbx, rbx
	cmp rax, 0
	jge handle_negative_end
	mov rbx,1
	neg rax
handle_negative_end:
	mov rcx, 10
itoa_loop:
	xor rdx, rdx
	div rcx
	add dl, '0'
	dec rdi
 	mov [rdi], dl
	test rax, rax
	jnz itoa_loop
	cmp rbx, 0
	je itoa_end
	dec rdi
	mov byte [rdi], 45
itoa_end:
	ret
