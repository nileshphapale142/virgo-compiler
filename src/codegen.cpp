#include "codegen.h"
#include <iostream>
#include <algorithm>
#include <cassert>

CodeGenerator::CodeGenerator(NodeProgram* root)
: root(root), label_cnt(0) {}

std::string CodeGenerator::generate() {

	// collect_section_data(root.stmt_list);
	
	collect_section_bss(root->stmt_list);

	output_code.text << "section .text\n";
	output_code.text << "	global _start\n";

	output_code.start << "_start:\n";

	label_cnt = 0;

	handle_stmt_list(root->stmt_list);

	add_exit_code();

	return output_code.bss.str() + 
	output_code.data.str() + 
	output_code.text.str() + 
	output_code.start.str() + output_code.procs.str();
}



void CodeGenerator::collect_section_data(const NodeStmtList *stmt_list) {
	label_cnt = 0;
}

void CodeGenerator::collect_section_bss(const NodeStmtList *stmt_list) {
	output_code.bss << "section .bss\n";
	output_code.bss << "	print_str resb 20\n";
}


void CodeGenerator::handle_stmt_list(const NodeStmtList *stmt_list) {
	for (const auto &stmt : stmt_list->stmts) {
		handle_stmt(stmt);
	}
} 


void CodeGenerator::handle_stmt(const NodeStmt *stmt) {
	if (std::holds_alternative<NodeDeclaration*>(stmt->stmt)) {
		handle_declaration(std::get<NodeDeclaration*>(stmt->stmt));
	} else if (std::holds_alternative<NodeAssignment*>(stmt->stmt)) {
		handle_assignment(std::get<NodeAssignment*>(stmt->stmt));
	} else if(std::holds_alternative<NodePrint*>(stmt->stmt)) {
		handle_print(std::get<NodePrint*>(stmt->stmt));
	} else if (std::holds_alternative<NodeScope*>(stmt->stmt)) {
		handle_scope(std::get<NodeScope*>(stmt->stmt));
	} else if (std::holds_alternative<NodeCondition*>(stmt->stmt)) {
		handle_condition(std::get<NodeCondition*>(stmt->stmt));
	} else if (std::holds_alternative<NodeIncrement*>(stmt->stmt)) {
		handle_increment(std::get<NodeIncrement*>(stmt->stmt));
	} else if (std::holds_alternative<NodeDecrement*>(stmt->stmt)) {
		handle_decrement(std::get<NodeDecrement*>(stmt->stmt));
	} else if (std::holds_alternative<NodeWhile*>(stmt->stmt)) {
		handle_while(std::get<NodeWhile*>(stmt->stmt));
	} else {
		std::cerr << "Unknown statement type" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void CodeGenerator::handle_scope(const NodeScope *scope) {
	size_t scope_begin_var_size = -1;

	for (const auto stmt : scope->stmt_list->stmts) {
		if (std::holds_alternative<NodeDeclaration*>(stmt->stmt)) {
			scope_begin_var_size = (scope_begin_var_size == -1 ? vars.size() : scope_begin_var_size);
			handle_declaration(std::get<NodeDeclaration*>(stmt->stmt));
		} else if (std::holds_alternative<NodeAssignment*>(stmt->stmt)) {
			handle_assignment(std::get<NodeAssignment*>(stmt->stmt));
		} else if (std::holds_alternative<NodePrint*>(stmt->stmt)) {
			handle_print(std::get<NodePrint*>(stmt->stmt));
		} else if (std::holds_alternative<NodeScope*>(stmt->stmt)) {
			handle_scope(std::get<NodeScope*>(stmt->stmt));
		} else if (std::holds_alternative<NodeCondition*>(stmt->stmt)) {
			handle_condition(std::get<NodeCondition*>(stmt->stmt));
		} else if (std::holds_alternative<NodeIncrement*>(stmt->stmt)) {
			handle_increment(std::get<NodeIncrement*>(stmt->stmt));
		} else if (std::holds_alternative<NodeDecrement*>(stmt->stmt)) {
			handle_decrement(std::get<NodeDecrement*>(stmt->stmt));
		} else if (std::holds_alternative<NodeWhile*>(stmt->stmt)) {
			handle_while(std::get<NodeWhile*>(stmt->stmt));
		} else {
			std::cerr << "Unknown statement type" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if (scope_begin_var_size == -1) return;

	while (vars.size() != scope_begin_var_size) {
		output_code.start << "	pop rax\n";
		vars.pop_back();
	}
}


void CodeGenerator::handle_while(const NodeWhile *while_node) {
	while_cnt ++;

	const std::string while_end = "while_end_" + std::to_string(while_cnt);
	const std::string while_start = "while_start_" + std::to_string(while_cnt);

	output_code.start << while_start << ":\n";
	handle_bool_expr(while_node->bool_expr, while_end);

	handle_scope(while_node->scope);
	output_code.start << "	jmp " << while_start << "\n";
	output_code.start << while_end << ":\n";
}

void CodeGenerator::handle_condition(const NodeCondition* condition) {
	++if_stmt_cnt;

	const int cond_cnt = if_stmt_cnt;
	handle_if(condition->if_cond, cond_cnt);

	if (condition->elif_cond.has_value()) {
		for (int i = 0; i < condition->elif_cond.value().size(); i ++) {
			const auto elif_cond = condition->elif_cond.value().at(i);
			handle_elif(elif_cond, cond_cnt,  i + 1);
		}
	}

	if (condition->else_cond.has_value()) {
		handle_else(condition->else_cond.value());
	}

	output_code.start << "cond_end_" << cond_cnt << ":\n";
}

void CodeGenerator::handle_if(const NodeIf *if_node, const int cond_cnt) {
	const std::string if_end = "if_end_" + std::to_string(cond_cnt);
	handle_bool_expr(if_node->bool_expr, if_end);

	handle_scope(if_node->scope);

	output_code.start << "	jmp cond_end_" << cond_cnt<< "\n";
	output_code.start << if_end << ":\n";
}

void CodeGenerator::handle_elif(const NodeElif *elif_node,  const int cond_cnt, const int elif_cnt) {
	const std::string elif_end = "elif_end_" + std::to_string(cond_cnt) + "_" + std::to_string(elif_cnt);
	handle_bool_expr(elif_node->bool_expr, elif_end);

	handle_scope(elif_node->scope);

	output_code.start << "	jmp cond_end_" << cond_cnt<< "\n";
	output_code.start << elif_end << ":\n";
}

void CodeGenerator::handle_else(const NodeElse *else_node) {
	handle_scope(else_node->scope);
}


void CodeGenerator::handle_bool_expr(const NodeBoolExpr *bool_expr, const std::string &jmp_to) {
	handle_expr(bool_expr->expr1);

	output_code.start << "	push rax\n";

	std::string jmp_if = "je";

	if (bool_expr->expr2.has_value()) {
		handle_expr(bool_expr->expr2.value());

		switch (bool_expr->bool_operator.value().type) {
			case TokenType::DOUBLE_EQUAL:
				jmp_if = "jne";
			break;
			case TokenType::NOT_EQUAL:
				jmp_if = "je";
			break;
			case TokenType::GREATER_THAN:
				jmp_if = "jle";
			break;
			case TokenType::LESS_THAN:
				jmp_if = "jge";
			break;
			case TokenType::GREATER_EQUAL:
				jmp_if = "jl";
			break;
			case TokenType::LESS_EQUAL:
				jmp_if = "jg";
			break;
			default:
				std::cerr << "Unknown operation type" << std::endl;
				exit(EXIT_FAILURE);
		}

		output_code.start << "	mov rbx, rax\n";
	} else {
		output_code.start << "	mov rbx, 0\n";
	}

	output_code.start << "	pop rax\n";

	output_code.start << "	cmp rax, rbx\n";
	output_code.start << "\t" << jmp_if << " " <<  jmp_to<<"\n";
}


void CodeGenerator::handle_print(const NodePrint *node) {
	++label_cnt;

	handle_expr(node->expr);

	output_code.start << "	lea rdi, [print_str + 19]\n";
	output_code.start << "	mov r8, rdi\n";
	output_code.start << "	call itoa\n";
	output_code.start << "	mov rdx, r8\n";
	output_code.start << "	sub rdx, rdi\n";
	output_code.start << "	inc rdx\n";

	if (node->is_println) {
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
		output_code.procs << "	cmp rax, 0\n";
		output_code.procs << "	jge handle_negative_end\n";
		output_code.procs << "	mov rbx,1\n";
		output_code.procs << "	neg rax\n";
		output_code.procs << "handle_negative_end:\n";
		output_code.procs << "	mov rcx, 10\n";
		output_code.procs << "itoa_loop:\n";
		output_code.procs << "	xor rdx, rdx\n";
		output_code.procs << "	div rcx\n";
		output_code.procs << "	add dl, '0'\n";
		output_code.procs << "	dec rdi\n";
		output_code.procs <<" 	mov [rdi], dl\n";
		output_code.procs << "	test rax, rax\n";
		output_code.procs << "	jnz itoa_loop\n";
		output_code.procs << "	cmp rbx, 0\n";
		output_code.procs << "	je itoa_end\n";
		output_code.procs << "	dec rdi\n";
		output_code.procs << "	mov byte [rdi], 45\n";
		output_code.procs << "itoa_end:\n";
		output_code.procs << "	ret\n";
	}
}

void CodeGenerator::handle_declaration(const NodeDeclaration *decl) {
	const auto itr = std::ranges::find_if(vars.begin(), vars.end(), [&](const std::string& var) {
		return var == decl->ident->name.value.value();
	});

	if (itr != vars.end()) {
		std::cerr << "Identifier " << decl->ident->name.value.value() << " already declared" << std::endl;
		exit(EXIT_FAILURE);
	}

	handle_expr(decl->expr);

	output_code.start << "	push rax\n";

	vars.push_back(decl->ident->name.value.value());
}


void CodeGenerator::handle_assignment(const NodeAssignment *assign) {
	const auto itr = std::ranges::find_if(vars.begin(), vars.end(), [&](const std::string& var) {
		return var == assign->ident->name.value.value();
	});

	if (itr == vars.end()) {
		std::cerr << "Identifier " << assign->ident->name.value.value() << " does not exist" << std::endl;
		exit(EXIT_FAILURE);
	}

	handle_expr(assign->expr);

	const size_t index = vars.size() - 1 - std::distance(vars.begin(), itr);

	output_code.start << "	mov [rsp + " << index * 8 << "], rax\n";
}


void CodeGenerator::handle_increment(const NodeIncrement *increment) {
	const auto itr = std::ranges::find_if(vars.begin(), vars.end(), [&](const std::string& var) {
		return var == increment->ident->name.value.value();
	});

	if (itr == vars.end()) {
		std::cerr << "Identifier " << increment->ident->name.value.value() << " does not exist" << std::endl;
		exit(EXIT_FAILURE);
	}

	const size_t index = vars.size() - 1 - std::distance(vars.begin(), itr);
	output_code.start << "	inc qword [rsp + " << index * 8 << "]\n";
}

void CodeGenerator::handle_decrement(const NodeDecrement *decrement) {
	const auto itr = std::ranges::find_if(vars.begin(), vars.end(), [&](const std::string& var) {
		return var == decrement->ident->name.value.value();
	});

	if (itr == vars.end()) {
		std::cerr << "Identifier " << decrement->ident->name.value.value() << " does not exist" << std::endl;
		exit(EXIT_FAILURE);
	}

	const size_t index = vars.size() - 1 - std::distance(vars.begin(), itr);
	output_code.start << "	dec qword [rsp + " << index * 8 << "]\n";
}



void CodeGenerator::handle_expr(const NodeExpr *expr) {
	output_code.start << "	mov rax, 0\n";
	std::string operation = "add";

	for (const auto &val : expr->val_list) {

		if (std::holds_alternative<NodeFactor*>(val)) {

			const NodeFactor* factor = std::get<NodeFactor*>(val);
			
			handle_factor(factor);

			output_code.start << "\t" <<  operation << " rax, rbx\n";

		} else if (std::holds_alternative<Token>(val)) {
			auto token = std::get<Token>(val);
			
			if (token.type != TokenType::PLUS && token.type != TokenType::MINUS) {
				std::cerr << "Unexpected token" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (token.type == TokenType::PLUS) operation = "add";
			else  operation = "sub";
		}
	}
}


void CodeGenerator::handle_factor(const NodeFactor *factor) {
	output_code.start << "	mov rbx, 1\n";
	bool is_mul = true;

	for (const auto& val : factor->val_list) {

		if (std::holds_alternative<NodeTerm*>(val)) {
			const NodeTerm* term = std::get<NodeTerm*>(val);
			if (std::holds_alternative<Token>(term->value)) {
				const Token u_int = std::get<Token>(term->value);

				if (is_mul) {
					output_code.start << "	mov rcx, " << u_int.value.value() << "\n";  
					output_code.start << "	imul rbx, rcx\n";
				} else {
					output_code.start << "	push rax\n";
					output_code.start << "	mov rax, rbx\n";
					output_code.start << "	mov rcx, " << u_int.value.value()  << "\n";
					output_code.start << "	xor rdx, rdx\n";
					output_code.start << "	div rcx\n";
					output_code.start << "	mov rbx, rax\n";
					output_code.start << "	pop rax\n";
				}
			} else if (std::holds_alternative<NodeIdentifier*>(term->value)) {

				const NodeIdentifier* ident = std::get<NodeIdentifier*>(term->value);

				auto itr = std::ranges::find_if(vars,  [&](const std::string& var_name) {
					if (ident->name.value.value() == var_name) return true;
					return false;
				});

				if (itr == vars.end()) {
					std::cerr << "Unknown identifier " << ident->name.value.value() << std::endl;
					exit(EXIT_FAILURE);
				}

				const size_t index = vars.size() - 1 - std::distance(vars.begin(), itr);

				if (is_mul) {
					output_code.start << "	mov rcx, [rsp + " << index * 8 << "]\n";
					output_code.start << "	imul rbx, rcx\n";
				} else {
					output_code.start << "	push rax\n";
					output_code.start << "	mov rax, rbx\n";
					output_code.start << "	mov rcx, [rsp + " << index * 8   << "]\n";
					output_code.start << "	xor rdx, rdx\n";
					output_code.start << "	div rcx\n";
					output_code.start << "	mov rbx, rax\n";
					output_code.start << "	pop rax\n";
				}
			}


		} else if (std::holds_alternative<Token>(val)) {
			const auto [type, value] = std::get<Token>(val); 

			if (type == TokenType::STAR) is_mul = true;
			else if (type == TokenType::BACKWARD_SLASH) is_mul = false;
		}
	}
}


void CodeGenerator::add_exit_code() {
	// output_code.start << "exit:\n";
	output_code.start << "	mov rax, 60\n";
	output_code.start << "	xor rdi, rdi\n";
	output_code.start << "	syscall\n";
}