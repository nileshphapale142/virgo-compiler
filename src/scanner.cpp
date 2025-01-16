#include "scanner.h"
#include <iostream>

Scanner::Scanner(std::string code)
: code(std::move(code)), curr_index(0) {}

std::vector<Token> Scanner::scan() {
    std::vector<Token> tokens;

    while (peek().has_value()) {

        switch(const char c = consume().value()) {
            case '(':
                tokens.push_back(Token{TokenType::LEFT_PAREN});
            break;

            case ')':
                tokens.push_back(Token({TokenType::RIGHT_PAREN}));
            break;
            
            case ';':
                tokens.push_back(Token({TokenType::SEMICOLON}));
            break;
            
            case '+':
                tokens.push_back(Token({TokenType::PLUS}));
            break;
            
            case '-':
                tokens.push_back(Token({TokenType::MINUS}));
            break;

            case '*':
                tokens.push_back(Token({TokenType::STAR}));
            break;

            case '/': {
                if (peek().has_value() && peek().value() == '/') {
                    consume();
                    while (peek().has_value() && peek().value() != '\n') {
                        consume();
                    }
                    consume();
                } else if (
                    peek().has_value() && 
                    peek().value() == '*' && 
                    peek(1).has_value() && 
                    peek(1).value() == '*') {
                        consume();
                        consume();

                        
                        bool is_closing_found = false;

                        while (peek().has_value()) {
                            if (
                                peek().value() == '*' && 
                                peek(1).has_value() && 
                                peek(1).value() == '*' && 
                                peek(2).has_value() && 
                                peek(2).value() == '/') {

                                    is_closing_found = true;
                                    
                                    consume();
                                    consume();
                                    consume();
                                    break;
                            } 

                            consume();
                        }

                        if (!is_closing_found) {
                            std::cerr << "Closing for multi line comment not found" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                } else {
                    tokens.push_back(Token({TokenType::BACKWARD_SLASH}));
                }
            }
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
                    
                    while (peek().has_value() && isdigit(peek().value())) {
                        integer.push_back(consume().value());
                    }

                    tokens.push_back(Token({TokenType::INTEGER, integer}));

                } else if (isalpha(c)) {
                    std::string token;

                    token.push_back(c);

                    while (peek().has_value() && isalpha(peek().value())) {
                        token.push_back(consume().value());
                    }

                    if (token == "print") {
                        tokens.push_back(Token({TokenType::PRINT}));
                    } else if (token == "println") {
                        tokens.push_back(Token({TokenType::PRINTLN}));
                    }
                    else {
                        std::cerr << "Unexpected token" << std::endl;
                        exit(EXIT_FAILURE);
                    }

                } else {
                    std::cerr << "Unexpected token" << std::endl;
                    exit(EXIT_FAILURE);
                }
            break;
        }
    }

    return tokens;
}


std::optional<char> Scanner::peek(int offset) {
    if (curr_index + offset < code.length()) return code.at(curr_index + offset);

    return std::nullopt;
}   

std::optional<char> Scanner::consume() {
    if (peek().has_value()) {
        return code.at(curr_index++);
    }

    return std::nullopt;
}