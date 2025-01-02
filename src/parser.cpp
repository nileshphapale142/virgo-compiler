#include "parser.h"
#include <iostream>
#include <algorithm>
#include "../include/parser.h"


Parser::Parser(std::vector<Token> tokens) : tokens(tokens) , curr_index(0) {}

NodeProgram Parser::parse() {

	NodeProgram program;

	program.stmt_list = parse_stmt_list();

	return program;
}


NodeStmtList Parser::parse_stmt_list() {
	NodeStmtList stmt_list;

	while (auto stmt = parse_stmt()) {
		stmt_list.stmts.push_back(stmt.value());
	}

	if (curr_index < tokens.size())  {
		std::cerr << "Unknown Syntax" << std::endl;
		exit(EXIT_FAILURE);
	}

	return stmt_list;
}

std::optional<NodeStmt> Parser::parse_stmt() {

	NodeStmt stmt;

	if (auto print_stmt = parse_print()) {
		stmt.print = print_stmt.value();
		return stmt;
	} else {
		return {};
	}
};


std::optional<NodePrint> Parser::parse_print() {
	NodePrint print_node;

	if (curr_index >= tokens.size() || tokens.at(curr_index++).type != TokenType::PRINT) return {};
	
	if (curr_index >= tokens.size() || tokens.at(curr_index++).type != TokenType::LEFT_PAREN) {
		std::cerr << "Missing '(' " << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (tokens.at(curr_index).type == TokenType::INTEGER) {
		print_node.node = TerminalNode {.token = tokens.at(curr_index++) };
	}
	
	
	if (curr_index >= tokens.size() || tokens.at(curr_index++).type != TokenType::RIGHT_PAREN) {
		std::cerr << "Missing ')'" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (curr_index >= tokens.size() || tokens.at(curr_index++).type != TokenType::SEMICOLON) {
		std::cerr << "Missing ';'" << std::endl;
		exit(EXIT_FAILURE);
	}

	return  print_node;
}