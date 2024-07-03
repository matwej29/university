//
// Created by Матвей Щербаков on 20.05.2024.
//

#include "parser.h"
#include <iostream>
#include <algorithm>


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
            std::cout << "Variable " + name + " already exists in scope " + scope << std::endl;
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
            std::cout << "Function " + name + " already exist in scope global";
            return std::unexpected("Function " + name + " already exists in global scope");
        }
    }
    varTable["-1"].push_back({name, "-1", type, "", "func", length, newVarCounter++});
    return std::to_string(newVarCounter - 1);
}

std::expected<std::string, std::string> Parser::checkVar(const std::string &name, const std::string &scope) {
    if (varTable.find(scope) == varTable.end()) {
        return std::unexpected("Scope " + scope + " does not exist");
    }
    for (auto &var: varTable[scope]) {
        if (var.name == name) {
            return std::to_string(var.cnt);
        }
    }
    return std::unexpected("Variable " + name + " does not exist in scope " + scope);
}

std::expected<std::string, std::string> Parser::checkFunc(const std::string &name, const std::string &length) {
    for (auto &var: varTable["-1"]) {
        if (var.name == name && var.kind == "func" && var.length == length) {
            return std::to_string(var.cnt);
        }
    }
    std::cout << "Function " + name + " does not exists in scope global" << std::endl;
    return std::unexpected("Function " + name + " does not exists in scope global");
}


void Parser::generateAtom(const std::string &context, const std::string &text, const std::string &first,
                          const std::string &second, const std::string &third) {
    atoms.push_back({context, text, first, second, third});
}


void Parser::printAtoms() {
    std::vector<VarOrFunc> sortedTable;
    for (auto &scope: varTable) {
        for (auto &var: scope.second) {
            sortedTable.push_back(var);
        }
    }
    std::ranges::sort(sortedTable, [](auto a, auto b) { return a.cnt < b.cnt; });

    std::cout << "Name Scope Type Init Kind Length" << std::endl;
    for (auto &var: sortedTable) {
        std::cout << var.cnt << " " << var.name << " " << var.scope << " " << var.type << " " << var.init << " " << var.
                kind << " "
                << var.length << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Context Text First Second Third" << std::endl;

    for (auto &atom: atoms) {
        std::cout << atom.context << " " << atom.text << " " << atom.first << " " << atom.second << " " << atom.third
                << std::endl;
    }
}
