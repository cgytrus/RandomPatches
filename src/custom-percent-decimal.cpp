#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "patch.hpp"
#include <sinaps.hpp>

$execute {
    // address of "%.2f%%"
    constexpr ptrdiff_t addr =
#ifdef GEODE_IS_WINDOWS
        0x5ad850;
#elif GEODE_IS_ANDROID32
        0x932e13;
#elif GEODE_IS_ANDROID64
        0xe79c08;
#elif GEODE_IS_ARM_MAC
        0x7350cc;
#elif GEODE_IS_INTEL_MAC
        0x811d82;
#elif GEODE_IS_IOS
        0x6c2dd9;
#else
        0x0;
    return;
#endif
    const auto c = static_cast<unsigned char>('0' + Mod::get()->getSettingValue<int64_t>("custom-percent-decimal"));
    LOG_ERR(Mod::get()->patch(reinterpret_cast<void*>(base::get() + addr + 2), { c }));
}
