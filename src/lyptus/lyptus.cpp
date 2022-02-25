#include "lyptus.hpp"
#include "patcher/patcher.hpp"

#include "koalabox/config_parser/config_parser.hpp"
#include "koalabox/loader/loader.hpp"
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"

#include <build_config.h>

namespace lyptus {

    Config config = {}; // NOLINT(cert-err58-cpp)

    void validate_config() {
        const auto contains_only = [](const String& string, const std::set<char>& whitelist) {
            return std::ranges::all_of(string.begin(), string.end(),
                [&](char symbol) {
                    return whitelist.contains(symbol);
                }
            );
        };

        std::set<char> valid_replacement_chars{
            ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f',
        };

        std::set<char> valid_pattern_chars(valid_replacement_chars);
        valid_pattern_chars.insert({ '?' });

        for (const auto& patch: config.patches) {
            if (not contains_only(patch.pattern, valid_pattern_chars)) {
                util::panic("Patch '{}' contains invalid pattern", patch.name);
            }

            if (not contains_only(patch.replacement, valid_replacement_chars)) {
                util::panic("Patch '{}' contains invalid replacement", patch.name);
            }
        }
    }

    void init(const HMODULE& self_module) {
        DisableThreadLibraryCalls(self_module);

        const auto self_directory = loader::get_module_dir(self_module);

        config = config_parser::parse<Config>(self_directory / PROJECT_NAME".jsonc");

        validate_config();

        if (config.logging) {
            logger = file_logger::create(self_directory / PROJECT_NAME".log");
        }

        logger->info("🐨 Lyptus 🎋 v{}", PROJECT_VERSION);

        const auto process_handle = win_util::get_module_handle(nullptr);
        const auto process_info = win_util::get_module_info(process_handle);

        for (const auto& patch: config.patches) {
            const auto address = patcher::find_pattern_address(process_info, patch);

            // TODO: Check required
            if (address == nullptr) {
                continue;
            }

            patcher::patch_memory(address, patch);

            logger->info("🍀 Patch '{}' was successfully applied", patch.name);
        }

        logger->info("🚀 Initialization complete");
    }

    void shutdown() {
        logger->info("💀 Shutdown complete");
    }

}
