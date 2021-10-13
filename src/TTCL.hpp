#include <vector>
#include <cmath>
#include <iostream>

class Point {
private:
    double x;
    double y;
public:
    inline Point(double x, double y) : x(x),y(y) {};
    inline Point operator-(const Point &rhs) const { return {this->x-rhs.x, this->y-rhs.y};};
    inline double operator*(const Point &rhs) const { return this->x*rhs.x + this->y*rhs.y; }; // Dot Product
    inline double square_norm() const {return *this*(*this);};

    inline double get_x() const {return this->x;}
    inline double get_y() const {return this->y;}

    inline bool is_zero() const {
        return this->x==0.0 && this->y == 0.0; // Needs an epsilon environment
    }
};

class Geometry;
class Circle;
class Polygon;

inline bool ll_collision(const Point &a_s, const Point &a_e, const Point &b_s, const Point &b_e);
inline bool lc_collision(const Circle &rhs, const Point &s, const Point &e);
inline bool collision_dispatch(const Geometry *lhs, const Geometry *rhs);

class Geometry {
public:
    inline bool collides(const Geometry &rhs) {
        return collision_dispatch(this, &rhs);
    };

    virtual inline bool collides(const Polygon & p) const = 0;
    virtual inline bool collides(const Circle & c) const = 0;
};

class Polygon : public Geometry {
private:
    std::vector<Point> points;
    bool closed;
public:
    Polygon() = delete;
    explicit inline Polygon(std::vector<Point> points, bool closed = false) : closed(closed), points(std::move(points)) {
        if (this->points.size() < 2) {
            throw std::logic_error("A Polygon needs at least two points");
        }
    };

    inline bool collides(const Polygon &rhs) const override {
        for (int i = 0; i < this->points.size(); i++) {
            int in = i + 1;
            if (in == this->points.size()) {
                if (this->closed) {
                  in = 0;
                } else {
                    continue;
                }
            }
            for (int j = 0; j < rhs.points.size(); j++) {
                int jn = j + 1;
                if (jn == rhs.points.size()) {
                    if (rhs.closed) {
                        jn = 0;
                    } else {
                        continue;
                    }
                }

                if (ll_collision(
                        this->points[i],
                        this->points[in],
                        rhs.points[j],
                        rhs.points[jn])) {
                    return true;
                }
            }
        }
        return false;
    };

    inline bool collides(const Circle &rhs) const override {
        for (int i = 0; i < this->points.size(); i++) {
            int in = i + 1;
            if (in == this->points.size()) {
                if (this->closed) {
                    in = 0;
                } else {
                    continue;
                }
            }
            if( lc_collision(rhs, this->points[i], this->points[in])) {
                return true;
            }
        }
        return false;
    };
};

class Circle : public Geometry{
private:
    Point center;
    double radius_squared;
public:
    inline Circle(const Point &center, double radius) : center(center), radius_squared(radius*radius) {};

    inline bool collides(const Polygon &rhs) const override {
        return rhs.collides(*this);
    }

    inline bool collides(const Circle &rhs) const override {
        return (this->center - rhs.center).square_norm() < (this->radius_squared + rhs.radius_squared + 2 * sqrt(this->radius_squared* rhs.get_radius_squared()));
    }

    inline const Point & get_center() const {
        return this->center;
    }

    inline double get_radius_squared() const {
        return this->radius_squared;
    }
};

inline bool collision_dispatch(const Geometry *lhs, const Geometry *rhs) {
    if (auto p = dynamic_cast<const Polygon *>(rhs)) {
        return lhs->collides(*p);
    } else if (auto c = dynamic_cast<const Circle *>(rhs)) {
        return lhs->collides(*c);
    } else {
        throw std::logic_error("not implemented");
    }
}

inline bool ll_collision(const Point &a_s, const Point &a_e, const Point &b_s, const Point &b_e) {
    Point a_d = a_e - a_s;
    Point b_d = b_e - b_s;
    Point s   = b_s - a_s;

    if ( a_d.is_zero() || b_d.is_zero() ) {
        throw std::logic_error("Lines need two different Points");
    }

    double det = a_d.get_y() * b_d.get_x() - a_d.get_x() * b_d.get_y();
    if (det == 0) { // TODO epsilon
        return false;
    }

    double alpha =  Point(-b_d.get_y(), b_d.get_x()) * s / det;
    if (alpha < 0.0 || alpha > 1.0) {
        return false;
    }

    double beta =  Point(-a_d.get_y(), a_d.get_x()) * s / det;
    if (beta < 0.0 || beta > 1.0) {
        return false;
    }

    return true;
}

// tbh: i also calculated it myself, but was to lazy to clean it up.
// I just took this one:
// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
// I would mostly do the same
inline bool lc_collision(const Circle &rhs, const Point &s, const Point &e) {
    Point d = e-s;
    Point cl = s - rhs.get_center();

    double a = d * d;
    double b = 2.0 * (d * cl);
    double c = cl * cl - rhs.get_radius_squared();

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    } else {
        discriminant = sqrt( discriminant ); // probably enhancable by squaring
    }

    double t1 = (-b - discriminant)/(2*a);
    double t2 = (-b + discriminant)/(2*a);

    if( t1 >= 0 && t1 <= 1 )
    {
        return true;
    }

    if( t2 >= 0 && t2 <= 1 )
    {
        return true ;
    }

    return false ;
}