#pragma once

#include <vector>
#include <optional>
#include "token.h"


struct TerminalNode {
    Token token;
};

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