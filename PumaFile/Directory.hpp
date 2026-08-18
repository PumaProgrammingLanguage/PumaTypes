#ifndef PUMA_FILE_DIRECTORY_HPP
#define PUMA_FILE_DIRECTORY_HPP

#pragma once

#include "../PumaType/String.hpp"

namespace PumaFile
{
    namespace Directory
    {
        // Get current working directory - normalized to forward slashes
        PumaType::String GetCurrentDirectory() noexcept;

        // Set current working directory - accepts a Puma String path
        bool SetCurrentDirectory(const PumaType::String& path) noexcept;
    } // namespace Directory
} // namespace PumaFile

#endif // PUMA_FILE_DIRECTORY_HPP