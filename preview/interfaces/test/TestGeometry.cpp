#include "preview/interfaces/Geometry.hpp"
#include "gmock/gmock.h"

TEST(GeometryTest, vector_construction)
{
    infra::Vector defaultVector;
    EXPECT_EQ(0, defaultVector.deltaX);
    EXPECT_EQ(0, defaultVector.deltaY);

    infra::Vector vector(3, 4);
    EXPECT_EQ(3, vector.deltaX);
    EXPECT_EQ(4, vector.deltaY);

    EXPECT_EQ(std::numeric_limits<int16_t>::max(), infra::Vector::Maximum().deltaX);
    EXPECT_EQ(std::numeric_limits<int16_t>::max(), infra::Vector::Maximum().deltaY);
}

TEST(GeometryTest, vector_equality)
{
    EXPECT_TRUE(infra::Vector(2, 3) == infra::Vector(2, 3));
    EXPECT_FALSE(infra::Vector(2, 3) != infra::Vector(2, 3));
    EXPECT_FALSE(infra::Vector(2, 3) == infra::Vector(3, 3));
    EXPECT_FALSE(infra::Vector(2, 3) == infra::Vector(2, 4));
}

TEST(GeometryTest, vector_math)
{
    EXPECT_EQ(infra::Vector(4, 6), infra::Vector(1, 2) + infra::Vector(3, 4));
    EXPECT_EQ(infra::Vector(2, 3), infra::Vector(3, 5) - infra::Vector(1, 2));
    EXPECT_EQ(infra::Vector(6, 10), infra::Vector(3, 5) * 2);
    EXPECT_EQ(infra::Vector(6, 10), 2 * infra::Vector(3, 5));
    EXPECT_EQ(infra::Vector(3, 5), infra::Vector(6, 10) / 2);
    EXPECT_EQ(infra::Vector(4, 6), infra::Vector(3, 6) + infra::DeltaX(1));
    EXPECT_EQ(infra::Vector(3, 7), infra::Vector(3, 6) + infra::DeltaY(1));
    EXPECT_EQ(infra::Vector(2, 6), infra::Vector(3, 6) - infra::DeltaX(1));
    EXPECT_EQ(infra::Vector(3, 5), infra::Vector(3, 6) - infra::DeltaY(1));
    EXPECT_EQ(infra::Vector(-3, -6), -infra::Vector(3, 6));
}

TEST(GeometryTest, vector_absolute)
{
    EXPECT_EQ(infra::Vector(3, 5), infra::Absolute(infra::Vector(3, 5)));
    EXPECT_EQ(infra::Vector(3, 5), infra::Absolute(infra::Vector(-3, 5)));
    EXPECT_EQ(infra::Vector(3, 5), infra::Absolute(infra::Vector(3, -5)));
}

TEST(GeometryTest, point_construction)
{
    infra::Point defaultPoint;
    EXPECT_EQ(0, defaultPoint.x);
    EXPECT_EQ(0, defaultPoint.y);

    infra::Point point(3, 4);
    EXPECT_EQ(3, point.x);
    EXPECT_EQ(4, point.y);
}

TEST(GeometryTest, point_equality)
{
    EXPECT_TRUE(infra::Point(2, 3) == infra::Point(2, 3));
    EXPECT_FALSE(infra::Point(2, 3) != infra::Point(2, 3));
    EXPECT_FALSE(infra::Point(2, 3) == infra::Point(3, 3));
    EXPECT_FALSE(infra::Point(2, 3) == infra::Point(2, 4));
}

TEST(GeometryTest, point_math)
{
    EXPECT_EQ(infra::Point(4, 6), infra::Point(1, 2) + infra::Vector(3, 4));
    EXPECT_EQ(infra::Point(2, 3), infra::Point(3, 5) - infra::Vector(1, 2));
    EXPECT_EQ(infra::Vector(2, 3), infra::Point(3, 5) - infra::Point(1, 2));
    EXPECT_EQ(infra::Point(4, 6), infra::Point(3, 6) + infra::DeltaX(1));
    EXPECT_EQ(infra::Point(3, 7), infra::Point(3, 6) + infra::DeltaY(1));
    EXPECT_EQ(infra::Point(2, 6), infra::Point(3, 6) - infra::DeltaX(1));
    EXPECT_EQ(infra::Point(3, 5), infra::Point(3, 6) - infra::DeltaY(1));
}

TEST(GeometryTest, region_construction)
{
    infra::Region defaultRegion;
    EXPECT_TRUE(defaultRegion.Empty());
    EXPECT_EQ(infra::Point(), defaultRegion.Origin());
    EXPECT_EQ(infra::Vector(), defaultRegion.Size());

    infra::Region region(infra::Point(1, 2), infra::Vector(3, 4));
    EXPECT_FALSE(region.Empty());
    EXPECT_EQ(infra::Point(1, 2), region.Origin());
    EXPECT_EQ(infra::Vector(3, 4), region.Size());
    EXPECT_EQ(3, region.Width());
    EXPECT_EQ(4, region.Height());
    EXPECT_EQ(infra::Point(1, 2), region.TopLeft());
    EXPECT_EQ(infra::Point(4, 2), region.TopRight());
    EXPECT_EQ(infra::Point(1, 6), region.BottomLeft());
    EXPECT_EQ(infra::Point(4, 6), region.BottomRight());
    EXPECT_EQ(1, region.Left());
    EXPECT_EQ(4, region.Right());
    EXPECT_EQ(2, region.Top());
    EXPECT_EQ(6, region.Bottom());

    infra::Region region2(2, 1, 4, 3);
    EXPECT_EQ(1, region2.Left());
    EXPECT_EQ(3, region2.Right());
    EXPECT_EQ(2, region2.Top());
    EXPECT_EQ(4, region2.Bottom());
    EXPECT_EQ(infra::Point(2, 3), region2.Centre());

    infra::Region region3(infra::Point(1, 2), infra::Point(3, 4));
    EXPECT_EQ(1, region3.Left());
    EXPECT_EQ(3, region3.Right());
    EXPECT_EQ(2, region3.Top());
    EXPECT_EQ(4, region3.Bottom());
}

TEST(GeometryTest, region_contains)
{
    infra::Region region(infra::Point(1, 2), infra::Vector(3, 4));
    EXPECT_TRUE(region.Contains(infra::Point(1, 2)));
    EXPECT_TRUE(region.Contains(infra::Point(3, 2)));
    EXPECT_TRUE(region.Contains(infra::Point(1, 5)));
    EXPECT_TRUE(region.Contains(infra::Point(3, 5)));

    EXPECT_FALSE(region.Contains(infra::Point(0, 2)));
    EXPECT_FALSE(region.Contains(infra::Point(4, 2)));
    EXPECT_FALSE(region.Contains(infra::Point(0, 5)));
    EXPECT_FALSE(region.Contains(infra::Point(4, 5)));

    EXPECT_FALSE(region.Contains(infra::Point(1, 1)));
    EXPECT_FALSE(region.Contains(infra::Point(3, 1)));
    EXPECT_FALSE(region.Contains(infra::Point(1, 6)));
    EXPECT_FALSE(region.Contains(infra::Point(3, 6)));
}

TEST(GeometryTest, region_contains_or_on_boundary)
{
    infra::Region region(infra::Point(1, 2), infra::Vector(3, 4));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(1, 2)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(3, 2)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(1, 5)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(3, 5)));

    EXPECT_FALSE(region.ContainsOrOnBoundary(infra::Point(0, 2)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(4, 2)));
    EXPECT_FALSE(region.ContainsOrOnBoundary(infra::Point(0, 5)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(4, 5)));

    EXPECT_FALSE(region.ContainsOrOnBoundary(infra::Point(1, 1)));
    EXPECT_FALSE(region.ContainsOrOnBoundary(infra::Point(3, 1)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(1, 6)));
    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(3, 6)));

    EXPECT_TRUE(region.ContainsOrOnBoundary(infra::Point(4, 6)));
}

TEST(GeometryTest, region_equality)
{
    EXPECT_TRUE(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) == infra::Region(infra::Point(1, 2), infra::Vector(3, 4)));
    EXPECT_FALSE(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) != infra::Region(infra::Point(1, 2), infra::Vector(3, 4)));
    EXPECT_FALSE(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) == infra::Region(infra::Point(2, 2), infra::Vector(3, 4)));
    EXPECT_FALSE(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) == infra::Region(infra::Point(1, 2), infra::Vector(4, 4)));
}

TEST(GeometryTest, region_math)
{
    EXPECT_EQ(infra::Region(infra::Point(6, 8), infra::Vector(3, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) >> infra::Vector(5, 6));
    EXPECT_EQ(infra::Region(infra::Point(6, 2), infra::Vector(3, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) >> infra::DeltaX(5));
    EXPECT_EQ(infra::Region(infra::Point(1, 8), infra::Vector(3, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) >> infra::DeltaY(6));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(8, 10)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) + infra::Vector(5, 6));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(8, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) + infra::DeltaX(5));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 10)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) + infra::DeltaY(6));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(8, 10)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) - -infra::Vector(5, 6));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(8, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) - infra::DeltaX(-5));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 10)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) - infra::DeltaY(-6));

    EXPECT_EQ(infra::Region(), infra::Region(infra::Point(0, 0), infra::Vector(1, 1)) & infra::Region(infra::Point(1, 0), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(), infra::Region(infra::Point(0, 0), infra::Vector(1, 1)) & infra::Region(infra::Point(0, 1), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(), infra::Region(infra::Point(0, 0), infra::Vector(1, 1)) & infra::Region(infra::Point(-1, 0), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(), infra::Region(infra::Point(0, 0), infra::Vector(1, 1)) & infra::Region(infra::Point(0, -1), infra::Vector(1, 1)));

    EXPECT_EQ(infra::Region(infra::Point(1, 1), infra::Vector(1, 1)), infra::Region(infra::Point(0, 0), infra::Vector(2, 2)) & infra::Region(infra::Point(1, 1), infra::Vector(2, 2)));
    EXPECT_EQ(infra::Region(infra::Point(1, 0), infra::Vector(1, 1)), infra::Region(infra::Point(0, 0), infra::Vector(2, 2)) & infra::Region(infra::Point(1, -1), infra::Vector(2, 2)));
    EXPECT_EQ(infra::Region(infra::Point(0, 1), infra::Vector(1, 1)), infra::Region(infra::Point(0, 0), infra::Vector(2, 2)) & infra::Region(infra::Point(-1, 1), infra::Vector(2, 2)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(1, 1)), infra::Region(infra::Point(0, 0), infra::Vector(2, 2)) & infra::Region(infra::Point(-1, -1), infra::Vector(2, 2)));

    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(2, 2)), infra::Region(infra::Point(0, 0), infra::Vector(2, 2)) | infra::Region(infra::Point(5, 5), infra::Vector(0, 2)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(2, 2)), infra::Region(infra::Point(5, 5), infra::Vector(0, 2)) | infra::Region(infra::Point(0, 0), infra::Vector(2, 2)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(3, 3)), infra::Region(infra::Point(0, 0), infra::Vector(1, 1)) | infra::Region(infra::Point(2, 2), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(3, 3)), infra::Region(infra::Point(0, 2), infra::Vector(1, 1)) | infra::Region(infra::Point(2, 0), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(3, 3)), infra::Region(infra::Point(2, 0), infra::Vector(1, 1)) | infra::Region(infra::Point(0, 2), infra::Vector(1, 1)));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(3, 3)), infra::Region(infra::Point(2, 2), infra::Vector(1, 1)) | infra::Region(infra::Point(0, 0), infra::Vector(1, 1)));
}

TEST(GeometryTest, region_resize)
{
    infra::Region region(infra::Point(), infra::Vector(3, 4));
    region.Resize(infra::Vector(5, 6));
    EXPECT_EQ(infra::Vector(5, 6), region.Size());
    region.Resize(infra::DeltaX(7));
    EXPECT_EQ(infra::Vector(7, 6), region.Size());
    region.Resize(infra::DeltaY(8));
    EXPECT_EQ(infra::Vector(7, 8), region.Size());
}

TEST(GeometryTest, RegionOffset)
{
    EXPECT_EQ(infra::Region(infra::Point(4, 6), infra::Vector(5, 6)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) + infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }));
    EXPECT_EQ(infra::Region(infra::Point(-1, -1), infra::Vector(2, 3)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) - infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 5)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) - infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }) + infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }));
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)), infra::Region(infra::Point(1, 2), infra::Vector(3, 4)) + infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }) - infra::RegionOffset({ 1, 0 }, { 1, 50 }, { 1, 0 }, { 1, 50 }));
}

TEST(GeometryTest, RestrictedSum)
{
    EXPECT_EQ(9, infra::RestrictedInt16Sum(5, 4));
    EXPECT_EQ(std::numeric_limits<int16_t>::max(), infra::RestrictedInt16Sum(5, std::numeric_limits<int16_t>::max() - 3));
    EXPECT_EQ(std::numeric_limits<int16_t>::max(), infra::RestrictedInt16Sum(std::numeric_limits<int16_t>::max() - 2, 4));
    EXPECT_EQ(infra::Vector(9, std::numeric_limits<int16_t>::max()), infra::RestrictedSum(infra::Vector(5, 3), infra::Vector(4, std::numeric_limits<int16_t>::max())));
}

TEST(GeometryTest, flip_region)
{
    EXPECT_EQ(infra::Region(infra::Point(1, 2), infra::Vector(3, 4)), infra::Flip(infra::Region(infra::Point(2, 1), infra::Vector(4, 3))));
}

TEST(GeometryTest, ManhattanDistance)
{
    EXPECT_EQ(0, infra::ManhattanDistance(infra::Point(), infra::Point()));
    EXPECT_EQ(10, infra::ManhattanDistance(infra::Point(), infra::Point(10, 0)));
    EXPECT_EQ(10, infra::ManhattanDistance(infra::Point(), infra::Point(0, 10)));
    EXPECT_EQ(10, infra::ManhattanDistance(infra::Point(10, 0), infra::Point()));
    EXPECT_EQ(10, infra::ManhattanDistance(infra::Point(0, 10), infra::Point()));
    EXPECT_EQ(20, infra::ManhattanDistance(infra::Point(10, 10), infra::Point()));
}

TEST(GeometryTest, Aligned)
{
    EXPECT_EQ(infra::Region(infra::Point(2, 3), infra::Vector(4, 6)), infra::AlignedRegion(infra::Region(infra::Point(2, 3), infra::Vector(4, 6)), 2, 3));
    EXPECT_EQ(infra::Region(infra::Point(0, 0), infra::Vector(4, 9)), infra::AlignedRegion(infra::Region(infra::Point(1, 2), infra::Vector(2, 5)), 2, 3));
}

TEST(GeometryTest, Rotated)
{
    EXPECT_EQ(infra::Vector(1, 2), infra::Rotated(infra::Vector(1, 2), infra::RightAngle::angle_0));
    EXPECT_EQ(infra::Vector(-2, 1), infra::Rotated(infra::Vector(1, 2), infra::RightAngle::angle_90));
    EXPECT_EQ(infra::Vector(-1, -2), infra::Rotated(infra::Vector(1, 2), infra::RightAngle::angle_180));
    EXPECT_EQ(infra::Vector(2, -1), infra::Rotated(infra::Vector(1, 2), infra::RightAngle::angle_270));
}
