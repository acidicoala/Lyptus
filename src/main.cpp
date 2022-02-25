#include "lyptus/lyptus.hpp"

[[maybe_unused]]
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        lyptus::init(instance);
    } else if (reason == DLL_PROCESS_DETACH) {
        lyptus::shutdown();
    }

    return TRUE;
}
