#pragma once

#include "koalabox/koalabox.hpp"

namespace lyptus {

    using namespace koalabox;

    struct Patch {
        String name;
        String pattern;
        int offset = 0;
        String replacement;
        bool enabled = true;
        bool required = false;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Patch, name, pattern, offset, replacement, enabled, required)
    };

    struct Config {
        bool logging = false;
        Vector<Patch> patches;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, logging, patches)
    };

    extern Config config;

    void init(const HMODULE& module);

    void shutdown();

}