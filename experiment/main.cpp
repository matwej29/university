#include <iostream>
#include <string>
#include <cctype>

bool check_password(std::string &password) {
    bool large_letters = false, small_letters = false, digits = false, other = false;

    if (password.size() < 8 or password.size() > 14) {
        return false;
    }

    for (char c: password) {
        if (static_cast<int>(c) < 33 or static_cast<int>(c) > 126) {
            return false;
        }

        if (std::isupper(c))
            large_letters = true;
        else if (std::islower(c))
            small_letters = true;
        else if (std::isdigit(c))
            digits = true;
        else
            other = true;
    }

    return (static_cast<int>(large_letters) + static_cast<int>(small_letters) + static_cast<int>(digits) +
           static_cast<int>(other)) >= 3;
}

int main() {
    std::string password;
    std::getline(std::cin, password);

    if (check_password(password)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    return 0;
}