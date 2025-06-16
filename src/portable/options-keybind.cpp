#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <geode.custom-keybinds/include/Keybinds.hpp>

$execute {
    using namespace keybinds;

    BindManager::get()->registerBindable({
        "open-options"_spr,
        "Open Options",
        "Opens the options menu",
        { Keybind::create(KEY_F2, Modifier::None) },
        "Global/ConfiG's Random Patches",
        false
    });

    new EventListener([=](InvokeBindEvent* event) {
        if (event->isDown())
            MoreOptionsLayer::create()->show();
        return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "open-options"_spr));
}
