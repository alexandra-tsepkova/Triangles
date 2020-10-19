#include <algorithm>
#include <iostream>
#include <map>
#include "triangles.h"

using namespace triangles;

void quick_intersection (const std::vector<triangle> &triangles, std::vector<int> indexes,
                                     std::map<int, bool> &intersected) {
    if(indexes.size() <= 1) return;
    std::vector<int> l;
    std::vector<int> r;
    bool is_first_intersected = false;
    for (int i = 1; i < indexes.size(); ++i) {
        auto result = triangles[indexes[0]].is_intersecting(triangles[indexes[i]]);
        if (result.first) {
            if (!is_first_intersected) {
                intersected[indexes[0]] = true;
                is_first_intersected = true;
            }
            intersected[indexes[i]] = true;
        }
        switch (result.second) {
            case triangles::to_the_left:
                l.push_back(indexes[i]);
                break;
            case triangles::to_the_right:
                r.push_back(indexes[i]);
                break;
            case triangles::intersecting:
                l.push_back(indexes[i]);
                r.push_back(indexes[i]);
                break;
        }
    }
    quick_intersection (triangles, l, intersected);
    quick_intersection (triangles, r, intersected);
}

int main() {
    int N;
    double coord[9];
    std::cin >> N;
    auto triangles = std::vector<triangle>{};
    auto indexes = std::vector<int>{};
    std::map<int, bool> intersected{};

    for (int i = 0; i < N; i++) {
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

    quick_intersection(triangles, indexes, intersected);

    for(auto i : intersected) {
        if(i.second)
        std::cout << i.first << "  ";
    }
    return 0;
}
