#pragma once

#include <vector>
#include <variant>
#include "token.h"

struct NodeTerm {
    Token u_int_lit;
};

struct NodeIdentifier {
    Token name;
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
    std::variant<NodePrint, NodeDeclaration> stmt;
};

struct NodeStmtList {
    std::vector<NodeStmt> stmts;
};

struct NodeProgram {
    NodeStmtList stmt_list;
};