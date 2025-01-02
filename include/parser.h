#pragma once

#include <vector>
#include <optional>
#include <stack>
#include <variant>
#include "token.h"


struct NodeProgram;
struct NodeStmtList;
struct NodeStmt;
struct TerminalNode;
struct NonTerminalNode;
struct NodePrint;

struct TerminalNode {
    Token token;
};

// struct NonTerminalNode {
//     P_Name name;
//     std::vector<std::variant<TerminalNode, NonTerminalNode>> child;
// };

struct NodePrint {
    std::optional<TerminalNode> node;
};

struct NodeStmt {
    NodePrint print;
};

struct NodeStmtList {
    std::vector<NodeStmt> stmts;
};

struct NodeProgram {
    NodeStmtList stmt_list;
};

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
};