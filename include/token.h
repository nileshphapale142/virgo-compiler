#pragma once

#include <optional>
#include <string>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, 
    PLUS, MINUS, STAR, BACKWARD_SLASH, EQUAL,
    LEFT_CURLY, RIGHT_CURLY, LESS_THAN, GREATER_THAN,
    // Double-character tokens
    DOUBLE_EQUAL, LESS_EQUAL, GREATER_EQUAL, NOT_EQUAL,
    // Literals
    INTEGER,

    IDENTIFIER,
    // Keywords
    PRINT, PRINTLN, LET, IF, ELIF, ELSE
};

struct Token {
    TokenType type;
    std::optional<std::string> value;

    [[nodiscard]] std::string tokenTypeToString() const;
};