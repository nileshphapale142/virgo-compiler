#pragma once

#include <string>
#include "parser.h"	
#include <sstream>

class CodeGenerator {
	public:
		explicit CodeGenerator(NodeProgram root);
		std::string generate();
	private:

		void generate_print_code(const NodePrint& node);
		void handle_stmt_list(const NodeStmtList& stmt_list);
		void handle_stmt(const NodeStmt &stmt);
		void collect_section_data(const NodeStmtList &stmt_list);
		void collect_section_bss(const NodeStmtList &stmt_list);
		
		NodeProgram root;
		std::stringstream output_code;
		int label_cnt; 
};