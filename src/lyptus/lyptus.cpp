#include "lyptus.hpp"
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "patcher/patcher.hpp"
#include "util/util.hpp"
#include "win_util/win_util.hpp"
#include "build_config.h"

#include <chrono>
#include <utility>
#include <cstring>

using namespace koalabox;

// Source: https://gist.github.com/xsleonard/7341172
unsigned char* hex_str_to_bytes(const char* hex_str) {
    size_t len = strlen(hex_str);
    if (len % 2 != 0) {
        util::panic(__func__, "Hex string must have even number of characters");
    }
    size_t final_len = len / 2;
    auto* data = (unsigned char*) malloc((final_len + 1) * sizeof(char));
    for (size_t i = 0, j = 0; j < final_len; i += 2, j++) {
        data[j] = (hex_str[i] % 32 + 9) % 25 * 16 + (hex_str[i + 1] % 32 + 9) % 25;
    }
    data[final_len] = '\0';
    return data;
}

char* find_pattern_location(MODULEINFO process_info, const config::Patch& patch) {
    const auto t1 = std::chrono::high_resolution_clock::now();
    const auto address = patcher::scan(
        static_cast<PCSTR>(process_info.lpBaseOfDll),
        process_info.SizeOfImage,
        patch.pattern
    );
    const auto t2 = std::chrono::high_resolution_clock::now();

    const double elapsedTime = std::chrono::duration<double, std::milli>(t2 - t1).count();
    logger::debug(
        "'{}' address: {}. Search time: {:.2f} ms",
        patch.name,
        fmt::ptr(address),
        elapsedTime
    );

    return address;
}

void lyptus::init(HMODULE self_module) {
    DisableThreadLibraryCalls(self_module);

    const auto self_directory = util::get_module_dir(self_module);

    const auto config = config::read(self_directory / "Lyptus.json");

    if (config.logging) {
        logger::init(self_directory / "Lyptus.log");
    }

    logger::info("🐨 Lyptus 🎋 v{}", PROJECT_VERSION);

    const auto process_handle = win_util::get_current_process_handle();
    const auto process_info = win_util::get_module_info(process_handle);

    for (const auto& patch: config.patches) {
        const auto address = find_pattern_location(process_info, patch);

        auto bytes = hex_str_to_bytes(patch.replacement.c_str());

        win_util::write_process_memory(
            ::GetCurrentProcess(),
            address + patch.offset,
            bytes,
            patch.replacement.length() / 2
        );

        logger::info("🍀 Patch '{}' was successfully applied", patch.name);
    }

    logger::info("🚀 Initialization complete");
}

void lyptus::shutdown() {
    logger::info("💀 Shutdown complete");
}
