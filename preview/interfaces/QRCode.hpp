/**
 * The MIT License (MIT)
 *
 * This library is written and maintained by Richard Moore.
 * Major parts were derived from Project Nayuki's library.
 *
 * Copyright (c) 2017 Richard Moore     (https://github.com/ricmoo/QRCode)
 * Copyright (c) 2017 Project Nayuki    (https://www.nayuki.io/page/qr-code-generator-library)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 *  Special thanks to Nayuki (https://www.nayuki.io/) from which this library was
 *  heavily inspired and compared against.
 *
 *  See: https://github.com/nayuki/QR-Code-generator/tree/master/cpp
 */

#ifndef QR_CODE_HPP
#define QR_CODE_HPP

#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include <cstdint>

namespace services
{
    enum class QRCodeEcc : uint8_t
    {
        low,
        medium,
        quartile,
        high
    };

    namespace detail
    {
        static constexpr uint16_t GridSizeInBytes(uint8_t bits)
        {
            return (bits * bits + 7) / 8;
        }

        static constexpr uint16_t BufferSize(uint8_t version)
        {
            return GridSizeInBytes(4 * version + 17);
        }

        class BitBucket
        {
        public:
            template<uint8_t Version>
            struct ForVersion;

            BitBucket(uint8_t size, infra::ByteRange buffer);

            void Set(infra::Point position, bool on);
            bool Get(infra::Point position) const;
            void Invert(infra::Point position, bool invert);

            uint32_t PenaltyScore() const;

        public:
            infra::Bitmap bitmap;
        };

        template<uint8_t Version>
        struct BitBucket::ForVersion
            : BitBucket
        {
            ForVersion();

            static constexpr uint8_t size = Version * 4 + 17;
            std::array<uint8_t, GridSizeInBytes(size)> buffer{};
        };

        class BitBuffer
        {
        private:
            static constexpr std::array<uint16_t, 41> numRawDataModulesForVersion{
                0, 208, 359, 567, 807, 1079, 1383, 1568, 1936, 2336, 2768, 3232, 3728, 4256, 4651, 5243, 5867, 6523,
                7211, 7931, 8683, 9252, 10068, 10916, 11796, 12708, 13652, 14628, 15371, 16411, 17483, 18587,
                19723, 20891, 22091, 23008, 24272, 25568, 26896, 28256, 29648
            };

            static constexpr auto numErrorCorrectionCodewords = []()
            {
                std::array<std::array<uint16_t, 40>, 4> result{ {
                    // 1,  2,  3,  4,  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40    Error correction level
                    { 10, 16, 26, 36, 48, 64, 72, 88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560, 588, 644, 700, 728, 784, 812, 868, 924, 980, 1036, 1064, 1120, 1204, 1260, 1316, 1372 },             // Medium
                    { 7, 10, 15, 20, 26, 36, 40, 48, 60, 72, 80, 96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300, 312, 336, 360, 390, 420, 450, 480, 510, 540, 570, 570, 600, 630, 660, 720, 750 },                         // Low
                    { 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430 }, // High
                    { 13, 22, 36, 52, 72, 96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810, 870, 952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040 },    // Quartile
                } };

                return result;
            }();

            static constexpr auto numErrorCorrectionBlocks = []()
            {
                const std::array<std::array<uint8_t, 40>, 4> result{ {
                    // Version: (note that index 0 is for padding, and is set to an illegal value)
                    // 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
                    { 1, 1, 1, 2, 2, 4, 4, 4, 5, 5, 5, 8, 9, 9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49 },     // Medium
                    { 1, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 4, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25 },              // Low
                    { 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81 }, // High
                    { 1, 1, 2, 2, 4, 4, 6, 6, 8, 8, 8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68 },  // Quartile
                } };

                return result;
            }();

            static constexpr uint16_t RoundBitsToByte(uint16_t bits)
            {
                return (bits + 7) / 8;
            }

            static constexpr uint8_t BlockEccLen(uint8_t version, uint8_t ecc)
            {
                uint8_t numBlocks = numErrorCorrectionBlocks[ecc][version - 1];
                uint16_t totalEcc = numErrorCorrectionCodewords[ecc][version - 1];
                return totalEcc / numBlocks;
            }

        public:
            BitBuffer(uint8_t version, uint8_t ecc);

            void Generate(infra::BoundedConstString text);

            uint16_t Length() const;
            bool Bit(uint16_t index) const;

        private:
            void Append(uint32_t val, uint8_t length);
            void EncodeDataCodewords(infra::BoundedConstString text);
            void PerformErrorCorrection(uint8_t ecc);

        private:
            class ReedSolomon
            {
            public:
                ReedSolomon(uint8_t degree);

                void Remainder(uint8_t* data, uint8_t length, uint8_t* result, uint8_t stride) const;

            private:
                uint8_t Multiply(uint8_t x, uint8_t y) const;

            private:
                uint8_t degree;
                uint8_t* coeff;
            };

        private:
            uint8_t version;
            uint8_t ecc;
            uint16_t moduleCount;
            uint16_t bitOffset = 0;
            uint16_t capacityBytes;
            uint8_t* data;
            uint8_t* result;
            ReedSolomon reedSolomon;
        };

        class QRCodeGenerator
        {
        public:
            template<uint8_t Version, QRCodeEcc Ecc>
            struct ForVersionAndEcc;

            QRCodeGenerator(BitBucket& modules, BitBucket& isFunction, uint8_t version, QRCodeEcc ecc);

            void Generate(infra::BoundedConstString text);

        private:
            uint8_t BestMask();

            void ApplyMask(uint8_t mask);
            void DrawFunctionPatterns();
            void DrawVersion();
            void DrawFinderPattern(infra::Point position);
            void DrawAlignmentPattern(infra::Point position);
            void DrawCodewords();
            void DrawFormatBits(uint8_t mask);
            void SetFunctionModule(infra::Point position, bool on);

        private:
            uint8_t version;
            uint8_t ecc;

            BitBucket& modules;
            BitBucket& isFunction;

            BitBuffer codewords;

            uint8_t alignCount;
            uint8_t* alignPosition;
        };
    }

    template<uint8_t Version, QRCodeEcc Ecc>
    class QRCode
    {
    public:
        QRCode(infra::BoundedConstString text);

        const infra::Bitmap& GetBitmap() const;

    public:
        detail::BitBucket::ForVersion<Version> modules;
    };

    //// Implementation ////

    namespace detail
    {
        template<uint8_t Version>
        BitBucket::ForVersion<Version>::ForVersion()
            : BitBucket(size, buffer)
        {}
    }

    template<uint8_t Version, QRCodeEcc Ecc>
    QRCode<Version, Ecc>::QRCode(infra::BoundedConstString text)
    {
        detail::BitBucket::ForVersion<Version> isFunction;

        detail::QRCodeGenerator generator(modules, isFunction, Version, Ecc);
        generator.Generate(text);
    }

    template<uint8_t Version, QRCodeEcc Ecc>
    const infra::Bitmap& QRCode<Version, Ecc>::GetBitmap() const
    {
        return modules.bitmap;
    }
}
#endif
