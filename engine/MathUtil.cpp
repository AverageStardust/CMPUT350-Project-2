#include "MathUtil.h"

#include <algorithm>

#include "SFML/Graphics/Rect.hpp"

namespace ECSEngine {

Point2D Point2D::operator+(const Point2D& other) const { return Point2D(x + other.x, y + other.y); }
Point2D Point2D::operator+(const float& scalar) const { return Point2D(x + scalar, y + scalar); }
Point2D Point2D::operator-(const Point2D& other) const { return Point2D(x - other.x, y - other.y); }
Point2D Point2D::operator-(const float& scalar) const { return Point2D(x - scalar, y - scalar); }
Point2D Point2D::operator*(const float& scalar) const { return Point2D(x * scalar, y * scalar); }
Point2D Point2D::operator/(const float& scalar) const { return Point2D(x / scalar, y / scalar); }
float Point2D::operator*(const Point2D& other) const { return x * other.x + y * other.y; }
bool Point2D::operator==(const Point2D& other) const { return x == other.x && y == other.y; }
bool Point2D::operator<(const Point2D& other) const { return x < other.x && y < other.y; }
bool Point2D::operator>(const Point2D& other) const { return x > other.x && y > other.y; }
bool Point2D::operator<=(const Point2D& other) const { return x <= other.x && y <= other.y; }
bool Point2D::operator>=(const Point2D& other) const { return x >= other.x && y >= other.y; }

Point2D& Point2D::operator+=(const Point2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Point2D& Point2D::operator+=(const float& scalar) {
    x += scalar;
    y += scalar;
    return *this;
}
Point2D& Point2D::operator-=(const Point2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Point2D& Point2D::operator-=(const float& scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
}
Point2D& Point2D::operator*=(const float& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
Point2D& Point2D::operator/=(const float& scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Point2D Point2D::Min(Point2D other) const {
    return Point2D(std::min(x, other.x), std::min(y, other.y));
}
Point2D Point2D::Max(Point2D other) const {
    return Point2D(std::max(x, other.x), std::max(y, other.y));
}
Point2D Point2D::Abs() const { return Point2D(std::abs(x), std::abs(y)); }
Point2D Point2D::Sign() const { return Point2D((x > 0) ? 1 : -1, (y > 0) ? 1 : -1); }
Point2D Point2D::PairwiseMult(Point2D other) const { return {x * other.x, y * other.y}; }
Point2D Point2D::PairwiseDiv(Point2D other) const { return {x / other.x, y / other.y}; }
float Point2D::Dot(Point2D other) const { return x * other.x + y * other.y; }
float Point2D::Cross(Point2D other) const { return x * other.y - y * other.x; }
float Point2D::Distance(const Point2D& other) const {
    return std::hypotf(x - other.x, y - other.y);
}
float Point2D::DistanceSq(const Point2D& other) const {
    float distX = x - other.x;
    float distY = y - other.y;
    return distX * distX + distY * distY;
}
float Point2D::Angle() const { return std::atan2(y, x); }
void Point2D::Normalize() { *this /= this->Length(); }
Point2D Point2D::Normalized() const { return *this / this->Length(); }
void Point2D::Rotate(float thetaRadians) {
    float sinTheta = sinf(thetaRadians);
    float cosTheta = cosf(thetaRadians);
    float tempX = x;
    x = x * cosTheta - y * sinTheta;
    y = tempX * sinTheta + y * cosTheta;
}
Point2D Point2D::Rotated(float thetaRadians) const {
    float sinTheta = sinf(thetaRadians);
    float cosTheta = cosf(thetaRadians);
    return Point2D(x * cosTheta - y * sinTheta, x * sinTheta + y * cosTheta);
}
Point2D Point2D::Perpendicular() const { return Point2D(-y, x); }
float Point2D::Length() const { return hypot(x, y); }
float Point2D::LengthSq() const { return x * x + y * y; }
Point2D Point2D::PairwiseMult(Point2D a, Point2D b) { return {a.x * b.x, a.y * b.y}; }
Point2D Point2D::PairwiseDiv(Point2D a, Point2D b) { return {a.x / b.x, a.y / b.y}; }
float Point2D::Dot(Point2D a, Point2D b) { return a.x * b.x + a.y * b.y; }
float Point2D::Cross(Point2D a, Point2D b) { return a.x * b.y - a.y * b.x; }

sf::Vector2f Point2D::AsSfmlVectorF() const { return sf::Vector2f(x, y); }
sf::Vector2i Point2D::AsSfmlVectorI() const { return sf::Vector2i(x, y); }

std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
Point2D operator*(float scalar, const Point2D& point) {
    return Point2D(scalar * point.x, scalar * point.y);
}

float Line::Length() const { return p1.Distance(p2); }
Point2D Line::ClosestPoint(const Point2D& p) const {
    Point2D diff = p2 - p1;

    float t = (p - p1).Dot(diff) / diff.LengthSq();
    t = std::clamp(t, 0.0f, 1.0f);  // clamp

    return p1 + diff * t;
}
Point2D Line::Lerp(float t) const { return (p1 + (p2 - p1) * t); }
bool lineInterectT(const Line& a, const Line& b, float& t) {
    float determinant = (a.p2 - a.p1).Cross(b.p2 - b.p1);
    if (determinant == 0) return false;

    t = (b.p1 - a.p1).Cross(b.p2 - b.p1) / determinant;
    if (t < 0 || t > 1) return false;

    float u = (a.p1 - b.p1).Cross(a.p2 - a.p1) / -determinant;
    return !(u < 0 || u > 1);
}
bool Line::Intersects(const Line& other, Point2D& point) const {
    float t;
    bool intersects = lineInterectT(*this, other, t);

    if (intersects) point = Lerp(t);

    return intersects;
}
bool Line::Intersects(const Circle& other, Point2D& point) const {
    Point2D deepestPoint = ClosestPoint(other.center);
    float depthSquared = other.radius * other.radius - deepestPoint.DistanceSq(point);

    if (depthSquared < 0) return false;

    // find entry point from p1
    point = deepestPoint + std::sqrt(depthSquared) * (p1 - p2).Normalized();
    return true;
}
bool Line::Intersects(const Rect& other, Point2D& point) const {
    return Intersects(Quad(other), point);
}
bool Line::Intersects(const Quad& other, Point2D& point) const {
    float minT = 2;
    float t;

    if (lineInterectT(*this, other.Segment(0), t)) minT = std::min(minT, t);
    if (lineInterectT(*this, other.Segment(1), t)) minT = std::min(minT, t);
    if (lineInterectT(*this, other.Segment(2), t)) minT = std::min(minT, t);
    if (lineInterectT(*this, other.Segment(3), t)) minT = std::min(minT, t);

    bool intersects = minT < 2;

    if (intersects) point = Lerp(minT);

    return intersects;
}

std::ostream& operator<<(std::ostream& os, const Line& l) {
    os << "(" << l.p1 << ", " << l.p2 << ")";
    return os;
}

bool Circle::Intersects(const Line& other, Point2D& point) const {
    return other.Intersects(*this, point);
}
bool Circle::Intersects(const Circle& other, Point2D& point) const {
    if (center.Distance(other.center) > radius + other.radius) return false;

    point = (center + other.center) / 2;
    return true;
}
bool Circle::Intersects(const Rect& other, Point2D& point) const {
    Point2D centerDist = (center - other.Center()).Abs();
    Point2D edgeDist = centerDist - other.Size() / 2;

    bool intersects = edgeDist.LengthSq() <= radius * radius;

    if (intersects) {
        point = other.Center() + edgeDist * (center - other.Center()).Sign();
    }

    return intersects;
}
float lineInterectDepthSquared(const Circle& a, const Line& b, Point2D& point) {
    Point2D deepestPoint = b.ClosestPoint(a.center);
    float depthSquared = a.radius * a.radius - deepestPoint.DistanceSq(point);

    if (depthSquared < 0) return 0;

    // find entry point from p1
    point = deepestPoint;
    return depthSquared;
}
bool Circle::Intersects(const Quad& other, Point2D& point) const {
    float depth;
    float maxDepth = 0;
    Point2D candidatePoint;

    depth = lineInterectDepthSquared(*this, other.Segment(0), candidatePoint);
    if (depth > maxDepth) {
        point = candidatePoint;
        maxDepth = depth;
    }
    depth = lineInterectDepthSquared(*this, other.Segment(1), candidatePoint);
    if (depth > maxDepth) {
        point = candidatePoint;
        maxDepth = depth;
    }
    depth = lineInterectDepthSquared(*this, other.Segment(2), candidatePoint);
    if (depth > maxDepth) {
        point = candidatePoint;
        maxDepth = depth;
    }
    depth = lineInterectDepthSquared(*this, other.Segment(3), candidatePoint);
    if (depth > maxDepth) {
        point = candidatePoint;
        maxDepth = depth;
    }

    return depth > 0;
}

std::ostream& operator<<(std::ostream& os, const Circle& l) {
    os << "(" << l.center << ", " << l.radius << ")";
    return os;
}

Rect Rect::operator|(const Point2D& other) const {
    if (Exists()) {
        Rect result;
        result.topLeft = topLeft.Min(other);
        result.bottomRight = bottomRight.Max(other);
        return result;
    } else {
        return Rect(other);
    }
}
Rect Rect::operator|(const Line& other) const { return *this | other.p1 | other.p2; }
Rect Rect::operator|(const Rect& other) const {
    if (!other.Exists()) return *this;

    return *this | other.topLeft | other.bottomRight;
}
Rect Rect::operator|(const Quad& other) const {
    return *this | other.points[0] | other.points[1] | other.points[2] | other.points[3];
}
Rect Rect::operator&(const Rect& other) const {
    if (Exists() && other.Exists()) {
        Rect result;
        result.topLeft = topLeft.Max(other.topLeft);
        result.bottomRight = bottomRight.Min(other.bottomRight);
        return result;
    } else {
        return {};
    }
}

Rect Rect::operator+(const Point2D& other) const {
    if (Exists()) {
        Rect result;
        result.topLeft = topLeft + other;
        result.bottomRight = bottomRight + other;
        return result;
    } else {
        return *this;
    }
}

Rect Rect::operator-(const Point2D& other) const {
    if (Exists()) {
        Rect result;
        result.topLeft = topLeft - other;
        result.bottomRight = bottomRight - other;
        return result;
    } else {
        return *this;
    }
}

Rect& Rect::operator|=(const Point2D& other) {
    if (Exists()) {
        topLeft = topLeft.Min(other);
        bottomRight = bottomRight.Max(other);
    } else {
        topLeft = other;
        bottomRight = other;
    }

    return *this;
}
Rect& Rect::operator|=(const Line& other) {
    *this |= other.p1;
    *this |= other.p2;
    return *this;
}
Rect& Rect::operator|=(const Rect& other) {
    if (other.Exists()) {
        *this |= other.topLeft;
        *this |= other.bottomRight;
    }

    return *this;
}
Rect& Rect::operator|=(const Quad& other) {
    *this |= other.points[0];
    *this |= other.points[1];
    *this |= other.points[2];
    *this |= other.points[3];
    return *this;
}
Rect& Rect::operator&=(const Rect& other) {
    if (!Exists()) return *this;
    if (!other.Exists()) {
        return *this = {};
    }

    topLeft = topLeft.Max(other.topLeft);
    bottomRight = bottomRight.Min(other.bottomRight);

    return *this;
}

Rect& Rect::operator+=(const Point2D& other) {
    if (Exists()) {
        topLeft += other;
        bottomRight += other;
    }

    return *this;
}

Rect& Rect::operator-=(const Point2D& other) {
    if (Exists()) {
        topLeft -= other;
        bottomRight -= other;
    }

    return *this;
}

void Rect::Inset(int inset) {
    topLeft += inset;
    bottomRight -= inset;
}
Rect Rect::Inseted(int inset) const {
    Rect result;
    result.topLeft = topLeft + inset;
    result.bottomRight = topLeft - inset;
    return result;
}
bool Rect::IsInside(const Point2D& p) const {
    return p.x >= topLeft.x && p.x <= bottomRight.x && p.y >= topLeft.y && p.y <= bottomRight.y;
}
bool Rect::IsOverlapping(const Rect& rect) const { return (*this & rect).Exists(); }
float Rect::Width() const {
    if (!Exists()) return 0;
    return bottomRight.x - topLeft.x;
}
float Rect::Height() const {
    if (!Exists()) return 0;
    return bottomRight.y - topLeft.y;
}
Point2D Rect::Size() const {
    if (!Exists()) return {};
    return bottomRight - topLeft;
}
Point2D Rect::Center() const {
    if (!Exists()) return {};
    return (bottomRight + topLeft) / 2.0;
}
bool Rect::Exists() const { return bottomRight >= topLeft; }
bool Rect::Intersects(const Line& other, Point2D& point) const {
    return other.Intersects(*this, point);
}
bool Rect::Intersects(const Circle& other, Point2D& point) const {
    return other.Intersects(*this, point);
}
bool Rect::Intersects(const Rect& other, Point2D& point) const {
    Rect intersection = *this & other;
    bool intersects = intersection.Exists();

    if (intersects) point = intersection.Center();

    return intersects;
}
bool Rect::Intersects(const Quad& other, Point2D& point) const {
    return Quad(*this).Intersects(other, point);
}

sf::FloatRect Rect::AsSfmlRectF() const {
    return sf::FloatRect(topLeft.AsSfmlVectorF(), Size().AsSfmlVectorF());
}
sf::IntRect Rect::AsSfmlRectI() const {
    return sf::IntRect(topLeft.AsSfmlVectorI(), Size().AsSfmlVectorI());
}

std::ostream& operator<<(std::ostream& os, const Rect& r) {
    if (r.Exists()) {
        os << "(" << r.topLeft << ", " << r.bottomRight << ")";
    } else {
        os << "(Null Rect)";
    }
    return os;
}

Line Quad::Segment(int index) const { return Line(points[index], points[(index + 1) % 4]); }
bool Quad::Intersects(const Line& other, Point2D& point) const {
    return other.Intersects(*this, point);
}
bool Quad::Intersects(const Circle& other, Point2D& point) const {
    return other.Intersects(*this, point);
}
bool Quad::Intersects(const Rect& other, Point2D& point) const {
    return Quad(other).Intersects(*this, point);
}
bool Quad::Intersects(const Quad& other, Point2D& point) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Segment(i).Intersects(other.Segment(j), point)) return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Quad& q) {
    os << "(" << q.points[0] << ", " << q.points[1] << ", " << q.points[2] << ", " << q.points[3]
       << ")";
    return os;
}

Matrix2::Matrix2(float (&&basis)[4]) { std::copy(basis, basis + 4, this->cells); }

Matrix2 Matrix2::operator*(const float& scalar) const {
    return Matrix2(cells[0] * scalar, cells[1] * scalar, cells[2] * scalar, cells[3] * scalar);
}
Point2D Matrix2::operator*(const Point2D& vector) const {
    return Point2D(cells[0] * vector.x + cells[1] * vector.y,
                   cells[2] * vector.x + cells[3] * vector.y);
}
Matrix2 Matrix2::operator*(const Matrix2& other) const {
    return Matrix2(cells[0] * other.cells[0] + cells[1] * other.cells[2],
                   cells[0] * other.cells[1] + cells[1] * other.cells[3],
                   cells[2] * other.cells[0] + cells[3] * other.cells[2],
                   cells[2] * other.cells[1] + cells[3] * other.cells[3]);
}

float Matrix2::Determinant() const { return cells[0] * cells[3] - cells[1] * cells[2]; }
Matrix2 Matrix2::Inverse() const {
    float determinant = Determinant();
    assert(determinant != 0);
    return Matrix2(cells[3], -cells[1], -cells[2], cells[0]) * (1.0f / determinant);
}

Point2D operator*(const Point2D& vector, const Matrix2& transform) {
    return transform.Inverse() * vector;
}
std::ostream& operator<<(std::ostream& os, const Matrix2& m) {
    os << "(" << m.cells[0] << ", " << m.cells[1] << ", " << m.cells[2] << ", " << m.cells[3]
       << ")";
    return os;
}

Transform2D Transform2D::Rotation(float angle) {
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);
    return Transform2D(Matrix2(cosAngle, -sinAngle, sinAngle, cosAngle), {});
}
Transform2D Transform2D::Translation(Point2D offset) { return Transform2D({}, offset); }
Transform2D Transform2D::Scale(float factor) { return Transform2D({factor, 0.0, 0.0, factor}, {}); }

Point2D Transform2D::operator*(const Point2D& vector) const { return basis * vector + origin; }
Line Transform2D::operator*(const Line& line) const {
    return Line(*this * line.p1, *this * line.p2);
}
Circle Transform2D::operator*(const Circle& circle) const {
    return Circle(circle.center, circle.radius * Scale());
}
Quad Transform2D::operator*(const Rect& rect) const { return *this * Quad(rect); }
Quad Transform2D::operator*(const Quad& quad) const {
    return Quad(*this * quad.points[0], *this * quad.points[1], *this * quad.points[2],
                *this * quad.points[3]);
}
Transform2D Transform2D::operator*(const Transform2D& other) const {
    return Transform2D(basis * other.basis, basis * other.origin + origin);
}

Transform2D Transform2D::Inverse() const { return Transform2D(basis.Inverse(), origin * -1); }
float Transform2D::Scale() const { return std::hypotf(basis.cells[0], basis.cells[2]); }
Point2D Transform2D::Translation() const { return origin; }
float Transform2D::Rotation() const { return std::atan2(basis.cells[2], basis.cells[0]); }

Point2D& operator*=(Point2D& vector, const Transform2D& transform) {
    return vector = transform * vector;
}
Line& operator*=(Line& line, const Transform2D& transform) { return line = transform * line; }
Circle& operator*=(Circle& circle, const Transform2D& transform) {
    return circle = transform * circle;
}
Quad& operator*=(Quad& quad, const Transform2D& transform) { return quad = transform * quad; }
Transform2D& operator*=(Transform2D& other, const Transform2D& transform) {
    return other = transform * other;
}
Point2D operator*(const Point2D& vector, const Transform2D& transform) {
    return transform.Inverse() * vector;
}
std::ostream& operator<<(std::ostream& os, const Transform2D& t) {
    os << "(" << t.basis << ", " << t.origin << ")";
    return os;
}

}  // namespace ECSEngine
