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
    std::variant<Token, NodeIdentifier*> value;
};

struct NodeFactor {
    std::vector<std::variant<NodeTerm*, Token>> val_list;
};

struct NodeExpr {
    std::vector<std::variant<NodeFactor*, Token>> val_list;
};

struct NodeBoolExpr {
    NodeExpr* expr1{};
    std::optional<Token> bool_operator;
    std::optional<NodeExpr*> expr2;
};

struct NodeWhile {
    NodeBoolExpr* bool_expr;
    NodeScope* scope;
};

struct NodeIf {
    NodeBoolExpr* bool_expr;
    NodeScope* scope;
};

struct NodeElif {
    NodeBoolExpr* bool_expr;
    NodeScope* scope;
};

struct NodeElse {
    NodeScope* scope;
};

struct NodeCondition {
    NodeIf* if_cond{};
    std::optional<std::vector<NodeElif*>> elif_cond;
    std::optional<NodeElse*> else_cond;
};

struct NodePrint {
    NodeExpr* expr;
    bool is_println;
};

struct NodeDeclaration {
    NodeIdentifier* ident;
    NodeExpr* expr;
};

struct NodeAssignment {
    NodeIdentifier* ident;
    NodeExpr* expr;
};

struct NodeIncrement {
    NodeIdentifier* ident;
};

struct NodeDecrement {
    NodeIdentifier* ident;
};
//TODO: think of merging node increment and node decrement

struct NodeStmt {
    std::variant<NodePrint*, NodeDeclaration*, NodeAssignment*,
    NodeScope*, NodeCondition*, NodeIncrement*, NodeDecrement*,
    NodeWhile*> stmt;
};

struct NodeStmtList {
    std::vector<NodeStmt*> stmts;
};


struct NodeScope {
    NodeStmtList* stmt_list;
};

struct NodeProgram {
    NodeStmtList* stmt_list;
};