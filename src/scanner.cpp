#include "scanner.h"
#include <iostream>

Scanner::Scanner() {}

std::vector<Token> Scanner::scan(const std::string &code)
{
    std::vector<Token> tokens;

    for (int i = 0; i < code.length(); i ++) {
        char c = code[i];

        switch(c) {
            case '(':
                tokens.push_back(Token{TokenType::LEFT_PAREN});
            break;

            case ')':
                tokens.push_back(Token({TokenType::RIGHT_PAREN}));
            break;
            
            case ';':
                tokens.push_back(Token({TokenType::SEMICOLON}));
            break;
            case ' ':
            case '\r':
            case '\n':
            case '\t':
            break;

            default:
                if (isdigit(c)) {
                    std::string integer;
                    
                    integer.push_back(c);
                    i ++;
                    
                    while (i < code.length() && isdigit(code[i])) {
                        integer.push_back(code[i++]);
                    }

                    tokens.push_back(Token({TokenType::INTEGER, integer}));

                    i --;
                } else if (isalpha(c)) {
                    std::string token;

                    token.push_back(c);
                    i ++;

                    while (i < code.length() && isalpha(code[i])) {
                        token.push_back(code[i++]);
                    }

                    if (token == "print") {
                        tokens.push_back(Token({TokenType::PRINT}));
                    } else {
                        std::cerr << "Unexpected token" << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    i --;
                } else {
                    std::cerr << "Unexpected token" << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
        }
    }

    return tokens;
}