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
    private:
        // Layout (private)
        struct { std::uint8_t tag; char data[15]; } shortStr;

    #if INTPTR_MAX == INT64_MAX
        struct { std::uint8_t tag; std::uint8_t reserved[3]; std::uint32_t size; const char* ptr; } longStr;
    #elif INTPTR_MAX == INT32_MAX
        struct { std::uint8_t tag; std::uint8_t reserved[3]; std::uint32_t size; std::uint32_t reserved2; const char* ptr; } longStr;
    #else
    #error Unsupported pointer size
    #endif

        // Masks (private) - UPPER_CASE
        static constexpr std::uint8_t SHORT_MASK = 0x80;
        static constexpr std::uint8_t SIZE_MASK  = 0x0F;
        static constexpr std::uint8_t LONG_MASK  = 0x80;

        // Helpers (private) - lowerCamelCase
        bool isShort() const noexcept;
        bool isLong()  const noexcept;
		void initialize(const char* cstr) noexcept;

    public:
        // Public API
        // Construct from a null-terminated C string
        explicit String(const char* cstr) noexcept;
		// Destructor: free heap storage for long strings if owned
		~String() noexcept;

        std::size_t Size() const noexcept;

        // Releases heap storage for long strings.
        // zero the size
        void Finalize() noexcept;

        // Move semantics: transfer ownership of long-string storage.
        String(String&& other) noexcept;
        String& operator=(String&& other) noexcept;

        // Disallow copying to avoid double-free or leaks for long strings.
        String(const String&) = delete;
        String& operator=(const String&) = delete;
    };
#pragma pack(pop)

} // namespace Types
} // namespace Puma