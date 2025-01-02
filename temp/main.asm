section .data
	msg_1 db "", 0
	msg_2 db "543", 0
section .bss
	length_1 resb 1
	length_2 resb 1
section .text
	global _start
_start:
	mov rsi, msg_1
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
	mov rcx, msg_1
	mov dl, [length_1]
	int 0x80
	mov rsi, msg_2
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
	mov rcx, msg_2
	mov dl, [length_2]
	int 0x80
	mov rax, 1
	xor rbx, rbx
	int 0x80
