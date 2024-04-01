#include <iostream>

typedef double f64;

struct Point {
    f64 x, y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

struct Segment {
    Point first, second;

    f64 length() const {
        f64 dx = first.x - second.x;
        f64 dy = first.y - second.y;

        return sqrt(dx * dx + dy * dy);
    }
};

int main() {
    Segment s = {{1, 2},
                 {3, 4}};

    std::cout << s.length();
    return 0;
}


