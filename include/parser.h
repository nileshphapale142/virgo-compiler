#pragma once

#include <vector>
#include <optional>
#include <stack>
#include <variant>
#include "token.h"

enum class P_Name {
    PRINT
};

struct P_Rule {
    P_Name lhs;
    std::vector<Token> rhs; //can have 
};

class Parser {
    public:
        Parser(std::vector<Token> tokens);
        P_Rule parse(); 
    private:
        std::vector<Token> tokens;
        size_t curr_index;
        std::stack<std::variant<Token, P_Rule>> parseStack;

        inline static std::vector<P_Rule> grammer = {
            P_Rule{
                .lhs = P_Name::PRINT, 
                .rhs = {
                    {.type = TokenType::PRINT}, 
                    {.type = TokenType::LEFT_PAREN},
                    {.type = TokenType::INTEGER},
                    {.type = TokenType::RIGHT_PAREN},
                    {.type = TokenType::SEMICOLON}
                }
            }
        };
        
        bool is_pr_matches(P_Rule rule);
};