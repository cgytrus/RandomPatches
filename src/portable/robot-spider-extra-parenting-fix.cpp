#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/GJRobotSprite.hpp>
#include <utility>
class $modify(GJRobotSprite) {
    // TODO: move to ctor when modify with it is fixed in geode
    $override bool init(int p0, std::string p1) {
        // m_headSprite not being initialized sometimes causes a check in init to pass
        // causing the extra sprite to be attached to whatever was in m_headSprite
        // instead of the actual head sprite
        if (Mod::get()->getSettingValue<bool>("robot-spider-extra-parenting-fix"))
            m_headSprite = nullptr;
        return GJRobotSprite::init(p0, std::move(p1));
    }
};
