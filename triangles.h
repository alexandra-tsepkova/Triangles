//
// Created by Alexandra on 28/09/2020.
//

#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

class point {
    double x, y, z;

public:
    point (double x, double y, double z) : x(x), y(y), z(z) {}
    double get_x() const {
        return x;
    };
    double get_y() const {
        return y;
    };
    double get_z() const {
        return z;
    };
    point operator+(const point &rv) const {
        return point(x + rv.x, y + rv.y, z + rv.z);
    }

    point operator-() const {
        return point(-x, -y, -z);
    }

    point operator-(const point &rv) const {
        return *this + (-rv);
    }
    point vector_mul (const point &rv) const { //vector multiplication
        return point(get_y() * rv.get_z() - get_z() * rv.get_y(),
                     get_z() * rv.get_x() - get_x() * rv.get_z(),
                     get_x() * rv.get_y() - get_y() * rv.get_x());
    }

};

bool point_in_unit_triangle (const point p) {
    return (((p.get_x() + p.get_y()) < 1) && (p.get_x() > 0) && (p.get_y() > 0));
}

bool segment_intersects_or_in_unit_triangle(const point rv1, const point rv2) {
    if ((point_in_unit_triangle(rv1)) || (point_in_unit_triangle(rv2))) {
        return true;
    } else {
        if ((rv1.get_x() * rv2.get_x()) < 0) {
            double y_c = ((rv1.get_y() < rv2.get_y()) ? rv1.get_y() : rv2.get_y()) +
                         std::abs(rv1.get_y() - rv2.get_y()) *
                         std::abs((rv1.get_y() < rv2.get_y()) ? rv1.get_x() : rv2.get_x()) /
                         std::abs(rv1.get_x() - rv2.get_x());
            if ((y_c <= 1) && (y_c >= 0)) {
                return true;
            }
        }
        if ((rv1.get_y() * rv2.get_y()) < 0) {
            double x_c = ((rv1.get_x() < rv2.get_x()) ? rv1.get_x() : rv2.get_x()) +
                         std::abs(rv1.get_x() - rv2.get_x()) *
                         std::abs((rv1.get_x() < rv2.get_x()) ? rv1.get_y() : rv2.get_y()) /
                         std::abs(rv1.get_y() - rv2.get_y());
            if ((x_c <= 1) && (x_c >= 0)) {
                return true;
            }
        }
    }
    return false;
}
point intersect_xy (const point p1, const point p2) {
    return point(p1.get_x() + (p2.get_x() - p1.get_x()) * std::abs(p1.get_z()) / (std::abs(p1.get_z()) + std::abs(p2.get_z())),
                 p1.get_y() + (p2.get_y() - p1.get_y()) * std::abs(p1.get_z()) / (std::abs(p1.get_z()) + std::abs(p2.get_z())),
                 0);
}

class matrix { //matrix 3x3
    double A[3][3];
    void assign_column (point v, int i) {
        A[0][i] = v.get_x();
        A[1][i] = v.get_y();
        A[2][i] = v.get_z();
    }

public:
    matrix (point v1, point v2, point v3) {
        assign_column(v1, 0);
        assign_column(v2, 1);
        assign_column(v3, 2);
    }

    matrix () {
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                A[i][j] = 0;
            }
        }
    }

    double get_minor(int i, int j) const {
        double m, a[4];
        int s = 0;
        for(int k = 0; k < 3; k++){
            for(int p = 0; p < 3; p++){
                if((k != i) && (p != j)) {
                    a[s] = A[k][p];
                    s++;
                }
            }
        }
        m = a[0] * a[3] - a[1] * a[2];
        return m;
    }
    double det() const {
        return (A[0][0]) * get_minor(0, 0) - (A[0][1]) * get_minor(0, 1) + (A[0][2]) * get_minor(0, 2);
    }

    matrix transpose () const {
        matrix B;
        for (int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++) {
                B.A[i][j] = A[j][i];
            }
        }
        return B;
    }

    matrix inverse () const {
        matrix B;
        double d = det();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                B.A[i][j] = get_minor(i, j) * ((i + j) % 2 == 0 ? 1 : -1) / d;
            }
        }
        return B.transpose();
    }

    point mul_by_vector (const point &v) const{
        return point (A[0][0] * v.get_x() + A[0][1] * v.get_y() + A[0][2] * v.get_z(),
                      A[1][0] * v.get_x() + A[1][1] * v.get_y() + A[1][2] * v.get_z(),
                      A[2][0] * v.get_x() + A[2][1] * v.get_y() + A[2][2] * v.get_z());
    }

};

class triangle {
    point p1, p2, p3;

public:
    triangle (point p1, point p2, point p3) : p1(p1), p2(p2), p3(p3) {}
    bool is_intersecting (const triangle &rv) const {
        triangle t1 = move_by_vec(p1);
        triangle t2 = rv.move_by_vec(p1);
        point n = t1.p2.vector_mul(t1.p3);
        matrix A = matrix(t1.p2, t1.p3, n).inverse();
        t1 = t1.apply_operator(A);
        t2 = t2.apply_operator(A);

        auto z_coords = std::vector{t2.p1.get_z(), t2.p2.get_z(), t2.p3.get_z()};
        std::sort(z_coords.begin(), z_coords.end());

        if((z_coords[0] > 0) || (z_coords[2] < 0)) {
            return false;
        } else if(z_coords[0] == z_coords[2]) { //case with triangle in plane
            // Tut shto to bufet

            if(segment_intersects_or_in_unit_triangle(t2.p1, t2.p2) ||
            segment_intersects_or_in_unit_triangle(t2.p2, t2.p3) ||
            segment_intersects_or_in_unit_triangle(t2.p1, t2.p3)) {
                return true;
            }

            triangle t3 = move_by_vec(rv.p1);
            triangle t4 = rv.move_by_vec(rv.p1);
            point n = t4.p2.vector_mul(t4.p3);
            matrix A = matrix(t4.p2, t4.p3, n).inverse();
            t3 = t3.apply_operator(A);

            return point_in_unit_triangle(t3.p1);

        } else if((z_coords[0] == 0) || (z_coords[2] == 0)) { //case with dot in unit triangle
            point p_in_plane(0, 0, 0);
            for (const auto &p : std::vector{t2.p1, t2.p2, t2.p3}) {
                if (p.get_z() == 0) p_in_plane = p;
            }
            return point_in_unit_triangle (p_in_plane);
        } else { //case with segment in plane
            point a(0, 0, 0), b(0, 0, 0);
            if (t2.p1.get_z() * t2.p2.get_z() >= 0) {
                a = intersect_xy(t2.p1, t2.p3);// p3
                b = intersect_xy(t2.p2, t2.p3);
            } else if (t2.p1.get_z() * t2.p3.get_z() >= 0) {
                a = intersect_xy(t2.p1, t2.p2);
                b = intersect_xy(t2.p3, t2.p2);// p2
            } else {
                a = intersect_xy(t2.p2, t2.p1);
                b = intersect_xy(t2.p3, t2.p1);// p1
            }
            return segment_intersects_or_in_unit_triangle(a, b);
        }


//            std::vector<point> p = std::vector{t2.p1, t2.p2, t2.p3};
//            std::sort(p.begin(), p.end(), [](const point &rv, const point &lv) -> bool {return rv.get_z() < lv.get_z();} ); //comparing by z
//            if(p[1].get_z() > 0) {
//                point a(p[1].get_x(), p[1].get_y(), 0), b(p[0].get_x(), p[0].get_y(), 0), c(p[2].get_x(), p[2].get_y(), 0);
//            } else if(p[1].get_z() < 0) {
//                point a(p[0].get_x(), p[0].get_y(), 0), b(p[2].get_x(), p[2].get_y(), 0), c(p[1].get_x(), p[1].get_y(), 0);
//            } else {
//                point a(p[2].get_x(), p[2].get_y(), 0), b(p[1].get_x(), p[1].get_y(), 0), c(p[0].get_x(), p[0].get_y(), 0);
//            }
//            //point a(p[0].get_x(), p[0].get_y(), 0), c(p[2].get_x(), p[2].get_y(), 0);
//            point new_a((a.get_x() * std::abs(p[1].get_z()) / (std::abs(p[1].get_z()) + std::abs(p[0].get_z()))),
//                        (a.get_y() * std::abs(p[1].get_z()) / (std::abs(p[1].get_z()) + std::abs(p[0].get_z()))), 0);
//            point new_c((c.get_x() * std::abs(p[1].get_z()) / (std::abs(p[1].get_z()) + std::abs(p[2].get_z()))),
//                        (c.get_y() * std::abs(p[1].get_z()) / (std::abs(p[1].get_z()) + std::abs(p[2].get_z()))), 0);
//            return segment_intersects_or_in_unit_triangle(new_a, new_c);



//        if(((t2.p1.get_z() < 0) && (t2.p2.get_z() < 0) && (t2.p3.get_z() < 0)) || ((t2.p1.get_z() > 0) && (t2.p2.get_z() > 0) && (t2.p3.get_z() > 0))) {
//            return false;
//        }

//t1.print_triangle();
//t2.print_triangle();
//return true;
    }
    triangle move_by_vec (const point &p) const {
        return triangle (p1 - p, p2 - p, p3 - p);
    }
    triangle apply_operator (const matrix &A) const{
        return triangle (A.mul_by_vector(p1), A.mul_by_vector(p2), A.mul_by_vector(p3));
    }
    void print_triangle() const {
        std::cout << "p1: " << p1.get_x() << " " << p1.get_y() << " " << p1.get_z() << std::endl;
        std::cout << "p2: " << p2.get_x() << " " << p2.get_y() << " " << p2.get_z() << std::endl;
        std::cout << "p3: " << p3.get_x() << " " << p3.get_y() << " " << p3.get_z() << std::endl;
    }

};



#endif //TRIANGLES_H
