#include "preview/interfaces/Bitmap.hpp"
#include "gmock/gmock.h"

class BitmapTest
    : public testing::Test
{};

TEST_F(BitmapTest, check_construction_and_storage_size)
{
    infra::Bitmap::Rgb565<4, 3> bitmapRgb565;
    infra::Bitmap::Rgb888<4, 3> bitmapRgb888;
    infra::Bitmap::BlackAndWhite<4, 3> bitmapBlackAndWhite;

    EXPECT_EQ(bitmapRgb565.BufferSize(), 24);
    EXPECT_EQ(bitmapRgb888.BufferSize(), 36);
    EXPECT_EQ(bitmapBlackAndWhite.BufferSize(), 2);
}

TEST_F(BitmapTest, construction_from_rgb565_to_blackandwhite)
{
    infra::Bitmap::Rgb565<4, 3> bitmapRgb565;
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(0, 0))) = 0xffff; // white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(1, 0))) = 0x0000; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(2, 0))) = 0x0001; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(3, 0))) = 0x0001; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(0, 1))) = 0xfffd; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(1, 1))) = 0x1234; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(2, 1))) = 0x1000; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(3, 1))) = 0x1000; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(0, 2))) = 0x2222; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(1, 2))) = 0x1234; // non-white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(2, 2))) = 0xffff; // white
    *reinterpret_cast<uint16_t*>(bitmapRgb565.BufferAddress(infra::Point(3, 2))) = 0xffff; // white

    uint8_t buffer[2];
    infra::Bitmap bitmapBlackAndWhite(infra::MakeByteRange(buffer), infra::Vector(4, 3), infra::PixelFormat::blackandwhite);
    bitmapBlackAndWhite.ConvertToBlackAndWhiteFromRgb565(bitmapRgb565);

    // buffer should be 0xc01 after conversion
    EXPECT_EQ(0x01, buffer[0]);
    EXPECT_EQ(0xfc, buffer[1] | 0xf0);
}

TEST_F(BitmapTest, construction_from_rgb888_to_blackandwhite)
{
    infra::Bitmap::Rgb888<4, 3> bitmapRgb888;

    uint32_t colour = 0x00ffffff; // white;
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 0))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 0))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00000000; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 0))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 0))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00000001; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 0))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 0))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00011111; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 0))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 0))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00fffffd; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 1))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 1))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00121234; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 1))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 1))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00001000; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 1))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 1))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00ff1000; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 1))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 1))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00222222; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 2))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(0, 2))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00211234; // non-white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 2))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(1, 2))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00ffffff; // white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 2))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(2, 2))) + 2) = static_cast<uint8_t>(colour >> 16);

    colour = 0x00ffffff; // white
    *reinterpret_cast<volatile uint16_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 2))) = static_cast<uint16_t>(colour);
    *(reinterpret_cast<volatile uint8_t*>(bitmapRgb888.BufferAddress(infra::Point(3, 2))) + 2) = static_cast<uint8_t>(colour >> 16);

    uint8_t buffer[2];
    infra::Bitmap bitmapBlackAndWhite(infra::MakeByteRange(buffer), infra::Vector(4, 3), infra::PixelFormat::blackandwhite);
    bitmapBlackAndWhite.ConvertToBlackAndWhiteFromRgb888(bitmapRgb888);

    // buffer should be 0x101 after conversion
    EXPECT_EQ(0x01, buffer[0]);
    EXPECT_EQ(0xfc, buffer[1] | 0xf0);
}
