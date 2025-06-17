#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    struct Fields {
        bool m_scalingSwing = false;
    };

    $override void togglePlayerScale(bool mini, bool fromPortal) {
        m_fields->m_scalingSwing = m_isSwing && Mod::get()->getSettingValue<bool>("swing-default-mini-icon-fix");
        PlayerObject::togglePlayerScale(mini, fromPortal);
        m_fields->m_scalingSwing = false;
    }

    $override void updatePlayerFrame(int frame) {
        if (!m_fields->m_scalingSwing)
            PlayerObject::updatePlayerFrame(frame);
    }
};
