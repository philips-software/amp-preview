#ifndef PREVIEW_QR_CODE_HPP
#define PREVIEW_QR_CODE_HPP

#include "infra/util/BoundedString.hpp"
#include "infra/util/ByteRange.hpp"
#include "preview/interfaces/Bitmap.hpp"
#include <array>
#include <cstdint>

namespace services
{
    enum class QrCodeEcc : uint8_t
    {
        low,
        medium,
        quartile,
        high
    };

    template<uint8_t Version, QrCodeEcc Ecc>
    struct QrCode
        : infra::Bitmap::BlackAndWhite<Version * 4 + 17, Version * 4 + 17>
    {
        QrCode(infra::BoundedConstString text);

        void Update(infra::BoundedConstString text);

        static constexpr std::size_t MaxSizeNumeric();
        static constexpr std::size_t MaxSizeAlphanumeric();
        static constexpr std::size_t MaxSizeLatin1();

        using BitmapType = infra::Bitmap::BlackAndWhite<Version * 4 + 17, Version * 4 + 17>;
    };

    namespace detail
    {
        class TextEncoder
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

            static constexpr uint8_t BlockEccLen(uint8_t version, QrCodeEcc ecc)
            {
                uint8_t numBlocks = numErrorCorrectionBlocks[static_cast<uint8_t>(ecc)][version - 1];
                uint16_t totalEcc = numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];
                return totalEcc / numBlocks;
            }

            static constexpr uint8_t ModeBitsNumeric(uint8_t version)
            {
                if (version <= 9)
                    return 10;
                if (version <= 26)
                    return 12;
                return 14;
            }

            static constexpr uint8_t ModeBitsAlphanumeric(uint8_t version)
            {
                if (version <= 9)
                    return 9;
                if (version <= 26)
                    return 11;
                return 13;
            }

            static constexpr uint8_t ModeBitsLatin1(uint8_t version)
            {
                if (version <= 9)
                    return 8;
                if (version <= 26)
                    return 16;
                return 16;
            }

        public:
            static constexpr std::size_t MaxSizeNumeric(uint8_t version, QrCodeEcc ecc)
            {
                uint16_t moduleCount = numRawDataModulesForVersion[version];
                uint16_t dataCapacityInBytes = moduleCount / 8 - numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];
                uint16_t digitBits = dataCapacityInBytes * 8 - 4 - ModeBitsNumeric(version);

                uint16_t blocksOf3 = digitBits / 10;
                digitBits -= blocksOf3 * 10;

                return blocksOf3 * 3 + (digitBits >= 7 ? 2 : digitBits >= 4 ? 1
                                                                            : 0);
            }

            static constexpr std::size_t MaxSizeAlphanumeric(uint8_t version, QrCodeEcc ecc)
            {
                uint16_t moduleCount = numRawDataModulesForVersion[version];
                uint16_t dataCapacityInBytes = moduleCount / 8 - numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];
                uint16_t digitBits = dataCapacityInBytes * 8 - 4 - ModeBitsAlphanumeric(version);

                uint16_t blocksOf2 = digitBits / 11;
                digitBits -= blocksOf2 * 11;

                return blocksOf2 * 2 + digitBits / 6;
            }

            static constexpr std::size_t MaxSizeLatin1(uint8_t version, QrCodeEcc ecc)
            {
                uint16_t moduleCount = numRawDataModulesForVersion[version];
                uint16_t dataCapacityInBytes = moduleCount / 8 - numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];
                uint16_t digitBits = dataCapacityInBytes * 8 - 4 - ModeBitsLatin1(version);

                return digitBits / 8;
            }

        public:
            template<uint8_t Version, QrCodeEcc ecc>
            struct ForVersionAndEcc;

            TextEncoder(infra::ByteRange data, infra::ByteRange result, infra::ByteRange coeff, uint8_t version, QrCodeEcc ecc);

            void Encode(infra::BoundedConstString text);

            uint16_t Length() const;
            bool Bit(uint16_t index) const;

        private:
            static constexpr uint8_t numeric = 0;
            static constexpr uint8_t alphanumeric = 1;
            static constexpr uint8_t latin1 = 2;

        private:
            void Append(uint32_t val, uint8_t length);
            void EncodeDataCodewords(infra::BoundedConstString text);
            void EncodeNumeric(infra::BoundedConstString text);
            void EncodeAlphanumeric(infra::BoundedConstString text);
            void EncodeLatin1(infra::BoundedConstString text);
            void PerformErrorCorrection(QrCodeEcc ecc);

            static int8_t GetAlphanumeric(char c);
            static bool IsAlphanumeric(infra::BoundedConstString text);
            static bool IsNumeric(infra::BoundedConstString text);

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
            QrCodeEcc ecc;
            uint16_t moduleCount;
            uint16_t bitOffset = 0;
            infra::ByteRange data;
            infra::ByteRange result;
            ReedSolomon reedSolomon;
        };

        template<uint8_t Version, QrCodeEcc Ecc>
        struct TextEncoder::ForVersionAndEcc
        {
            ForVersionAndEcc();

            static constexpr uint16_t moduleCount = numRawDataModulesForVersion[Version];
            std::array<uint8_t, RoundBitsToByte(moduleCount)> data{};
            std::array<uint8_t, RoundBitsToByte(moduleCount)> result{};
            std::array<uint8_t, BlockEccLen(Version, Ecc)> coeff{};

            TextEncoder buffer;
        };

        class QrCodeGenerator
        {
        public:
            template<uint8_t Version, QrCodeEcc ecc>
            struct ForVersionAndEcc;

            QrCodeGenerator(infra::Bitmap& modules, infra::Bitmap& isFunction, TextEncoder& encoder, infra::ByteRange alignPosition, uint8_t version, QrCodeEcc ecc);

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
            QrCodeEcc ecc;

            infra::Bitmap& modules;
            infra::Bitmap& isFunction;

            TextEncoder& encoder;

            infra::ByteRange alignPosition;
        };

        template<uint8_t Version, QrCodeEcc Ecc>
        struct QrCodeGenerator::ForVersionAndEcc
            : QrCodeGenerator
        {
            ForVersionAndEcc(infra::Bitmap& modules);

            typename QrCode<Version, Ecc>::BitmapType isFunction;
            TextEncoder::ForVersionAndEcc<Version, Ecc> encoder;
            std::array<uint8_t, Version / 7 + 2> alignPosition;
        };
    }

    //// Implementation ////

    namespace detail
    {
        template<uint8_t Version, QrCodeEcc Ecc>
        TextEncoder::ForVersionAndEcc<Version, Ecc>::ForVersionAndEcc()
            : buffer(data, result, coeff, Version, Ecc)
        {}

        template<uint8_t Version, QrCodeEcc Ecc>
        QrCodeGenerator::ForVersionAndEcc<Version, Ecc>::ForVersionAndEcc(infra::Bitmap& modules)
            : QrCodeGenerator(modules, isFunction, encoder.buffer, alignPosition, Version, Ecc)
        {
            isFunction.Clear();
        }
    }

    template<uint8_t Version, QrCodeEcc Ecc>
    QrCode<Version, Ecc>::QrCode(infra::BoundedConstString text)
    {
        Update(text);
    }

    template<uint8_t Version, QrCodeEcc Ecc>
    void QrCode<Version, Ecc>::Update(infra::BoundedConstString text)
    {
        this->Clear();
        detail::QrCodeGenerator::ForVersionAndEcc<Version, Ecc> generator(*this);
        generator.Generate(text);
    }

    template<uint8_t Version, QrCodeEcc Ecc>
    constexpr std::size_t QrCode<Version, Ecc>::MaxSizeNumeric()
    {
        return detail::TextEncoder::MaxSizeNumeric(Version, Ecc);
    }

    template<uint8_t Version, QrCodeEcc Ecc>
    constexpr std::size_t QrCode<Version, Ecc>::MaxSizeAlphanumeric()
    {
        return detail::TextEncoder::MaxSizeAlphanumeric(Version, Ecc);
    }

    template<uint8_t Version, QrCodeEcc Ecc>
    constexpr std::size_t QrCode<Version, Ecc>::MaxSizeLatin1()
    {
        return detail::TextEncoder::MaxSizeLatin1(Version, Ecc);
    }
}

#endif
