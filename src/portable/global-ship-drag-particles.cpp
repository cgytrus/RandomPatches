#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    $override bool init(int p0, int p1, GJBaseGameLayer* p2, CCLayer* p3, bool p4) {
        if (!PlayerObject::init(p0, p1, p2, p3, p4))
            return false;
        if (!Mod::get()->getSettingValue<bool>("global-ship-drag-particles"))
            return true;
        if (m_vehicleGroundParticles)
            m_vehicleGroundParticles->setPositionType(kCCPositionTypeFree);
        return true;
    }
};
