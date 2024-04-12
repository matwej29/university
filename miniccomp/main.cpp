//
// Created by Матвей Щербаков on 08.04.2024.
//
#include <iostream>
#include <fstream>

#include "lex/include/lexer.h"
#include "parser/include/parser.h"

int main() {
    std::ifstream file("../expr.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

//    Lexer lex(file);
//    Token token = lex.getNextToken();
//    while (token.type != TokenType::END_OF_FILE && token.type != TokenType::INVALID) {
//        std::cout << TokenTypeToString.at(token.type) << ' ' << token.value << std::endl;
//        token = lex.getNextToken();
//    }
//    std::cout << TokenTypeToString.at(token.type) << std::endl;

    Lexer lex(file);
    auto a = [&lex]() { return lex.getNextToken(); };
    Parser parser(a);
    std::cout << parser.validate();
}
