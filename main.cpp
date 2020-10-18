#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "triangles.h"

using namespace triangles;

int main() {
    int N;
    double coord[9];
    std::cin >> N;
    auto triangles = std::vector<triangle>{};
    auto result = std::vector<int>{};
    std::unordered_map<int, bool> is_checked(N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 9; j++) {
            std::cin >> coord[j];
        }
        is_checked[i] = false;
        triangles.emplace_back(
            point{coord[0], coord[1], coord[2]},
            point{coord[3], coord[4], coord[5]},
            point{coord[6], coord[7], coord[8]}
        );
    }

    for(int i = 0; i < N; i++) {
        if(!is_checked[i]) {
            bool is_current_checked = false;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    if (triangles[i].is_intersecting(triangles[j])) {
                        if(!is_checked[j]) {
                            is_checked[j] = true;
                            result.push_back(j);
                        }
                        if (!is_current_checked) {
                            result.push_back(i);
                            is_current_checked = true;
                        }
                    }
                }
            }
        }
    }
    std::sort(result.begin(), result.end());
    for(auto i : result) {
        std::cout << i << "  ";
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

//2
//0 0 0 1 0 0 0 1 0
//0 0.5 -0.5 0 0.5 0.5 -1 0 0
//
//3
//0 0 0 1 0 0 0 1 0
//5 5 5 5 5 5 5 5 5
//0 0.5 -0.5 0 0.5 0.5 -1 0 0

//2
//0 0 0 1000000000 0 0 0 1000000000 0
//0 0.0000005 -0.0000005 0 0.0000005 0.0000005 -0.000001 0 0
//
//2
//0 0 0 0.0001 0 0 0 0.0001 0
//0 0.0000005 -0.0000005 0 0.0000005 0.0000005 -0.000001 0 0
//5
//10 0 0 0 10 0 0 0 10
//30 0 0 0 30 0 0 0 30
//50 0 0 0 50 0 0 0 50
//70 0 0 0 70 0 0 0 70
//0 0 0 100 100 0 100 100 100