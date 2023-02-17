#include "preview/interfaces/Geometry.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace infra
{
    namespace
    {
        uint32_t Square(int16_t x)
        {
            return x * x;
        }

        uint16_t SquareRoot(uint32_t x)
        {
            return static_cast<uint16_t>(std::sqrt(x) + 0.5);
        }
    }

    DeltaX::DeltaX(int16_t value)
        : value(value)
    {}

    DeltaY::DeltaY(int16_t value)
        : value(value)
    {}

    Vector::Vector(int16_t x, int16_t y)
        : deltaX(x)
        , deltaY(y)
    {}

    Vector& Vector::operator+=(Vector other)
    {
        deltaX += other.deltaX;
        deltaY += other.deltaY;

        return *this;
    }

    Vector& Vector::operator-=(Vector other)
    {
        deltaX -= other.deltaX;
        deltaY -= other.deltaY;

        return *this;
    }

    Vector& Vector::operator*=(int16_t factor)
    {
        deltaX *= factor;
        deltaY *= factor;

        return *this;
    }

    Vector& Vector::operator/=(int16_t divisor)
    {
        deltaX /= divisor;
        deltaY /= divisor;

        return *this;
    }

    Vector Vector::operator+(Vector other) const
    {
        return Vector(*this) += other;
    }

    Vector Vector::operator-(Vector other) const
    {
        return Vector(*this) -= other;
    }

    Vector Vector::operator*(int16_t factor) const
    {
        return Vector(*this) *= factor;
    }

    Vector operator*(int16_t factor, Vector vector)
    {
        return vector *= factor;
    }

    Vector Vector::operator/(int16_t factor) const
    {
        return Vector(*this) /= factor;
    }

    Vector& Vector::operator+=(DeltaX delta)
    {
        deltaX += delta.value;
        return *this;
    }

    Vector& Vector::operator+=(DeltaY delta)
    {
        deltaY += delta.value;
        return *this;
    }

    Vector& Vector::operator-=(DeltaX delta)
    {
        deltaX -= delta.value;
        return *this;
    }

    Vector& Vector::operator-=(DeltaY delta)
    {
        deltaY -= delta.value;
        return *this;
    }

    Vector Vector::operator+(DeltaX delta) const
    {
        return Vector(*this) += delta;
    }

    Vector Vector::operator+(DeltaY delta) const
    {
        return Vector(*this) += delta;
    }

    Vector Vector::operator-(DeltaX delta) const
    {
        return Vector(*this) -= delta;
    }

    Vector Vector::operator-(DeltaY delta) const
    {
        return Vector(*this) -= delta;
    }

    Vector Vector::operator-() const
    {
        return Vector(-deltaX, -deltaY);
    }

    bool Vector::operator==(const Vector& other) const
    {
        return deltaX == other.deltaX && deltaY == other.deltaY;
    }

    bool Vector::operator!=(const Vector& other) const
    {
        return !(*this == other);
    }

    Vector Vector::Maximum()
    {
        return Vector(std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max());
    }

    Vector Absolute(Vector value)
    {
        return Vector(std::abs(value.deltaX), std::abs(value.deltaY));
    }

    Point::Point(int16_t x, int16_t y)
        : x(x)
        , y(y)
    {}

    Point& Point::operator+=(Vector vector)
    {
        x += vector.deltaX;
        y += vector.deltaY;

        return *this;
    }

    Point& Point::operator-=(Vector vector)
    {
        x -= vector.deltaX;
        y -= vector.deltaY;

        return *this;
    }

    Point Point::operator+(Vector vector) const
    {
        return Point(*this) += vector;
    }

    Point Point::operator-(Vector vector) const
    {
        return Point(*this) -= vector;
    }

    Vector Point::operator-(Point other) const
    {
        return Vector(x - other.x, y - other.y);
    }

    Point& Point::operator+=(DeltaX delta)
    {
        x += delta.value;
        return *this;
    }

    Point& Point::operator+=(DeltaY delta)
    {
        y += delta.value;
        return *this;
    }

    Point& Point::operator-=(DeltaX delta)
    {
        x -= delta.value;
        return *this;
    }

    Point& Point::operator-=(DeltaY delta)
    {
        y -= delta.value;
        return *this;
    }

    Point Point::operator+(DeltaX delta) const
    {
        return Point(*this) += delta;
    }

    Point Point::operator+(DeltaY delta) const
    {
        return Point(*this) += delta;
    }

    Point Point::operator-(DeltaX delta) const
    {
        return Point(*this) -= delta;
    }

    Point Point::operator-(DeltaY delta) const
    {
        return Point(*this) -= delta;
    }

    bool Point::operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool Point::operator!=(const Point& other) const
    {
        return !(*this == other);
    }

    RegionOffset::RegionOffset(Transformation transformTopByHeight, Transformation transformBottomByHeight, Transformation transformLeftByWidth, Transformation transformRightByWidth)
        : transformTopByHeight(transformTopByHeight)
        , transformBottomByHeight(transformBottomByHeight)
        , transformLeftByWidth(transformLeftByWidth)
        , transformRightByWidth(transformRightByWidth)
    {}

    RegionOffset RegionOffset::operator-() const
    {
        return RegionOffset(-(transformTopByHeight / transformBottomByHeight), -transformTopByHeight, -(transformLeftByWidth / transformRightByWidth), -transformLeftByWidth);
    }

    Region::Region(Point origin, Vector size)
        : origin(origin)
        , size(size)
    {
        assert(size.deltaX >= 0);
        assert(size.deltaY >= 0);
    }

    Region::Region(Point origin, Point bottomRight)
        : Region(origin, bottomRight - origin)
    {}

    Region::Region(int16_t top, int16_t left, int16_t bottom, int16_t right)
        : origin(left, top)
        , size(right - left, bottom - top)
    {
        assert(bottom >= top);
        assert(right >= left);
    }

    Point Region::Origin() const
    {
        return origin;
    }

    Vector Region::Size() const
    {
        return size;
    }

    int16_t Region::Width() const
    {
        return size.deltaX;
    }

    int16_t Region::Height() const
    {
        return size.deltaY;
    }

    int16_t Region::Top() const
    {
        return origin.y;
    }

    int16_t Region::Left() const
    {
        return origin.x;
    }

    int16_t Region::Bottom() const
    {
        return origin.y + size.deltaY;
    }

    int16_t Region::Right() const
    {
        return origin.x + size.deltaX;
    }

    Point Region::TopLeft() const
    {
        return origin;
    }

    Point Region::TopRight() const
    {
        return Point(origin.x + size.deltaX, origin.y);
    }

    Point Region::BottomLeft() const
    {
        return Point(origin.x, origin.y + size.deltaY);
    }

    Point Region::BottomRight() const
    {
        return origin + size;
    }

    Point Region::Centre() const
    {
        return origin + size / 2;
    }

    bool Region::Empty() const
    {
        return size.deltaX == 0 || size.deltaY == 0;
    }

    bool Region::Contains(Point point) const
    {
        return origin.x <= point.x && origin.y <= point.y && origin.x + size.deltaX > point.x && origin.y + size.deltaY > point.y;
    }

    bool Region::ContainsOrOnBoundary(Point point) const
    {
        return origin.x <= point.x && origin.y <= point.y && origin.x + size.deltaX >= point.x && origin.y + size.deltaY >= point.y;
    }

    void Region::Resize(Vector newSize)
    {
        assert(newSize.deltaX >= 0);
        assert(newSize.deltaY >= 0);
        size = newSize;
    }

    void Region::Resize(DeltaX newXSize)
    {
        assert(newXSize.value >= 0);
        size.deltaX = newXSize.value;
    }

    void Region::Resize(DeltaY newYSize)
    {
        assert(newYSize.value >= 0);
        size.deltaY = newYSize.value;
    }

    Region& Region::operator>>=(Vector vector)
    {
        origin += vector;
        return *this;
    }

    Region& Region::operator>>=(DeltaX delta)
    {
        origin += delta;
        return *this;
    }

    Region& Region::operator>>=(DeltaY delta)
    {
        origin += delta;
        return *this;
    }

    Region Region::operator>>(Vector vector) const
    {
        return Region(*this) >>= vector;
    }

    Region Region::operator>>(DeltaX delta) const
    {
        return Region(*this) >>= delta;
    }

    Region Region::operator>>(DeltaY delta) const
    {
        return Region(*this) >>= delta;
    }

    Region& Region::operator+=(Vector vector)
    {
        size += vector;
        return *this;
    }

    Region& Region::operator+=(DeltaX delta)
    {
        size += delta;
        return *this;
    }

    Region& Region::operator+=(DeltaY delta)
    {
        size += delta;
        return *this;
    }

    Region Region::operator+(Vector vector) const
    {
        return Region(*this) += vector;
    }

    Region Region::operator+(DeltaX delta) const
    {
        return Region(*this) += delta;
    }

    Region Region::operator+(DeltaY delta) const
    {
        return Region(*this) += delta;
    }

    Region& Region::operator-=(Vector vector)
    {
        size -= vector;
        return *this;
    }

    Region& Region::operator-=(DeltaX delta)
    {
        size -= delta;
        return *this;
    }

    Region& Region::operator-=(DeltaY delta)
    {
        size -= delta;
        return *this;
    }

    Region Region::operator-(Vector vector) const
    {
        return Region(*this) -= vector;
    }

    Region Region::operator-(DeltaX delta) const
    {
        return Region(*this) -= delta;
    }

    Region Region::operator-(DeltaY delta) const
    {
        return Region(*this) -= delta;
    }

    Region& Region::operator+=(RegionOffset offset)
    {
        int32_t width = Width();
        int32_t height = Height();

        int16_t top = Top() + (offset.transformTopByHeight * height).Rounded();
        int16_t left = Left() + (offset.transformLeftByWidth * width).Rounded();
        int16_t bottom = Bottom() + (offset.transformBottomByHeight * height).Rounded();
        int16_t right = Right() + (offset.transformRightByWidth * width).Rounded();

        *this = Region(top, left, bottom, right);

        return *this;
    }

    Region& Region::operator-=(RegionOffset offset)
    {
        return *this += -offset;
    }

    Region Region::operator+(RegionOffset offset) const
    {
        Region result(*this);
        result += offset;
        return result;
    }

    Region Region::operator-(RegionOffset offset) const
    {
        Region result(*this);
        result -= offset;
        return result;
    }

    bool Region::operator==(const Region& other) const
    {
        return origin == other.origin && size == other.size;
    }

    bool Region::operator!=(const Region& other) const
    {
        return !(*this == other);
    }

    Region Intersection(Region first, Region second)
    {
        if (first.Top() >= second.Bottom() || first.Bottom() <= second.Top() || first.Left() >= second.Right() || first.Right() <= second.Left())
            return Region();
        else
            return Region(std::max(first.Top(), second.Top()), std::max(first.Left(), second.Left()), std::min(first.Bottom(), second.Bottom()), std::min(first.Right(), second.Right()));
    }

    Region Union(Region first, Region second)
    {
        if (first.Empty())
            return second;
        if (second.Empty())
            return first;

        return Region(std::min(first.Top(), second.Top()), std::min(first.Left(), second.Left()), std::max(first.Bottom(), second.Bottom()), std::max(first.Right(), second.Right()));
    }

    Region operator&(Region first, Region second)
    {
        return Intersection(first, second);
    }

    Region operator|(Region first, Region second)
    {
        return Union(first, second);
    }

    int16_t RestrictedInt16Sum(int16_t x, int16_t y)
    {
        if (std::numeric_limits<int16_t>::max() - y < x)
            return std::numeric_limits<int16_t>::max();
        else
            return x + y;
    }

    Vector RestrictedSum(Vector x, Vector y)
    {
        return Vector(RestrictedInt16Sum(x.deltaX, y.deltaX), RestrictedInt16Sum(x.deltaY, y.deltaY));
    }

    Point Flip(Point point)
    {
        return Point(point.y, point.x);
    }

    Vector Flip(Vector vector)
    {
        return Vector(vector.deltaY, vector.deltaX);
    }

    Region Flip(Region region)
    {
        return Region(Flip(region.Origin()), Flip(region.Size()));
    }

    uint32_t ManhattanDistance(Point first, Point second)
    {
        return std::abs(first.x - second.x) + std::abs(first.y - second.y);
    }

    uint32_t Distance(Point first, Point second)
    {
        return SquareRoot(Square(first.x - second.x) + Square(first.y - second.y));
    }

    Point AlignedUp(Point point, uint16_t alignX, uint16_t alignY)
    {
        return Point(point.x - point.x % alignX, point.y - point.y % alignY);
    }

    Point AlignedDown(Point point, uint16_t alignX, uint16_t alignY)
    {
        return AlignedUp(Point(point.x + alignX - 1, point.y + alignY - 1), alignX, alignY);
    }

    Region AlignedRegion(Region region, uint16_t alignX, uint16_t alignY)
    {
        return Region(AlignedUp(region.TopLeft(), alignX, alignY), AlignedDown(region.BottomRight(), alignX, alignY));
    }

    Vector operator*(Vector x, RegionOffset y)
    {
        auto factorX = y.transformRightByWidth - y.transformLeftByWidth;
        auto factorY = y.transformBottomByHeight - y.transformTopByHeight;

        return x + Vector((static_cast<int32_t>(x.deltaX) * factorX).Rounded(), (static_cast<int32_t>(x.deltaY) * factorY).Rounded());
    }

    Vector operator/(Vector x, RegionOffset y)
    {
        auto factorX = y.transformRightByWidth - y.transformLeftByWidth;
        auto factorY = y.transformBottomByHeight - y.transformTopByHeight;

        return x + Vector((static_cast<int32_t>(x.deltaX) / factorX).Rounded(), (static_cast<int32_t>(x.deltaY) / factorY).Rounded());
    }

    RightAngle operator+(RightAngle x, RightAngle y)
    {
        return static_cast<RightAngle>((static_cast<uint8_t>(x) + static_cast<uint8_t>(y)) % 4);
    }

    RightAngle operator-(RightAngle x, RightAngle y)
    {
        return static_cast<RightAngle>((4 + static_cast<uint8_t>(x) - static_cast<uint8_t>(y)) % 4);
    }

    RightAngle operator-(RightAngle x)
    {
        return static_cast<RightAngle>((4 - static_cast<uint8_t>(x)) % 4);
    }

    Point Rotated(Point p, RightAngle angle)
    {
        return Point() + Rotated(p - Point(), angle);
    }

    Vector Rotated(Vector v, RightAngle angle)
    {
        switch (angle)
        {
            case RightAngle::angle_0:
                return v;
            case RightAngle::angle_90:
                return Vector(-v.deltaY, v.deltaX);
            case RightAngle::angle_180:
                return Vector(-v.deltaX, -v.deltaY);
            case RightAngle::angle_270:
                return Vector(v.deltaY, -v.deltaX);
            default:
                std::abort();
        }
    }

    Point RotatedPoint(Point centre, float arc, uint16_t offsetFromCentre)
    {
        return centre + Vector(static_cast<int16_t>(std::cos(arc) * offsetFromCentre), static_cast<int16_t>(std::sin(arc) * offsetFromCentre));
    }

    infra::Point RotatedPointInRegion(infra::Point point, RightAngle angle, infra::Region region)
    {
        switch (angle)
        {
            case infra::RightAngle::angle_0:
                return point;
            case infra::RightAngle::angle_90:
                return infra::Point(region.Left() + point.y - region.Top(), region.Top() + region.Width() - point.x);
            case infra::RightAngle::angle_180:
                return infra::Point(region.Left() + region.Width() - point.x, region.Top() + region.Height() - point.y);
            case infra::RightAngle::angle_270:
                return infra::Point(region.Left() + region.Height() - point.y, region.Top() + point.x - region.Left());
            default:
                std::abort();
        }
    }

    infra::Vector RotatedVectorInRegion(infra::Vector vector, infra::RightAngle angle, infra::Region region)
    {
        return RotatedPointInRegion(infra::Point() + vector, angle, region) - infra::Point();
    }

    infra::Region RotatedRegionInRegion(infra::Region rotatingRegion, RightAngle angle, infra::Region region)
    {
        switch (angle)
        {
            case infra::RightAngle::angle_0:
                return rotatingRegion;
            case infra::RightAngle::angle_90:
                return infra::Region(RotatedPointInRegion(rotatingRegion.TopRight(), angle, region), RotatedPointInRegion(rotatingRegion.BottomLeft(), angle, region));
            case infra::RightAngle::angle_180:
                return infra::Region(RotatedPointInRegion(rotatingRegion.BottomRight(), angle, region), RotatedPointInRegion(rotatingRegion.TopLeft(), angle, region));
            case infra::RightAngle::angle_270:
                return infra::Region(RotatedPointInRegion(rotatingRegion.BottomLeft(), angle, region), RotatedPointInRegion(rotatingRegion.TopRight(), angle, region));
            default:
                std::abort();
        }
    }

#ifdef _MSC_VER
    void PrintTo(Vector vector, std::ostream* os)
    {
        *os << '(' << vector.deltaX << ", " << vector.deltaY << ')';
    }

    void PrintTo(Point point, std::ostream* os)
    {
        *os << '(' << point.x << ", " << point.y << ')';
    }

    void PrintTo(Region region, std::ostream* os)
    {
        *os << '[';
        PrintTo(region.TopLeft(), os);
        *os << " -> ";
        PrintTo(region.Size(), os);
        *os << ']';
    }
#endif
}
