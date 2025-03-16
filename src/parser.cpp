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
		throw_error("Unknown Syntax");
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
	peek().value().type != TokenType::SHOW &&
	peek().value().type != TokenType::SHOWLINE)) return std::nullopt;

	auto* print_node = allocator->allocate<NodePrint>();

	print_node->is_println = peek().value().type == TokenType::SHOWLINE;

	consume();
	
	print_node->expr = parse_expr();

	return  print_node;
}

std::optional<NodeDeclaration *> Parser::parse_declaration() {

	if (!peek().has_value() || peek().value().type != TokenType::CREATE) return std::nullopt;

	auto* decl = allocator->allocate<NodeDeclaration>();

	consume();

	throw_error_if_not(TokenType::IDENTIFIER);

	decl->ident = allocator->allocate<NodeIdentifier>();
	decl->ident->name = consume().value();

	throw_error_if_not(TokenType::AS);

	consume();

	decl->expr = parse_expr();

	return decl;
}


std::optional<NodeAssignment* > Parser::parse_assignment() {
	if (!peek().has_value() || peek().value().type != TokenType::SET) return std::nullopt;

	consume();

	throw_error_if_not(TokenType::IDENTIFIER);

	auto* assign = allocator->allocate<NodeAssignment>();
	assign->ident = allocator->allocate<NodeIdentifier>();
	assign->ident->name = consume().value();

	throw_error_if_not(TokenType::TO);

	consume();

	assign->expr = parse_expr();

	return assign;
}

std::optional<NodeIncrement*> Parser::parse_increment() {
	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER
		|| !peek(1).has_value() || peek(1).value().type != TokenType::PLUS
		|| !peek(2).has_value() || peek(2).value().type != TokenType::PLUS
		) return std::nullopt;

	auto increment = allocator->allocate<NodeIncrement>();
	increment->ident = allocator->allocate<NodeIdentifier>();
	increment->ident->name = consume().value();


	consume();
	consume();

	throw_error_if_not(TokenType::SEMICOLON);

	consume();


	return increment;
}


std::optional<NodeDecrement*> Parser::parse_decrement() {
	if (!peek().has_value() || peek().value().type != TokenType::IDENTIFIER
		|| !peek(1).has_value() || peek(1).value().type != TokenType::MINUS
		|| !peek(2).has_value() || peek(2).value().type != TokenType::MINUS
		) return std::nullopt;

	auto decrement = allocator->allocate<NodeDecrement>();
	decrement->ident = allocator->allocate<NodeIdentifier>();
	decrement->ident->name = consume().value();

	consume();
	consume();

	throw_error_if_not(TokenType::SEMICOLON);

	consume();

	return decrement;
}


std::optional<NodeWhile* > Parser::parse_while() {
	if (!peek().has_value() || peek().value().type != TokenType::REPEAT) return std::nullopt;

	consume();

	auto repeat_node = allocator->allocate<NodeWhile>();

	if (const auto while_node = parse_repeat_while()) {
		repeat_node->loop = while_node.value();
	} else if (const auto times_node = parse_repeat_times()) {
		repeat_node->loop = times_node.value();
	} else {
		throw_error("Unexpected repeat syntax");
	}

	throw_error_if_not(TokenType::END);
	consume();

	throw_error_if_not(TokenType::REPEAT);
	consume();

	return repeat_node;
}

std::optional<NodeRepeatWhile*> Parser::parse_repeat_while() {
	if (!check_if(TokenType::WHILE)) return std::nullopt;

	consume();

	auto while_node = allocator->allocate<NodeRepeatWhile>();
	while_node->bool_expr = parse_bool_expr();

	throw_error_if_not(TokenType::DO);
	consume();

	if (const auto scope = parse_scope()) {
		while_node->scope = scope.value();
	}

	return while_node;
}

std::optional<NodeRepeatTimes*> Parser::parse_repeat_times() {
	const auto expr = parse_expr();

	throw_error_if_not(TokenType::TIMES);
	consume();

	throw_error_if_not(TokenType::DO);
	consume();

	auto times_node = allocator->allocate<NodeRepeatTimes>();
	times_node->expr = expr;

	if (const auto scope = parse_scope()) {
		times_node->scope = scope.value();
	}

	return times_node;
}

std::optional<NodeScope*> Parser::parse_scope() {
	if (!peek().has_value()) return std::nullopt;

	auto scope = allocator->allocate<NodeScope>();

	NodeStmtList* stmt_list = parse_stmt_list();

	scope->stmt_list = stmt_list;

	return scope;
}

std::optional<NodeCondition*> Parser::parse_condition() {

	const auto if_cond = parse_if();

	if (!if_cond) return std::nullopt;

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

	throw_error_if_not(TokenType::END);
	consume();

	throw_error_if_not(TokenType::CHECK);
	consume();

	return condition;
}

std::optional<NodeIf*> Parser::parse_if() {
	if (!peek().has_value() || peek().value().type != TokenType::CHECK) return std::nullopt;
	consume();

	throw_error_if_not(TokenType::IF);
	consume();

	auto if_node = allocator->allocate<NodeIf>();

	if_node->bool_expr = parse_bool_expr();

	throw_error_if_not(TokenType::THEN);

	consume();

	if (const auto scope = parse_scope()) {
		if_node->scope = scope.value();
	}

	return if_node;
}


std::optional<NodeElif*> Parser::parse_elif() {
	if (!peek().has_value() || peek().value().type != TokenType::OTHERWISE) {
		return std::nullopt;
	}

	if (!peek(1).has_value() || (peek(1).value().type != TokenType::IF && peek(1).value().type != TokenType::THEN)) {
		std::cerr << "Expected \"if\" after otherwise" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (peek(1).value().type != TokenType::IF) return std::nullopt;

	consume();
	consume();

	auto elif_node = allocator->allocate<NodeElif>();
	elif_node->bool_expr = parse_bool_expr();

	throw_error_if_not(TokenType::THEN);

	consume();


	if (const auto scope = parse_scope()) {
		elif_node->scope = scope.value();
	}

	return elif_node;
}

std::optional<NodeElse*> Parser::parse_else() {
	if (!peek().has_value() || peek().value().type != TokenType::OTHERWISE) return std::nullopt;

	consume();

	throw_error_if_not(TokenType::THEN);

	consume();

	auto else_node = allocator->allocate<NodeElse>();

	if (const auto scope = parse_scope()) {
		else_node->scope = scope.value();
	}

	return else_node;
}


NodeBoolExpr* Parser::parse_bool_expr() {
	const auto bool_expr = allocator->allocate<NodeBoolExpr>();

	bool_expr->expr1 = parse_expr();

	if (peek().has_value()) {
		if (peek().value().type == TokenType::NOT) {
			if (!peek(1).has_value()
				|| peek(1).value().type != TokenType::EQUALS) {
				std::cerr << "Expected \"equals\" after not" << std::endl;
				exit(EXIT_FAILURE);
			}
			bool_expr->bool_operator = Token({.type = TokenType::NOT_EQUAL});
			consume();
			consume();

			goto parse_expr2;
		}

		if (peek().value().type == TokenType::LESS) {
			if (!peek(1).has_value() || peek(1).value().type != TokenType::THAN) {
				std::cerr << "Expected \"than\" after less" << std::endl;
				exit(EXIT_FAILURE);
			}

			consume();
			consume();

			if (peek().has_value() && peek().value().type == TokenType::OR) {
				if (!peek(1).has_value() || peek(1).value().type != TokenType::EQUALS) {
					throw_error("Expected \"equals\" after or");
				}

				consume();
				consume();

				bool_expr->bool_operator = Token({.type = TokenType::LESS_EQUAL});

				goto parse_expr2;
			}

			bool_expr->bool_operator = Token({.type = TokenType::LESS_THAN});


			goto parse_expr2;
		}

		if (peek().value().type == TokenType::GREATER) {
			if (!peek(1).has_value() || peek(1).value().type != TokenType::THAN) {
				std::cerr << "Expected \"than\" after greater" << std::endl;
				exit(EXIT_FAILURE);
			}

			consume();
			consume();

			if (peek().has_value() && peek().value().type == TokenType::OR) {
				if (!peek(1).has_value() || peek(1).value().type != TokenType::EQUALS) {
					throw_error("Expected \"equals\" after or");
				}

				consume();
				consume();

				bool_expr->bool_operator = Token({.type = TokenType::GREATER_EQUAL});

				goto parse_expr2;
			}

			bool_expr->bool_operator = Token({.type = TokenType::GREATER_THAN});


			goto parse_expr2;
		}
	}

	if (!peek().has_value() ||
		(
			peek().value().type != TokenType::DOUBLE_EQUAL &&
			peek().value().type != TokenType::EQUALS &&
			peek().value().type != TokenType::GREATER_THAN &&
			peek().value().type != TokenType::LESS_THAN &&
			peek().value().type != TokenType::NOT_EQUAL &&
			peek().value().type != TokenType::LESS_EQUAL &&
			peek().value().type != TokenType::GREATER_EQUAL)) return bool_expr;

	bool_expr->bool_operator = consume().value();

	parse_expr2:
	bool_expr->expr2 = parse_expr();

	return bool_expr;
}



NodeExpr* Parser::parse_expr() {
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
		auto term = allocator->allocate<NodeTerm>();

		if (peek().value().type == TokenType::INTEGER) {
			term->value = consume().value();
		} else if (peek().value().type == TokenType::IDENTIFIER) {
			term->value = new NodeIdentifier({consume().value()});
		} else {
			throw_error("Expected an unsigned integer or an identifier");
		}
		
		return term;
	}

	throw_error("Expected an unsigned integer or an identifier");

	return std::nullopt;
}

std::optional<Token> Parser::peek(const int offset) {
	if (curr_index + offset < tokens.size()) return tokens.at(curr_index + offset);
	return std::nullopt;
}

std::optional<Token> Parser::consume() {
	if (peek().has_value()) return tokens.at(curr_index++);
	return std::nullopt;
}

void Parser::throw_error(const std::string& msg) {
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);
}

void Parser::throw_error_if_not(const TokenType expected_token_type) {
	if (!peek().has_value() || peek().value().type != expected_token_type) {
		switch (expected_token_type) {
			case TokenType::IDENTIFIER:
				throw_error("Expected an identifier");
			break;
			case TokenType::AS:
				throw_error("Expected \"as\"");
			break;
			case TokenType::TO:
				throw_error("Expected \"to\"");
			break;
			case TokenType::WHILE:
				throw_error("Expected \"while\"");
			break;
			case TokenType::TIMES:
				throw_error("Expected \"times\"");
			break;
			case TokenType::DO:
				throw_error("Expected \"do\"");
			break;
			case TokenType::END:
				throw_error("Expected \"end\"");
			break;
			case TokenType::REPEAT:
				throw_error("Expected \"repeat\" after end");
			break;
			case TokenType::CHECK:
				throw_error("Expected \"check\" after end");
			break;
			case TokenType::IF:
				throw_error("Expected \"if\" after check");
			break;
			case TokenType::THEN:
				throw_error("Expected \"then\"");
			break;
			case TokenType::EQUALS:
				throw_error("Expected \"equals\" after not");
			break;
			case TokenType::THAN:
				throw_error("Expected \"than\" after less/greater");
			break;
			default:
				throw_error("Unexpected token");
		}
    }
}

bool Parser::check_if(const TokenType expected_token_type) {
	return peek().has_value() && peek().value().type == expected_token_type;
}
