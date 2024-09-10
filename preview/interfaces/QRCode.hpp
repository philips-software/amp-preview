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
    template<uint8_t version>
    using Version = infra::WithStorage<QRCode, std::array<uint8_t, BufferSize(version)>>;

    QRCode(infra::ByteRange modules, uint8_t version, Ecc ecc, infra::BoundedConstString text);
    bool getModule(uint8_t x, uint8_t y) const;

public:
    uint8_t version;
    uint8_t size;
    Ecc ecc;
    infra::ByteRange modules;
};

#endif
