#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "lexer.h"

int main() {
    std::ifstream file("main.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    Lexer lex(file);
    Token token = lex.getNextLexem();
    while (token.type != TokenType::END_OF_FILE) {
        std::cout << TokenTypeToString.at(token.type) << ' ' << token.value << std::endl;
        token = lex.getNextLexem();
    }
    std::cout << TokenTypeToString.at(token.type);
}
