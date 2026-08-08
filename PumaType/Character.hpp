// Standard library for the Puma programming language
//   
// Copyright © 2026 by Darryl Anthony Burchfield
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef PUMA_TYPE_CHARACTER_HPP
#define PUMA_TYPE_CHARACTER_HPP

#pragma once

#include <cstdint>

namespace Puma {
namespace Type
{
    // Forward declaration to avoid circular include with String.hpp
    union String;

    // Represents a UTF-8 character (code point) as a sequence of up to 4 bytes.
    union Character
    {
    public:
        // Lifetime
        Character() noexcept;
        Character(const Character& source) noexcept;
        Character(const uint8_t* utf8) noexcept;
        ~Character() noexcept;

        // Assignment
        Character& operator=(const Character& source) noexcept;

		// Less-than operator for ordering (e.g., for sorting)
		bool operator<(const Character& other) const noexcept;
		// Less-than-or-equal operator for ordering
		bool operator<=(const Character& other) const noexcept;
        // Equality
        bool operator==(const Character& other) const noexcept;
		// Inequality
		bool operator!=(const Character& other) const noexcept;
		// Greater-than-or-equal operator for ordering
		bool operator>=(const Character& other) const noexcept;
		// Greater-than operator for ordering
		bool operator>(const Character& other) const noexcept;

        // Convert this UTF-8 character into a Puma String.
        String ToString() const noexcept;

        // Get pointer to the UTF-8 code unit.
        const uint8_t* ToUTF8() const noexcept;

        // Returns the number of bytes in the UTF‑8 code unit sequence stored in this Character.
        const uint8_t GetCharSize() const noexcept;

        // Returns the number of bytes in the UTF‑8 code unit sequence starting with 'firstCodeUnit'.
        // Invalid leading bytes and continuation bytes return 1.
        static const uint8_t GetCharSize(const uint8_t firstCodeUnit)  noexcept;

    private:
        // Raw 4-byte representation (e.g., UTF-8 bytes)
        uint8_t  codeUnits[4];
        // Packed 32-bit representation of the same 4 bytes
        uint32_t codePoint;
    };
} // namespace Type
} // namespace Puma

#endif // PUMA_TYPE_CHARACTER_HPP
