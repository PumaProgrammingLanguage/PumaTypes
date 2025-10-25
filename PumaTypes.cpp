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
    String str;
    // private helpers (renamed to lowerCamelCase)
    bool String::isShort() const noexcept
    {
        return (str.shortStr.tag & SHORT_MASK) == 0;
    }

    bool String::isLong() const noexcept
    {
        return (str.longStr.tag & LONG_MASK) != 0;
    }

    std::uint32_t String::Size() const noexcept
    {
        return isShort()
            ? static_cast<std::uint32_t>(str.shortStr.tag & SIZE_MASK)
            : static_cast<std::uint32_t>(str.longStr.size);
    }

    String String::initialize(const char* cstr) noexcept
    {
        if (!cstr)
        {
            // Treat null as empty short string
            str.shortStr.tag = 0;
            return str;
        }
 
        const std::size_t len = std::strlen(cstr);

        if (len <= 15)
        {
            // Short string
            // MSB stays 0
            str.shortStr.tag = static_cast<std::uint8_t>(len & SIZE_MASK); 
            if (len > 0)
            {
                std::memcpy(str.shortStr.data, cstr, len);
            }
        }
        else
        {
            // Long string
            str.longStr.tag = LONG_MASK; // set MSB=1
            str.longStr.size = static_cast<std::uint32_t>(len);

            char* buf = new (std::nothrow) char[len]; // no terminator stored
            if (buf)
            {
                std::memcpy(buf, cstr, len);
                // Store pointer directly; width matches platform.
                str.longStr.ptr = buf;
            }
            else
            {
                // Allocation failed: fall back to empty short
                str.shortStr.tag = 0;
            }
        }
    }

    // delete the long string buffer if any
    // zero the long or short size
    void String::Finalize(bool isOwner) noexcept
    {
		if (isOwner && isLong() && str.longStr.ptr != nullptr)
        {
            delete[] str.longStr.ptr;
        }
        // Leave string in a benign state
		// Set to short empty string
        str.shortStr.tag = 0;
    }

    // Assignment: if owner, free current, then copy from source
    String String::operator=(String other) noexcept
    {
        if (other.isShort())
        {
			// short/long flag and size are in tag
            str.shortStr.tag = other.shortStr.tag;
            std::memcpy(str.shortStr.data, other.shortStr.data, other.shortStr.tag & SIZE_MASK);
			return str;
        }
        else
        {
            str.longStr.tag = other.longStr.tag;
            str.longStr.size = other.longStr.size;
            str.longStr.ptr = other.longStr.ptr;
			return str;
        }
    }
} // namespace Types
} // namespace Puma