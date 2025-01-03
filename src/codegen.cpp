#include "codegen.h"
#include <iostream>

CodeGenerator::CodeGenerator(NodeProgram root)  
: root(root), label_cnt(0) {}

std::string CodeGenerator::generate() {

	output_code << "section .data\n";
	collect_section_data(root.stmt_list);
	
	output_code << "section .bss\n";
	collect_section_bss(root.stmt_list);

	output_code << "section .text\n";
	output_code << "	global _start\n";

	output_code << "_start:\n";

	label_cnt = 0;

	handle_stmt_list(root.stmt_list);

	output_code << "	mov rax, 1\n";
	output_code << "	xor rbx, rbx\n";
	output_code << "	int 0x80\n";

	return output_code.str();
}

void CodeGenerator::handle_stmt_list(NodeStmtList stmt_list) {
	for (auto stmt : stmt_list.stmts) {
		handle_stmt(stmt);
	}
} 


void CodeGenerator::handle_stmt(NodeStmt stmt) {
	generate_print_code(stmt.print);
}


void CodeGenerator::collect_section_data(NodeStmtList stmt_list) {
	label_cnt = 0;

	for (auto& stmt : stmt_list.stmts) {
		++label_cnt;
		std::string value = stmt.print.expr.term.u_int.value.value_or("");

		output_code << "	msg_" << label_cnt << " db \"" << value << "\", 0\n";
	}
}

void CodeGenerator::collect_section_bss(NodeStmtList stmt_list) {
	label_cnt = 0;
	
	for (auto stmt : stmt_list.stmts) {
		++label_cnt;
		output_code << "	length_" << label_cnt << " resb 1\n"; //todo: 1 byte length variable
	}
}



void CodeGenerator::generate_print_code(NodePrint print_node) {
	++label_cnt;

	output_code << "	mov rsi, msg_"  << label_cnt << "\n";
	output_code << "	xor rcx, rcx\n";
	output_code << "find_length_" << label_cnt << ":\n";
	output_code << "	cmp byte [rsi + rcx], 0\n";
	output_code << "	je done_" << label_cnt << "\n";
	output_code << "	inc rcx\n";
	output_code << "	jmp find_length_" << label_cnt << "\n";
	output_code << "done_" << label_cnt << ":\n";
	output_code << "	mov [length_" << label_cnt << "], rcx\n";
	output_code << "	mov rax, 4\n";
	output_code << "	mov rbx, 1\n";
	output_code << "	mov rcx, msg_" << label_cnt << "\n";
	output_code << "	mov dl, [length_" << label_cnt << "]\n";
	output_code << "	int 0x80\n";
}