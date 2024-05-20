//
// Created by Матвей Щербаков on 08.04.2024.
//

#ifndef MINICCOMP_LANG_H
#define MINICCOMP_LANG_H

#include <string>
#include <unordered_set>

const std::unordered_set<std::string> keywords = {
        "int", "char", "if", "else", "switch", "case",
        "while", "for", "return", "in", "out", "default"
};

enum TokenType {
    opinc, // 0
    opplus, // 1
    opeq, // 2
    opassign, // 3
    opne, // 4
    opnot, // 5
    oplt, // 6
    ople, // 7
    opor, // 8
    opand, // 9
    lpar, // ( 10
    rpar, // 11
    lbrace, // { 12
    rbrace, // 13
    semicolon, // 14
    colon, // 15
    comma, // 16
    period, // 17
    opgt, // 18
    opmul, // 19
    kchar, // 20
    kstr, // 21
    kid, // 22
    keyword, // 22
    opminus, // 24
    knum, // 25
    INVALID, // 26
    END_OF_FILE // 27
};

struct Token {
    TokenType type;
    std::string value;

    explicit Token(TokenType type)
        : type(type)
    {
    }

    Token(TokenType type, std::string value)
        : type(type)
        , value(std::move(value))
    {
    }


    bool operator == (const Token& other){
        return std::tie(type, value) == std::tie(other.type, other.value);
    }

    bool operator != (const Token& other) {
        return type != other.type or value != other.value;
    }
};

#endif // MINICCOMP_LANG_H
