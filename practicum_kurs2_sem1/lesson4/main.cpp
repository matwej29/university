#include <iostream>
#include <string>
#include <sstream>
#include <set>


void two_char_arr() {
    char w1[25], w2[25];

    std::cin >> w1 >> w2;
    std::cout << w2 << ' ' << w1;
}

void two_strings() {
    std::string s1, s2;
    std::cin >> s1 >> s2;
    std::cout << s2 << ' ' << s1;
}

void set_strings() {
    std::string input;
    getline(std::cin, input);

    std::set<std::string> my_set;

    std::stringstream ss(input);
    std::string temp;
    while (ss >> temp) {
        my_set.insert(temp);
    }

    for (const auto &s: my_set) {
        std::cout << s << ' ';
    }
}

int main() {
    two_char_arr();
    std::cout << '\n';

    two_strings();
    std::cout << '\n';

    set_strings();
}
