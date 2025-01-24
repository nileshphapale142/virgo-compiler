#pragma once

#include "node.h"
#include "token.h"


class Parser {
public:
    explicit Parser(std::vector<Token> &tokens);
    NodeProgram* parse();

private:
    std::vector<Token> tokens;
    size_t curr_index;

    NodeStmtList* parse_stmt_list();
    std::optional<NodeStmt*> parse_stmt();
    std::optional<NodePrint*> parse_print();
    std::optional<NodeDeclaration*> parse_declaration();
    std::optional<NodeScope*> parse_scope();
    std::optional<NodeCondition*> parse_condition();
    std::optional<NodeIf*> parse_if();
    std::optional<NodeElif*> parse_elif();
    std::optional<NodeElse*> parse_else();
    NodeExpr* parse_expr();
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeFactor*> parse_factor();
    std::optional<Token> peek();
    std::optional<Token> consume();
};