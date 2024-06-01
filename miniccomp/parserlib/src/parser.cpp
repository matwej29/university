//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

#include <iostream>
#include <utility>

#include "lexer.hpp"

// сейчас я переделываю парсер, добавляя в него семантику

// FIXME: rename to setNextToken
void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
}

bool Parser::validate() {
    std::cout.setf(std::ios::unitbuf);

    contextStack.emplace("-1");

    bool result = Parser::StmtList();

    // graph
    std::vector<std::pair<std::string, int>> activeNonterms;
    bool wasLineBreak = false;
    for (const auto &[current_depth,
                current_entered,
                current_term,
                current_nonterm]: callsHierarchy) {

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

    if (currentToken.type != TokenType::END_OF_FILE) {
        std::cout << std::endl;
        std::cout << TokenTypeToString.at(currentToken.type) << std::endl;
        return false;
    }

    std::cout << '\n';
    return result;
}

bool Parser::DeclareStmt() {
    const std::string func_name = "DeclareStmt";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "DeclareStmt");
    auto TypeRes = Type();
    if (not TypeRes.has_value()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "DeclareStmt");
    if (currentToken.type != TokenType::kid) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    auto value = currentToken.value;
    callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
    setCurrentToken();

    if (not DeclareStmtL(TypeRes.value(), value)) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    call_depth = current_depth;
    return true;
}

bool Parser::DeclareStmtL(const std::string &type, const std::string &name) {
    const std::string func_name = "DeclareStmtL";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "DeclareStmtL");
    if (currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        std::string TC = addFunc(name, type, "0").value(); // FIXME: potential crash
        contextStack.push(TC);

        auto ParamRes = Param();
        if (!ParamRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << "because ParamRes: " << ParamRes.error() << '\n';
            return false;
        }
        for (auto &i: varTable["-1"]) {
            if (std::to_string(i.cnt) == contextStack.top()) {
                i.length = ParamRes.value();
            }
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
        generateAtom(contextStack.top(), "RET", "", "", "'0'");
        call_depth = current_depth;
        return true;
    } else if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type != TokenType::knum) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        auto temp = addVar(name, contextStack.top(), type, currentToken.value).value(); // FIXME: potential crash
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();

        auto DeclVarListRes = DeclVarList(type);
        if (not DeclVarListRes) {
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
    } else {
        std::string temp = addVar(name, contextStack.top(), type, "0").value(); // FIXME: potential crash

        auto DeclVarListRes = DeclVarList(type);
        if (not DeclVarListRes) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }

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

Semantic Parser::Type() {
    const std::string func_name = "Type";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::keyword and (currentToken.value == "int" or currentToken.value == "char")) {
        callsHierarchy.emplace_back(current_depth, true, "", "Type");
        callsHierarchy.emplace_back(current_depth, false, "", "Type");
        callsHierarchy.emplace_back(current_depth, false,
                                    TokenTypeToString.at(currentToken.type) + " " + currentToken.value, "");
        auto type = currentToken.value;
        setCurrentToken();
        call_depth = current_depth;
        return type;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return std::unexpected("NOT PARSED HERE: " + func_name);
}

bool Parser::DeclVarList(const std::string &type) {
    const std::string func_name = "DeclVarList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "DeclVarList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type != TokenType::kid) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        auto temp = currentToken.value;
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();

        if (not InitVar(type, temp)) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "DeclVarList");
        bool result = DeclVarList(type);
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "DeclVarList");
    call_depth = current_depth;
    return true;
}

bool Parser::InitVar(const std::string &r, const std::string &s) {
    const std::string func_name = "InitVar";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "InitVar");
        callsHierarchy.emplace_back(current_depth, false, "", "InitVar");
    if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
    } else {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    if (currentToken.type == TokenType::knum or
        currentToken.type == TokenType::kchar) {
        std::string temp = addVar(s, contextStack.top(), r, currentToken.value).value(); // FIXME: potential crash
        callsHierarchy.emplace_back(current_depth + 1, false, currentToken.value, "");
        call_depth = current_depth;
        setCurrentToken();
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false; // никаких UB
}

Semantic Parser::Param() {
    const std::string func_name = "Param";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Param");
    if (currentToken.type == TokenType::keyword and
        (currentToken.value == "int" or currentToken.value == "char")) {
        auto TypeRes = Type();
        if (TypeRes) {
            callsHierarchy.emplace_back(current_depth, false, "", "Param");
            if (currentToken.type != TokenType::kid) {
                std::cout << "NOT PARSED HERE: " + func_name << '\n';
                std::cout << "expected kid but current token is " << TokenTypeToString.at(currentToken.type) << '\n';
                return std::unexpected("NOT PARSED HERE: " + func_name + " expected kid");
            }
            std::string temp = addVar(currentToken.value, contextStack.top(), TypeRes.value(),
                                      "0").value(); // FIXME: potential crash
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();

            auto ParamListRes = ParamList();
            if (not ParamListRes.has_value()) {
                return ParamListRes.error();
            }
            return std::to_string(stoi(ParamListRes.value()) + 1);
        }
    } else {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "Param");
    }
    call_depth = current_depth;
    return "0";
}

Semantic Parser::ParamList() {
    const std::string func_name = "ParamList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "ParamList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto TypeRes = Type();
        if (!TypeRes.has_value()) {
            std::cout << "NOT PARSED HERE " << func_name << " because Type\n";
            return TypeRes.error();
        }


        callsHierarchy.emplace_back(current_depth, false, "", "ParamList");
        if (currentToken.type == TokenType::kid) {
            std::string temp = addVar(currentToken.value, contextStack.top(), TypeRes.value(),
                                      "0").value(); // FIXME: potential crash
            callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name + " expected kid");
        }
        auto ParamListRes = ParamList();
        if (not ParamListRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        return std::to_string(stoi(ParamListRes.value()) + 1);
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "ParamList");
    }
    call_depth = current_depth;
    return "0";
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

    if (contextStack.top() == "-1") return false;

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
        if (currentToken.type != TokenType::rbrace) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }

        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
        call_depth = current_depth;
        setCurrentToken();
        return true;
    }
    if (currentToken.type == TokenType::keyword and currentToken.value == "return") {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto ExprRes = Expr();
        if (!ExprRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        generateAtom(contextStack.top(), "RET", "", "", ExprRes.value());
        if (currentToken.type != TokenType::semicolon) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "Stmt");
        callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        call_depth = current_depth;
        return true;
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
        auto name = currentToken.value;
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCall");
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        setCurrentToken();
        if (not AssignOrCallL(name)) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::AssignOrCallL(const std::string &name) {
    const std::string func_name = "AssignOrCallL";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "AssignOrCallL");
    if (currentToken.type == TokenType::opassign) {
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCallL");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto ExprRes = Expr();
        if (!ExprRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        auto r = checkVar(name, contextStack.top()).value();
        generateAtom(contextStack.top(), "MOV", ExprRes.value(), "", r);
        call_depth = current_depth;
        return true;
    }
    if (currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto ArgRes = Arg();
        if (!ArgRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        if (currentToken.type != TokenType::rpar) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "AssignOrCallL");
        callsHierarchy.emplace_back(current_depth, false, TokenTypeToString.at(currentToken.type), "");
        call_depth = current_depth;

        auto q = checkFunc(name, ArgRes.value()).value();
        auto r = alloc(contextStack.top());
        generateAtom(contextStack.top(), "CALL", q, "", r);
        setCurrentToken();
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::WhileOp() {
    const std::string func_name = "WhileOp";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "WhileOp");

    auto l1 = newLabel();
    auto l2 = newLabel();

    generateAtom(contextStack.top(), "LBL", "", "", "L" + l1);

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
        auto ExprRes = Expr();
        if (!ExprRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        generateAtom(contextStack.top(), "EQ", ExprRes.value(), "'0'", "L" + l2);
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
        generateAtom(contextStack.top(), "JMP", "", "", "L" + l1);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l2);
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

    auto l1 = newLabel();
    auto l2 = newLabel();
    auto l3 = newLabel();
    auto l4 = newLabel();

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
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l1);
        if (currentToken.type == semicolon) {
            callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
            setCurrentToken();
        } else {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        auto ForExprRes = ForExpr();
        if (!ForExprRes.has_value()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }

        generateAtom(contextStack.top(), "EQ", ForExprRes.value(), "'0'", "L" + l4);
        generateAtom(contextStack.top(), "JMP", "", "", "L" + l3);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l2);

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

        generateAtom(contextStack.top(), "JMP", "", "", "L" + l1);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l3);

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

        generateAtom(contextStack.top(), "JMP", "", "", "L" + l2);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l4);

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

Semantic Parser::ForExpr() {
    const std::string func_name = "ForExpr";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "ForExpr");
    auto ExprRes = Expr();
    if (!ExprRes.has_value()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "ForExpr");
        return "'1'";
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ForExpr");
    current_depth = call_depth;
    return ExprRes.value();
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
            auto p = checkVar(currentToken.value, contextStack.top()).value();
            generateAtom(contextStack.top(), "ADD", p, "'1'", p);
            setCurrentToken();
            return true;
        }
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    // else if (currentToken.type == TokenType::opdec) {
    //     callsHierarchy.emplace_back(current_depth, true, "", "ForLoop");
    //     callsHierarchy.emplace_back(current_depth, false, "", "ForLoop");
    //     callsHierarchy.emplace_back(current_depth, true, TokenTypeToString.at(currentToken.type), "");
    //     setCurrentToken();
    //     if (currentToken.type == TokenType::kid) {
    //         callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
    //         current_depth = call_depth;
    //         auto p = checkVar(currentToken.value, contextStack.top()).value();
    //         generateAtom(contextStack.top(), "SUB", p, "'1'", p);
    //         setCurrentToken();
    //         return true;
    //     }
    //     std::cout << "NOT PARSED HERE: " + func_name << '\n';
    //     return false;
    // }
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
        auto ExprRes = Expr();
        if (!ExprRes.has_value()) {
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
        auto l1 = newLabel();
        generateAtom(contextStack.top(), "EQ", ExprRes.value(), "'0'", "L" + l1);

        if (not Stmt()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        auto l2 = newLabel();
        generateAtom(contextStack.top(), "JMP", "", "", "L" + l2);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l1);

        callsHierarchy.emplace_back(current_depth, false, "", "IfOp");
        if (currentToken == Token{TokenType::keyword, "else"}) {
            if (not ElsePart()) {
                std::cout << "NOT PARSED HERE: " + func_name << '\n';
                return false;
            }
            generateAtom(contextStack.top(), "LBL", "", "", "L" + l2);
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
        auto ExprRes = Expr();
        if (!ExprRes.has_value()) {
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
        auto end = newLabel();
        if (not Cases(ExprRes.value(), end)) {
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
        generateAtom(contextStack.top(), "LBL", "", "", "L" + end);
        call_depth = current_depth;
        return true;
    }
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

bool Parser::Cases(const std::string &p, const std::string &end) {
    const std::string func_name = "Cases";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Cases");
    auto ACaseRes = ACase(p, end);
    if (!ACaseRes.has_value()) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "Cases");
    if (not CasesL(p, end, ACaseRes.value())) {
        std::cout << "NOT PARSED HERE: " + func_name << '\n';
        return false;
    }
    call_depth = current_depth;
    return true;
}

bool Parser::CasesL(const std::string &p, const std::string &end, const std::string &def) {
    const std::string func_name = "CasesL";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken == Token{TokenType::keyword, "case"} or
        currentToken == Token{TokenType::keyword, "default"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "CasesL");
        auto ACaseRes = ACase(p, end);
        if (ACaseRes.has_value()) {
            callsHierarchy.emplace_back(current_depth, false, "", "CasesL");
            auto maxDef = stoi(def) < stoi(ACaseRes.value()) ? ACaseRes.value() : def;
            if (not CasesL(p, end, maxDef)) {
                std::cout << "NOT PARSED HERE: " + func_name << '\n';
                return false;
            }
        }
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "CasesL");
    }
    std::string q = stoi(def) >= 0 ? def : end;
    generateAtom(contextStack.top(), "JMP", "", "", "L" + q);

    call_depth = current_depth;
    return true;
}

Semantic Parser::ACase(const std::string &p, const std::string &end) {
    const std::string func_name = "ACase";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken == Token{TokenType::keyword, "case"}) {
        callsHierarchy.emplace_back(current_depth, true, "", "ACase");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (currentToken.type != TokenType::knum) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        callsHierarchy.emplace_back(current_depth + 1, false, currentToken.value, "");
        std::string next = newLabel();
        generateAtom(contextStack.top(), "NE", p, currentToken.value, "L" + next);
        setCurrentToken();

        if (currentToken.type != TokenType::colon) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        callsHierarchy.emplace_back(current_depth, false, "", "ACase");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();

        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        generateAtom(contextStack.top(), "JMP", "", "", "L" + end);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + next);

        call_depth = current_depth;
        return "-1";
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
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        auto next = newLabel();
        auto def = newLabel();

        generateAtom(contextStack.top(), "JMP", "", "", "L" + next);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + def);

        if (not StmtList()) {
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("NOT PARSED HERE: " + func_name);
        }
        generateAtom(contextStack.top(), "JMP", "", "", "L" + end);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + next);
        call_depth = current_depth;
        return def;
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return std::unexpected("NOT PARSED HERE: " + func_name);
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
        if (currentToken.type != TokenType::kid) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return false;
        }
        callsHierarchy.emplace_back(current_depth + 1, true, currentToken.value, "");
        auto name = currentToken.value;
        setCurrentToken();

        if (currentToken.type == TokenType::semicolon) {
            callsHierarchy.emplace_back(current_depth + 1, false, TokenTypeToString.at(currentToken.type), "");
            auto p = checkVar(name, contextStack.top()).value();
            generateAtom(contextStack.top(), "IN", "", "", p);
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
        generateAtom(contextStack.top(), "OUT", "", "", "\"" + currentToken.value + "\"");
        setCurrentToken();
        return true;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "OutOpL");
    callsHierarchy.emplace_back(current_depth, false, "", "OutOpL");
    if (auto Eres = Expr(); Eres.has_value()) {
        call_depth = current_depth;
        generateAtom(contextStack.top(), "OUT", "", "", Eres.value());
        return true;
    }
    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return false;
}

Semantic Parser::Expr() {
    const std::string func_name = "E";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E");
    callsHierarchy.emplace_back(current_depth, false, "", "E");
    auto result = Expr7();
    call_depth = current_depth;
    return result;
}

Semantic Parser::Expr7() {
    const std::string func_name = "E7";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E7");
    auto E6result = Expr6();
    if (!E6result.has_value()) {
        return E6result.error();
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7");
    auto result = Expr7List(E6result.value());
    call_depth = current_depth;
    return result;
}

Semantic Parser::Expr7List(const std::string &funcID) {
    const std::string func_name = "E7L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opor) {
        callsHierarchy.emplace_back(current_depth, true, "", "E7L");
        callsHierarchy.emplace_back(current_depth + 1, true, "opor", "");
        setCurrentToken();
        auto E6result = Expr6();
        if (!E6result.has_value()) {
            call_depth = current_depth;
            return E6result.error();
        }
        auto s = alloc(contextStack.top());
        generateAtom(contextStack.top(), "OR", funcID, E6result.value(), s);

        callsHierarchy.emplace_back(current_depth, false, "", "E7L");
        auto result = Expr7List(s);
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7L");
    call_depth = current_depth;
    return funcID;
}

Semantic Parser::Expr6() {
    const std::string func_name = "E6";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E6");
    auto E5result = Expr5();
    if (!E5result.has_value()) {
        call_depth = current_depth;
        return E5result.error();
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6");
    auto result = Expr6List(E5result.value());
    call_depth = current_depth;
    return result;
}

Semantic Parser::Expr6List(const std::string &funcID) {
    const std::string func_name = "E6L";
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opand) {
        callsHierarchy.emplace_back(current_depth, true, "", "E6L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E5result = Expr5();
        if (!E5result.has_value()) {
            return E5result.error();
        }
        auto s = alloc(contextStack.top());
        generateAtom(contextStack.top(), "AND", funcID, E5result.value(), s);

        callsHierarchy.emplace_back(current_depth, false, "", "E6L");
        auto result = Expr6List(s);
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6L");
    call_depth = current_depth;
    return funcID;
}

Semantic Parser::Expr5() {
    const std::string func_name = "E5";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E5");

    auto E4result = Expr4();
    if (!E4result.has_value()) {
        call_depth = current_depth;
        return E4result.error();
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E5");
    auto result = Expr5List(E4result.value());
    call_depth = current_depth;
    return result;
}

Semantic Parser::Expr5List(const std::string &funcID) {
    const std::string func_name = "E5L";
    // TODO: refaaaaaaactor please
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opeq) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E4result = Expr4();
        if (!E4result.has_value()) {
            call_depth = current_depth;
            return E4result.error();
        }

        auto s = alloc(contextStack.top());
        auto l = newLabel();

        generateAtom(contextStack.top(), "MOV", "'1'", "", s);
        generateAtom(contextStack.top(), "EQ", funcID, E4result.value(), "L" + l);
        generateAtom(contextStack.top(), "MOV", "'0'", "", s);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l);

        return s;
    } else if (currentToken.type == TokenType::opne) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E4result = Expr4();
        if (!E4result.has_value()) {
            call_depth = current_depth;
            return E4result.error();
        }
        auto s = alloc(contextStack.top());
        auto l = newLabel();

        generateAtom(contextStack.top(), "MOV", "'1'", "", s);
        generateAtom(contextStack.top(), "NE", funcID, E4result.value(), "L" + l);
        generateAtom(contextStack.top(), "MOV", "'0'", "", s);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l);

        return s;
    } else if (currentToken.type == TokenType::opgt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E4result = Expr4();
        if (!E4result.has_value()) {
            call_depth = current_depth;
            return E4result.error();
        }
        auto s = alloc(contextStack.top());
        auto l = newLabel();

        generateAtom(contextStack.top(), "MOV", "'1'", "", s);
        generateAtom(contextStack.top(), "GT", funcID, E4result.value(), "L" + l);
        generateAtom(contextStack.top(), "MOV", "'0'", "", s);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l);

        return s;
    } else if (currentToken.type == TokenType::oplt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E4result = Expr4();
        if (!E4result.has_value()) {
            call_depth = current_depth;
            return E4result.error();
        }
        auto s = alloc(contextStack.top());
        auto l = newLabel();

        generateAtom(contextStack.top(), "MOV", "'1'", "", s);
        generateAtom(contextStack.top(), "LT", funcID, E4result.value(), "L" + l);
        generateAtom(contextStack.top(), "MOV", "'0'", "", s);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l);

        return s;
    } else if (currentToken.type == TokenType::ople) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E4result = Expr4();
        if (!E4result.has_value()) {
            call_depth = current_depth;
            return E4result.error();
        }
        auto s = alloc(contextStack.top());
        auto l = newLabel();

        generateAtom(contextStack.top(), "MOV", "'1'", "", s);
        generateAtom(contextStack.top(), "LE", funcID, E4result.value(), "L" + l);
        generateAtom(contextStack.top(), "MOV", "'0'", "", s);
        generateAtom(contextStack.top(), "LBL", "", "", "L" + l);

        return s;
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "E5L");
    }
    call_depth = current_depth;
    return funcID;
}

Semantic Parser::Expr4() {
    const std::string func_name = "E4";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E4");
    auto E3result = Expr3();
    if (!E3result.has_value()) {
        return E3result.error();
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4");
    auto result = Expr4List(E3result.value());
    call_depth = current_depth;
    return result;
}

Semantic Parser::Expr4List(const std::string &funcID) {
    const std::string func_name = "E4L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opplus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E3result = Expr3();
        if (!E3result.has_value()) {
            call_depth = current_depth;
            return E3result.error();
        }

        auto s = alloc(contextStack.top());
        generateAtom(contextStack.top(), "ADD", funcID, E3result.value(), s);

        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        auto result = Expr4List(s);
        call_depth = current_depth;
        return result;
    } else if (currentToken.type == TokenType::opminus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E3result = Expr3();
        if (!E3result.has_value()) {
            call_depth = current_depth;
            return E3result.error();
        }

        auto s = alloc(contextStack.top());
        generateAtom(contextStack.top(), "SUB", funcID, E3result.value(), s);

        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        auto result = Expr4List(s);
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4L");
    call_depth = current_depth;
    return funcID;
}

Semantic Parser::Expr3() {
    const std::string func_name = "E3";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E3");
    auto Expr2result = Expr2();
    if (!Expr2result.has_value()) {
        call_depth = current_depth;
        return Expr2result.error();
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3");
    auto E3result = Expr3List(Expr2result.value());
    call_depth = current_depth;
    return E3result;
}

Semantic Parser::Expr3List(const std::string &funcID) {
    const std::string func_name = "E3L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opmul) {
        callsHierarchy.emplace_back(current_depth, true, "", "E3L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E2result = Expr2();
        if (!E2result.has_value()) {
            call_depth = current_depth;
            return E2result.error();
        }

        auto s = alloc(contextStack.top());
        generateAtom(contextStack.top(), "MUL", funcID, E2result.value(), s);

        callsHierarchy.emplace_back(current_depth, false, "", "E3L");
        auto result = Expr3List(funcID);
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3L");
    call_depth = current_depth;
    return funcID;
}

Semantic Parser::Expr2() {
    const std::string func_name = "E2";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E2");
    if (currentToken.type == TokenType::opnot) {
        callsHierarchy.emplace_back(current_depth, false, "", "E2");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto E1result = Expr1();
        if (!E1result.has_value()) {
            call_depth = current_depth;
            return E1result.error();
        }

        auto r = alloc(contextStack.top());
        generateAtom(contextStack.top(), "NOT", E1result.value(), "", r);

        call_depth = current_depth;
        return r;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E2");
    auto E1result = Expr1();
    call_depth = current_depth;
    return E1result;
}

Semantic Parser::Expr1() {
    const std::string func_name = "E1";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E1");
    if (currentToken.type == TokenType::opinc) {
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "opinc kid", "");
            auto q = checkVar(currentToken.value, contextStack.top()).value();
            generateAtom(contextStack.top(), "ADD", q, "'1'", q);
            setCurrentToken();
            call_depth = current_depth;
            return q;
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("currentToken is not kid");
        }
    }
        // TODO: to be implemented
        // else if(currentToken.type == TokenType::opdec)
        // {
        //     setCurrentToken();
        //     if (currentToken.type == TokenType::kid) {
        //         callsHierarchy.emplace_back(current_depth, false, "", "E1");
        //         callsHierarchy.emplace_back(current_depth, false, "opdec kid", "");
        //         auto q = checkVar(currentToken.value, contextStack.top()).value();
        //         generateAtom(contextStack.top(), "SUB", q, "'1'", q);
        //         setCurrentToken();
        //         call_depth = current_depth;
        //         return q;
        //     } else {
        //         call_depth = current_depth;
        //         return std::unexpected("currentToken is not kid");
        //     }
        // }
    else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        auto Eresult = Expr();
        if (!Eresult.has_value()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected(Eresult.error());
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            this->setCurrentToken();
            call_depth = current_depth;
            return Eresult.value();
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("currentToken is not rpar");
        }
    } else if (currentToken.type == TokenType::knum) {
        auto token_value = currentToken.value;
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
        setCurrentToken();
        call_depth = current_depth;
        return "'" + token_value + "'";
    } else if (currentToken.type == TokenType::kid) {
        auto r = currentToken.value;
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, true, currentToken.value, "");
        setCurrentToken();
        auto result = Expr1List(r);
        call_depth = current_depth;
        return result;
    }

    call_depth = current_depth;
    std::cout << "NOT PARSED HERE: " + func_name << '\n';
    return std::unexpected("NOT PARSED HERE: " + func_name);
}

Semantic Parser::Expr1List(const std::string &funcID) {
    const std::string func_name = "E1L";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opinc) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth, false, "", "E1L");
        callsHierarchy.emplace_back(call_depth, false, "opinc", "");
        setCurrentToken();

        auto s = checkVar(funcID, contextStack.top()).value();
        auto r = alloc(contextStack.top());

        generateAtom(contextStack.top(), "MOV", s, "", r);
        generateAtom(contextStack.top(), "ADD", s, "'1'", s);
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        auto ArgResult = Arg();
        if (!ArgResult.has_value()) {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("ArgResult has no value");
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1L");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            setCurrentToken();
        } else {
            call_depth = current_depth;
            std::cout << "NOT PARSED HERE: " + func_name << '\n';
            return std::unexpected("currentToken is not rpar");
        }

        auto s = checkFunc(funcID, ArgResult.value());
        if (!s.has_value()) {
            std::cout << s.error();
        }
        auto r = alloc(contextStack.top());

        generateAtom(contextStack.top(), "CALL", s.value(), "", r);

        call_depth = current_depth;
        return r;
    }
    auto q = checkVar(funcID, contextStack.top());
    callsHierarchy.emplace_back(current_depth, false, "", "E1L");
    call_depth = current_depth;
    return q;
}

Semantic Parser::Arg() {
    const std::string func_name = "Arg";
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Arg");
    auto Eresult = Expr();
    if (!Eresult.has_value()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "Arg");
        return "0";
    }
    generateAtom(contextStack.top(), "PARAM", "", "", Eresult.value());
    callsHierarchy.emplace_back(current_depth, false, "", "Arg");
    auto result = ArgList().value();
    call_depth = current_depth;
    return std::to_string(stoi(result) + 1);;
}

Semantic Parser::ArgList() {
    const std::string func_name = "ArgList";
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "ArgList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        auto Eresult = Expr();
        if (!Eresult.has_value()) {
            call_depth = current_depth;
            return Eresult;
        }
        generateAtom(contextStack.top(), "PARAM", "", "", Eresult.value());
        callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
        std::string result = ArgList().value();
        call_depth = current_depth;
        return std::to_string(stoi(result) + 1);
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
    call_depth = current_depth;
    return "0";
}
