// PumaLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <cstdint>
#include <cstddef>

namespace Puma {
namespace Types
{
    // UTF-8 encoded Unicode string representation.
    // String objects consist of a union of two records:
    //  - ShortString: first byte has MSB=0, next 3 bits reserved, low 4 bits = length (0..15), followed by 15 bytes of data.
    //  - LongString:  first byte has MSB=1, then 3 reserved bytes, 32-bit count, and a pointer to the data
    //                 (64-bit pointer on 64-bit, 32-bit pointer on 32-bit; the spec notes extra reserved bytes on 32-bit).
    // The string LongString points to is immutable; modifications are performed by creating a new string and assigning it to 
    // the pointer in one of the long strings..
#pragma pack(push, 1)

    struct ShortString
    {
        // Bit layout: [7]=0 (short marker), [6..4]=reserved, [3..0]=length (bytes in data[])
        std::uint8_t meta{0};
        char data[15]{0};

        // Helpers (implementation detail; optional to use)
        static constexpr std::uint8_t ShortMask = 0x80; // bit7
        static constexpr std::uint8_t LenMask   = 0x0F; // low 4 bits

        std::uint8_t length() const noexcept { return static_cast<std::uint8_t>(meta & LenMask); }
        void set_length(std::uint8_t len) noexcept { meta = static_cast<std::uint8_t>((meta & ~LenMask) | (len & LenMask)); }
        void mark_short() noexcept { meta = static_cast<std::uint8_t>(meta & ~ShortMask); }
        bool is_short() const noexcept { return (meta & ShortMask) == 0; }
    };

    struct LongString64
    {
        // MSB of 'tag' must be 1 to indicate long-string
        std::uint8_t tag{0x80};
        std::uint8_t reserved[3]{0};
        std::uint32_t count{0};   // number of bytes in UTF-8 string (excluding any terminator)
        std::uint64_t ptr{0};     // 64-bit pointer to immutable storage

        // Helpers to inspect and mutate metadata
        static constexpr std::uint8_t LongMask = 0x80; // bit7

        std::uint32_t length() const noexcept { return count; }
        void set_length(std::uint32_t len) noexcept { count = len; }
        void mark_long() noexcept { tag = static_cast<std::uint8_t>(tag | LongMask); }
        bool is_long() const noexcept { return (tag & LongMask) != 0; }
    };

    struct LongString32
    {
        // MSB of 'tag' must be 1 to indicate long-string
        std::uint8_t tag{0x80};
        std::uint8_t reserved[3]{0};
        std::uint32_t count{0};   // number of bytes in UTF-8 string (excluding any terminator)
        std::uint32_t reserved2{0};
        std::uint32_t ptr{0};     // 32-bit pointer to immutable storage
        // Spec note: could also be 4 reserved bytes followed by a 32-bit pointer.

        // Helpers to inspect and mutate metadata
        static constexpr std::uint8_t LongMask = 0x80; // bit7

        std::uint32_t length() const noexcept { return count; }
        void set_length(std::uint32_t len) noexcept { count = len; }
        void mark_long() noexcept { tag = static_cast<std::uint8_t>(tag | LongMask); }
        bool is_long() const noexcept { return (tag & LongMask) != 0; }
    };

    // Discriminated by the MSB of the first byte:
    //  - 0 => ShortString
    //  - 1 => LongString (size depends on target pointer width)
    union String
    {
        ShortString shortStr;
    #if INTPTR_MAX == INT64_MAX
        LongString64 longStr;
    #elif INTPTR_MAX == INT32_MAX
        LongString32 longStr;
    #else
    #error Unsupported pointer size
    #endif
        bool is_short() const noexcept { return shortStr.is_short(); }
        bool is_long() const noexcept { return longStr.is_long(); }
    };

#pragma pack(pop)
} // namespace Types
} // namespace Puma