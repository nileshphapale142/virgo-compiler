section .bss
	print_str resb 20
section .text
	global _start
_start:
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
	add rax, rbx
	push rax
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
	push rax
	mov rax, rbx
	mov rcx, [rsp + 8]
	xor rdx, rdx
	div rcx
	mov rbx, rax
	pop rax
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
