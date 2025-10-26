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
	// test for short string
    bool String::isShort() const noexcept
    {
        return (shortStr.tag & SHORT_MASK) == 0;
    }

	// test for long string
    bool String::isLong() const noexcept
    {
        return (longStr.tag & LONG_MASK) != 0;
    }

	// get length
    std::uint32_t String::Length() const noexcept
    {
        return isShort()
            ? static_cast<std::uint32_t>(shortStr.tag & LENGTH_MASK)
            : static_cast<std::uint32_t>(longStr.length);
    }

	// Initialize from C-style string or create empty string
    void String::Initialize(const char* cstr) noexcept
    {
        // zero the string - this creates an empty string
        str.firstHalf = 0;
        str.secondHalf = 0;

        if (!cstr)
        {
			// return empty string
            return;
        }
 
        const std::size_t length = std::strlen(cstr);

        if (length <= 15)
        {
            // Short string
            // MSB stays 0
            shortStr.tag = static_cast<std::uint8_t>(length & LENGTH_MASK);
            if (length > 0)
            {
                std::memcpy(shortStr.data, cstr, length);
            }
			// else empty string
        }
        else
        {
            // Long string
            longStr.tag = LONG_MASK; // set MSB=1
            longStr.length = static_cast<std::uint32_t>(length);

            char* buf = new (std::nothrow) char[length]; // no terminator stored
            if (buf)
            {
                std::memcpy(buf, cstr, length);
                // Store pointer directly; width matches platform.
                longStr.ptr = buf;
            }
            else
            {
                // Allocation failed: fall back to empty string
                str.firstHalf = 0;
                str.secondHalf = 0;
            }
        }

		return;
    }

    // delete the long string buffer if any
    // zero the long or short length
    void String::Finalize(bool isOwner) noexcept
    {
		if (isOwner && isLong() && longStr.ptr != nullptr)
        {
            delete[] longStr.ptr;
        }
		// Set to empty string
        str.firstHalf = 0;
		str.secondHalf = 0;
    }

    // Assignment
    // if lvalue is owner, call finalize before the assignment
    void String::operator=(String other) noexcept
    {
        str.firstHalf = other.str.firstHalf;
		str.secondHalf = other.str.secondHalf;
		return;
    }
} // namespace Types
} // namespace Puma