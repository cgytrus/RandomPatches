#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCDrawNode.hpp>
class $modify(CCDrawNode) {
    $override bool init() {
        m_bUseArea = false;
        return CCDrawNode::init();
    }
};
