#include <iostream>
#include "triangles.h"

point dot_init(double coord[3]) { //makes a point from 3 coordinates from array
    return point(coord[0], coord[1], coord[2]);
}

int main() {
    int N, counter = 0;
    double coord[3];
    std::cin >> N;
    auto dots = std::vector<point>{};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                std::cin >> coord[k];
            }
            dots.push_back(dot_init(coord));
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++ ) {
            if(i != j) {
                if(triangle(dots[3 * i], dots[3 * i + 1], dots[3 * i + 2]).is_intersecting(
                        triangle(dots[3 * j], dots[3 * j + 1], dots[3 * j + 2]))) {
                    if(counter != (i + 1)) {
                        counter = i + 1;
                        std::cout << (i + 1) << "  ";
                    }
                }
            }
        }
    }

    return 0;
}

void test () { //not used
    triangle(
            point(0, 0, 0), point(1, 0, 0), point(0, 0, 1)
    ).is_intersecting(
            triangle(
                    point(1, 2, 3), point(3, 4, 5), point(5, 6, 7)
            )
    );
}