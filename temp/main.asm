section .bss
	print_str resb 20
section .text
	global _start
_start:
	mov rax, 0
	mov rbx, 543
	add rax, rbx
	mov rbx, 123
	sub rax, rbx
	mov rbx, 10
	add rax, rbx
	lea rdi, [print_str + 19]
	mov r8, rdi
	call itoa
	mov rdx, r8
	sub rdx, rdi
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
