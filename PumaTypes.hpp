#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <new>

namespace Puma {
namespace Types
{
    // Public String type; internal layout details are not exposed as separate types.
    // Short-string and long-string representations are defined as anonymous structs
    // within the union, so ShortString/LongString types are not visible to users.

#pragma pack(push, 1)
    union String
    {
        // Short string: MSB of tag = 0, next 3 bits reserved, low 4 bits = length, then 15 bytes payload
        struct
        {
            std::uint8_t tag;
            char data[15];
        } shortStr;

    #if INTPTR_MAX == INT64_MAX
        // Long string (64-bit): MSB of tag = 1, 3 reserved bytes, 32-bit bytecount, 64-bit pointer
        struct
        {
            std::uint8_t tag;
            std::uint8_t reserved[3];
            std::uint32_t bytecount;
            const char* ptr; // 64-bit pointer on 64-bit platforms
        } longStr;
    #elif INTPTR_MAX == INT32_MAX
        // Long string (32-bit): MSB of tag = 1, 3 reserved bytes, 32-bit bytecount,
        // 32-bit reserved (per spec note), 32-bit pointer
        struct
        {
            std::uint8_t tag;
            std::uint8_t reserved[3];
            std::uint32_t bytecount;
            std::uint32_t reserved2;
            const char* ptr; // 32-bit pointer on 32-bit platforms
        } longStr;
    #else
    #error Unsupported pointer size
    #endif

        // Helpers (kept as inline for header-only access)
        static constexpr std::uint8_t ShortMask     = 0x80; // bit7
        static constexpr std::uint8_t ByteCountMask = 0x0F; // low 4 bits
        static constexpr std::uint8_t LongMask      = 0x80; // bit7

        // Discriminators
        bool is_short() const noexcept { return (shortStr.tag & ShortMask) == 0; }
        bool is_long()  const noexcept { return (longStr.tag  & LongMask)  != 0; }

        // Total length in bytes (short or long)
        std::size_t bytecount() const noexcept
        {
            return is_short()
                ? static_cast<std::size_t>(shortStr.tag & ByteCountMask)
                : static_cast<std::size_t>(longStr.bytecount);
        }

        // Construct from a null-terminated C string
        explicit String(const char* cstr) noexcept
        {
            if (!cstr)
            {
                // Treat null as empty short string
                shortStr.tag = 0;
                std::memset(shortStr.data, 0, sizeof(shortStr.data));
                return;
            }

            const std::size_t len = std::strlen(cstr);

            if (len <= 15)
            {
                // Short string
                shortStr.tag = static_cast<std::uint8_t>(len & ByteCountMask); // MSB stays 0
                std::memset(shortStr.data, 0, sizeof(shortStr.data));
                if (len > 0)
                {
                    std::memcpy(shortStr.data, cstr, len);
                }
            }
            else
            {
                // Long string
                longStr.tag = LongMask; // set MSB=1
                longStr.bytecount = static_cast<std::uint32_t>(len);
                longStr.reserved[0] = longStr.reserved[1] = longStr.reserved[2] = 0;
            #if INTPTR_MAX == INT32_MAX
                longStr.reserved2 = 0;
            #endif

                char* buf = new (std::nothrow) char[len]; // no terminator stored
                if (buf)
                {
                    std::memcpy(buf, cstr, len);
                    // Store pointer directly; size is 64-bit on 64-bit, 32-bit on 32-bit.
                    longStr.ptr = buf;
                }
                else
                {
                    // Allocation failed: zero out as empty long
                    longStr.bytecount = 0;
                    longStr.ptr = nullptr;
                }
            }
        }
    };
#pragma pack(pop)

} // namespace Types
} // namespace Puma