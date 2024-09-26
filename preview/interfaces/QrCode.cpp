#include "preview/interfaces/QRCode.hpp"
#include <algorithm>
#include <cassert>
#include <limits>

namespace services
{
    namespace detail
    {
        TextEncoder::TextEncoder(infra::ByteRange data, infra::ByteRange result, infra::ByteRange coeff, uint8_t version, QrCodeEcc ecc)
            : version(version)
            , ecc(ecc)
            , moduleCount(numRawDataModulesForVersion[version])
            , data(data)
            , result(result)
            , reedSolomon(coeff)
        {}

        void TextEncoder::Encode(infra::BoundedConstString text)
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

        uint16_t TextEncoder::Length() const
        {
            return bitOffset;
        }

        bool TextEncoder::Bit(uint16_t index) const
        {
            return ((result[index >> 3] >> (7 - (index & 7))) & 1) != 0;
        }

        void TextEncoder::Append(uint32_t val, uint8_t length)
        {
            for (int8_t i = length - 1; i >= 0; --i, ++bitOffset)
                data[bitOffset >> 3] |= ((val >> i) & 1) << (7 - (bitOffset & 7));
        }

        void TextEncoder::EncodeDataCodewords(infra::BoundedConstString text)
        {
            if (IsNumeric(text))
                EncodeNumeric(text);
            else if (IsAlphanumeric(text))
                EncodeAlphanumeric(text);
            else
                EncodeLatin1(text);
        }

        void TextEncoder::EncodeNumeric(infra::BoundedConstString text)
        {
            assert(text.size() <= MaxSizeNumeric(version, ecc));
            Append(1 << numeric, 4);
            Append(text.size(), ModeBitsNumeric(version));

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

        void TextEncoder::EncodeAlphanumeric(infra::BoundedConstString text)
        {
            assert(text.size() <= MaxSizeAlphanumeric(version, ecc));
            Append(1 << alphanumeric, 4);
            Append(text.size(), ModeBitsAlphanumeric(version));

            uint16_t accumData = 0;
            uint8_t accumCount = 0;
            for (auto c : text)
            {
                accumData = accumData * 45 + GetAlphanumeric(c);
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

        void TextEncoder::EncodeLatin1(infra::BoundedConstString text)
        {
            assert(text.size() <= MaxSizeLatin1(version, ecc));
            Append(1 << latin1, 4);
            Append(text.size(), ModeBitsLatin1(version));
            for (auto c : text)
                Append(c, 8);
        }

        void TextEncoder::PerformErrorCorrection(QrCodeEcc ecc)
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

            // Interleave long blocks (versions less than 5 only have short blocks)
            uint16_t index = shortDataBlockLen * (numShortBlocks + 1);
            uint8_t stride = shortDataBlockLen;
            for (uint8_t blockNum = 0; blockNum != numBlocks - numShortBlocks; ++blockNum)
            {
                result[offset++] = dataBytes[index];

                if (blockNum == 0)
                    ++stride;

                index += stride;
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

        int8_t TextEncoder::GetAlphanumeric(char c)
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

        bool TextEncoder::IsAlphanumeric(infra::BoundedConstString text)
        {
            for (auto c : text)
                if (GetAlphanumeric(c) == -1)
                    return false;

            return true;
        }

        bool TextEncoder::IsNumeric(infra::BoundedConstString text)
        {
            for (auto c : text)
                if (c < '0' || c > '9')
                    return false;

            return true;
        }

        TextEncoder::ReedSolomon::ReedSolomon(infra::ByteRange coeff)
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

        void TextEncoder::ReedSolomon::Remainder(uint8_t* data, uint8_t length, uint8_t* result, uint8_t stride) const
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

        uint8_t TextEncoder::ReedSolomon::Multiply(uint8_t x, uint8_t y) const
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

        uint32_t PenaltyScoreRowRuns(const infra::Bitmap& bitmap)
        {
            uint32_t penalty = 0;

            for (uint8_t y = 0; y != bitmap.size.deltaY; ++y)
            {
                bool runColour = bitmap.BlackAndWhitePixel(infra::Point(0, y));
                uint8_t run = 1;
                for (uint8_t x = 1; x != bitmap.size.deltaX; ++x)
                {
                    bool colour = bitmap.BlackAndWhitePixel(infra::Point(x, y));
                    if (colour != runColour)
                    {
                        runColour = colour;
                        run = 1;
                    }
                    else
                    {
                        ++run;
                        if (run == 5)
                            penalty += 3;
                        else if (run > 5)
                            ++penalty;
                    }
                }
            }

            return penalty;
        }

        uint32_t PenaltyScoreColumnRuns(const infra::Bitmap& bitmap)
        {
            uint32_t penalty = 0;

            for (uint8_t x = 0; x != bitmap.size.deltaX; ++x)
            {
                bool runColour = bitmap.BlackAndWhitePixel(infra::Point(x, 0));
                uint8_t run = 1;
                for (uint8_t y = 1; y != bitmap.size.deltaY; ++y)
                {
                    bool colour = bitmap.BlackAndWhitePixel(infra::Point(x, y));
                    if (colour != runColour)
                    {
                        runColour = colour;
                        run = 1;
                    }
                    else
                    {
                        ++run;
                        if (run == 5)
                            penalty += 3;
                        else if (run > 5)
                            ++penalty;
                    }
                }
            }

            return penalty;
        }

        uint32_t PenaltyScoreBlocks(const infra::Bitmap& bitmap)
        {
            uint32_t penalty = 0;

            for (uint8_t y = 0; y != bitmap.size.deltaY; ++y)
                for (uint8_t x = 0; x != bitmap.size.deltaX; ++x)
                {
                    bool color = bitmap.BlackAndWhitePixel(infra::Point(x, y));

                    // 2*2 blocks of modules having same color
                    if (x > 0 && y > 0)
                    {
                        bool colorUL = bitmap.BlackAndWhitePixel(infra::Point(x - 1, y - 1));
                        bool colorUR = bitmap.BlackAndWhitePixel(infra::Point(x, y - 1));
                        bool colorL = bitmap.BlackAndWhitePixel(infra::Point(x - 1, y));
                        if (color == colorUL && color == colorUR && color == colorL)
                            penalty += 3;
                    }
                }

            return penalty;
        }

        uint32_t PenaltyScoreFinderLike(const infra::Bitmap& bitmap)
        {
            uint32_t penalty = 0;

            for (uint8_t y = 0; y != bitmap.size.deltaY; ++y)
            {
                uint16_t bitsRow = 0;
                uint16_t bitsCol = 0;
                for (uint8_t x = 0; x != bitmap.size.deltaX; ++x)
                {
                    // Finder-like pattern in rows and columns
                    bitsRow = ((bitsRow << 1) & 0x7FF) | static_cast<int>(bitmap.BlackAndWhitePixel(infra::Point(x, y)));
                    bitsCol = ((bitsCol << 1) & 0x7FF) | static_cast<int>(bitmap.BlackAndWhitePixel(infra::Point(y, x)));

                    // Needs 11 bits accumulated
                    if (x >= 10)
                    {
                        if (bitsRow == 0x05D || bitsRow == 0x5D0)
                            penalty += 40;

                        if (bitsCol == 0x05D || bitsCol == 0x5D0)
                            penalty += 40;
                    }
                }
            }

            return penalty;
        }

        uint32_t PenaltyScoreBalance(const infra::Bitmap& bitmap)
        {
            uint32_t penalty = 0;

            uint16_t numDark = 0;
            for (uint8_t y = 0; y != bitmap.size.deltaY; ++y)
                for (uint8_t x = 0; x != bitmap.size.deltaX; ++x)
                    if (bitmap.BlackAndWhitePixel(infra::Point(x, y)))
                        ++numDark;

            // Find smallest k such that (45-5k)% <= dark/total <= (55+5k)%
            uint16_t total = bitmap.size.deltaX * bitmap.size.deltaY;
            for (uint16_t k = 0; numDark * 20 < (9 - k) * total || numDark * 20 > (11 + k) * total; ++k)
                penalty += 10;

            return penalty;
        }

        // Calculates and returns the penalty score based on the state of this QR Code's current modules.
        // This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
        uint32_t PenaltyScore(const infra::Bitmap& bitmap)
        {
            return PenaltyScoreRowRuns(bitmap) + PenaltyScoreColumnRuns(bitmap) + PenaltyScoreBlocks(bitmap) + PenaltyScoreFinderLike(bitmap) + PenaltyScoreBalance(bitmap);
        }

        QrCodeGenerator::QrCodeGenerator(infra::Bitmap& modules, infra::Bitmap& isFunction, TextEncoder& encoder, infra::ByteRange alignPosition, uint8_t version, QrCodeEcc ecc)
            : version(version)
            , ecc(ecc)
            , modules(modules)
            , isFunction(isFunction)
            , encoder(encoder)
            , alignPosition(alignPosition)
        {}

        void QrCodeGenerator::Generate(infra::BoundedConstString text)
        {
            encoder.Encode(text);

            DrawFunctionPatterns();
            DrawCodewords();

            uint8_t mask = BestMask();
            DrawFormatBits(mask);
            ApplyMask(mask);
        }

        uint8_t QrCodeGenerator::BestMask()
        {
            uint8_t mask = 0;

            int32_t minPenalty = std::numeric_limits<int32_t>::max();
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
                ApplyMask(i); // Removes the mask due to XOR
            }

            return mask;
        }

        // XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
        // properties, calling ApplyMask(m) twice with the same value is equivalent to no change at all.
        // This means it is possible to apply a mask, undo it, and try another mask. Note that a final
        // well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
        void QrCodeGenerator::ApplyMask(uint8_t mask)
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

                if (invert)
                    modules.SetBlackAndWhitePixel(position, !modules.BlackAndWhitePixel(position));
            }
        }

        void QrCodeGenerator::DrawFunctionPatterns()
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
        void QrCodeGenerator::DrawVersion()
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
        void QrCodeGenerator::DrawFinderPattern(infra::Point position)
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
        void QrCodeGenerator::DrawAlignmentPattern(infra::Point position)
        {
            auto alignmentRegion = infra::Region(infra::Point(-2, -2), infra::Point(3, 3)) >> (position - infra::Point());

            for (auto i : infra::RowFirstPoints(alignmentRegion))
                SetFunctionModule(i, infra::ChebyshevDistance(i, position) != 1);
        }

        // Draws the given sequence of 8-bit codewords (data and error correction) onto the entire
        // data area of this QR Code symbol. Function modules need to be marked off before this is called.
        void QrCodeGenerator::DrawCodewords()
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
                            modules.SetBlackAndWhitePixel(position, encoder.Bit(i));
                            ++i;

                            if (i == encoder.Length())
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
        void QrCodeGenerator::DrawFormatBits(uint8_t mask)
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

        void QrCodeGenerator::SetFunctionModule(infra::Point position, bool on)
        {
            modules.SetBlackAndWhitePixel(position, on);
            isFunction.SetBlackAndWhitePixel(position, true);
        }
    }
}
