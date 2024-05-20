//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

#include <iostream>
#include <map>
#include <utility>

#include "lexer.hpp"

Parser::Parser(std::function<Token()> getNextToken)
        : getNextToken(std::move(getNextToken)) {
    currentToken = this->getNextToken();
}

// FIXME: rename to setNextToken
void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
}

bool Parser::validate() {
    std::cout.setf(std::ios::unitbuf);
    bool result = Parser::StmtList();

    if (currentToken.type != TokenType::END_OF_FILE) {
        return false;
    }

    // graph
    std::vector<std::pair<std::string, int>> activeNonterms;
    bool wasLineBreak = false;
    for (const auto &x: callsHierarchy) {
        int current_depth = std::get<0>(x);
        bool current_entered = std::get<1>(x);
        std::string current_term = std::get<2>(x);
        std::string current_nonterm = std::get<3>(x);

        if (wasLineBreak) {
            for (auto i = 0; i < current_depth - current_entered; ++i) {
                bool found = false;
                for (const auto &a_nT: activeNonterms) {
                    // вроде можно за O(1)
                    if (a_nT.second == i) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    std::cout << "│ ";
                } else {
                    std::cout << "  ";
                }
            }

            if (current_entered) {
                std::cout << "├ ";
            }
        }

        if (!current_term.empty()) {
            std::cout << current_term << ' ';
            if (current_entered) {
                wasLineBreak = false;
                continue;
            }
        }
        if (current_entered and !current_nonterm.empty()) {
            activeNonterms.emplace_back(current_nonterm, current_depth);
            std::cout << current_nonterm << '\n';
            wasLineBreak = true;
        } else {
            if (!current_nonterm.empty()) {
                if (current_nonterm == activeNonterms.back().first) {
                    activeNonterms.pop_back();
                    if (!current_term.empty()) {
                        wasLineBreak = false;
                    } else {
                        std::cout << "└ ";
                        wasLineBreak = false;
                    }
                } else {
                    std::cout << current_nonterm << '\n';
                    wasLineBreak = true;
                }
            } else {
                std::cout << '\n';
                wasLineBreak = true;
            }
        }
    }

    std::cout << '\n';
    return result;
}

bool Parser::DeclareStmt() {
    const std::string func_name = "DeclareStmt";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "DeclareStmt");
    if (not Type()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "DeclareStmt");
    if (currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
    } else {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    if (not DeclareStmtL()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    call_depth = current_depth;
    return true;
}

bool Parser::DeclareStmtL() {
    const std::string func_name = "DeclareStmtL";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "DeclareStmtL");
    if (currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Param()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::lbrace) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rbrace) {
            callsHierarchy.emplace_back(current_depth, false, "", "DeclareStmtL");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    } else if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::knum) {
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not DeclVarList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::semicolon) {
            callsHierarchy.emplace_back(current_depth, false, "", "DeclareStmtL");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    } else if (DeclVarList()) {
        if (currentToken.type == TokenType::semicolon) {
            callsHierarchy.emplace_back(current_depth, false, "", "DeclareStmtL");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::Type() {
    const std::string func_name = "Type";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::keyword and (currentToken.value == "int" or currentToken.value == "char")) {
        callsHierarchy.emplace_back(current_depth, true, "", "Type");
        callsHierarchy.emplace_back(current_depth, false, "", "Type");
        callsHierarchy.emplace_back(current_depth, false,
                                    TokenTypeToString.at(currentToken.type) + " " + currentToken.value, "");
        setCurrentToken();
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::DeclVarList() {
    const std::string func_name = "DeclVarList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "DeclVarList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not InitVar()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "DeclVarList");
        bool result = DeclVarList();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "DeclVarList");
    call_depth = current_depth;
    return true;
}

bool Parser::InitVar() {
    const std::string func_name = "InitVar";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "InitVar");
    if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
    } else {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    if (currentToken.type == TokenType::knum or
        currentToken.type == TokenType::kchar) {
        callsHierarchy.emplace_back(current_depth, false, "", "InitVar");
        callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
        call_depth = current_depth;
        setCurrentToken();
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false; // никаких UB
}

bool Parser::Param() {
    const std::string func_name = "Param";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Param");
    if (Type()) {
        callsHierarchy.emplace_back(current_depth, false, "", "Param");
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not ParamList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "Param");
    }
    call_depth = current_depth;
    return true;
}

bool Parser::ParamList() {
    const std::string func_name = "ParamList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "ParamList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Type()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "ParamList");
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not ParamList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "ParamList");
    }
    call_depth = current_depth;
    return true;
}

bool Parser::StmtList() {
    const std::string func_name = "StmtList";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "StmtList");
    if (not Stmt()) {
        callsHierarchy.pop_back(); // вход в StmtList не нужен, потому что обращаемся в \eps
        callsHierarchy.pop_back(); // для stmt - грязь
        callsHierarchy.emplace_back(current_depth, false, "", "StmtList");
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "StmtList");
    if (not StmtList()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    call_depth = current_depth;
    return true;
}

bool Parser::Stmt() {
    const std::string func_name = "Stmt";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Stmt");
    if (currentToken.type == TokenType::keyword and (currentToken.value == "int" or currentToken.value == "char")) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (DeclareStmt()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken.type == TokenType::semicolon) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        current_depth = call_depth;
        return true;
    }
    if (currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        if (AssignOrCallOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "while"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (WhileOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "for"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (ForOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "if"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (IfOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "switch"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (SwitchOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "in"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (InOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken == Token{TokenType::keyword, "out"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        if (OutOp()) {
            current_depth = call_depth;
            return true;
        }
    }
    if (currentToken.type == TokenType::lbrace) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rbrace) {
            callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            call_depth = current_depth;
            setCurrentToken();
            return true;
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    }
    if (currentToken.type == TokenType::keyword and currentToken.value == "return") {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::semicolon) {
            callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
            call_depth = current_depth;
            return true;
        }
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::AssignOrCallOp() {
    const std::string func_name = "AssignOrCallOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "AssignOrCallOp");
    if (not AssignOrCall()) {
        callsHierarchy.pop_back();
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    if (currentToken.type == TokenType::semicolon) {
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCallOp");
        callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::AssignOrCall() {
    const std::string func_name = "AssignOrCall";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "AssignOrCall");
    if (currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCall");
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (not AssignOrCallL()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::AssignOrCallL() {
    const std::string func_name = "AssignOrCallL";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "AssignOrCallL");
    if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCallL");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    if (currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Arg()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCallL");
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            call_depth = current_depth;
            setCurrentToken();
            return true;
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::WhileOp() {
    const std::string func_name = "WhileOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "WhileOp");
    if (currentToken == Token{TokenType::keyword, "while"}) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::lpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Expr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "WhileOp");
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Stmt()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::ForOp() {
    const std::string func_name = "ForOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "ForOp");
    if (currentToken == Token{TokenType::keyword, "for"}) {
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (currentToken.type == lpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not ForInit()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == semicolon) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not ForExpr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == semicolon) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not ForLoop()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "ForOp");
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Stmt()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::ForInit() {
    const std::string func_name = "ForInit";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "ForInit");
    if (not AssignOrCall()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "ForInit");
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ForInit");
    current_depth = call_depth;
    return true;
}

bool Parser::ForExpr() {
    const std::string func_name = "ForExpr";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "ForExpr");
    if (not Expr()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "ForExpr");
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ForExpr");
    current_depth = call_depth;
    return true;
}

bool Parser::ForLoop() {
    const std::string func_name = "ForLoop";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opinc) {
        callsHierarchy.emplace_back(current_depth, true, "", "ForLoop");
        callsHierarchy.emplace_back(current_depth, false, "", "ForLoop");
        callsHierarchy.emplace_back(current_depth, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
            current_depth = call_depth;
            setCurrentToken();
            return true;
        }
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ForLoop");
    AssignOrCall(); // TODO: ???
    current_depth = call_depth;

    return true;
}

bool Parser::IfOp() {
    const std::string func_name = "IfOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "IfOp");
    if (currentToken == Token{TokenType::keyword, "if"}) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::lpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Expr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Stmt()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "IfOp");
        if (not ElsePart()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::ElsePart() {
    const std::string func_name = "ElsePart";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken == Token{TokenType::keyword, "else"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "ElsePart");
        callsHierarchy.emplace_back(current_depth, false, "", "ElsePart");
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (not Stmt()) {
            callsHierarchy.pop_back();
        }
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ElsePart");
    call_depth = current_depth;
    return true;
}

bool Parser::SwitchOp() {
    const std::string func_name = "SwitchOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "SwitchOp");
    if (currentToken == Token{TokenType::keyword, "switch"}) {
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (currentToken.type == TokenType::lpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Expr()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::lbrace) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not Cases()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "SwitchOp");
        if (currentToken.type == TokenType::rbrace) {
            callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::Cases() {
    const std::string func_name = "Cases";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Cases");
    if (not ACase()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "Cases");
    if (not CasesL()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    call_depth = current_depth;
    return true;
}

bool Parser::CasesL() {
    const std::string func_name = "CasesL";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken == Token{TokenType::keyword, "case"} or
        currentToken == Token{TokenType::keyword, "default"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "CasesL");
        if (ACase()) {
            callsHierarchy.emplace_back(current_depth, false, "", "CasesL");
            if (not CasesL()) {
                std::cout << "NOT PARSED HERE: " + func_name << '\n';
                return false;
            }
        }
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "CasesL");
    }
    call_depth = current_depth;
    return true;
}

bool Parser::ACase() {
    const std::string func_name = "ACase";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken == Token{TokenType::keyword, "case"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "ACase");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::knum) {
            callsHierarchy.emplace_back(current_depth + 1, false, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::colon) {
            callsHierarchy.emplace_back(current_depth, false, "", "ACase");
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    if (currentToken == Token{TokenType::keyword, "default"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "ACase");
        callsHierarchy.emplace_back(current_depth, false, "", "ACase");
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (currentToken.type == TokenType::colon) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << "whyyyyyyyy\n";
            return false;
        }
        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << "noooooot\n";
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::InOp() {
    const std::string func_name = "InOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "InOp");
    callsHierarchy.emplace_back(current_depth, false, "", "InOp");
    if (currentToken == Token{TokenType::keyword, "in"}) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type == TokenType::semicolon) {
            callsHierarchy.emplace_back(current_depth + 1, false, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::OutOp() {
    const std::string func_name = "OutOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "OutOp");
    if (currentToken == Token{TokenType::keyword, "out"}) {
        callsHierarchy.emplace_back(current_depth, false, "", "OutOp");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        bool result = OutOpL();
        call_depth = current_depth;
        return result;
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::OutOpL() {
    const std::string func_name = "OutOpL";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::kstr) {
        callsHierarchy.emplace_back(current_depth, true, "", "OutOpL");
        callsHierarchy.emplace_back(current_depth, false, "", "OutOpL");
        callsHierarchy.emplace_back(current_depth + 1, false, currentToken.value, "");
        call_depth = current_depth;
        setCurrentToken();
        return true;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "OutOpL");
    callsHierarchy.emplace_back(current_depth, false, "", "OutOpL");
    if (Expr()) {
        call_depth = current_depth;
        return true;
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::Expr() {
    const std::string func_name = "E";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E");
    callsHierarchy.emplace_back(current_depth, false, "", "E");
    bool result = Expr7();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr7() {
    const std::string func_name = "E7";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E7");
    if (!Expr6()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7");
    bool result = Expr7List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr7List() {
    const std::string func_name = "E7L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opor) {
        callsHierarchy.emplace_back(current_depth, true, "", "E7L");
        callsHierarchy.emplace_back(current_depth + 1, true, "opor", "");
        setCurrentToken();
        if (!Expr6()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E7L");
        bool result = Expr7List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr6() {
    const std::string func_name = "E6";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E6");
    if (!Expr5()) {
        call_depth = current_depth;
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6");
    bool result = Expr6List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr6List() {
    const std::string func_name = "E6L";
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opand) {
        callsHierarchy.emplace_back(current_depth, true, "", "E6L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr5()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E6L");
        bool result = this->Expr6List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr5() {
    const std::string func_name = "E5";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E5");
    if (!Expr4()) {
        call_depth = current_depth;
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E5");
    bool result = Expr5List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr5List() {
    const std::string func_name = "E5L";
    // TODO: refactor
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opeq) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (currentToken.type == TokenType::opne) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (currentToken.type == TokenType::opgt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (currentToken.type == TokenType::oplt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (currentToken.type == TokenType::ople) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
    }
    call_depth = current_depth;
    return true;
}

bool Parser::Expr4() {
    const std::string func_name = "E4";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E4");
    if (!Expr3()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4");
    bool result = Expr4List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr4List() {
    const std::string func_name = "E4L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opplus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr3()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        bool result = Expr4List();
        call_depth = current_depth;
        return result;
    } else if (currentToken.type == TokenType::opminus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr3()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        bool result = Expr4List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr3() {
    const std::string func_name = "E3";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E3");
    if (not Expr2()) {
        call_depth = current_depth;
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3");
    bool result = Expr3List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr3List() {
    const std::string func_name = "E3L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opmul) {
        callsHierarchy.emplace_back(current_depth, true, "", "E3L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr2()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E3L");
        bool result = Expr3List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr2() {
    const std::string func_name = "E2";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E2");
    if (currentToken.type == TokenType::opnot) {
        callsHierarchy.emplace_back(current_depth, false, "", "E2");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr1()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E2");
    bool result = Expr1();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr1() {
    const std::string func_name = "E1";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E1");
    if (currentToken.type == TokenType::opinc) {
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "opinc kid", "");
            setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        if (!Expr()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            this->setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
    } else if (currentToken.type == TokenType::knum) {
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
        setCurrentToken();
        call_depth = current_depth;
        return true;
    } else if (currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, true, currentToken.value, "");
        setCurrentToken();
        bool result = Expr1List();
        call_depth = current_depth;
        return result;
    }

    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::Expr1List() {
    const std::string func_name = "E1L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opinc) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth, false, "", "E1L");
        callsHierarchy.emplace_back(call_depth, false, "opinc", "");
        setCurrentToken();
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        if (!Arg()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1L");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            setCurrentToken();
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E1L");
    call_depth = current_depth;
    return true;
}

bool Parser::Arg() {
    const std::string func_name = "Arg";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Arg");
    if (not Expr()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "Arg");
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "Arg");
    bool result = ArgList();
    call_depth = current_depth;
    return result;
}

bool Parser::ArgList() {
    const std::string func_name = "ArgList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "ArgList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
        bool result = ArgList();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
    call_depth = current_depth;
    return true;
}
