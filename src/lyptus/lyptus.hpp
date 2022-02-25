#pragma once

#include "koalabox/koalabox.hpp"

namespace lyptus {

    using namespace koalabox;

    struct Patch {
        String name;
        String pattern;
        int offset;
        String replacement;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Patch, name, pattern, offset, replacement)
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