#pragma once

#include <vector>
#include <variant>
#include "token.h"

struct NodeTerm {
    Token u_int_lit;
};

struct NodeFactor {
    std::vector<std::variant<NodeTerm, Token>> val_list;
};

struct NodeExpr {
    std::vector<std::variant<NodeFactor, Token>> val_list;
};


struct NodePrint {
    NodeExpr expr;
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