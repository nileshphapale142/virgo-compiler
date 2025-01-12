section .bss
	length_1 resb 1
	msg_1 resb 20
	length_2 resb 1
	msg_2 resb 20
section .text
	global _start
_start:
	mov rax, 0
	add rax, 00000
	lea rdi, [msg_1+ 20]
	call int_to_str_1
 	mov byte [length_1], al
int_to_str_1:
	push rbx
	push rcx
	push rdx
	xor rcx, rcx
	mov rbx, 10
convert_loop_1:
	xor rdx, rdx
	div rbx
	add dl, '0'
	dec rdi
	mov [rdi], dl
	inc rcx
	test rax, rax
	jnz convert_loop_1
	mov rax, rcx
	pop rdx
	pop rcx
	pop rbx
	ret
	lea rsi, [msg_1]
	xor rcx, rcx
find_length_1:
	cmp byte [rsi + rcx], 0
	je done_1
	inc rcx
	jmp find_length_1
done_1:
	mov [length_1], rcx
	mov rax, 4
	mov rbx, 1
	lea rcx, [msg_1]
	mov dl, [length_1]
	int 0x80
	mov rax, 0
	add rax, 543
	lea rdi, [msg_2+ 20]
	call int_to_str_2
 	mov byte [length_2], al
int_to_str_2:
	push rbx
	push rcx
	push rdx
	xor rcx, rcx
	mov rbx, 10
convert_loop_2:
	xor rdx, rdx
	div rbx
	add dl, '0'
	dec rdi
	mov [rdi], dl
	inc rcx
	test rax, rax
	jnz convert_loop_2
	mov rax, rcx
	pop rdx
	pop rcx
	pop rbx
	ret
	lea rsi, [msg_2]
	xor rcx, rcx
find_length_2:
	cmp byte [rsi + rcx], 0
	je done_2
	inc rcx
	jmp find_length_2
done_2:
	mov [length_2], rcx
	mov rax, 4
	mov rbx, 1
	lea rcx, [msg_2]
	mov dl, [length_2]
	int 0x80
	mov rax, 1
	xor rbx, rbx
	int 0x80
