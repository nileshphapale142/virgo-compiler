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
    NodeExpr* parse_expr();
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeFactor*> parse_factor();
    std::optional<Token> peek();
    std::optional<Token> consume();
};