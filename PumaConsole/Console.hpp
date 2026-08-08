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

#ifndef PUMA_CONSOLE_CONSOLE_HPP
#define PUMA_CONSOLE_CONSOLE_HPP

#pragma once

#include "../PumaType/String.hpp"
#include "../PumaType/Character.hpp"
#include <cstdint>

namespace Puma {
namespace Console
{
    // Initializes console IO (UTF-8, unsynced stdio)
    void Initialize() noexcept;

    // Restores console IO state changed during Initialize
    void Finalize() noexcept;

    // Writes a Puma String to standard output
    void Write(const Type::String& str) noexcept;

    // Writes a uint8_t string to standard output
    void Write(const uint8_t* str, const uint32_t size) noexcept;

    // Writes a C-string to standard output
    void Write(const char* cstr) noexcept;

    // Writes a single Puma Character to standard output
    void Write(const Type::Character& ch) noexcept;

    // Writes a Puma String to standard output followed by a newline
    void WriteLn(const Type::String& str) noexcept;

    // Writes a C-string to standard output followed by a newline
    void WriteLn(const char* cstr) noexcept;

    // Writes a single Puma Character followed by a newline
    void WriteLn(const Type::Character& ch) noexcept;

    // Flushes the standard output buffer
    void Flush() noexcept;

    // Reads the next whitespace-delimited token from standard input
    Type::String Read() noexcept;

    // Reads the next line from standard input (newline excluded)
    Type::String ReadLn() noexcept;

    class CommandPrompt
    {
    public:
        CommandPrompt() noexcept;
        ~CommandPrompt() noexcept;

    private:
        // Internal state
        bool m_visible;
    };
} // namespace Console
} // namespace Puma

#endif // PUMA_CONSOLE_CONSOLE_HPP