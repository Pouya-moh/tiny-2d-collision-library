#include "TTCL.hpp"
#include <gtest/gtest.h>

TEST(Geometry, Circles) {
    Circle a( Point(0,0), 1);
    Circle b( Point(2,0), 1.5);
    Circle c( Point(0,2.5), 1);

    EXPECT_TRUE(a.collides(b));
    EXPECT_FALSE(b.collides(c));
    EXPECT_FALSE(a.collides(c));
}

TEST(Geometry, Lines) {
    Polygon a({
        Point(0,0),
        Point(0,2),
        Point(3,2)
    });

    Polygon b({
        Point(1,0),
        Point(1,3),
        Point(4,3)
    });

    Polygon c({
        Point(2,2.5),
        Point(2,1),
        Point(4,1)
    });

    Polygon d({
        Point(0.5,0.5),
        Point(3.5,0.5),
        Point(3.5,3.5)
    });

    EXPECT_TRUE(a.collides(b));
    EXPECT_TRUE(a.collides(c));
    EXPECT_FALSE(a.collides(d));
    EXPECT_FALSE(b.collides(c));
    EXPECT_TRUE(b.collides(d));
    EXPECT_TRUE(c.collides(d));
}

TEST(Geometry, Mix) {
    Circle c1(Point(0,0), 1);
    Circle c2(Point(0,3), 1);

    Polygon l1({Point(-2,0.5), Point(2,0.5)});
    Polygon l2({Point(-2,2), Point(2,2)});

    EXPECT_TRUE(c1.collides(l1));
    EXPECT_FALSE(c1.collides(l2));
    EXPECT_TRUE(c2.collides(l2));
    EXPECT_FALSE(c2.collides(l1));
}

TEST(Geometry, Loops_Lines) {
    std::vector<Point> a = {Point(0,0), Point(0,1), Point(1,1), Point(1,0)};
    std::vector<Point> b = {Point(0.5,0.5), Point(0.5,-0.5), Point(1.5,-0.5), Point(1.5,0.5)};

    Polygon a_o(a);
    Polygon a_c(a,true);
    Polygon b_o(b);
    Polygon b_c(b,true);

    EXPECT_FALSE(a_o.collides(b_o));
    EXPECT_TRUE(a_o.collides(b_c));
    EXPECT_TRUE(a_c.collides(b_o));
    EXPECT_TRUE(a_c.collides(b_c));
}

TEST(Geometry, Loops_Circle) {
    std::vector<Point> a = {Point(-2,0), Point(-1,2), Point(1,2), Point(2,0)};

    Polygon a_o(a);
    Polygon a_c(a,true);

    Circle c(Point(0,0), 1);

    EXPECT_FALSE(a_o.collides(c));
    EXPECT_TRUE(a_c.collides(c));
    EXPECT_FALSE(c.collides(a_o));
    EXPECT_TRUE(c.collides(a_c));
}

TEST(Polymorphism, Mix) {
    Circle c1_o(Point(0,0), 1);
    Circle c2_o(Point(0,3), 1);

    Polygon l1_o({Point(-2,0.5), Point(2,0.5)});
    Polygon l2_o({Point(-2,2), Point(2,2)});

    Geometry & c1 = c1_o;
    Geometry & c2 = c2_o;
    Geometry & l1 = l1_o;
    Geometry & l2 = l2_o;

    EXPECT_TRUE(c1.collides(l1));
    EXPECT_TRUE(l1.collides(c1));
    EXPECT_FALSE(c1.collides(l2));
    EXPECT_FALSE(l2.collides(c1));
    EXPECT_TRUE(c2.collides(l2));
    EXPECT_TRUE(l2.collides(c2));
    EXPECT_FALSE(c2.collides(l1));
    EXPECT_FALSE(l1.collides(c2));
}