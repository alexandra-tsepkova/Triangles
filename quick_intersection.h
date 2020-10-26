#ifndef TRIANGLES_QUICK_INTERSECTION_H
#define TRIANGLES_QUICK_INTERSECTION_H

#include <map>
#include "triangles.h"

using namespace triangles;

class quick_intersection {
private:
    std::vector<triangle> &triangles;
    std::map<int, bool> &intersected;

public:
    quick_intersection(std::vector<triangle> &triangles, std::map<int, bool> &intersected) :
            triangles(triangles), intersected(intersected) {};

    void operator()(std::vector<int> indexes) {
        if (indexes.size() <= 1) return;
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
        operator()(l);
        operator()(r);
    }
};


#endif //TRIANGLES_QUICK_INTERSECTION_H
