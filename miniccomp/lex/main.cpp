#include <iostream>
#include <fstream>

#include "lexer.h"

int main() {
    std::ifstream file("../../main.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    Lexer lex(file);
    Token token = lex.getNextToken();
    while (token.type != TokenType::END_OF_FILE && token.type != TokenType::INVALID) {
        std::cout << TokenTypeToString.at(token.type) << ' ' << token.value << std::endl;
        token = lex.getNextToken();
    }
    std::cout << TokenTypeToString.at(token.type);
}
