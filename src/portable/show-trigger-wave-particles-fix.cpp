#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    $override void toggleVisibility(bool show) {
        bool willShowWave = m_isDart && m_isHidden && show && Mod::get()->getSettingValue<bool>("show-trigger-wave-particles-fix");
        PlayerObject::toggleVisibility(show);
        // the check in toggleVisibility gets into an "always has trail" branch
        // and it has m_trailingParticles->resetSystem() because all the game modes in it
        // until the wave was added (ship and ufo) also have trailing particles,
        // however it's not the case for wave and rob has a branch
        // specific to the wave inside the "always has trail" branch
        // but he forgot to check for wave for m_trailingParticles
        if (willShowWave)
            m_trailingParticles->stopSystem();
    }
};
