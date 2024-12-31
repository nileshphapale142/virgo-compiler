#include "parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) , curr_index(0) {}


P_Rule Parser::parse() {

	while (curr_index < tokens.size()) {
		if (curr_index < tokens.size()) {
			parseStack.push(tokens.at(curr_index++));
		}

		for (const P_Rule& rule : grammer) {
			if (is_pr_matches(rule)) {
				P_Rule p_rule {.lhs = rule.lhs};

				for (size_t i = 0; i < rule.rhs.size(); i ++) {
					//TODO: add holds alternative
					const Token token = std::get<Token>(parseStack.top());
					p_rule.rhs.push_back(token);
			
					parseStack.pop();
				}

				std::reverse(p_rule.rhs.begin(), p_rule.rhs.end());

				parseStack.push(p_rule);
			}
		}
	}


	if (parseStack.size() != 1) {
		std::cerr << "Parsing failed; Incorrect Syntax" << std::endl;
		exit(EXIT_FAILURE);
	}

	return std::get<P_Rule>(parseStack.top());
}


bool Parser::is_pr_matches(P_Rule rule) {
	if (parseStack.size() < rule.rhs.size()) return false;

	auto itr = rule.rhs.rbegin();

	std::stack<std::variant<Token, P_Rule>> tempStack = parseStack;

	while (itr != rule.rhs.rend()) {
		if (std::holds_alternative<Token>(tempStack.top()) &&
			std::get<Token>(tempStack.top()).type == itr->type) {

			tempStack.pop();
			++itr;
		}
		else {
			return false;
		}
	}

	return true;
}