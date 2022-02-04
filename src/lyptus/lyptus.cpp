#include "lyptus.hpp"
#include "config/config.hpp"
#include "patcher/patcher.hpp"
#include "koalabox/logger/logger.hpp"
#include "koalabox/util/util.hpp"
#include "koalabox/win_util/win_util.hpp"
#include "build_config.h"

using namespace koalabox;

void lyptus::init(HMODULE self_module) {
    DisableThreadLibraryCalls(self_module);

    koalabox::project_name = "Lyptus";

    const auto self_directory = util::get_module_dir(self_module);

    const auto config = config::read(self_directory / "Lyptus.json");

    if (config.logging) {
        logger::init(self_directory / "Lyptus.log");
    }

    logger::info("🐨 Lyptus 🎋 v{}", PROJECT_VERSION);

    const auto process_handle = win_util::get_current_process_handle();
    const auto process_info = win_util::get_module_info(process_handle);

    for (const auto& patch: config.patches) {
        const auto address = patcher::find_pattern_address(process_info, patch);

        if (address == nullptr) {
            continue;
        }

        patcher::patch_memory(address, patch);

        logger::info("🍀 Patch '{}' was successfully applied", patch.name);
    }

    logger::info("🚀 Initialization complete");
}

void lyptus::shutdown() {
    logger::info("💀 Shutdown complete");
}
