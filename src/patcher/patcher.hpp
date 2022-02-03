#pragma once

#include "koalabox/koalabox.hpp"
#include "config/config.hpp"

namespace patcher {

    char* find_pattern_address(MODULEINFO process_info, const config::Patch& patch);

    void patch_memory(char* address, const config::Patch& patch);

}