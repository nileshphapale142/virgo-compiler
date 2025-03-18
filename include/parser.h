#pragma once

#include "node.h"
#include "token.h"
#include "arenaAllocator.h"

class Parser {
public:
    explicit Parser(std::vector<Token> &tokens);
    NodeProgram* parse();

private:
    std::vector<Token> tokens;
    size_t curr_index;

    ArenaAllocator *allocator;

    NodeStmtList* parse_stmt_list();
    std::optional<NodeStmt*> parse_stmt();
    std::optional<NodePrint*> parse_print();
    std::optional<NodeEndline*> parse_endline();
    std::optional<NodeDeclaration*> parse_declaration();
    std::optional<NodeAssignment*> parse_assignment();
    std::optional<NodeIncrement*> parse_increment();
    std::optional<NodeDecrement*> parse_decrement();
    std::optional<NodeWhile*> parse_while();
    std::optional<NodeRepeatWhile*> parse_repeat_while();
    std::optional<NodeRepeatTimes*> parse_repeat_times();
    std::optional<NodeRepeatFor*> parse_repeat_for();
    std::optional<NodeScope*> parse_scope();
    std::optional<NodeCondition*> parse_condition();
    std::optional<NodeIf*> parse_if();
    std::optional<NodeElif*> parse_elif();
    std::optional<NodeElse*> parse_else();
    NodeBoolExpr* parse_bool_expr();
    NodeExpr* parse_expr();
    std::optional<NodeTerm*> parse_term();
    std::optional<NodeFactor*> parse_factor();
    std::optional<Token> peek(int offset = 0);
    std::optional<Token> consume();
    static void throw_error(const std::string& msg);
    void throw_error_if_not(TokenType expected_token_type);
    bool check_if(TokenType expected_token_type);

};