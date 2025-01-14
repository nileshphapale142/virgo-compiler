#pragma once

#include <string>
#include "parser.h"	
#include <sstream>

class CodeGenerator {
	public:
		explicit CodeGenerator(NodeProgram root);
		std::string generate();
	private:
		struct buffer {
			std::stringstream bss;
			std::stringstream data;
			std::stringstream text;
			std::stringstream start;
			std::stringstream procs; // procedures	
		};

		
		void handle_stmt_list(const NodeStmtList& stmt_list);
		void handle_stmt(const NodeStmt &stmt);
		void handle_print(const NodePrint& node);
		void handle_expr(const NodeExpr &expr);
		void handle_factor(const NodeFactor &factor);

		void collect_section_data(const NodeStmtList &stmt_list);
		void collect_section_bss(const NodeStmtList &stmt_list);
		
		void add_exit_code();
		
		
		NodeProgram root;
		buffer output_code;
		int label_cnt; 		
};