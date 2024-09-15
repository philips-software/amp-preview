#ifndef PREVIEW_QR_CODE_HPP
#define PREVIEW_QR_CODE_HPP

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

    template<uint8_t Version, QRCodeEcc Ecc>
    class QRCode
    {
    public:
        QRCode(infra::BoundedConstString text);

        const infra::Bitmap& GetBitmap() const;

        using Bitmap = infra::Bitmap::BlackAndWhite<Version * 4 + 17, Version * 4 + 17>;

    private:
        Bitmap bitmap;
    };

    namespace detail
    {
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
                    { 7, 10, 15, 20, 26, 36, 40, 48, 60, 72, 80, 96, 104, 120, 132, 144, 168, 180, 196, 224, 224, 252, 270, 300, 312, 336, 360, 390, 420, 450, 480, 510, 540, 570, 570, 600, 630, 660, 720, 750 },                         // Low
                    { 10, 16, 26, 36, 48, 64, 72, 88, 110, 130, 150, 176, 198, 216, 240, 280, 308, 338, 364, 416, 442, 476, 504, 560, 588, 644, 700, 728, 784, 812, 868, 924, 980, 1036, 1064, 1120, 1204, 1260, 1316, 1372 },             // Medium
                    { 13, 22, 36, 52, 72, 96, 108, 132, 160, 192, 224, 260, 288, 320, 360, 408, 448, 504, 546, 600, 644, 690, 750, 810, 870, 952, 1020, 1050, 1140, 1200, 1290, 1350, 1440, 1530, 1590, 1680, 1770, 1860, 1950, 2040 },    // Quartile
                    { 17, 28, 44, 64, 88, 112, 130, 156, 192, 224, 264, 308, 352, 384, 432, 480, 532, 588, 650, 700, 750, 816, 900, 960, 1050, 1110, 1200, 1260, 1350, 1440, 1530, 1620, 1710, 1800, 1890, 1980, 2100, 2220, 2310, 2430 }, // High
                } };

                return result;
            }();

            static constexpr auto numErrorCorrectionBlocks = []()
            {
                const std::array<std::array<uint8_t, 40>, 4> result{ {
                    // Version: (note that index 0 is for padding, and is set to an illegal value)
                    // 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
                    { 1, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 4, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25 },              // Low
                    { 1, 1, 1, 2, 2, 4, 4, 4, 5, 5, 5, 8, 9, 9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49 },     // Medium
                    { 1, 1, 2, 2, 4, 4, 6, 6, 8, 8, 8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68 },  // Quartile
                    { 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81 }, // High
                } };

                return result;
            }();

            static constexpr uint16_t RoundBitsToByte(uint16_t bits)
            {
                return (bits + 7) / 8;
            }

            static constexpr uint8_t BlockEccLen(uint8_t version, QRCodeEcc ecc)
            {
                uint8_t numBlocks = numErrorCorrectionBlocks[static_cast<uint8_t>(ecc)][version - 1];
                uint16_t totalEcc = numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];
                return totalEcc / numBlocks;
            }

        public:
            template<uint8_t Version, QRCodeEcc ecc>
            struct ForVersionAndEcc;

            BitBuffer(infra::ByteRange data, infra::ByteRange result, infra::ByteRange coeff, uint8_t version, QRCodeEcc ecc);

            void Generate(infra::BoundedConstString text);

            uint16_t Length() const;
            bool Bit(uint16_t index) const;

        private:
            void Append(uint32_t val, uint8_t length);
            void EncodeDataCodewords(infra::BoundedConstString text);
            void PerformErrorCorrection(QRCodeEcc ecc);

        private:
            class ReedSolomon
            {
            public:
                ReedSolomon(infra::ByteRange coeff);

                void Remainder(uint8_t* data, uint8_t length, uint8_t* result, uint8_t stride) const;

            private:
                uint8_t Multiply(uint8_t x, uint8_t y) const;

            private:
                infra::ByteRange coeff;
            };

        private:
            uint8_t version;
            QRCodeEcc ecc;
            uint16_t moduleCount;
            uint16_t bitOffset = 0;
            infra::ByteRange data;
            infra::ByteRange result;
            ReedSolomon reedSolomon;
        };

        template<uint8_t Version, QRCodeEcc Ecc>
        struct BitBuffer::ForVersionAndEcc
        {
            ForVersionAndEcc();

            static constexpr uint16_t moduleCount = numRawDataModulesForVersion[Version];
            std::array<uint8_t, RoundBitsToByte(moduleCount)> data{};
            std::array<uint8_t, RoundBitsToByte(moduleCount)> result{};
            std::array<uint8_t, BlockEccLen(Version, Ecc)> coeff{};

            BitBuffer buffer;
        };

        class QRCodeGenerator
        {
        public:
            template<uint8_t Version, QRCodeEcc ecc>
            struct ForVersionAndEcc;

            QRCodeGenerator(infra::Bitmap& modules, infra::Bitmap& isFunction, BitBuffer& codewords, infra::ByteRange alignPosition, uint8_t version, QRCodeEcc ecc);

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
            QRCodeEcc ecc;

            infra::Bitmap& modules;
            infra::Bitmap& isFunction;

            BitBuffer& codewords;

            infra::ByteRange alignPosition;
        };

        template<uint8_t Version, QRCodeEcc Ecc>
        struct QRCodeGenerator::ForVersionAndEcc
            : QRCodeGenerator
        {
            ForVersionAndEcc(infra::Bitmap& modules);

            typename QRCode<Version, Ecc>::Bitmap isFunction;
            BitBuffer::ForVersionAndEcc<Version, Ecc> codewords;
            std::array<uint8_t, Version / 7 + 2> alignPosition;
        };
    }

    //// Implementation ////

    namespace detail
    {
        template<uint8_t Version, QRCodeEcc Ecc>
        BitBuffer::ForVersionAndEcc<Version, Ecc>::ForVersionAndEcc()
            : buffer(data, result, coeff, Version, Ecc)
        {}

        template<uint8_t Version, QRCodeEcc Ecc>
        QRCodeGenerator::ForVersionAndEcc<Version, Ecc>::ForVersionAndEcc(infra::Bitmap& modules)
            : QRCodeGenerator(modules, isFunction, codewords.buffer, alignPosition, Version, Ecc)
        {
            isFunction.Clear();
        }
    }

    template<uint8_t Version, QRCodeEcc Ecc>
    QRCode<Version, Ecc>::QRCode(infra::BoundedConstString text)
    {
        bitmap.Clear();
        detail::QRCodeGenerator::ForVersionAndEcc<Version, Ecc> generator(bitmap);
        generator.Generate(text);
    }

    template<uint8_t Version, QRCodeEcc Ecc>
    const infra::Bitmap& QRCode<Version, Ecc>::GetBitmap() const
    {
        return bitmap;
    }
}

#endif
