#include "preview/interfaces/QRCode.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace services
{
    namespace detail
    {
        static int8_t getAlphanumeric(char c)
        {
            if (c >= '0' && c <= '9')
                return (c - '0');
            if (c >= 'A' && c <= 'Z')
                return (c - 'A' + 10);

            switch (c)
            {
                case ' ':
                    return 36;
                case '$':
                    return 37;
                case '%':
                    return 38;
                case '*':
                    return 39;
                case '+':
                    return 40;
                case '-':
                    return 41;
                case '.':
                    return 42;
                case '/':
                    return 43;
                case ':':
                    return 44;
                default:
                    return -1;
            }
        }

        static bool isAlphanumeric(infra::BoundedConstString text)
        {
            for (auto c : text)
                if (getAlphanumeric(c) == -1)
                    return false;

            return true;
        }

        static bool isNumeric(infra::BoundedConstString text)
        {
            for (auto c : text)
                if (c < '0' || c > '9')
                    return false;

            return true;
        }

        // We store the following tightly packed (less 8) in modeInfo
        //               <=9  <=26  <= 40
        // NUMERIC      ( 10,   12,    14);
        // ALPHANUMERIC (  9,   11,    13);
        // BYTE         (  8,   16,    16);
        static char getModeBits(uint8_t version, uint8_t mode)
        {
            // Note: We use 15 instead of 16; since 15 doesn't exist and we cannot store 16 (8 + 8) in 3 bits
            // hex(int("".join(reversed([('00' + bin(x - 8)[2:])[-3:] for x in [10, 9, 8, 12, 11, 15, 14, 13, 15]])), 2))
            unsigned int modeInfo = 0x7bbb80a;

            if (version > 9)
                modeInfo >>= 9;

            if (version > 26)
                modeInfo >>= 9;

            char result = 8 + ((modeInfo >> (3 * mode)) & 0x07);

            if (result == 15)
                result = 16;

            return result;
        }

        BitBuffer::ReedSolomon::ReedSolomon(infra::ByteRange coeff)
            : coeff(coeff)
        {
            coeff.back() = 1;

            // Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{coeff.size()-1}),
            // drop the highest term, and store the rest of the coefficients in order of descending powers.
            // Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
            uint16_t root = 1;
            for (uint8_t i = 0; i < coeff.size(); i++)
            {
                // Multiply the current product by (x - r^i)
                for (uint8_t j = 0; j != coeff.size(); ++j)
                {
                    coeff[j] = Multiply(coeff[j], root);
                    if (j + 1 < coeff.size())
                        coeff[j] ^= coeff[j + 1];
                }
                root = (root << 1) ^ ((root >> 7) * 0x11D); // Multiply by 0x02 mod GF(2^8/0x11D)
            }
        }

        void BitBuffer::ReedSolomon::Remainder(uint8_t* data, uint8_t length, uint8_t* result, uint8_t stride) const
        {
            // Compute the remainder by performing polynomial division
            for (uint8_t i = 0; i != length; ++i)
            {
                uint8_t factor = data[i] ^ result[0];
                for (uint8_t j = 1; j != coeff.size(); ++j)
                    result[(j - 1) * stride] = result[j * stride];

                result[(coeff.size() - 1) * stride] = 0;

                for (uint8_t j = 0; j != coeff.size(); ++j)
                    result[j * stride] ^= Multiply(coeff[j], factor);
            }
        }

        uint8_t BitBuffer::ReedSolomon::Multiply(uint8_t x, uint8_t y) const
        {
            // Russian peasant multiplication
            // See: https://en.wikipedia.org/wiki/Ancient_Egyptian_multiplication
            uint16_t z = 0;
            for (int8_t i = 7; i >= 0; i--)
            {
                z = (z << 1) ^ ((z >> 7) * 0x11D);
                z ^= ((y >> i) & 1) * x;
            }
            return z;
        }

        BitBuffer::BitBuffer(infra::ByteRange data, infra::ByteRange result, infra::ByteRange coeff, uint8_t version, QRCodeEcc ecc)
            : version(version)
            , ecc(ecc)
            , moduleCount(numRawDataModulesForVersion[version])
            , data(data)
            , result(result)
            , reedSolomon(coeff)
        {}

        void BitBuffer::Generate(infra::BoundedConstString text)
        {
            uint16_t dataCapacity = moduleCount / 8 - numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];

            EncodeDataCodewords(text);

            // Add terminator and pad up to a byte if applicable
            uint32_t padding = std::min<uint32_t>((dataCapacity * 8) - bitOffset, 4);

            Append(0, padding);
            Append(0, (8 - bitOffset % 8) % 8);

            // Pad with alternate bytes until data capacity is reached
            for (uint8_t padByte = 0xEC; bitOffset != dataCapacity * 8; padByte ^= 0xEC ^ 0x11)
                Append(padByte, 8);

            PerformErrorCorrection(ecc);
        }

        uint16_t BitBuffer::Length() const
        {
            return bitOffset;
        }

        bool BitBuffer::Bit(uint16_t index) const
        {
            return ((result[index >> 3] >> (7 - (index & 7))) & 1) != 0;
        }

        void BitBuffer::Append(uint32_t val, uint8_t length)
        {
            for (int8_t i = length - 1; i >= 0; --i, ++bitOffset)
                data[bitOffset >> 3] |= ((val >> i) & 1) << (7 - (bitOffset & 7));
        }

        void BitBuffer::EncodeDataCodewords(infra::BoundedConstString text)
        {
#define MODE_NUMERIC 0
#define MODE_ALPHANUMERIC 1
#define MODE_BYTE 2

            if (isNumeric(text))
            {
                Append(1 << MODE_NUMERIC, 4);
                Append(text.size(), getModeBits(version, MODE_NUMERIC));

                uint16_t accumData = 0;
                uint8_t accumCount = 0;
                for (auto c : text)
                {
                    accumData = accumData * 10 + (c - '0');
                    ++accumCount;
                    if (accumCount == 3)
                    {
                        Append(accumData, 10);
                        accumData = 0;
                        accumCount = 0;
                    }
                }

                // 1 or 2 digits remaining
                if (accumCount > 0)
                    Append(accumData, accumCount * 3 + 1);
            }
            else if (isAlphanumeric(text))
            {
                Append(1 << MODE_ALPHANUMERIC, 4);
                Append(text.size(), getModeBits(version, MODE_ALPHANUMERIC));

                uint16_t accumData = 0;
                uint8_t accumCount = 0;
                for (auto c : text)
                {
                    accumData = accumData * 45 + getAlphanumeric(c);
                    ++accumCount;
                    if (accumCount == 2)
                    {
                        Append(accumData, 11);
                        accumData = 0;
                        accumCount = 0;
                    }
                }

                if (accumCount == 1)
                    Append(accumData, 6);
            }
            else
            {
                Append(1 << MODE_BYTE, 4);
                Append(text.size(), getModeBits(version, MODE_BYTE));
                for (auto c : text)
                    Append(c, 8);
            }
        }

        void BitBuffer::PerformErrorCorrection(QRCodeEcc ecc)
        {
            // See: http://www.thonky.com/qr-code-tutorial/structure-final-message
            uint8_t numBlocks = numErrorCorrectionBlocks[static_cast<uint8_t>(ecc)][version - 1];
            uint16_t totalEcc = numErrorCorrectionCodewords[static_cast<uint8_t>(ecc)][version - 1];

            uint8_t numShortBlocks = numBlocks - moduleCount / 8 % numBlocks;
            uint8_t shortBlockLen = moduleCount / 8 / numBlocks;

            uint8_t shortDataBlockLen = shortBlockLen - BlockEccLen(version, ecc);

            uint16_t offset = 0;
            uint8_t* dataBytes = data.begin();

            // Interleave all short blocks
            for (uint8_t i = 0; i != shortDataBlockLen; ++i)
            {
                uint16_t index = i;
                uint8_t stride = shortDataBlockLen;
                for (uint8_t blockNum = 0; blockNum != numBlocks; ++blockNum)
                {
                    result[offset++] = dataBytes[index];

                    if (blockNum == numShortBlocks)
                        ++stride;

                    index += stride;
                }
            }

            // Version less than 5 only have short blocks
            {
                // Interleave long blocks
                uint16_t index = shortDataBlockLen * (numShortBlocks + 1);
                uint8_t stride = shortDataBlockLen;
                for (uint8_t blockNum = 0; blockNum != numBlocks - numShortBlocks; ++blockNum)
                {
                    result[offset++] = dataBytes[index];

                    if (blockNum == 0)
                        ++stride;

                    index += stride;
                }
            }

            // Add all ecc blocks, interleaved
            uint8_t blockSize = shortDataBlockLen;
            for (uint8_t blockNum = 0; blockNum != numBlocks; ++blockNum)
            {

                if (blockNum == numShortBlocks)
                    ++blockSize;

                reedSolomon.Remainder(dataBytes, blockSize, &result[offset + blockNum], numBlocks);
                dataBytes += blockSize;
            }

            bitOffset = moduleCount;
        }

        void Invert(infra::Bitmap& bitmap, infra::Point position, bool invert)
        {
            if (invert)
                bitmap.SetBlackAndWhitePixel(position, !bitmap.BlackAndWhitePixel(position));
        }

#define PENALTY_N1 3
#define PENALTY_N2 3
#define PENALTY_N3 40
#define PENALTY_N4 10

        // Calculates and returns the penalty score based on state of this QR Code's current modules.
        // This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
        uint32_t PenaltyScore(const infra::Bitmap& bitmap)
        {
            uint32_t result = 0;

            uint8_t size = bitmap.size.deltaX;

            // Adjacent modules in row having same color
            for (uint8_t y = 0; y < size; ++y)
            {
                bool colorX = bitmap.BlackAndWhitePixel(infra::Point(0, y));
                for (uint8_t x = 1, runX = 1; x != size; ++x)
                {
                    bool cx = bitmap.BlackAndWhitePixel(infra::Point(x, y));
                    if (cx != colorX)
                    {
                        colorX = cx;
                        runX = 1;
                    }
                    else
                    {
                        ++runX;
                        if (runX == 5)
                            result += PENALTY_N1;
                        else if (runX > 5)
                            ++result;
                    }
                }
            }

            // Adjacent modules in column having same color
            for (uint8_t x = 0; x != size; ++x)
            {
                bool colorY = bitmap.BlackAndWhitePixel(infra::Point(x, 0));
                for (uint8_t y = 1, runY = 1; y != size; ++y)
                {
                    bool cy = bitmap.BlackAndWhitePixel(infra::Point(x, y));
                    if (cy != colorY)
                    {
                        colorY = cy;
                        runY = 1;
                    }
                    else
                    {
                        ++runY;
                        if (runY == 5)
                            result += PENALTY_N1;
                        else if (runY > 5)
                            ++result;
                    }
                }
            }

            uint16_t black = 0;
            for (uint8_t y = 0; y != size; ++y)
            {
                uint16_t bitsRow = 0, bitsCol = 0;
                for (uint8_t x = 0; x != size; ++x)
                {
                    bool color = bitmap.BlackAndWhitePixel(infra::Point(x, y));

                    // 2*2 blocks of modules having same color
                    if (x > 0 && y > 0)
                    {
                        bool colorUL = bitmap.BlackAndWhitePixel(infra::Point(x - 1, y - 1));
                        bool colorUR = bitmap.BlackAndWhitePixel(infra::Point(x, y - 1));
                        bool colorL = bitmap.BlackAndWhitePixel(infra::Point(x - 1, y));
                        if (color == colorUL && color == colorUR && color == colorL)
                            result += PENALTY_N2;
                    }

                    // Finder-like pattern in rows and columns
                    bitsRow = ((bitsRow << 1) & 0x7FF) | static_cast<int>(color);
                    bitsCol = ((bitsCol << 1) & 0x7FF) | static_cast<int>(bitmap.BlackAndWhitePixel(infra::Point(y, x)));

                    // Needs 11 bits accumulated
                    if (x >= 10)
                    {
                        if (bitsRow == 0x05D || bitsRow == 0x5D0)
                            result += PENALTY_N3;

                        if (bitsCol == 0x05D || bitsCol == 0x5D0)
                            result += PENALTY_N3;
                    }

                    // Balance of black and white modules
                    if (color)
                        ++black;
                }
            }

            // Find smallest k such that (45-5k)% <= dark/total <= (55+5k)%
            uint16_t total = size * size;
            for (uint16_t k = 0; black * 20 < (9 - k) * total || black * 20 > (11 + k) * total; k++)
                result += PENALTY_N4;

            return result;
        }

        detail::QRCodeGenerator::QRCodeGenerator(infra::Bitmap& modules, infra::Bitmap& isFunction, BitBuffer& codewords, infra::ByteRange alignPosition, uint8_t version, QRCodeEcc ecc)
            : version(version)
            , ecc(ecc)
            , modules(modules)
            , isFunction(isFunction)
            , codewords(codewords)
            , alignPosition(alignPosition)
        {}

        void QRCodeGenerator::Generate(infra::BoundedConstString text)
        {
            codewords.Generate(text);

            // Draw function patterns, draw all codewords, do masking
            DrawFunctionPatterns();
            DrawCodewords();

            // Find the best (lowest penalty) mask
            uint8_t mask = BestMask();

            // Overwrite old format bits
            DrawFormatBits(mask);

            // Apply the final choice of mask
            ApplyMask(mask);
        }

        uint8_t detail::QRCodeGenerator::BestMask()
        {
            uint8_t mask = 0;

            int32_t minPenalty = INT32_MAX;
            for (uint8_t i = 0; i != 8; ++i)
            {
                DrawFormatBits(i);
                ApplyMask(i);
                int penalty = PenaltyScore(modules);
                if (penalty < minPenalty)
                {
                    mask = i;
                    minPenalty = penalty;
                }
                ApplyMask(i); // Undoes the mask due to XOR
            }

            return mask;
        }

        // XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
        // properties, calling ApplyMask(m) twice with the same value is equivalent to no change at all.
        // This means it is possible to apply a mask, undo it, and try another mask. Note that a final
        // well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
        void detail::QRCodeGenerator::ApplyMask(uint8_t mask)
        {
            uint8_t size = modules.size.deltaX;

            for (auto position : infra::RowFirstPoints(infra::Region(infra::Point(), modules.size)))
            {
                if (isFunction.BlackAndWhitePixel(position))
                    continue;

                bool invert = 0;
                switch (mask)
                {
                    case 0:
                        invert = (position.x + position.y) % 2 == 0;
                        break;
                    case 1:
                        invert = position.y % 2 == 0;
                        break;
                    case 2:
                        invert = position.x % 3 == 0;
                        break;
                    case 3:
                        invert = (position.x + position.y) % 3 == 0;
                        break;
                    case 4:
                        invert = (position.x / 3 + position.y / 2) % 2 == 0;
                        break;
                    case 5:
                        invert = position.x * position.y % 2 + position.x * position.y % 3 == 0;
                        break;
                    case 6:
                        invert = (position.x * position.y % 2 + position.x * position.y % 3) % 2 == 0;
                        break;
                    case 7:
                        invert = ((position.x + position.y) % 2 + position.x * position.y % 3) % 2 == 0;
                        break;
                }

                Invert(modules, position, invert);
            }
        }

        void detail::QRCodeGenerator::DrawFunctionPatterns()
        {
            uint8_t size = modules.size.deltaX;

            // Draw the horizontal and vertical timing patterns
            for (uint8_t i = 0; i != size; ++i)
            {
                SetFunctionModule(infra::Point(6, i), i % 2 == 0);
                SetFunctionModule(infra::Point(i, 6), i % 2 == 0);
            }

            // Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
            DrawFinderPattern(infra::Point(3, 3));
            DrawFinderPattern(infra::Point(size - 4, 3));
            DrawFinderPattern(infra::Point(3, size - 4));

            if (version > 1)
            {
                // Draw the numerous alignment patterns

                uint8_t step;
                if (version != 32)
                    step = (version * 4 + alignPosition.size() * 2 + 1) / (2 * alignPosition.size() - 2) * 2;
                else
                    step = 26;

                uint8_t alignPositionIndex = alignPosition.size() - 1;

                alignPosition[0] = 6;

                uint8_t size = version * 4 + 17;
                for (uint8_t i = 0, pos = size - 7; i != alignPosition.size() - 1; ++i, pos -= step)
                    alignPosition[alignPositionIndex--] = pos;

                for (uint8_t i = 0; i != alignPosition.size(); ++i)
                    for (uint8_t j = 0; j != alignPosition.size(); ++j)
                        // Skip the three finder corners
                        if (!(i == 0 && j == 0) && !(i == 0 && j == alignPosition.size() - 1) && !(i == alignPosition.size() - 1 && j == 0))
                            DrawAlignmentPattern(infra::Point(alignPosition[i], alignPosition[j]));
            }

            // Draw configuration data
            DrawFormatBits(0); // Dummy mask value; overwritten later in the constructor
            DrawVersion();
        }

        // Draws two copies of the version bits (with its own error correction code),
        // based on this object's version field (which only has an effect for 7 <= version <= 40).
        void detail::QRCodeGenerator::DrawVersion()
        {
            int8_t size = modules.size.deltaX;

            if (version < 7)
                return;

            // Calculate error correction code and pack bits
            uint32_t rem = version; // version is uint6, in the range [7, 40]
            for (uint8_t i = 0; i != 12; ++i)
                rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);

            uint32_t data = version << 12 | rem;

            // Draw two copies
            for (uint8_t i = 0; i != 18; ++i)
            {
                bool bit = ((data >> i) & 1) != 0;
                uint8_t a = size - 11 + i % 3;
                uint8_t b = i / 3;
                SetFunctionModule(infra::Point(a, b), bit);
                SetFunctionModule(infra::Point(b, a), bit);
            }
        }

        // Draws a 9*9 finder pattern including the border separator, with the center module at (x, y).
        void detail::QRCodeGenerator::DrawFinderPattern(infra::Point position)
        {
            auto bitmapRegion = infra::Region(infra::Point(), modules.size);
            auto finderRegion = infra::Region(infra::Point(-4, -4), infra::Point(5, 5)) >> (position - infra::Point());

            for (auto i : infra::RowFirstPoints(infra::Intersection(finderRegion, bitmapRegion)))
            {
                auto dist = infra::ChebyshevDistance(i, position);
                SetFunctionModule(i, dist != 2 && dist != 4);
            }
        }

        // Draws a 5*5 alignment pattern, with the center module at (x, y).
        void detail::QRCodeGenerator::DrawAlignmentPattern(infra::Point position)
        {
            auto alignmentRegion = infra::Region(infra::Point(-2, -2), infra::Point(3, 3)) >> (position - infra::Point());

            for (auto i : infra::RowFirstPoints(alignmentRegion))
                SetFunctionModule(i, infra::ChebyshevDistance(i, position) != 1);
        }

        // Draws the given sequence of 8-bit codewords (data and error correction) onto the entire
        // data area of this QR Code symbol. Function modules need to be marked off before this is called.
        void detail::QRCodeGenerator::DrawCodewords()
        {
            uint8_t size = modules.size.deltaX;

            // Bit index into the data
            uint32_t i = 0;

            // Do the funny zigzag scan
            for (int16_t right = size - 1; right >= 1; right -= 2)
            { // Index of right column in each column pair
                if (right == 6)
                    right = 5;

                for (uint8_t vert = 0; vert != size; ++vert)
                { // Vertical counter
                    for (int j = 0; j != 2; ++j)
                    {
                        uint8_t x = right - j; // Actual x coordinate
                        bool upwards = ((right & 2) == 0) ^ (x < 6);
                        uint8_t y = upwards ? size - 1 - vert : vert; // Actual y coordinate
                        auto position = infra::Point(x, y);
                        if (!isFunction.BlackAndWhitePixel(position))
                        {
                            modules.SetBlackAndWhitePixel(position, codewords.Bit(i));
                            ++i;

                            if (i == codewords.Length())
                                return;
                        }
                        // If there are any remainder bits (0 to 7), they are already
                        // set to 0/false/white when the grid of modules was initialized
                    }
                }
            }
        }

        // Draws two copies of the format bits (with its own error correction code)
        // based on the given mask and this object's error correction level field.
        void detail::QRCodeGenerator::DrawFormatBits(uint8_t mask)
        {
            static constexpr std::array<uint8_t, 4> eccFormatBits{
                0x01, 0x00, 0x03, 0x02
            };

            uint8_t size = modules.size.deltaX;

            // Calculate error correction code and pack bits
            uint32_t data = eccFormatBits[static_cast<uint8_t>(ecc)] << 3 | mask; // ecc is uint2, mask is uint3
            uint32_t rem = data;
            for (int i = 0; i != 10; ++i)
                rem = (rem << 1) ^ ((rem >> 9) * 0x537);

            data = data << 10 | rem;
            data ^= 0x5412; // uint15

            // Draw first copy
            for (uint8_t i = 0; i != 6; ++i)
                SetFunctionModule(infra::Point(8, i), ((data >> i) & 1) != 0);

            SetFunctionModule(infra::Point(8, 7), ((data >> 6) & 1) != 0);
            SetFunctionModule(infra::Point(8, 8), ((data >> 7) & 1) != 0);
            SetFunctionModule(infra::Point(7, 8), ((data >> 8) & 1) != 0);

            for (int8_t i = 9; i != 15; ++i)
                SetFunctionModule(infra::Point(14 - i, 8), ((data >> i) & 1) != 0);

            // Draw second copy
            for (int8_t i = 0; i != 8; ++i)
                SetFunctionModule(infra::Point(size - 1 - i, 8), ((data >> i) & 1) != 0);

            for (int8_t i = 8; i != 15; ++i)
                SetFunctionModule(infra::Point(8, size - 15 + i), ((data >> i) & 1) != 0);

            SetFunctionModule(infra::Point(8, size - 8), true);
        }

        void detail::QRCodeGenerator::SetFunctionModule(infra::Point position, bool on)
        {
            modules.SetBlackAndWhitePixel(position, on);
            isFunction.SetBlackAndWhitePixel(position, true);
        }
    }
}
