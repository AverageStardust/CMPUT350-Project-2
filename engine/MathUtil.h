#pragma once

#include <iostream>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace ECSEngine {

struct Circle;
struct Rect;
struct Quad;

struct Point2D {
    float x, y;

    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    Point2D(sf::Vector2f vec): x(vec.x), y(vec.y) {}
    Point2D(sf::Vector2i vec): x(vec.x), y(vec.y) {}

    Point2D operator+(const Point2D& other) const;
    Point2D operator+(const float& scalar) const;
    Point2D operator-(const Point2D& other) const;
    Point2D operator-(const float& scalar) const;
    Point2D operator*(const float& scalar) const;
    Point2D operator/(const float& scalar) const;
    float operator*(const Point2D& other) const;
    bool operator==(const Point2D& other) const;
    bool operator<(const Point2D& other) const;
    bool operator>(const Point2D& other) const;
    bool operator<=(const Point2D& other) const;
    bool operator>=(const Point2D& other) const;

    Point2D& operator+=(const Point2D& other);
    Point2D& operator+=(const float& scalar);
    Point2D& operator-=(const Point2D& other);
    Point2D& operator-=(const float& scalar);
    Point2D& operator*=(const float& scalar);
    Point2D& operator/=(const float& scalar);

    Point2D Min(Point2D other) const;
    Point2D Max(Point2D other) const;
    Point2D Abs() const;
    Point2D Round() const;
    Point2D Sign() const;
    Point2D Lerp(Point2D other, float fraction) const;
    Point2D MoveTowards(Point2D other, float distance) const;
    Point2D PairwiseMult(Point2D other) const;
    Point2D PairwiseDiv(Point2D other) const;
    float Dot(Point2D other) const;
    float Cross(Point2D other) const;
    float Distance(const Point2D& other) const;
    float DistanceSq(const Point2D& other) const;
    float Angle() const;
    void Normalize();
    Point2D Normalized() const;
    void Rotate(float thetaRadians);
    Point2D Rotated(float thetaRadians) const;
    Point2D Perpendicular() const;
    float Length() const;
    float LengthSq() const;
    static Point2D PairwiseMult(Point2D a, Point2D b);
    static Point2D PairwiseDiv(Point2D a, Point2D b);
    static float Dot(Point2D a, Point2D b);
    static float Cross(Point2D a, Point2D b);

    sf::Vector2f AsSfmlVectorF() const;
    sf::Vector2i AsSfmlVectorI() const;
};

std::ostream& operator<<(std::ostream& os, const Point2D& p);
Point2D operator*(float scalar, const Point2D& point);

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}

    float Length() const;
    Point2D Lerp(float t) const;
    Point2D ClosestPoint(const Point2D& p) const;
    bool Intersects(const Line& other, Point2D& point) const;
    bool Intersects(const Circle& other, Point2D& point) const;
    bool Intersects(const Rect& other, Point2D& point) const;
    bool Intersects(const Quad& other, Point2D& point) const;
};

std::ostream& operator<<(std::ostream& os, const Line& l);

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}
    Circle(float x, float y, float r) : center(x, y), radius(r) {}

    bool Intersects(const Line& other, Point2D& point) const;
    bool Intersects(const Circle& other, Point2D& point) const;
    bool Intersects(const Rect& other, Point2D& point) const;
    bool Intersects(const Quad& other, Point2D& point) const;
};

std::ostream& operator<<(std::ostream& os, const Circle& l);

struct Rect {
    Point2D topLeft;
    Point2D bottomRight;

    Rect() : topLeft(Point2D{0, 0}), bottomRight(Point2D{-1, -1}) {}
    Rect(float left, float top, float width = 0, float height = 0)
        : topLeft(Point2D(left, top)), bottomRight(Point2D(left + width, top + height)) {}
    Rect(Point2D tl, int w = 0, int h = 0) : topLeft(tl), bottomRight(tl + Point2D(w, h)) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2) : topLeft(p1.Min(p2)), bottomRight(p1.Max(p2)) {}
    Rect(Point2D center, float radius)
        : topLeft(center.x - radius), bottomRight(center.x + radius) {}

    Rect(Circle circle)
        : topLeft(circle.center - circle.radius), bottomRight(circle.center + circle.radius) {}
    Rect(Line line) : topLeft(line.p1.Min(line.p2)), bottomRight(line.p1.Max(line.p2)) {}

    Rect operator|(const Point2D& other) const;
    Rect operator|(const Line& other) const;
    Rect operator|(const Rect& other) const;
    Rect operator|(const Quad& other) const;
    Rect operator&(const Rect& other) const;
    Rect operator+(const Point2D& other) const;
    Rect operator-(const Point2D& other) const;

    Rect& operator|=(const Point2D& other);
    Rect& operator|=(const Line& other);
    Rect& operator|=(const Rect& other);
    Rect& operator|=(const Quad& other);
    Rect& operator&=(const Rect& other);

    Rect& operator+=(const Point2D& other);
    Rect& operator-=(const Point2D& other);

    void Inset(int inset);
    Rect Inseted(int inset) const;
    bool IsInside(const Point2D& p) const;
    Point2D NearestInside(const Point2D& p) const;
    bool IsOverlapping(const Rect& rect) const;
    float Width() const;
    float Height() const;
    Point2D Size() const;
    float Area() const;
    Point2D Center() const;
    bool Exists() const;
    bool Intersects(const Line& other, Point2D& point) const;
    bool Intersects(const Circle& other, Point2D& point) const;
    bool Intersects(const Rect& other, Point2D& point) const;
    bool Intersects(const Quad& other, Point2D& point) const;

    sf::FloatRect AsSfmlRectF() const;
    sf::IntRect AsSfmlRectI() const;
};

std::ostream& operator<<(std::ostream& os, const Rect& r);

struct Quad {
    Point2D points[4];

    Quad(Point2D p1, Point2D p2, Point2D p3, Point2D p4) : points{p1, p2, p3, p4} {}
    Quad(Rect rect)
        : points{rect.topLeft,
                 {rect.topLeft.x, rect.bottomRight.y},
                 rect.bottomRight,
                 {rect.bottomRight.x, rect.topLeft.y}} {}

    Line Segment(int index) const;
    bool Intersects(const Line& other, Point2D& point) const;
    bool Intersects(const Circle& other, Point2D& point) const;
    bool Intersects(const Rect& other, Point2D& point) const;
    bool Intersects(const Quad& other, Point2D& point) const;
};

std::ostream& operator<<(std::ostream& os, const Quad& q);

// 2x2 matrix
struct Matrix2 {
    float cells[4];  // row-major

    Matrix2() : cells{1.0, 0.0, 0.0, 1.0} {}
    Matrix2(float a, float b, float c, float d) : cells{a, b, c, d} {}
    Matrix2(float (&&basis)[4]);

    Matrix2 operator*(const float& scalar) const;
    Point2D operator*(const Point2D& vector) const;
    Matrix2 operator*(const Matrix2& other) const;

    float Determinant() const;
    Matrix2 Inverse() const;
};

Point2D operator*(const Point2D& vector, const Matrix2& transform);
std::ostream& operator<<(std::ostream& os, const Matrix2& m);

// Augmented 2x2 matrix representing 2d transformations
struct Transform2D {
    Matrix2 basis;
    Point2D origin;

    Transform2D(Matrix2 basis = {}, Point2D origin = {}) : basis{basis}, origin{origin} {}

    static Transform2D Rotation(float angle);
    static Transform2D Translation(Point2D offset);
    static Transform2D Scale(float factor);

    Point2D operator*(const Point2D& vector) const;
    Line operator*(const Line& line) const;
    Circle operator*(const Circle& circle) const;
    Quad operator*(const Rect& rect) const;
    Quad operator*(const Quad& rect) const;
    Transform2D operator*(const Transform2D& other) const;

    Transform2D Inverse() const;
    float Scale() const;
    Point2D Translation() const;
    float Rotation() const;
};

Point2D operator*(const Point2D& vector, const Transform2D& transform);
Point2D& operator*=(Point2D& vector, const Transform2D& transform);
Line& operator*=(Line& line, const Transform2D& transform);
Circle& operator*=(Circle& circle, const Transform2D& transform);
Quad& operator*=(Quad& quad, const Transform2D& transform);
Transform2D& operator*=(Transform2D& other, const Transform2D& transform);
std::ostream& operator<<(std::ostream& os, const Transform2D& t);

}  // namespace ECSEngine
