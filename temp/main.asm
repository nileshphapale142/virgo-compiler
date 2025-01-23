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
	mov rcx, [rsp + 0]
	imul rbx, rcx
	add rax, rbx
	cmp rax, 0
	je if_end1
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
	je if_end2
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
if_end2:
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
if_end1:
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
