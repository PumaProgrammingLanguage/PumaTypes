// Standard library for the Puma programming language
//   
// Copyright © 2026 by Darryl Anthony Burchfield
//
//   Licensed under the Apache License, Version 2.0 (the "License") WITH LLVM-exception;
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "pch.h"
#include "framework.h"
#include "Character.hpp"
#include "String.hpp" // needed for Character::ToString()
#include <cstddef>
#include <cstring>

namespace Puma {
namespace Type
{
    namespace
    {
        constexpr uint8_t UTF8CharSizeLookup[32] =
        {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3,
            4,
            1
        };
    }

    Character::Character() noexcept
        : codePoint(0U)
    {
    }

    Character::Character(const Character& source) noexcept
        : codePoint(source.codePoint)
    {
    }

    Character::Character(const uint8_t* utf8) noexcept
    : codePoint(0U)
    {
        if (utf8 == nullptr)
        {
            return;
        }

        const uint8_t charSize = GetCharSize(utf8[0]); // 1..4

        // Copy up to 4 bytes
        std::memcpy(codeUnits, utf8, charSize);
    }

    Character::~Character() noexcept = default;

    Character& Character::operator=(const Character& source) noexcept
    {
        if (this != &source)
        {
            codePoint = source.codePoint;
        }
        return *this;
    }

    // Less-than operator for ordering (e.g., for sorting)
    bool Character::operator<(const Character& other) const noexcept
	{
	    // Compare whole character
		return codePoint < other.codePoint;
	}
    // Greater-than operator for ordering
    bool Character::operator>(const Character& other) const noexcept
    {
		return codePoint > other.codePoint;
    }
    // Equality
    bool Character::operator==(const Character& other) const noexcept
    {
        return codePoint == other.codePoint;
    }
    // Inequality
    bool Character::operator!=(const Character& other) const noexcept
    {
        return codePoint != other.codePoint;
    }
    // Less-than-or-equal operator for ordering
    bool Character::operator<=(const Character& other) const noexcept
    {
        return codePoint <= other.codePoint;
    }
    // Greater-than-or-equal operator for ordering
    bool Character::operator>=(const Character& other) const noexcept
    {
        return codePoint >= other.codePoint;
    }

    String Character::ToString() const noexcept
    {
        const uint8_t charSize = GetCharSize(codeUnits[0]); // 1..4
        return String(&codeUnits[0], (uint32_t)charSize);
    }

    const uint8_t* Character::ToUTF8() const noexcept
    {
        return codeUnits;
    }

    const uint8_t Character::GetCharSize() const noexcept
    {
        return UTF8CharSizeLookup[codeUnits[0] >> 3];
    }

    const uint8_t Character::GetCharSize(const uint8_t firstCodeUnit) noexcept
    {
        return UTF8CharSizeLookup[firstCodeUnit >> 3];
    }
} // namespace Type
} // namespace Puma
