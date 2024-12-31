section .data
	msg db "8", 0
section .bss
	length resb 1
section .text
	global _start
_start:
	mov rsi, msg
	xor rcx, rcx
find_length:
	cmp byte [rsi + rcx], 0
	je done
	inc rcx
	jmp find_length
done:
	mov [length], rcx
	mov rax, 4
	mov rbx, 1
	mov rcx, msg
	mov dl, [length]
	int 0x80
	mov rax, 1
	xor rbx, rbx
	int 0x80
