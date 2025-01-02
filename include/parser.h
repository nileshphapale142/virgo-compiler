#pragma once

#include "node.h"
#include "token.h"


class Parser {
public:
    Parser(std::vector<Token> tokens);
    NodeProgram parse();

private:
    std::vector<Token> tokens;
    size_t curr_index;

    NodeStmtList parse_stmt_list(); 
    std::optional<NodeStmt> parse_stmt();
    std::optional<NodePrint> parse_print();
    std::optional<Token> peek();
    std::optional<Token> consume();
};