#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    $override void flipGravity(bool flip, bool fromPortal) {
        if (m_isUpsideDown != flip && Mod::get()->getSettingValue<bool>("flipped-particles-fix")) {
            m_playerGroundParticles->setAngle(m_playerGroundParticles->getAngle() - 180.f);
            m_ufoClickParticles->setAngle(m_ufoClickParticles->getAngle() - 180.f);
            m_robotBurstParticles->setAngle(m_robotBurstParticles->getAngle() - 180.f);
            m_vehicleGroundParticles->setAngle(m_vehicleGroundParticles->getAngle() - 180.f);
        }
        PlayerObject::flipGravity(flip, fromPortal);
    }
};
