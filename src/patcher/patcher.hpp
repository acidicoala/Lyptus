#pragma once

#include "koalabox.hpp"

namespace patcher {

    using namespace koalabox;

    char* scan(PCSTR pMemory, size_t length, String pattern);

}