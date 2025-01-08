#include "token.h"

std::string Token::tokenTypeToString() const {
    switch (type) {
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::INTEGER:
        return "INTEGER";
    case TokenType::PRINT:
        return "PRINT";
    default:
        return "UNKNOWN";
    }
}