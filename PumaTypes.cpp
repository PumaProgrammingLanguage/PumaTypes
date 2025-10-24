// PumaTypes.cpp : Defines the functions for the static library.
//
#include "pch.h"
#include "framework.h"
#include "PumaTypes.hpp"
#include <cstring>
#include <new>

namespace Puma {
namespace Types
{
    // private helpers (renamed to lowerCamelCase)
    bool String::isShort() const noexcept
    {
        return (shortStr.tag & SHORT_MASK) == 0;
    }

    bool String::isLong() const noexcept
    {
        return (longStr.tag & LONG_MASK) != 0;
    }

    std::size_t String::Size() const noexcept
    {
        return isShort()
            ? static_cast<std::size_t>(shortStr.tag & SIZE_MASK)
            : static_cast<std::size_t>(longStr.size);
    }

    String::String(const char* cstr) noexcept
    {
		initialize(cstr);
    }

	void String::initialize(const char* cstr) noexcept
    {
        if (!cstr)
        {
            // Treat null as empty short string
            shortStr.tag = 0;
            std::memset(shortStr.data, 0, sizeof(shortStr.data));
            return;
        }

        const std::size_t len = std::strlen(cstr);

        if (len <= 15)
        {
            // Short string
            shortStr.tag = static_cast<std::uint8_t>(len & SIZE_MASK); // MSB stays 0
            std::memset(shortStr.data, 0, sizeof(shortStr.data));
            if (len > 0)
            {
                std::memcpy(shortStr.data, cstr, len);
            }
        }
        else
        {
            // Long string
            longStr.tag = LONG_MASK; // set MSB=1
            longStr.size = static_cast<std::uint32_t>(len);
            longStr.reserved[0] = longStr.reserved[1] = longStr.reserved[2] = 0;
        #if INTPTR_MAX == INT32_MAX
            longStr.reserved2 = 0;
        #endif

            char* buf = new (std::nothrow) char[len]; // no terminator stored
            if (buf)
            {
                std::memcpy(buf, cstr, len);
                // Store pointer directly; width matches platform.
                longStr.ptr = buf;
            }
            else
            {
                // Allocation failed: fall back to empty short
                shortStr.tag = 0;
                std::memset(shortStr.data, 0, sizeof(shortStr.data));
            }
        }
    }

    // Destructor: free heap storage for long strings if owned
    String::~String() noexcept
    {
        Finalize();
    }

    // delete the long string buffer if any
    // zero the long or short size
    void String::Finalize() noexcept
    {
        if (isLong())
        {

            if (longStr.ptr != nullptr)
            {
                delete[] longStr.ptr;
                longStr.ptr = nullptr;
            }
            // Leave union in a benign state
            longStr.size = 0;
            longStr.tag = 0;
        }
        else
        {
            // Leave union in a benign state
            shortStr.tag = 0;
        }
    }

    // Move constructor: steal pointer/metadata; null out source
    String::String(String&& other) noexcept
    {
        if (other.isShort())
        {
            shortStr.tag = other.shortStr.tag;
            std::memcpy(shortStr.data, other.shortStr.data, sizeof(shortStr.data));
        }
        else
        {
            longStr.tag = other.longStr.tag;
            longStr.reserved[0] = other.longStr.reserved[0];
            longStr.reserved[1] = other.longStr.reserved[1];
            longStr.reserved[2] = other.longStr.reserved[2];
            longStr.size = other.longStr.size;
        #if INTPTR_MAX == INT32_MAX
            longStr.reserved2 = other.longStr.reserved2;
        #endif
            longStr.ptr = other.longStr.ptr;

            // Release source ownership
            other.longStr.ptr = nullptr;
            other.longStr.size = 0;
            other.longStr.tag = 0;
        }
    }

    // Move assignment: free current, then steal from source; null out source
    String& String::operator=(String&& other) noexcept
    {
        if (this == &other)
            return *this;

        // Release current owned buffer if any
        if (isLong() && longStr.ptr != nullptr)
        {
            delete[] longStr.ptr;
            longStr.ptr = nullptr;
            longStr.size = 0;
            longStr.tag = 0;
        }

        if (other.isShort())
        {
            shortStr.tag = other.shortStr.tag;
            std::memcpy(shortStr.data, other.shortStr.data, sizeof(shortStr.data));
        }
        else
        {
            longStr.tag = other.longStr.tag;
            longStr.reserved[0] = other.longStr.reserved[0];
            longStr.reserved[1] = other.longStr.reserved[1];
            longStr.reserved[2] = other.longStr.reserved[2];
            longStr.size = other.longStr.size;
        #if INTPTR_MAX == INT32_MAX
            longStr.reserved2 = other.longStr.reserved2;
        #endif
            longStr.ptr = other.longStr.ptr;

            // Release source ownership
            other.longStr.ptr = nullptr;
            other.longStr.size = 0;
            other.longStr.tag = 0;
        }

        return *this;
    }
} // namespace Types
} // namespace Puma