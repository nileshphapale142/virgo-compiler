#include "codegen.h"
#include <iostream>

CodeGenerator::CodeGenerator(P_Rule root) : root(root) {}

std::string CodeGenerator::generate() {
	if (root.lhs != P_Name::PRINT) {
		std::cerr << "Invalid syntax" << std::endl;
		exit(EXIT_FAILURE);
	}

	output_code << "section .data\n";
	output_code << "	msg db \"" << root.rhs.at(2).value.value_or("") << "\", 0\n" ;
	output_code << "section .bss\n";
	output_code << "	length resb 1\n";
	output_code << "section .text\n";
	output_code << "	global _start\n";
	output_code << "_start:\n";
	output_code << "	mov rsi, msg\n";
	output_code << "	xor rcx, rcx\n";
	output_code << "find_length:\n";
	output_code << "	cmp byte [rsi + rcx], 0\n";
	output_code << "	je done\n";
	output_code << "	inc rcx\n";
	output_code << "	jmp find_length\n";
	output_code << "done:\n";
	output_code << "	mov [length], rcx\n";
	output_code << "	mov rax, 4\n";
	output_code << "	mov rbx, 1\n";
	output_code << "	mov rcx, msg\n";
	output_code << "	mov dl, [length]\n";
	output_code << "	int 0x80\n";
	output_code << "	mov rax, 1\n";
	output_code << "	xor rbx, rbx\n";
	output_code << "	int 0x80\n";

	return output_code.str();
}