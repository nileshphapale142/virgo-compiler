 #pragma once

#include <string>
#include "parser.h"	
#include <sstream>

class CodeGenerator {
	public:
		explicit CodeGenerator(NodeProgram *root);
		std::string generate();
	private:
		struct buffer {
			std::stringstream bss;
			std::stringstream data;
			std::stringstream text;
			std::stringstream start;
			std::stringstream procs; // procedures	
		};

		
		void handle_stmt_list(const NodeStmtList *stmt_list);
		void handle_stmt(const NodeStmt *stmt);
		void handle_print(const NodePrint *node);
		void handle_declaration(const NodeDeclaration *decl);
		void handle_assignment(const NodeAssignment *assign);
		void handle_scope(const NodeScope *scope);
		void handle_condition(const NodeCondition *condition);
		void handle_if(const NodeIf *if_node, int cond_cnt);
		void handle_elif(const NodeElif *elif_node, int cond_cnt, int elif_cnt);
		void handle_else(const NodeElse *else_node);
		void handle_expr(const NodeExpr *expr);
		void handle_bool_expr(const NodeBoolExpr *bool_expr, const std::string &jmp_to);
		void handle_factor(const NodeFactor *factor);

		void collect_section_data(const NodeStmtList *stmt_list);
		void collect_section_bss(const NodeStmtList *stmt_list);
		
		void add_exit_code();
		
		NodeProgram* root;
		buffer output_code;
		int label_cnt;
		int if_stmt_cnt = 0;

		std::vector<std::string> vars;
};