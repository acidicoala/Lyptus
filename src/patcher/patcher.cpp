#include "patcher.hpp"

#include <regex>
#include <utility>

using namespace koalabox;

struct PatternMask {
    [[maybe_unused]] String binary_pattern;
    [[maybe_unused]] String mask;
};

/**
 * Converts user-friendly hex pattern string into a byte array
 * and generates corresponding string mask
 */
PatternMask get_pattern_and_mask(String pattern) {
    // Remove whitespaces
    pattern = std::regex_replace(pattern, std::regex("\\s+"), "");

    // Convert hex to binary
    std::stringstream patternStream;
    std::stringstream maskStream;
    for (size_t i = 0; i < pattern.length(); i += 2) {
        std::string byteString = pattern.substr(i, 2);

        maskStream << (byteString == "??" ? '?' : 'x');

        // Handle wildcards ourselves, rest goes to strtol
        patternStream << (
            byteString == "??" ? '?' : (char) strtol(byteString.c_str(), nullptr, 16)
        );
    }

    return { patternStream.str(), maskStream.str() };
}

// Credit: superdoc1234
// Source: https://www.unknowncheats.me/forum/1364641-post150.html
char* find(PCSTR base_address, size_t mem_length, PCSTR pattern, PCSTR mask) {
    auto DataCompare = [](
        const auto* data, const auto* mask, const auto* ch_mask, auto ch_last, size_t i_end
    ) -> bool {
        if (data[i_end] != ch_last) return false;
        for (size_t i = 0; i <= i_end; ++i) {
            if (ch_mask[i] == 'x' && data[i] != mask[i]) {
                return false;
            }
        }

        return true;
    };

    auto iEnd = strlen(mask) - 1;
    auto chLast = pattern[iEnd];

    for (size_t i = 0; i < mem_length - strlen(mask); ++i) {
        if (DataCompare(base_address + i, pattern, mask, chLast, iEnd)) {
            return const_cast<char*>(base_address + i);
        }
    }

    return nullptr;
}

// Credit: Rake
// Source: https://guidedhacking.com/threads/external-internal-pattern-scanning-guide.14112/
char* patcher::scan(PCSTR pMemory, size_t length, String pattern) {
    char* match = nullptr;
    MEMORY_BASIC_INFORMATION mbi{};

    auto[binaryPattern, mask] = get_pattern_and_mask(std::move(pattern));

    auto current_region = pMemory;
    do {
        // Skip irrelevant code regions
        auto result = VirtualQuery((LPCVOID) current_region, &mbi, sizeof(mbi));
        if (result && mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS) {
            match = find(current_region, mbi.RegionSize, binaryPattern.c_str(), mask.c_str());

            if (match) {
                break;
            }
        }

        current_region += mbi.RegionSize;
    } while (current_region < pMemory + length);

    return match;
}
