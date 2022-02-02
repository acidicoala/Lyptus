#pragma once

#include "koalabox.hpp"

#include <nlohmann/json.hpp>

namespace config {

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

    Config read(Path path);

}
