#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    struct Fields {
        bool m_isDeath = false;
    };

    void playerDestroyed(bool p0) {
        m_fields->m_isDeath = true;
        PlayerObject::playerDestroyed(p0);
        m_fields->m_isDeath = false;
    }

    void fadeOutStreak2(float time) {
        if (!m_fields->m_isDeath || !Mod::get()->getSettingValue<bool>("wave-trail-on-death"))
            PlayerObject::fadeOutStreak2(time);
    }
};
