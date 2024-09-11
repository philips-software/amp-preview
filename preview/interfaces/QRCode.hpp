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

class BitBucket
{
public:
    BitBucket(uint8_t size);

    void Set(infra::Point position, bool on);
    bool Get(infra::Point position) const;
    void Invert(infra::Point position, bool invert);

    uint32_t PenaltyScore() const;

public:
    std::uint8_t* buffer;
    infra::Bitmap bitmap;
};

class BitBuffer
{
private:
    static constexpr std::array<uint16_t, 41> numRawDataModulesForVersion{
        0, 208, 359, 567, 807, 1079, 1383, 1568, 1936, 2336, 2768, 3232, 3728, 4256, 4651, 5243, 5867, 6523,
        7211, 7931, 8683, 9252, 10068, 10916, 11796, 12708, 13652, 14628, 15371, 16411, 17483, 18587,
        19723, 20891, 22091, 23008, 24272, 25568, 26896, 28256, 29648
    };

    static constexpr uint16_t RoundBitsToByte(uint16_t bits)
    {
        return (bits + 7) / 8;
    }

public:
    BitBuffer(uint8_t version, infra::BoundedConstString text, uint8_t ecc);

    uint16_t Length() const;
    bool Bit(uint16_t index) const;

private:
    void Append(uint32_t val, uint8_t length);
    void EncodeDataCodewords(infra::BoundedConstString text);
    void PerformErrorCorrection(uint8_t ecc);

private:
    uint8_t version;
    uint16_t moduleCount;
    uint16_t bitOffset = 0;
    uint16_t capacityBytes;
    uint8_t* data;
    uint8_t* result;
};

class QRCode
{
public:
    static constexpr uint16_t GridSizeInBytes(uint8_t bits)
    {
        return (bits * bits + 7) / 8;
    }

    static constexpr uint16_t BufferSize(uint8_t version)
    {
        return GridSizeInBytes(4 * version + 17);
    }

public:
    enum class Ecc : uint8_t
    {
        low,
        medium,
        quartile,
        high
    };

public:
    QRCode(uint8_t version, Ecc ecc, infra::BoundedConstString text);

    const infra::Bitmap& GetBitmap() const;

private:
    class QRCodeGenerator
    {
    public:
        QRCodeGenerator(BitBucket& modules, uint8_t version, Ecc ecc, infra::BoundedConstString text);

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
        uint8_t size;
        uint8_t ecc;

        BitBucket& modules;
        BitBucket isFunction;

        BitBuffer codewords;

        uint8_t alignCount;
        uint8_t* alignPosition;
    };

public:
    BitBucket modules;
};

#endif
