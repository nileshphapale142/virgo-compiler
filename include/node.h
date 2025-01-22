#pragma once

#include <vector>
#include <variant>
#include "token.h"

struct NodePrint;
struct NodeDeclaration;
struct NodeScope;


struct NodeIdentifier {
    Token name;
};
struct NodeTerm {
    std::variant<Token, NodeIdentifier> value;
};

struct NodeFactor {
    std::vector<std::variant<NodeTerm, Token>> val_list;
};

struct NodeExpr {
    std::vector<std::variant<NodeFactor, Token>> val_list;
};

struct NodePrint {
    NodeExpr expr;
    bool is_println;
};

struct NodeDeclaration {
    NodeIdentifier ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodePrint, NodeDeclaration, NodeScope> stmt;
};

struct NodeStmtList {
    std::vector<NodeStmt> stmts;
};


struct NodeScope {
    NodeStmtList stmt_list;
};

struct NodeProgram {
    NodeStmtList stmt_list;
};


