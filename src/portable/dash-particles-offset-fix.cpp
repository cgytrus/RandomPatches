#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool s_enabled = false;

$execute {
    // i dont wanna do this every frame and i dont wanna add a new hook for it
    // so make it require restart 👍 its whatever
    s_enabled = Mod::get()->getSettingValue<bool>("dash-particles-offset-fix");
}

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    $override void setPosition(const CCPoint& position) {
        PlayerObject::setPosition(position);
        // this is the 2.113 behavior
        // 2.200 changed this to the position of m_trailingParticles
        // which are used for ship and ufo click particles
        if (s_enabled)
            m_dashParticles->setPosition(position);
    }
};
