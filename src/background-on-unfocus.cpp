#include <Geode/Geode.hpp>

using namespace geode::prelude;

// not updated for 2.2074
// idek what this does anymore

// (instead of iconify)

$execute {
    // setupWindow > glfwSetWindowIconifyCallback > +0x27
    auto _0 = Mod::get()->patch(reinterpret_cast<void*>(base::getCocos() + 0x116747), { 0xfc, 0x01 });
    // setupWindow > glfwSetWindowIconifyCallback > +0x2d
    auto _1 = Mod::get()->patch(reinterpret_cast<void*>(base::getCocos() + 0x11674d), { 0xfc, 0x01 });

    // swap background/foreground callbacks because
    // the iconify/focus callback parameters are also swapped
    // func after onGLFWWindowIconifyFunCallback (the one that has the symbol is unused,
    // it has been inlined into the callback wrapper func thats passed to glfw)
    auto _2 = Mod::get()->patch(reinterpret_cast<void*>(base::getCocos() + 0xc7b6e), { 0xc });
    auto _3 = Mod::get()->patch(reinterpret_cast<void*>(base::getCocos() + 0xc7b72), { 0x8 });
}
