#include "preview/interfaces/Hub75Bitmap.hpp"
#include "infra/util/BitLogic.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <bitset>

namespace infra
{
    Hub75Bitmap::Hub75Bitmap(infra::ByteRange buffer, infra::Vector size)
        : buffer(buffer)
        , size(size)
    {
        assert(buffer.size() == BufferSize(size.deltaX, size.deltaY));
    }

    void Hub75Bitmap::Clear()
    {
        std::fill(buffer.begin(), buffer.end(), 0);
    }

    uint32_t Hub75Bitmap::BufferSize(infra::Vector size)
    {
        return BufferSize(size.deltaX, size.deltaY);
    }

    uint32_t Hub75Bitmap::BufferSize()
    {
        return buffer.size();
    }

    bool Hub75Bitmap::operator==(const Hub75Bitmap& other) const
    {
        return other.size == size && other.buffer == buffer;
    }
}
