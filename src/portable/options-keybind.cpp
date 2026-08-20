#include <Geode/Geode.hpp>

using namespace geode::prelude;

$on_game(Loaded) {
    listenForKeybindSettingPresses("options-keybind", [](Keybind const& keybind, bool down, bool repeat, double timestamp) {
        if (down && !repeat) {
            MoreOptionsLayer::create()->show();
        }
    });
}
