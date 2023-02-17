#ifndef PREVIEW_GEOMETRY_HPP
#define PREVIEW_GEOMETRY_HPP

#include "infra/util/FixedPoint.hpp"
#include <cstdint>
#ifdef _MSC_VER
#include <ostream>
#endif

namespace infra
{
    struct DeltaX
    {
        DeltaX() = default;
        explicit DeltaX(int16_t value);

        int16_t value;
    };

    struct DeltaY
    {
        DeltaY() = default;
        explicit DeltaY(int16_t value);

        int16_t value;
    };

    struct Vector
    {
        Vector() = default;
        Vector(int16_t x, int16_t y);

        int16_t deltaX = 0;
        int16_t deltaY = 0;

        Vector& operator+=(Vector other);
        Vector& operator-=(Vector other);
        Vector& operator*=(int16_t factor);
        Vector& operator/=(int16_t divisor);

        Vector operator+(Vector other) const;
        Vector operator-(Vector other) const;
        Vector operator*(int16_t factor) const;
        friend Vector operator*(int16_t factor, Vector vector);
        Vector operator/(int16_t factor) const;

        Vector& operator+=(DeltaX delta);
        Vector& operator+=(DeltaY delta);
        Vector& operator-=(DeltaX delta);
        Vector& operator-=(DeltaY delta);

        Vector operator+(DeltaX delta) const;
        Vector operator+(DeltaY delta) const;
        Vector operator-(DeltaX delta) const;
        Vector operator-(DeltaY delta) const;

        Vector operator-() const;

        bool operator==(const Vector& other) const;
        bool operator!=(const Vector& other) const;

        static Vector Maximum();
    };

    Vector Absolute(Vector value);

    struct Point
    {
        Point() = default;
        Point(int16_t x, int16_t y);

        int16_t x = 0;
        int16_t y = 0;

        Point& operator+=(Vector vector);
        Point& operator-=(Vector vector);

        Point operator+(Vector vector) const;
        Point operator-(Vector vector) const;
        Vector operator-(Point other) const;

        Point& operator+=(DeltaX delta);
        Point& operator+=(DeltaY delta);
        Point& operator-=(DeltaX delta);
        Point& operator-=(DeltaY delta);

        Point operator+(DeltaX delta) const;
        Point operator+(DeltaY delta) const;
        Point operator-(DeltaX delta) const;
        Point operator-(DeltaY delta) const;

        bool operator==(const Point& other) const;
        bool operator!=(const Point& other) const;
    };

    struct RegionOffset
    {
        using Transformation = FixedPoint<int32_t, 100>;

        RegionOffset() = default;
        RegionOffset(Transformation transformTopByHeight, Transformation transformBottomByHeight, Transformation transformLeftByWidth, Transformation transformRightByWidth);

        RegionOffset operator-() const;

        Transformation transformTopByHeight;
        Transformation transformBottomByHeight;
        Transformation transformLeftByWidth;
        Transformation transformRightByWidth;
    };

    class Region
    {
    public:
        Region() = default;
        Region(Point origin, Vector size);
        Region(Point origin, Point bottomRight);
        Region(int16_t top, int16_t left, int16_t bottom, int16_t right);

        Point Origin() const;
        Vector Size() const;

        int16_t Width() const;
        int16_t Height() const;

        int16_t Top() const;
        int16_t Left() const;
        int16_t Bottom() const;
        int16_t Right() const;

        Point TopLeft() const;
        Point TopRight() const;
        Point BottomLeft() const;
        Point BottomRight() const;

        Point Centre() const;

        bool Empty() const;
        bool Contains(Point point) const;
        bool ContainsOrOnBoundary(Point point) const;

        void Resize(Vector newSize);
        void Resize(DeltaX newXSize);
        void Resize(DeltaY newYSize);

        // Shift: Move the origin
        Region& operator>>=(Vector vector);
        Region& operator>>=(DeltaX delta);
        Region& operator>>=(DeltaY delta);
        Region operator>>(Vector vector) const;
        Region operator>>(DeltaX delta) const;
        Region operator>>(DeltaY delta) const;
        // Addition: Add to the size
        Region& operator+=(Vector vector);
        Region& operator+=(DeltaX delta);
        Region& operator+=(DeltaY delta);
        Region operator+(Vector vector) const;
        Region operator+(DeltaX delta) const;
        Region operator+(DeltaY delta) const;
        Region& operator-=(Vector vector);
        Region& operator-=(DeltaX delta);
        Region& operator-=(DeltaY delta);
        Region operator-(Vector vector) const;
        Region operator-(DeltaX delta) const;
        Region operator-(DeltaY delta) const;

        Region& operator+=(RegionOffset offset);
        Region& operator-=(RegionOffset offset);
        Region operator+(RegionOffset offset) const;
        Region operator-(RegionOffset offset) const;

        bool operator==(const Region& other) const;
        bool operator!=(const Region& other) const;

    private:
        Point origin;
        Vector size;
    };

    Region Intersection(Region first, Region second);
    Region Union(Region first, Region second);
    Region operator&(Region first, Region second);
    Region operator|(Region first, Region second);
    int16_t RestrictedInt16Sum(int16_t x, int16_t y);
    Vector RestrictedSum(Vector x, Vector y);
    Point Flip(Point point);
    Vector Flip(Vector vector);
    Region Flip(Region region);
    uint32_t ManhattanDistance(Point first, Point second);
    uint32_t Distance(Point first, Point second);
    Point AlignedUp(Point point, uint16_t alignX, uint16_t alignY);
    Point AlignedDown(Point point, uint16_t alignX, uint16_t alignY);
    Region AlignedRegion(Region region, uint16_t alignX, uint16_t alignY);
    Vector operator*(Vector x, RegionOffset y);
    Vector operator/(Vector x, RegionOffset y);

    enum class RightAngle : uint8_t
    {
        angle_0,
        angle_90,
        angle_180,
        angle_270
    };

    RightAngle operator+(RightAngle x, RightAngle y);
    RightAngle operator-(RightAngle x, RightAngle y);
    RightAngle operator-(RightAngle x);
    Point Rotated(Point v, RightAngle angle);
    Vector Rotated(Vector v, RightAngle angle);
    Point RotatedPoint(Point centre, float arc, uint16_t offsetFromCentre);

    Point RotatedPointInRegion(Point point, RightAngle angle, Region region);
    Vector RotatedVectorInRegion(Vector vector, RightAngle angle, Region region);
    Region RotatedRegionInRegion(Region rotatingRegion, RightAngle angle, Region region);

#ifdef _MSC_VER
    // gtest uses PrintTo to display the contents of a class
    void PrintTo(Vector vector, std::ostream* os);
    void PrintTo(Point point, std::ostream* os);
    void PrintTo(Region region, std::ostream* os);
#endif
}

#endif
