#include <iostream>
#include "triangles.h"

int main() {
    triangle(
    point(0, 0, 0), point(1, 0, 0), point(0, 0, 1)
    ).is_intersecting(
            triangle(
                    point(1, 2, 3), point(3, 4, 5), point(5, 6, 7)
                    )
            );
    return 0;
}
