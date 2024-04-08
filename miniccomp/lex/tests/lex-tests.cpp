//
// Created by sr9000 on 02/03/24.
// https://github.com/ex9000/lexer_tests_cmake/

#include "lexer.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>


namespace lex_tests {

TEST(CorrectCode, BasicProgram) {
    std::string prog = R"(
int main() {
return 0;
}
)";
    const std::vector<Token> correct = {
        Token{keyword, "int"},
        Token{kid, "main"},
        Token{lpar},
        Token{rpar},
        Token{lbrace},
        Token{keyword, "return"},
        Token{knum, "0"},
        Token{semicolon},
        Token{rbrace},
        Token{END_OF_FILE}
    };

    std::stringstream stream{prog};
    Lexer lexer(stream);

    for (const auto& correct_token : correct) {
        Token test_token = lexer.getNextLexem();
        EXPECT_EQ(correct_token.type, test_token.type);
        if (!correct_token.value.empty()) {
            EXPECT_EQ(correct_token.value, test_token.value);
        }
    }
}

TEST(CorrectCode, ComplexProgram) {
    std::string prog = R"(
int fib(int n){
if (n < 2) return n;
return fib(n - 1) + fib(n-2);
}

int is_even(int x) {
while (x > 0) x = x - 2;
return x == 0;
}

int main() {
int n;
in n;
for (int i = 0; i < n; i++) {
    if (is_even(i)) {
        out fib(i);
    }
}
return 0;
})";

    const std::vector<Token> correct = {
        // int fib(int n) {
        Token{keyword, "int"},
        Token{kid, "fib"},
        Token{lpar},
        Token{keyword, "int"},
        Token{kid, "n"},
        Token{rpar},
        Token{lbrace},

        // if (n < 2) return n;
        Token{keyword, "if"},
        Token{lpar},
        Token{kid, "n"},
        Token{oplt},
        Token{knum, "2"},
        Token{rpar},
        Token{keyword, "return"},
        Token{kid, "n"},
        Token{semicolon},

        // return fib(n - 1)
        Token{keyword, "return"},
        Token{kid, "fib"},
        Token{lpar},
        Token{kid, "n"},
        Token{opminus},
        Token{knum, "1"},
        Token{rpar},

        //  + fib(n-2);
        Token{opplus},
        Token{kid, "fib"},
        Token{lpar},
        Token{kid, "n"},
        Token{knum, "-2"},
        Token{rpar},
        Token{semicolon},

        // }
        Token{rbrace},

        // int is_even(int x) {
        Token{keyword, "int"},
        Token{kid, "is_even"},
        Token{lpar},
        Token{keyword, "int"},
        Token{kid, "x"},
        Token{rpar},
        Token{lbrace},

        // while (x > 0)
        Token{keyword, "while"},
        Token{lpar},
        Token{kid, "x"},
        Token{opgt},
        Token{knum, "0"},
        Token{rpar},

        // x = x - 2;
        Token{kid, "x"},
        Token{opassign},
        Token{kid, "x"},
        Token{opminus},
        Token{knum, "2"},
        Token{semicolon},

        //    return x == 0;
        Token{keyword, "return"},
        Token{kid, "x"},
        Token{opeq},
        Token{knum, "0"},
        Token{semicolon},

        //}
        Token{rbrace},
        // int main() {
        Token{keyword, "int"},
        Token{kid, "main"},
        Token{lpar},
        Token{rpar},
        Token{lbrace},

        //    int n;
        Token{keyword, "int"},
        Token{kid, "n"},
        Token{semicolon},

        //    in n;
        Token{keyword, "in"},
        Token{kid, "n"},
        Token{semicolon},

        //    for (int i = 0;
        Token{keyword, "for"},
        Token{lpar},
        Token{keyword, "int"},
        Token{kid, "i"},
        Token{opassign},
        Token{knum, "0"},
        Token{semicolon},

        //  i < n; i++) {
        Token{kid, "i"},
        Token{oplt},
        Token{kid, "n"},
        Token{semicolon},

        Token{kid, "i"},
        Token{opinc},
        Token{rpar},
        Token{lbrace},

        //        if (is_even(i)) {
        Token{keyword, "if"},
        Token{lpar},
        Token{kid, "is_even"},
        Token{lpar},
        Token{kid, "i"},
        Token{rpar},
        Token{rpar},
        Token{lbrace},

        //            out fib(i);
        Token{keyword, "out"},
        Token{kid, "fib"},
        Token{lpar},
        Token{kid, "i"},
        Token{rpar},
        Token{semicolon},

        //        }
        Token{rbrace},
        //    }
        Token{rbrace},

        //    return 0;
        Token{keyword, "return"},
        Token{knum, "0"},
        Token{semicolon},

        //}
        Token{rbrace},
        Token{END_OF_FILE}
    };

    std::stringstream stream(prog);
    Lexer lexer(stream);

    for (auto&& correct_token : correct) {
        Token test_token = lexer.getNextLexem();
        std::cout << TokenTypeToString.at(correct_token.type) << " " <<
            TokenTypeToString.at(test_token.type) << " " << correct_token.value << " " << test_token.value;
        EXPECT_EQ(correct_token.type, test_token.type) << TokenTypeToString.at(correct_token.type) << " " <<
            TokenTypeToString.at(test_token.type) << " " << correct_token.value << " " << test_token.value;
        if (!correct_token.value.empty()) {
            EXPECT_EQ(correct_token.value, test_token.value);
        }
    }
}

TEST(TrickyCode, BasicProgram) {
    using namespace std;
    string prog = "int main(){return 0;}";
    vector<pair<string, string>> correct = {
        {"keyword", "int"},
        {"id", "main"},
        {"(", ""},
        {")", ""},
        {"{", ""},
        {"keyword", "return"},
        {"num", "0"},
        {";", ""},
        {"}", ""},
        {"eof", ""}
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "eof" and next_lex.type == TokenType::END_OF_FILE) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(TrickyCode, MultilineString) {
    using namespace std;
    string prog = R"(out "Hello
world
'
for everyone!!!
")";
    string content = "Hello\n"
            "world\n"
            "'\n"
            "for everyone!!!\n";
    vector<pair<string, string>> correct = {
        {"kwout", ""},
        {"str", content},
        {"eof", ""}
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "eof" and next_lex.type == TokenType::END_OF_FILE) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(TrickyCode, Expressions) {
    using namespace std;
    string prog = "int a=0;int aa=182;out aa+a==-123;";
    vector<pair<string, string>> correct = {
        // int a=0;
        {"kwint", ""},
        {"id", "a"},
        {"opassign", ""},
        {"num", "0"},
        {"semicolon", ""},

        // int aa=182;
        {"kwint", ""},
        {"id", "aa"},
        {"opassign", ""},
        {"num", "182"},
        {"semicolon", ""},

        // out aa+a==-123;
        {"kwout", ""},
        {"id", "aa"},
        {"opplus", ""},
        {"id", "a"},
        {"opeq", ""},
        {"num", "-123"},
        {"semicolon", ""},
        {"", ""}
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "eof" and next_lex.type == TokenType::END_OF_FILE) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(ErrorCode, BadString) {
    using namespace std;
    string prog = R"(out "incomplete string;)";
    vector<pair<string, string>> correct = {
        {"keyword", "out"},
        {"", ""},
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "eof" and next_lex.type == TokenType::END_OF_FILE) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(ErrorCode, EmptyChar) {
    using namespace std;
    string prog = R"(out '')";
    vector<pair<string, string>> correct = {
        {"keyword", "out"},
        {"invalid", ""},
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "invalid" and next_lex.type == TokenType::INVALID) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(ErrorCode, DoubleChar) {
    using namespace std;
    string prog = R"(out 'xy')";
    vector<pair<string, string>> correct = {
        {"keyword", "out"},
        {"invalid", ""},
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "invalid" and next_lex.type == TokenType::INVALID) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

TEST(ErrorCode, SingleOperator) {
    using namespace std;
    string prog = "a | b";
    vector<pair<string, string>> correct = {
        {"id", "a"},
        {"invalid", ""},
    };

    stringstream stream{prog};
    Lexer lexer(stream);

    for (auto &&lex: correct) {
        auto next_lex = lexer.getNextLexem();
        if (lex.first == "invalid" and next_lex.type == TokenType::INVALID) {
            break;
        }
        EXPECT_EQ(lex.first, TokenTypeToString.at(next_lex.type));
        EXPECT_EQ(lex.second, next_lex.value);
    }
}

}
