#pragma once

#include <vector>
#include <optional>
#include "token.h"

struct NodeTerm {
    Token u_int;
};

struct NodeExpr {
    NodeTerm term;
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