#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <expected>
#include <fstream>
#include <sstream>

#include "lexer.h"


TEST(LexerTest, TestSingleToken) {
    std::stringstream input("int");
    Lexer lexer(input);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::keyword);
    EXPECT_EQ(token.value, "int");
}

TEST(LexerTest, TestMultipleTokens) {
    std::stringstream input("int x = 10;");
    Lexer lexer(input);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::keyword);
    EXPECT_EQ(token.value, "int");

    token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::kid);
    EXPECT_EQ(token.value, "x");

    token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::opassign);

    token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::knum);
    EXPECT_EQ(token.value, "10");

    token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::semicolon);
}

 TEST(LexerTest, TestInvalidToken) {
    std::stringstream input(R"(\)");
    Lexer lexer(input);
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::INVALID);
}

