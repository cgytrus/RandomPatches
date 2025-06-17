#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/ExplodeItemSprite.hpp>
class $modify(FallApartSprite, ExplodeItemSprite) {
    struct Fields {
        float m_additionalGravity = 0.0f;
    };
};

#include <Geode/modify/ExplodeItemNode.hpp>
class $modify(FallApart, ExplodeItemNode) {
    void undoExplosion() {
        for (const auto& sprite : CCArrayExt<FallApartSprite*>(m_sprites)) {
            sprite->m_xVelocity = 0.0f;
            sprite->m_yVelocity = 0.0f;
            sprite->m_rotVelocity = 0.0f;
            sprite->m_fields->m_additionalGravity = -27.0f;
            sprite->m_fields->m_additionalGravity = static_cast<float>(rand()) / RAND_MAX * 10.0f;
        }
    }

    $override void update(float dt) {
        for (const auto& sprite : CCArrayExt<FallApartSprite*>(m_sprites)) {
            sprite->m_yVelocity -= m_noGravity ? 0.0f : dt * sprite->m_fields->m_additionalGravity;
        }
        ExplodeItemNode::update(dt);
    }
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObject) {
    $override void playDeathEffect() {
        PlayerObject::playDeathEffect();
        if (!Mod::get()->getSettingValue<bool>("icon-explosion-without-explosion"))
            return;
        if (GameManager::get()->getGameVariable("0153"))
            return;
        auto* parent = this->getParent() ? this->getParent() : m_parentLayer;
        if (!parent)
            return;
        CCArrayExt<CCNode*> children = parent->getChildren();
        for (size_t i = children.size(); i > 0; i--) {
            const auto& child = children[i - 1];
            auto* explode = static_cast<FallApart*>(typeinfo_cast<ExplodeItemNode*>(child));
            if (!explode)
                continue;
            explode->undoExplosion();
            break;
        }
    }
};
