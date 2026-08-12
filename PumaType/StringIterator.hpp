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

#ifndef PUMA_TYPE_STRINGITERATOR_HPP
#define PUMA_TYPE_STRINGITERATOR_HPP

#pragma once

#include "Character.hpp"
#include <cstdint>

using namespace std;

namespace PumaType
{
    struct StringIterator
    {
    public:
        // Constructors
        StringIterator() noexcept;
        StringIterator(const uint8_t* current, const uint8_t* limit) noexcept;
        StringIterator(const StringIterator& other) noexcept;
        StringIterator(const String& str) noexcept;

        // Assignment
        StringIterator& operator=(const StringIterator& other) noexcept;
        StringIterator& operator=(const uint8_t* ptr) noexcept;

        // Dereference - returns current UTF-8 code unit pointer
        const Character operator*() const noexcept;

        //// Add raw byte offset (no UTF-8 awareness, just pointer math)
        //StringIterator operator+(std::uint32_t offset) const noexcept;

        // Prefix increment / decrement - move by one UTF-8 character
        StringIterator& operator++() noexcept;
        StringIterator& operator--() noexcept;

        // Comparison operators
        bool operator==(const StringIterator& other) const noexcept;
        bool operator!=(const StringIterator& other) const noexcept;

        // Check validity
        bool IsValid() const noexcept;

    private:
        const uint8_t* _current;
        const uint8_t* _limit;
    };

} // namespace PumaType

#endif // PUMA_TYPE_STRINGITERATOR_HPP
