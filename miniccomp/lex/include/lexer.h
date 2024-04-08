#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <expected>
#include "./lang.h"

#ifndef MINICCOMP_LEXER_H
#define MINICCOMP_LEXER_H


const std::unordered_set<std::string> keywords = {
        "int",
        "char",
        "if",
        "else",
        "switch",
        "case",
        "while",
        "for",
        "return",
        "in",
        "out"
};

const std::unordered_map<size_t, std::string> TokenTypeToString = {
        {0,  "++"},
        {1,  "+"},
        {2,  "=="},
        {3,  "="},
        {4,  "!="},
        {5,  "!"},
        {6,  "<"},
        {7,  "<="},
        {8,  "||"},
        {9,  "&&"},
        {10, "("},
        {11, ")"},
        {12, "{"},
        {13, "}"},
        {14, ";"},
        {15, ","},
        {16, "."},
        {17, ">"},
        {18, "*"},
        {19, "char"},
        {20, "string"},
        {21, "id"},
        {22, "keyword"},
        {23, "-"},
        {24, "num"},
        {25, "INVALID"},
        {26, "EOF"},
};

inline bool FILTER_ANY(char c) {
    return true;
}

template<char filter>
bool FILTER_CHAR(char c) {
    return c == filter;
}

inline bool ANY_LETTER(char c) {
    // magic numbers
    // tooked from ASCII table
    return (65 <= c && c <= 90) || (97 <= c && c <= 122);
}

inline bool ANY_DIGIT(char c) {
    return 48 <= c && c <= 57;
}

inline bool ANY_LETTER_DIGIT(char c) {
    return ANY_LETTER(c) || ANY_DIGIT(c);
}

inline bool ID_FILTER(char c) {
    return ANY_LETTER_DIGIT(c) || c == '_';
}

struct Token {
    TokenType type;
    std::string value;

    explicit Token(TokenType type) : type(type) {
    }

    Token(TokenType type, std::string value) : type(type), value(std::move(value)) {
    }
};

struct None {
};

using Lexem = std::expected<Token, None>;

const Lexem NONE = std::unexpected(None{});

struct Edge {
    const u_int next_state;
    const std::function<bool(char)> filter;
    const std::function<Lexem()> action;
};

struct Lexer {
    std::istream &stream;
    std::string buffer;
    char current_char;
    u_int current_state = 0;

    explicit Lexer(std::istream &stream) : stream(stream) {
        current_char = 0;
        next_char();
    }

    Lexem processState() {
        for (const Edge &edge: graph.at(current_state)) {
            if (!edge.filter(current_char)) continue;

            auto result = edge.action();
            current_state = edge.next_state;
            if (result.has_value()) {
                return result.value();
            } else {
                return NONE;
            }
        }
        return Token{INVALID};
    }

    Token getNextLexem() {
        Lexem result = NONE;
        do {
            result = processState();
        } while (!result.has_value());

        return result.value();
    }

    void next_char() {
        char c;
        if (stream.get(c)) {
            current_char = c;
        } else {
            current_char = EOF;
        }
    }

    const std::unordered_map<u_int, std::vector<Edge> > graph = {
            {
                    0,
                    {
                            {
                                    0,
                                    FILTER_CHAR<' '>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'\n'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },
                            {
                                    0, // 101
                                    FILTER_CHAR<'('>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::lpar};
                                    }
                            },
                            {
                                    0, // 102
                                    FILTER_CHAR<')'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::rpar};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'{'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::lbrace};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'}'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::rbrace};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<';'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::semicolon};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<','>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::comma};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'.'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::period};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'>'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opgt};
                                    }
                            },
                            {
                                    0,
                                    FILTER_CHAR<'*'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opmul};
                                    }
                            },

                            {
                                    2,
                                    FILTER_CHAR<'<'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    4,
                                    FILTER_CHAR<'!'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    6,
                                    FILTER_CHAR<'='>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    8,
                                    FILTER_CHAR<'+'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    10,
                                    FILTER_CHAR<'|'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    12,
                                    FILTER_CHAR<'&'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    14,
                                    FILTER_CHAR<'\''>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    18,
                                    FILTER_CHAR<'"'>,
                                    [this]() {
                                        this->next_char();
                                        this->buffer = "";
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    21,
                                    ANY_LETTER,
                                    [this]() {
                                        this->buffer.assign(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    22,
                                    FILTER_CHAR<'-'>,
                                    [this]() {
                                        this->buffer = "-";
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {
                                    23,
                                    ANY_DIGIT,
                                    [this]() {
                                        this->buffer.assign(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },

                            {

                                    0,
                                    FILTER_CHAR<EOF>,
                                    []() {
                                        return Token{END_OF_FILE};
                                    }

                            }
                    }
            },

            {
                    2,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'='>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::ople};
                                    },
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    []() {
                                        return Token{TokenType::oplt};
                                    },
                            }
                    }
            },

            {
                    4,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'='>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opne};
                                    },
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    []() {
                                        return Token{TokenType::opnot};
                                    }
                            }
                    }
            },

            {
                    6,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'='>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opeq};
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    []() {
                                        return Token{TokenType::opassign};
                                    }
                            }
                    }
            },

            {
                    8,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'+'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opinc};
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    []() {
                                        return Token{TokenType::opplus};
                                    }
                            }
                    }
            },

            {
                    10,
                    {
                            {
                                    11,
                                    FILTER_CHAR<'|'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    },
                            }
                    }

            },

            {
                    11,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'|'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opor};
                                    }
                            }
                    }
            },

            {
                    12,
                    {
                            {
                                    13,
                                    FILTER_CHAR<'&'>,
                                    [this]() {
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            }
                    }
            },

            {
                    13,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'&'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opand};
                                    }
                            }
                    }
            },

            {
                    14,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'\''>,
                                    [this]() {
                                        //                                        this->next_char();
                                        return Token{TokenType::INVALID};
                                    }
                            },
                            {
                                    16,
                                    FILTER_ANY,
                                    [this]() {
                                        this->buffer.assign(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            }
                    }
            },

            {
                    16,
                    {
                            {
                                    0,
                                    FILTER_CHAR<'\''>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::kchar, this->buffer};
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    [this]() {
                                        //                                    this->next_char();
                                        return Token{TokenType::INVALID};
                                    }
                            }
                    }
            },

            {
                    18,
                    {
                            {
                                    0,
                                    FILTER_CHAR<EOF>,
                                    []() {
                                        return Token{TokenType::INVALID, "EOF"};
                                    },
                            },
                            {
                                    0,
                                    FILTER_CHAR<'"'>,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::kstr, this->buffer};
                                    }
                            },
                            {
                                    18,
                                    FILTER_ANY,
                                    [this]() {
                                        this->buffer.append(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            }
                    }
            },

            {
                    21,
                    {
                            {
                                    21,
                                    ID_FILTER,
                                    [this]() {
                                        this->buffer.append(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    [this]() {
                                        std::string identificator(this->buffer);
                                        if ('0' <= identificator[0] and identificator[0] <= '9') {
                                            return Token{TokenType::INVALID};
                                        }

                                        if (keywords.contains(identificator)) {
                                            return Token{TokenType::keyword, identificator};
                                        } else {
                                            return Token{TokenType::kid, identificator};
                                        }
                                    }
                            }
                    }
            },

            {
                    22,
                    {
                            {
                                    23,
                                    ANY_DIGIT,
                                    [this]() {
                                        this->buffer.append(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    [this]() {
                                        this->next_char();
                                        return Token{TokenType::opminus};
                                    }
                            }
                    }
            },

            {
                    23,
                    {
                            {
                                    23,
                                    ANY_DIGIT,
                                    [this]() {
                                        this->buffer.append(&this->current_char);
                                        this->next_char();
                                        return std::unexpected(None{});
                                    }
                            },
                            {
                                    0,
                                    FILTER_ANY,
                                    [this]() {
                                        // if (keywords.contains(this->buffer)) {
                                        //     return Token{TokenType::kid};
                                        // } else {
                                        return Token{TokenType::knum, this->buffer};
                                        // }
                                    }
                            }
                    }
            }
    };
};

#endif // MINICCOMP_LEXER_H
