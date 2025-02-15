#include "parser.h"
#include <iostream>
#include <ranges>
#include <valarray>


Parser::Parser(std::vector<Token> &tokens) : tokens(std::move(tokens)) , curr_index(0) {
	allocator =  new ArenaAllocator(1024 * 1024);
}

NodeProgram* Parser::parse() {

	auto* program = allocator->allocate<NodeProgram>();

	program->stmt_list = parse_stmt_list();

	if (peek().has_value()) {
		std::cerr << "Unknown Syntax" << std::endl;
		exit(EXIT_FAILURE);
	}

	return program;
}


NodeStmtList *Parser::parse_stmt_list() {
	auto* stmt_list = allocator->allocate<NodeStmtList>();

	while (auto stmt = parse_stmt()) {
		stmt_list->stmts.push_back(stmt.value());
	}

	return stmt_list;
}

std::optional<NodeStmt*> Parser::parse_stmt() {
	auto* stmt = allocator->allocate<NodeStmt>();

	if (auto print_stmt = parse_print()) {
		stmt->stmt = print_stmt.value();
		return stmt;
	}
	if (auto decl_stmt = parse_declaration()) {
		stmt->stmt = decl_stmt.value();
		return stmt;
	}

	if (auto assign_stmt = parse_assignment()) {
		stmt->stmt = assign_stmt.value();
		return stmt;
	}

	if (auto inc_stmt = parse_increment()) {
		stmt->stmt = inc_stmt.value();
		return stmt;
	}

	if (auto dec_stmt = parse_decrement()) {
		stmt->stmt = dec_stmt.value();
		return stmt;
	}

	if (auto scope = parse_scope()) {
		stmt->stmt = scope.value();
		return stmt;
	}

	if (auto cond_stmt = parse_condition()) {
		stmt->stmt = cond_stmt.value();
		return stmt;
	}


	if (auto while_stmt = parse_while()) {
		stmt->stmt = while_stmt.value();
		return stmt;
	}

	return std::nullopt;
};


std::optional<NodePrint*> Parser::parse_print() {

	if (!peek().has_value() || (
	peek().value().type != TokenType::PRINT &&
	peek().value().type != TokenType::PRINTLN)) return std::nullopt;

	auto* print_node = allocator->allocate<NodePrint>();

	print_node->is_println = peek().value().type == TokenType::PRINTLN;

	consume();

	if (!peek().has_value() || peek().value().type != TokenType::LEFT_PAREN) {
		std::cerr << "Missing '(' " << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();
	
	print_node->expr = parse_expr();
	
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

std::optional<NodeDeclaration *> Parser::parse_declaration() {

	if (!peek().has_value() || peek().value().type != TokenType::LET) return std::nullopt;

	auto* decl = allocator->allocate<NodeDeclaration>();

	consume();

	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER) {
		std::cerr << "Expected an identifier" << std::endl;
		exit(EXIT_FAILURE);
	}

	decl->ident = allocator->allocate<NodeIdentifier>();
	decl->ident->name = consume().value();


	if (!peek().has_value() || peek().value().type != TokenType::EQUAL) {
		std::cerr << "Expected = sign" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	decl->expr = parse_expr();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ;" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return decl;
}


std::optional<NodeAssignment* > Parser::parse_assignment() {
	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER
		|| !peek(1).has_value() || peek(1).value().type != TokenType::EQUAL) return std::nullopt;


	//todo: try to make it shorter
	auto* assign = allocator->allocate<NodeAssignment>();
	assign->ident = allocator->allocate<NodeIdentifier>();
	assign->ident->name = consume().value();
	// auto* assign = new NodeAssignment({.ident = new NodeIdentifier({.name = consume().value()})});

	consume();

	assign->expr = parse_expr();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ;" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return assign;
}

std::optional<NodeIncrement*> Parser::parse_increment() {
	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER
		|| !peek(1).has_value() || peek(1).value().type != TokenType::PLUS
		|| !peek(2).has_value() || peek(2).value().type != TokenType::PLUS
		) return std::nullopt;

	// auto increment = new NodeIncrement({.ident = new NodeIdentifier({.name = consume().value()})});
	auto increment = allocator->allocate<NodeIncrement>();
	increment->ident = allocator->allocate<NodeIdentifier>();
	increment->ident->name = consume().value();


	consume();
	consume();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ;" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();


	return increment;
}


std::optional<NodeDecrement*> Parser::parse_decrement() {
	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER
		|| !peek(1).has_value() || peek(1).value().type != TokenType::MINUS
		|| !peek(2).has_value() || peek(2).value().type != TokenType::MINUS
		) return std::nullopt;

	// auto decrement = new NodeDecrement({.ident = new NodeIdentifier({.name = consume().value()})});
	auto decrement = allocator->allocate<NodeDecrement>();
	decrement->ident = allocator->allocate<NodeIdentifier>();
	decrement->ident->name = consume().value();

	consume();
	consume();

	if (!peek().has_value() || peek().value().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ;" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return decrement;
}


std::optional<NodeWhile* > Parser::parse_while() {
	if (!peek().has_value() || peek().value().type != TokenType::WHILE) return std::nullopt;

	consume();

	// auto while_node = new NodeWhile({.bool_expr = parse_bool_expr()});
	auto while_node = allocator->allocate<NodeWhile>();
	while_node->bool_expr = parse_bool_expr();

	if (const auto scope = parse_scope()) {
		while_node->scope = scope.value();
	} else {
		std::cerr << "Unexpected end of while statement" << std::endl;
		exit(EXIT_FAILURE);
	}

	return while_node;
}


std::optional<NodeScope*> Parser::parse_scope() {
	if (!peek().has_value() || peek().value().type != TokenType::LEFT_CURLY) return std::nullopt;

	consume();

	// auto* scope = new NodeScope();
	auto scope = allocator->allocate<NodeScope>();

	NodeStmtList* stmt_list = parse_stmt_list();

	scope->stmt_list = stmt_list;


	if (!peek().has_value() || peek().value().type != TokenType::RIGHT_CURLY) {
		std::cerr << "Expected  '}'" << std::endl;
		exit(EXIT_FAILURE);
	}

	consume();

	return scope;
}

std::optional<NodeCondition*> Parser::parse_condition() {

	const auto if_cond = parse_if();

	if (!if_cond) return std::nullopt;

	// auto condition = new NodeCondition();
	auto condition = allocator->allocate<NodeCondition>();

	condition->if_cond = if_cond.value();

	std::vector<NodeElif*> elif_nodes;

	while (const auto elif_cond = parse_elif()) {
		elif_nodes.push_back(elif_cond.value());
	}

	if (!elif_nodes.empty()) {
		condition->elif_cond = std::move(elif_nodes);
	}

	if (const auto else_cond = parse_else()) {
		condition->else_cond = else_cond.value();
	}

	return condition;
}

std::optional<NodeIf*> Parser::parse_if() {
	if (!peek().has_value() || peek().value().type != TokenType::IF) return std::nullopt;

	// auto* if_node = new NodeIf();
	auto if_node = allocator->allocate<NodeIf>();

	consume();

	if_node->bool_expr = parse_bool_expr();

	if (const auto scope = parse_scope()) {
		if_node->scope = scope.value();
	} else {
		std::cerr << "Missing '{'" << std::endl;
		exit(EXIT_FAILURE);
	}

	return if_node;
}


std::optional<NodeElif*> Parser::parse_elif() {
	if (!peek().has_value() || peek().value().type != TokenType::ELIF) return std::nullopt;

	// auto* elif_node = new NodeElif();
	auto elif_node = allocator->allocate<NodeElif>();

	consume();

	elif_node->bool_expr = parse_bool_expr();

	if (const auto scope = parse_scope()) {
		elif_node->scope = scope.value();
	} else {
		std::cerr << "Missing '{'" << std::endl;
		exit(EXIT_FAILURE);
	}

	return elif_node;
}

std::optional<NodeElse*> Parser::parse_else() {
	if (!peek().has_value() || peek().value().type != TokenType::ELSE) return std::nullopt;

	consume();

	// auto *else_node = new NodeElse();
	auto else_node = allocator->allocate<NodeElse>();

	if (const auto scope = parse_scope()) {
		else_node->scope = scope.value();
	} else {
		std::cerr << "Missing '{'" << std::endl;
		exit(EXIT_FAILURE);
	}

	return else_node;
}


NodeBoolExpr* Parser::parse_bool_expr() {
	// auto* bool_expr = new NodeBoolExpr();
	const auto bool_expr = allocator->allocate<NodeBoolExpr>();

	bool_expr->expr1 = parse_expr();

	if (!peek().has_value() ||
		(
			peek().value().type != TokenType::DOUBLE_EQUAL &&
			peek().value().type != TokenType::GREATER_THAN &&
			peek().value().type != TokenType::LESS_THAN &&
			peek().value().type != TokenType::NOT_EQUAL &&
			peek().value().type != TokenType::LESS_EQUAL &&
			peek().value().type != TokenType::GREATER_EQUAL)) return bool_expr;

	bool_expr->bool_operator = consume().value();

	bool_expr->expr2 = parse_expr();

	return bool_expr;
}



NodeExpr* Parser::parse_expr() {
	// auto* expr = new NodeExpr();
	const auto expr = allocator->allocate<NodeExpr>();

	if (peek().has_value() && (peek().value().type == TokenType::PLUS || peek().value().type == TokenType::MINUS)) {
		expr->val_list.emplace_back(consume().value());
	}

	while (const auto &factor = parse_factor()) {
		expr->val_list.emplace_back(factor.value());

		if (!peek().has_value() || (
			peek().value().type != TokenType::PLUS &&
			peek().value().type != TokenType::MINUS)) break;
		
		expr->val_list.emplace_back(consume().value());
	}

	return expr;
}


std::optional<NodeFactor*> Parser::parse_factor() {
	// auto* factor = new NodeFactor();
	auto factor = allocator->allocate<NodeFactor>();

	while (const auto& term = parse_term()) {
		factor->val_list.emplace_back(term.value());

		if (!peek().has_value() || (
			peek().value().type != TokenType::STAR && 
			peek().value().type != TokenType::BACKWARD_SLASH)) break;

		factor->val_list.emplace_back(consume().value());
	}

	return factor;
}


std::optional<NodeTerm*> Parser::parse_term() {

	if (peek().has_value()){
		// auto* term = new NodeTerm();
		auto term = allocator->allocate<NodeTerm>();

		if (peek().value().type == TokenType::INTEGER) {
			term->value = consume().value();
		} else if (peek().value().type == TokenType::IDENTIFIER) {
			term->value = new NodeIdentifier({consume().value()});
		} else {
			std::cout << "Expected an unsigned integer or an identifier" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		return term;
	}

	std::cerr << "Expected an unsigned integer or an identifier" << std::endl;
	exit(EXIT_FAILURE);
}

std::optional<Token> Parser::peek(const int offset) {
	if (curr_index + offset < tokens.size()) return tokens.at(curr_index + offset);
	return std::nullopt;
}

std::optional<Token> Parser::consume() {
	if (peek().has_value()) return tokens.at(curr_index++);
	return std::nullopt;
}