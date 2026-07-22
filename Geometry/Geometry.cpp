#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;

const double pi = acos(-1.0);
const double eps = 1e-8;
const int maxp = 1010;

int sgn(double x) {
    if (fabs(x) < eps) return 0;
    return x < 0 ? -1 : 1;
}

int Dcmp(double x, double y) {
    return sgn(x - y);
}

double clamp_unit(double x) {
    return max(-1.0, min(1.0, x));
}

// -------------------- 2D: points and lines --------------------

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator+(const Point &b) const { return Point(x + b.x, y + b.y); }
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    Point operator*(double k) const { return Point(x * k, y * k); }
    Point operator/(double k) const { return Point(x / k, y / k); }

    bool operator==(const Point &b) const {
        return sgn(x - b.x) == 0 && sgn(y - b.y) == 0;
    }

    bool operator<(const Point &b) const {
        if (x != b.x) return x < b.x;
        return y < b.y;
    }
};

using Vector = Point;

double Dot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

double Cross(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

double Len2(Vector a) {
    return Dot(a, a);
}

double Len(Vector a) {
    return sqrt(Len2(a));
}

double Angle(Vector a, Vector b) {
    return acos(clamp_unit(Dot(a, b) / Len(a) / Len(b)));
}

// Twice the signed area of triangle ABC.
double Area2(Point a, Point b, Point c) {
    return Cross(b - a, c - a);
}

double Distance(Point a, Point b) {
    return Len(a - b);
}

double Dist(Point a, Point b) {
    return hypot(a.x - b.x, a.y - b.y);
}

Vector Normal(Vector a) {
    return Vector(-a.y, a.x) / Len(a);
}

bool Parallel(Vector a, Vector b) {
    return sgn(Cross(a, b)) == 0;
}

// Rotates a counterclockwise by rad radians.
Vector Rotate(Vector a, double rad) {
    return Vector(a.x * cos(rad) - a.y * sin(rad),
                  a.x * sin(rad) + a.y * cos(rad));
}

struct Line {
    Point p1, p2;

    Line() = default;
    Line(Point p1, Point p2) : p1(p1), p2(p2) {}

    // Builds an infinite line through p with direction angle.
    Line(Point p, double angle) : p1(p), p2(p + Point(cos(angle), sin(angle))) {}

    // Builds the line ax + by + c = 0. At least one of a and b must be nonzero.
    Line(double a, double b, double c) {
        assert(sgn(a) != 0 || sgn(b) != 0);
        if (sgn(a) == 0) {
            p1 = Point(0, -c / b);
            p2 = Point(1, -c / b);
        } else if (sgn(b) == 0) {
            p1 = Point(-c / a, 0);
            p2 = Point(-c / a, 1);
        } else {
            p1 = Point(0, -c / b);
            p2 = Point(1, -(a + c) / b);
        }
    }
};

using Segment = Line;

// Returns the direction angle in [0, pi).
double Line_angle(Line v) {
    double angle = atan2(v.p2.y - v.p1.y, v.p2.x - v.p1.x);
    if (angle < 0) angle += pi;
    if (angle >= pi) angle -= pi;
    return angle;
}

// Returns 1 for left, 2 for right, and 0 for on the line p1 -> p2.
int Point_line_relation(Point p, Line v) {
    int c = sgn(Cross(p - v.p1, v.p2 - v.p1));
    if (c < 0) return 1;
    if (c > 0) return 2;
    return 0;
}

bool Point_on_seg(Point p, Segment v) {
    return sgn(Cross(p - v.p1, v.p2 - v.p1)) == 0 &&
           sgn(Dot(p - v.p1, p - v.p2)) <= 0;
}

// Returns 0 for parallel, 1 for coincident, and 2 for intersecting.
int Line_relation(Line v1, Line v2) {
    if (sgn(Cross(v1.p2 - v1.p1, v2.p2 - v2.p1)) == 0) {
        return Point_line_relation(v1.p1, v2) == 0 ? 1 : 0;
    }
    return 2;
}

double Dis_point_line(Point p, Line v) {
    return fabs(Cross(p - v.p1, v.p2 - v.p1)) / Len(v.p2 - v.p1);
}

Point Point_line_proj(Point p, Line v) {
    Vector direction = v.p2 - v.p1;
    double k = Dot(direction, p - v.p1) / Len2(direction);
    return v.p1 + direction * k;
}

Point Point_line_symmetry(Point p, Line v) {
    Point q = Point_line_proj(p, v);
    return q * 2 - p;
}

double Dis_point_seg(Point p, Segment v) {
    if (v.p1 == v.p2) return Distance(p, v.p1);
    if (sgn(Dot(p - v.p1, v.p2 - v.p1)) < 0 ||
        sgn(Dot(p - v.p2, v.p1 - v.p2)) < 0) {
        return min(Distance(p, v.p1), Distance(p, v.p2));
    }
    return Dis_point_line(p, v);
}

// The caller must ensure that the two infinite lines are not parallel.
Point Cross_point(Point a, Point b, Point c, Point d) {
    double s1 = Cross(b - a, c - a);
    double s2 = Cross(b - a, d - a);
    return (c * s2 - d * s1) / (s2 - s1);
}

// Strict intersection: endpoints and collinear overlap are excluded.
bool Cross_segment(Point a, Point b, Point c, Point d) {
    double c1 = Cross(b - a, c - a);
    double c2 = Cross(b - a, d - a);
    double d1 = Cross(d - c, a - c);
    double d2 = Cross(d - c, b - c);
    return sgn(c1) * sgn(c2) < 0 && sgn(d1) * sgn(d2) < 0;
}

// -------------------- 2D: polygons --------------------

struct Polygon {
    int n = 0;
    Point p[maxp];
    Line v[maxp];
};

// Returns 3 on a vertex, 2 on an edge, 1 inside, and 0 outside.
int Point_in_polygon(Point pt, const Point *p, int n) {
    for (int i = 0; i < n; ++i) {
        if (pt == p[i]) return 3;
    }
    for (int i = 0; i < n; ++i) {
        if (Point_on_seg(pt, Segment(p[i], p[(i + 1) % n]))) return 2;
    }

    int winding = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        int c = sgn(Cross(pt - p[i], p[j] - p[i]));
        int u = sgn(p[i].y - pt.y);
        int v = sgn(p[j].y - pt.y);
        if (c > 0 && u < 0 && v >= 0) ++winding;
        if (c < 0 && u >= 0 && v < 0) --winding;
    }
    return winding != 0;
}

// Signed area; it is positive when vertices are counterclockwise.
double Polygon_area(const Point *p, int n) {
    double area2 = 0;
    for (int i = 0; i < n; ++i) {
        area2 += Cross(p[i], p[(i + 1) % n]);
    }
    return area2 / 2;
}

// The polygon must have nonzero signed area.
Point Polygon_center(const Point *p, int n) {
    Point ans;
    double cross_sum = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        double cross = Cross(p[i], p[j]);
        ans = ans + (p[i] + p[j]) * cross;
        cross_sum += cross;
    }
    if (sgn(cross_sum) == 0) return Point();
    return ans / (3 * cross_sum);
}

// Builds the strict convex hull in counterclockwise order without repeating
// the first vertex. Collinear points on hull edges are removed.
int Convex_hull(Point *p, int n, Point *ch) {
    if (n <= 0) return 0;
    sort(p, p + n);

    int unique_n = 0;
    for (int i = 0; i < n; ++i) {
        if (unique_n == 0 || !(p[i] == p[unique_n - 1])) {
            p[unique_n++] = p[i];
        }
    }
    n = unique_n;
    if (n == 1) {
        ch[0] = p[0];
        return 1;
    }

    int v = 0;
    for (int i = 0; i < n; ++i) {
        while (v >= 2 && sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 2])) <= 0) {
            --v;
        }
        ch[v++] = p[i];
    }

    int lower_size = v;
    for (int i = n - 2; i >= 0; --i) {
        while (v > lower_size &&
               sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 2])) <= 0) {
            --v;
        }
        ch[v++] = p[i];
    }
    return v - 1;
}

// -------------------- 2D: circles --------------------

struct Circle {
    Point c;
    double r;

    Circle() : c(), r(0) {}
    Circle(Point c, double r) : c(c), r(r) {}
    Circle(double x, double y, double r) : c(x, y), r(r) {}
};

// Returns 0 inside, 1 on, and 2 outside the circle.
int Point_circle_relation(Point p, Circle c) {
    return sgn(Distance(p, c.c) - c.r) + 1;
}

// Returns 0 when secant, 1 when tangent, and 2 when disjoint.
int Line_circle_relation(Line v, Circle c) {
    return sgn(Dis_point_line(c.c, v) - c.r) + 1;
}

// Compares the circle radius with the minimum distance to the segment:
// 0 for smaller distance, 1 for equal distance, and 2 for larger distance.
int Seg_circle_relation(Segment v, Circle c) {
    return sgn(Dis_point_seg(c.c, v) - c.r) + 1;
}

// Writes the intersection point(s) to pa and pb.
int Line_cross_circle(Line v, Circle c, Point &pa, Point &pb) {
    int relation = Line_circle_relation(v, c);
    if (relation == 2) return 0;

    Point q = Point_line_proj(c.c, v);
    double d = Distance(c.c, q);
    double k = sqrt(max(0.0, c.r * c.r - d * d));
    if (sgn(k) == 0) {
        pa = pb = q;
        return 1;
    }

    Vector n = (v.p2 - v.p1) / Len(v.p2 - v.p1);
    pa = q + n * k;
    pb = q - n * k;
    return 2;
}

// -------------------- 3D geometry --------------------

struct Point3 {
    double x, y, z;

    Point3() : x(0), y(0), z(0) {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}

    Point3 operator+(const Point3 &b) const { return Point3(x + b.x, y + b.y, z + b.z); }
    Point3 operator-(const Point3 &b) const { return Point3(x - b.x, y - b.y, z - b.z); }
    Point3 operator*(double k) const { return Point3(x * k, y * k, z * k); }
    Point3 operator/(double k) const { return Point3(x / k, y / k, z / k); }

    bool operator==(const Point3 &b) const {
        return sgn(x - b.x) == 0 && sgn(y - b.y) == 0 && sgn(z - b.z) == 0;
    }
};

using Vector3 = Point3;

double Dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Cross(Vector3 a, Vector3 b) {
    return Vector3(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

double Len2(Vector3 a) {
    return Dot(a, a);
}

double Len(Vector3 a) {
    return sqrt(Len2(a));
}

double Distance(Point3 a, Point3 b) {
    return Len(a - b);
}

double Angle(Vector3 a, Vector3 b) {
    return acos(clamp_unit(Dot(a, b) / Len(a) / Len(b)));
}

struct Line3 {
    Point3 p1, p2;

    Line3() = default;
    Line3(Point3 p1, Point3 p2) : p1(p1), p2(p2) {}
};

using Segment3 = Line3;

// Twice the area of triangle ABC.
double Area2(Point3 a, Point3 b, Point3 c) {
    return Len(Cross(b - a, c - a));
}

double Dis_point_line(Point3 p, Line3 v) {
    return Len(Cross(v.p2 - v.p1, p - v.p1)) / Len(v.p2 - v.p1);
}

bool Point_line(Point3 p, Line3 v) {
    return sgn(Len(Cross(v.p1 - p, v.p2 - p))) == 0;
}

bool Point_on_seg(Point3 p, Segment3 v) {
    return Point_line(p, v) && sgn(Dot(v.p1 - p, v.p2 - p)) <= 0;
}

double Dis_point_seg(Point3 p, Segment3 v) {
    if (v.p1 == v.p2) return Distance(p, v.p1);
    if (sgn(Dot(p - v.p1, v.p2 - v.p1)) < 0 ||
        sgn(Dot(p - v.p2, v.p1 - v.p2)) < 0) {
        return min(Distance(p, v.p1), Distance(p, v.p2));
    }
    return Dis_point_line(p, v);
}

Point3 Point_line_proj(Point3 p, Line3 v) {
    Vector3 direction = v.p2 - v.p1;
    double k = Dot(direction, p - v.p1) / Len2(direction);
    return v.p1 + direction * k;
}

struct Plane {
    Point3 p1, p2, p3;

    Plane() = default;
    Plane(Point3 p1, Point3 p2, Point3 p3) : p1(p1), p2(p2), p3(p3) {}
};

Vector3 Pvec(Point3 a, Point3 b, Point3 c) {
    return Cross(b - a, c - a);
}

Vector3 Pvec(Plane f) {
    return Pvec(f.p1, f.p2, f.p3);
}

bool Point_on_plane(Point3 a, Point3 b, Point3 c, Point3 d) {
    return sgn(Dot(Pvec(a, b, c), d - a)) == 0;
}

bool Point_on_plane(Point3 p, Plane f) {
    return sgn(Dot(Pvec(f), p - f.p1)) == 0;
}

bool Parallel(Plane f1, Plane f2) {
    return sgn(Len(Cross(Pvec(f1), Pvec(f2)))) == 0;
}

bool Vertical(Plane f1, Plane f2) {
    return sgn(Dot(Pvec(f1), Pvec(f2))) == 0;
}

// Returns -1 if the line lies in the plane, 0 if parallel, and 1 if they
// intersect at one point. Degenerate lines and planes are not supported.
int Line_cross_plane(Line3 u, Plane f, Point3 &p) {
    Vector3 normal = Pvec(f);
    double x = Dot(normal, u.p2 - f.p1);
    double y = Dot(normal, u.p1 - f.p1);
    double d = x - y;

    if (sgn(x) == 0 && sgn(y) == 0) return -1;
    if (sgn(d) == 0) return 0;
    p = (u.p1 * x - u.p2 * y) / d;
    return 1;
}

// Six times the signed volume of tetrahedron ABCD.
double volume4(Point3 a, Point3 b, Point3 c, Point3 d) {
    return Dot(Cross(b - a, c - a), d - a);
}

int main() {
    return 0;
}
