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

#ifndef PUMA_FILE_DIRECTORY_HPP
#define PUMA_FILE_DIRECTORY_HPP

#pragma once

#include "../PumaType/String.hpp"

namespace Puma {
namespace File
{
    namespace Directory
    {
        // Get current working directory - normalized to forward slashes
        Type::String GetCurrentDirectory() noexcept;

        // Set current working directory - accepts a Puma String path
        bool SetCurrentDirectory(const Type::String& path) noexcept;
    } // namespace Directory
} // namespace File
} // namespace Puma

#endif // PUMA_FILE_DIRECTORY_HPP