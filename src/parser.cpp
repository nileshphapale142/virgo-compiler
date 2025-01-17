#include "parser.h"
#include "../include/parser.h"
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
	NodeStmt stmt;

	if (auto print_stmt = parse_print()) {
		stmt.stmt = print_stmt.value();
		return stmt;
	} else if (auto decl_stmt = parse_declaration()) {
		stmt.stmt = decl_stmt.value();
		return stmt;
	}

	return std::nullopt;
};


std::optional<NodePrint> Parser::parse_print() {
	NodePrint print_node;

	if (!peek().has_value() || (
	peek().value().type != TokenType::PRINT &&
	peek().value().type != TokenType::PRINTLN)) return std::nullopt;

	print_node.is_println = peek().value().type == TokenType::PRINTLN;

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

std::optional<NodeDeclaration> Parser::parse_declaration() {
	NodeDeclaration decl;

	if (!peek().has_value() || peek().value().type != TokenType::LET) return std::nullopt;

	consume();

	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER) {
		std::cerr << "Expected an identifier" << std::endl;
		exit(EXIT_FAILURE);
	}

	decl.ident = NodeIdentifier({consume().value()});;

	if (!peek().has_value() || peek().value().type != TokenType::EQUAL) {
		std::cerr << "Expected = sign" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	decl.expr = parse_expr();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ;" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return decl;
}


NodeExpr Parser::parse_expr() {
	NodeExpr expr;

	while (const auto &factor = parse_factor()) {
		expr.val_list.push_back(factor.value());

		if (!peek().has_value() || (
			peek().value().type != TokenType::PLUS &&
			peek().value().type != TokenType::MINUS)) break;
		
		expr.val_list.push_back(consume().value());

	}

	return expr;
}


std::optional<NodeFactor> Parser::parse_factor() {
	NodeFactor factor;

	while (const auto& term = parse_term()) {
		factor.val_list.push_back(term.value());

		if (!peek().has_value() || (
			peek().value().type != TokenType::STAR && 
			peek().value().type != TokenType::BACKWARD_SLASH)) break;

		factor.val_list.push_back(consume().value());
	}

	return factor;
}


std::optional<NodeTerm> Parser::parse_term() {

	if (peek().has_value() && 
	(peek().value().type == TokenType::INTEGER || peek().value().type == TokenType::IDENTIFIER)){
		NodeTerm term;
		term.value = consume().value();
		return term;
	} else {
		std::cerr << "Expected an unsigned integer or an identifier" << std::endl;
		exit(EXIT_FAILURE);
	}

	return std::nullopt;
}

std::optional<Token> Parser::peek() {
	if (curr_index < tokens.size()) return tokens.at(curr_index);
	return std::nullopt;
}

std::optional<Token> Parser::consume() {
	if (peek().has_value()) return tokens.at(curr_index++);
	return std::nullopt;
}