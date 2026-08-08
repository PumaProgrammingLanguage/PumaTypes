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

#ifndef PUMA_FILE_TEXT_HPP
#define PUMA_FILE_TEXT_HPP

#pragma once

#include "../PumaType/String.hpp"
#include "../PumaType/Character.hpp"
#include <cstdio>
#include <cstdint>

namespace Puma {
namespace File
{
    class Text final
    {
    public:
        enum class OpenMode : std::uint8_t
        {
            // Open for reading
            READ,
            // Open for writing at end of file (appending or creating)
            WRITE,
            // Open for writing (overwriting existing file or creating)
            WRITE_NEW,
            // Open for reading and writing at end of file (appending or creating)
            READ_WRITE,
            // Open for reading and writing at beginning of file (replacing existing file)
            READ_WRITE_NEW
        };

        // Default constructor
        Text() noexcept;
        // Open file at path with mode (default is READ_WRITE)
        Text(const Type::String& path, OpenMode mode = OpenMode::READ_WRITE) noexcept;
        ~Text() noexcept;
        // Assign from another Text
        Text& operator=(Text&& other) noexcept;
        // Returns true if the file is open
        bool IsOpen() const noexcept;

        // Reads the next whitespace-delimited word from the file
        Type::String Read() noexcept;
        // Reads the next line from the file (newline excluded)
        Type::String ReadLn() noexcept;
        // Writes text to the file
        bool Write(const Type::String& text) noexcept;
        // Writes a single Character to the file
        bool Write(const Type::Character& ch) noexcept;
        // Writes text followed by a newline to the file
        bool WriteLn(const Type::String& text) noexcept;
        // Writes a single Character followed by a newline to the file
        bool WriteLn(const Type::Character& ch) noexcept;

    private:
        // Opens the file at path with mode (default is READ_WRITE)
        bool open(const Type::String& path, OpenMode mode) noexcept;
        // Closes the file if open
        void close() noexcept;

        std::FILE* handle;
        bool adjustPosition;
    };
} // namespace File
} // namespace Puma

#endif // PUMA_FILE_TEXT_HPP
