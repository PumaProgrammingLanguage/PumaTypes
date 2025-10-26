#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <new>

namespace Puma {
namespace Types
{
#pragma pack(push, 1)
    union String
    {
    public:
        // Public API
        std::uint32_t Length() const noexcept;

        // String factory, Initializes from a C-style string and returns the initialized string.
        // if cstr is null, return empty string
        void Initialize(const char* cstr = NULL) noexcept;

        // If is owner, releases heap storage for long strings.
        // Set to empty string
        void Finalize(bool isOwner = false) noexcept;

        // Assignment
        // if lvalue is owner, call finalize before the assignment
        void operator=(String other) noexcept;



    private:
        // Layout (private)
        struct { std::uint8_t tag; char data[15]; } shortStr;

    #if INTPTR_MAX == INT64_MAX
        struct { std::uint8_t tag; std::uint8_t reserved[3]; std::uint32_t length; const char* ptr; } longStr;
    #elif INTPTR_MAX == INT32_MAX
        struct { std::uint8_t tag; std::uint8_t reserved[3]; std::uint32_t length; std::uint32_t reserved2; const char* ptr; } longStr;
    #else
    #error Unsupported pointer length
    #endif
		// copy union
        struct { std::uint64_t firstHalf; std::uint64_t secondHalf; } str;

        // Masks (private) - UPPER_CASE
        static constexpr std::uint8_t SHORT_MASK = 0x80;
        static constexpr std::uint8_t LENGTH_MASK  = 0x0F;
        static constexpr std::uint8_t LONG_MASK  = 0x80;

        // Helpers (private) - lowerCamelCase
        bool isShort() const noexcept;
        bool isLong()  const noexcept;
    };
#pragma pack(pop)

} // namespace Types
} // namespace Puma