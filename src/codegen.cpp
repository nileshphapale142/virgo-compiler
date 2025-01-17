#include "codegen.h"
#include <iostream>
#include <cassert>

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
	output_code.bss << "	print_str resb 20\n";

	// label_cnt = 0;
	
	// for (auto stmt : stmt_list.stmts) {
	// 	++label_cnt;
	// }hel


}


void CodeGenerator::handle_stmt_list(const NodeStmtList &stmt_list) {
	for (const auto &stmt : stmt_list.stmts) {
		handle_stmt(stmt);
	}
} 


void CodeGenerator::handle_stmt(const NodeStmt &stmt) {
	if (std::holds_alternative<NodeDeclaration>(stmt.stmt)) {
		++label_cnt;
		handle_declaration(std::get<NodeDeclaration>(stmt.stmt));
	} else if(std::holds_alternative<NodePrint>(stmt.stmt)) {
		handle_print(std::get<NodePrint>(stmt.stmt));
	} else {
		std::cerr << "Unknown statment type" << std::endl;
		exit(EXIT_FAILURE);
	}
}


void CodeGenerator::handle_print(const NodePrint& print_node) {

	handle_expr(print_node.expr);

	output_code.start << "	lea rdi, [print_str + 19]\n";
	output_code.start << "	mov r8, rdi\n";
	output_code.start << "	call itoa\n";
	output_code.start << "	mov rdx, r8\n";
	output_code.start << "	sub rdx, rdi\n";
	output_code.start << "	inc rdx\n";

	if (print_node.is_println) {
		output_code.start << "	mov byte [r8 + 1], 10\n";
		output_code.start << "	inc rdx\n";
	}

	output_code.start << "	mov rax, 1\n";
	output_code.start << "	mov rsi, rdi\n";
	output_code.start << "	mov rdi, 1\n";
	output_code.start << "	syscall\n";

	if (label_cnt == 1) {
		output_code.procs << "itoa:\n";
		output_code.procs << "	xor rbx, rbx\n";
		output_code.procs << "	mov rcx, 10\n";
		output_code.procs << "itoa_loop:\n";
		output_code.procs << "	xor rdx, rdx\n";
		output_code.procs << "	div rcx\n";
		output_code.procs << "	add dl, '0'\n";
		output_code.procs << "	dec rdi\n";
		output_code.procs <<" 	mov [rdi], dl\n";
		output_code.procs << "	test rax, rax\n";
		output_code.procs << "	jnz itoa_loop\n";
		output_code.procs << "	ret\n";
	}
}

void CodeGenerator::handle_declaration(const NodeDeclaration &decl) {
	handle_expr(decl.expr);

	output_code.start << "	push rax\n";
}

void CodeGenerator::handle_expr(const NodeExpr &expr) {
	output_code.start << "	mov rax, 0\n";
	std::string operation = "add";

	for (const auto &val : expr.val_list) {	

		if (std::holds_alternative<NodeFactor>(val)) {

			const NodeFactor factor = std::get<NodeFactor>(val);
			
			handle_factor(factor);

			output_code.start << "\t" <<  operation << " rax, rbx\n";

		} else if (std::holds_alternative<Token>(val)) {
			Token token = std::get<Token>(val);
			
			if (token.type != TokenType::PLUS && token.type != TokenType::MINUS) {
				std::cerr << "Unexpected token" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (token.type == TokenType::PLUS) operation = "add";
			else if (token.type == TokenType::MINUS) operation = "sub";
		}
		//todo: else error throw
	}
}


void CodeGenerator::handle_factor(const NodeFactor& factor) {
	output_code.start << "	mov rbx, 1\n";
	bool is_mul = true;

	for (const auto& val : factor.val_list) {

		if (std::holds_alternative<NodeTerm>(val)) {
			const NodeTerm term = std::get<NodeTerm>(val);
			//todo: in case term.u_int_lit.value does not cotain value error
			if (std::holds_alternative<Token>(term.value)) {
				const Token u_int = std::get<Token>(term.value);

				if (is_mul) {
					output_code.start << "	mov rcx, " << u_int.value.value() << "\n";  
					output_code.start << "	imul rbx, rcx\n";
				} else {
					output_code.start << "	push rax\n";
					output_code.start << "	mov rax, rbx\n";
					output_code.start << "	mov rcx, " << u_int.value.value() << "\n";
					output_code.start << "	xor rdx, rdx\n";
					output_code.start << "	div rcx\n";
					output_code.start << "	mov rbx, rax\n";
					output_code.start << "	pop rax\n";
				}
			} else if (std::holds_alternative<NodeIdentifier>(term.value)) {
				const NodeIdentifier ident = std::get<NodeIdentifier>(term.value);
				std::cout << "Hello reached here" << std::endl;
				assert(false);
			}


		} else if (std::holds_alternative<Token>(val)) {
			const Token token = std::get<Token>(val); 

			if (token.type == TokenType::STAR) is_mul = true;
			else if (token.type == TokenType::BACKWARD_SLASH) is_mul = false;
		}
	}
}


void CodeGenerator::add_exit_code() {
	// output_code.start << "exit:\n";
	output_code.start << "	mov rax, 60\n";
	output_code.start << "	xor rdi, rdi\n";
	output_code.start << "	syscall\n";
}