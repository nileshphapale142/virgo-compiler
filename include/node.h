#pragma once

#include <vector>
#include "token.h"

struct NodeTerm {
    Token u_int_lit;
};

struct NodeExpr {
    std::vector<NodeTerm> term;
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