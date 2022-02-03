#include "config.hpp"
#include "koalabox/util/util.hpp"

#include <fstream>
#include <memory>
#include <set>
#include <algorithm>

namespace config {

    void fatal_crash(const String& message) {
        util::error_box(__func__, message);
        exit(::GetLastError()); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    bool contains_only(const String& string, const std::set<char>& whitelist) {
        return std::ranges::all_of(string.begin(), string.end(),
            [&](char symbol) {
                return whitelist.contains(symbol);
            }
        );
    }

    void validate_config(const Config& config) {
        std::set<char> valid_replacement_chars{
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f',
        };

        std::set<char> valid_pattern_chars(valid_replacement_chars);
        valid_pattern_chars.insert({ '?', ' ' });

        for (const auto& patch: config.patches) {
            if (not contains_only(patch.pattern, valid_pattern_chars)) {
                fatal_crash(fmt::format(
                    "Patch '{}' contains invalid pattern: '{}'",
                    patch.name, patch.pattern
                ));
            }

            if (not contains_only(patch.replacement, valid_replacement_chars)) {
                fatal_crash(fmt::format(
                    "Patch '{}' contains invalid replacement: '{}'",
                    patch.name, patch.replacement
                ));
            }
        }
    }


    Config read(Path path) { // NOLINT(performance-unnecessary-value-param)
        if (not std::filesystem::exists(path)) {
            return {};
        }

        try {
            std::ifstream ifs(path);
            nlohmann::json json;
            ifs >> json;

            Config config = json.get<Config>();

            validate_config(config);

            return config;
        } catch (const std::exception& ex) {
            fatal_crash(fmt::format("Failed to parse config file: {}", ex.what()));
            return {};
        }
    }

}
