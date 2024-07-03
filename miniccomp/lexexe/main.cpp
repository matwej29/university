#include <fstream>
#include <iostream>

#include "lexer.hpp"

int main() {
    std::ifstream file("example.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    Lexer lex(file);
    Token token = lex.getNextToken();
    while (token.type != TokenType::END_OF_FILE && token.type != TokenType::INVALID) {
        std::cout << TokenTypeToString.at(token.type);
        if (!token.value.empty()) {
            std::cout << ' ' << token.value;
        }
        std::cout << std::endl;
        token = lex.getNextToken();
    }
    std::cout << TokenTypeToString.at(token.type);
}
