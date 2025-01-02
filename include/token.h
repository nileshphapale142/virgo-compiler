#pragma once

#include <optional>
#include <string>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON,
    // Literals
    INTEGER,
    // Keywords
    PRINT
};

struct Token {
    TokenType type;
    std::optional<std::string> value;

    std::string tokenTypeToString();
};