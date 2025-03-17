section .bss
	print_str resb 20
section .text
	global _start
_start:
	mov rax, 0
	mov rbx, 1
	mov rcx, 5
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 5
	imul rbx, rcx
	push rax
	mov rax, rbx
	mov rcx, 2
	xor rdx, rdx
	div rcx
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
	add rax, rbx
	push rax
for_start_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 16]
	imul rbx, rcx
	add rax, rbx
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 2
	imul rbx, rcx
	add rax, rbx
	push rax
	cmp rax, 0
	jl for_check_dec_1
	pop rcx
	pop rbx
	pop rax
	cmp rax, rbx
	jg for_end_1
	push rax
	push rcx
	jmp for_check_end_1
for_check_dec_1:
	pop rcx
	pop rbx
	pop rax
	cmp rax, rbx
	jl for_end_1
	push rax
	push rcx
for_check_end_1:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	add rax, rbx
	push rax
for_start_2:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 24]
	imul rbx, rcx
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	jg for_end_2
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
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
	inc qword [rsp]
	jmp for_start_2
for_end_2:
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
	add rax, rbx
	push rax
for_start_3:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 16]
	imul rbx, rcx
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	jg for_end_3
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 0
	imul rbx, rcx
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
	inc qword [rsp]
	jmp for_start_3
for_end_3:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 8]
	imul rbx, rcx
	add rax, rbx
	push rax
for_start_4:
	mov rax, 0
	mov rbx, 1
	mov rcx, [rsp + 24]
	imul rbx, rcx
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	jg for_end_4
	push rax
	mov rax, 0
	mov rbx, 1
	mov rcx, 1
	imul rbx, rcx
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
	inc qword [rsp]
	jmp for_start_4
for_end_4:
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
	pop rbx
	pop rax
	add rax, rbx
	push rax
	jmp for_start_1
for_end_1:
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
