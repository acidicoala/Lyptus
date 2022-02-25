#pragma once

#include "lyptus/lyptus.hpp"

namespace patcher {

    using namespace lyptus;

    char* find_pattern_address(const MODULEINFO& process_info, const Patch& patch);

    void patch_memory(char* address, const Patch& patch);

}