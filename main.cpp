#include <algorithm>
#include <iostream>
#include <map>
#include "triangles.h"
#include "quick_intersection.h"

using namespace triangles;

int main() {
    int N;
    std::cin >> N;
    auto triangles = std::vector<triangle>{};
    auto indexes = std::vector<int>{};
    std::map<int, bool> intersected{};

    for (int i = 0; i < N; i++) {
        double coord[9];
        for (int j = 0; j < 9; j++) {
            std::cin >> coord[j];
        }
        intersected[i] = false;
        triangles.emplace_back(
            point{coord[0], coord[1], coord[2]},
            point{coord[3], coord[4], coord[5]},
            point{coord[6], coord[7], coord[8]}
        );
        if(!triangles[i].is_degenerate()) indexes.push_back((i));
    }

    quick_intersection q(triangles, intersected);
    q(indexes);

    for(auto i : intersected) {
        if(i.second)
        std::cout << i.first << "  ";
    }
    return 0;
}
