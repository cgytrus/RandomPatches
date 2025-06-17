#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/AudioEffectsLayer.hpp>
class $modify(AudioEffectsLayer) {
    $override void audioStep(float p0) {
        if (!Mod::get()->getSettingValue<bool>("practice-music-sync-pulse-fix"))
            return AudioEffectsLayer::audioStep(p0);
        auto* pl = PlayLayer::get();
        if (!pl)
            return AudioEffectsLayer::audioStep(p0);
        bool saved = pl->m_isPracticeMode;
        pl->m_isPracticeMode = pl->m_isPracticeMode && !pl->m_practiceMusicSync;
        AudioEffectsLayer::audioStep(p0);
        pl->m_isPracticeMode = saved;
    }
};
