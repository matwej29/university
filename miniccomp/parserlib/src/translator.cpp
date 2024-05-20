//
// Created by Матвей Щербаков on 20.05.2024.
//

#include "parser.h"


std::string Parser::newLabel() {
    return std::to_string(labelCounter++);
}

std::string Parser::alloc(const std::string &scope) {
    return addVar("!T" + std::to_string(newVarCounter++), scope, "int", "0").value();
}

std::expected<std::string, std::string>
Parser::addVar(const std::string &name, const std::string &scope, const std::string &type, const std::string &init) {
    if (varTable.find(scope) == varTable.end()) {
        varTable[scope] = std::vector<VarOrFunc>();
    }
    for (auto &var: varTable[scope]) {
        if (var.name == name) {
            return std::unexpected("Variable " + name + " already exists in scope " + scope);
        }
    }
    varTable[scope].push_back({name, scope, type, init, "var", "0", newVarCounter++});
    return name;
}

std::expected<std::string, std::string>
Parser::addFunc(const std::string &name, const std::string &type, const std::string &length) {
    if (varTable.find("-1") == varTable.end()) {
        varTable["-1"] = std::vector<VarOrFunc>();
    }
    for (auto &var: varTable["-1"]) {
        if (var.name == name) {
            return std::unexpected("Function " + name + " already exists in global scope");
        }
    }
    varTable["-1"].push_back({name, "-1", type, "", "func", length, newVarCounter++});
    return name;
}

std::expected<std::string, std::string> Parser::checkVar(const std::string &name, const std::string &scope) {
    if (varTable.find(scope) == varTable.end()) {
        return std::unexpected("Scope " + scope + " does not exist");
    }
    for (auto &var: varTable[scope]) {
        if (var.name == name) {
            return var.type;
        }
    }
    return std::unexpected("Variable " + name + " does not exist in scope " + scope);
}

std::expected<std::string, std::string> Parser::checkFunc(const std::string &name, const int length) {
    for (auto &var: varTable["-1"]) {
        if (var.name == name && var.kind == "func" && var.length == std::to_string(length)) {
            return var.type;
        }
    }
    return std::unexpected("Function " + name + " does not exist in scope global");
}
