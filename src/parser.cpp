#include "parser.h"
#include <iostream>


Parser::Parser(std::vector<Token> &tokens) : tokens(std::move(tokens)) , curr_index(0) {}

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

	if (peek().has_value()) {
		std::cerr << "Unknown Syntax" << std::endl;
		exit(EXIT_FAILURE);
	}

	return stmt_list;
}

std::optional<NodeStmt> Parser::parse_stmt() {
	if (auto print_stmt = parse_print()) {
		NodeStmt stmt;
		stmt.print = print_stmt.value();
		return stmt;
	}

	return std::nullopt;
};


std::optional<NodePrint> Parser::parse_print() {
	NodePrint print_node;

	if (!peek().has_value() || peek().value().type != TokenType::PRINT) return std::nullopt;

	consume();
	
	if (!peek().has_value() || peek().value().type != TokenType::LEFT_PAREN) {
		std::cerr << "Missing '(' " << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();
	
	print_node.expr = parse_expr();
	
	if (!peek().has_value() || peek().value().type != TokenType::RIGHT_PAREN) {
		std::cerr << "Missing ')'" << std::endl;
		exit(EXIT_FAILURE);
	}


	consume();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Missing ';'" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return  print_node;
}


NodeExpr Parser::parse_expr() {
	NodeExpr expr;

	expr.term = parse_term();

	return expr;
}


NodeTerm Parser::parse_term() {
	NodeTerm term;

	if (peek().has_value() && peek().value().type == TokenType::INTEGER) {
		term.u_int = consume().value();
	} else {
		std::cerr << "Expected an unsigned integer" << std::endl;
		exit(EXIT_FAILURE);
	}
	return term;
}

std::optional<Token> Parser::peek() {
	if (curr_index < tokens.size()) return tokens.at(curr_index);
	return std::nullopt;
}

std::optional<Token> Parser::consume() {
	if (peek().has_value()) return tokens.at(curr_index++);
	return std::nullopt;
}