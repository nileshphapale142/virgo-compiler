#pragma once

#include <string>
#include "parser.h"	
#include <sstream>

class CodeGenerator {
	public:
		CodeGenerator(NodeProgram root);
		std::string generate();
	private:

		void generate_print_code(NodePrint node);
		void handle_stmt_list(NodeStmtList stmt_list);
		void handle_stmt(NodeStmt stmt);
		void collect_section_data(NodeStmtList stmt_list);
		void collect_section_bss(NodeStmtList stmt_list);
		
		NodeProgram root;
		std::stringstream output_code;
		int label_cnt; 
};