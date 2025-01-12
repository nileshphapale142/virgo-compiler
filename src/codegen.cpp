#include "codegen.h"
#include <iostream>

CodeGenerator::CodeGenerator(NodeProgram root)  
: root(std::move(root)), label_cnt(0) {}

std::string CodeGenerator::generate() {

	// collect_section_data(root.stmt_list);
	
	collect_section_bss(root.stmt_list);

	output_code.text << "section .text\n";
	output_code.text << "	global _start\n";

	output_code.start << "_start:\n";

	label_cnt = 0;

	handle_stmt_list(root.stmt_list);

	add_exit_code();

	return output_code.bss.str() + 
	output_code.data.str() + 
	output_code.text.str() + 
	output_code.start.str() + output_code.procs.str();
}



void CodeGenerator::collect_section_data(const NodeStmtList &stmt_list) {
	label_cnt = 0;

	// output_code.data << "section .data\n";

	// for (const auto& [print] : stmt_list.stmts) {
	// 	++label_cnt;
	// 	// std::string value = print.expr.term.u_int_lit.value.value_or("");

	// 	output_code.start << "	msg_" << label_cnt << " db \"" << value << "\", 0\n";
	// }
}

void CodeGenerator::collect_section_bss(const NodeStmtList &stmt_list) {
	output_code.bss << "section .bss\n";
	output_code.bss << "	print_str" << label_cnt << " resb 20\n";

	// label_cnt = 0;
	
	// for (auto stmt : stmt_list.stmts) {
	// 	++label_cnt;
	// }
}


void CodeGenerator::handle_stmt_list(const NodeStmtList &stmt_list) {
	for (const auto &stmt : stmt_list.stmts) {
		handle_stmt(stmt);
	}
} 


void CodeGenerator::handle_stmt(const NodeStmt &stmt) {
	handle_print(stmt.print);
	++label_cnt;
}


void CodeGenerator::handle_print(const NodePrint& print_node) {

	handle_expr(print_node.expr);

	output_code.start << "	mov rdi, [print_str + 19]\n";
	output_code.start << "	r8, rdi\n";
	output_code.start << "	call itoa\n";

	output_code.start << "   mov rdx, r8\n";
	output_code.start << "   sub rdx, rdi\n";
	output_code.start << "	 inc rdx\n";

	output_code.start << "   mov rax, 1\n";
	output_code.start << "   mov rsi, rdi\n";
	output_code.start << "	 mov rdi, 1\n";
	output_code.start << "   syscall\n";

	if (label_cnt == 1) {
		output_code.procs << "itoa:\n";
		output_code.procs << "   xor rbx, rbx\n";
		output_code.procs << " 	 mov rcx, 10\n";
		output_code.procs << "itoa_loop:\n";
		output_code.procs << "   xor rdx, rdx\n";
		output_code.procs << "   div rcx\n";
		output_code.procs << "   add dl, '0'\n";
		output_code.procs << "   dec rdi\n";
		output_code.procs <<"    mov [rdi], dl\n";
		output_code.procs << "   test rax, rax\n";
		output_code.procs << "   jnz itoa_loop\n";
		output_code.procs << "   ret\n";
	}
}	


void CodeGenerator::handle_expr(const NodeExpr &expr) {
	output_code.start << "	mov rax, 0\n";

	for (const auto &term : expr.term) {
		output_code.start << "	add rax, " << term.u_int_lit.value.value_or(0) << "\n";		
	}
}

void CodeGenerator::add_exit_code() {
	// output_code.start << "exit:\n";
	output_code.start << "	mov rax, 60\n";
	output_code.start << "	xor rdi, rdi\n";
	output_code.start << "	syscall\n";
}