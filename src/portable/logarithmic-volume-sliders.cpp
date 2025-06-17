#include <Geode/Geode.hpp>

using namespace geode::prelude;

float mapGetVolume(float x) {
    // linear
    //return x;

    // square
    //return std::sqrtf(x);

    // logarithmic
    return std::log10(x * 9.0f + 1.0f);
}

float mapSetVolume(float x) {
    // linear
    //return x;

    // square
    //return x * x;

    // logarithmic
    return (std::pow(10.0f, x) - 1.0f) / 9.0f;
}

#include <Geode/modify/OptionsLayer.hpp>
class $modify(OptionsLayer) {
    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("OptionsLayer::musicSliderChanged", Priority::Last)) {
            log::warn("Failed to set hook priority");
        }
        if (!self.setHookPriorityPre("OptionsLayer::sfxSliderChanged", Priority::Last)) {
            log::warn("Failed to set hook priority");
        }
    }

    $override void customSetup() {
        OptionsLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return;
        m_musicSlider->setValue(mapGetVolume(FMODAudioEngine::get()->getBackgroundMusicVolume()));
        m_sfxSlider->setValue(mapGetVolume(FMODAudioEngine::get()->getEffectsVolume()));
    }

    $override void musicSliderChanged(CCObject* caller) {
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return OptionsLayer::musicSliderChanged(caller);
        auto* slider = typeinfo_cast<SliderThumb*>(caller);
        if (!slider)
            return OptionsLayer::musicSliderChanged(caller);
        float oldVolume = FMODAudioEngine::get()->getBackgroundMusicVolume();
        FMODAudioEngine::get()->setBackgroundMusicVolume(mapSetVolume(slider->getValue()));
        if (oldVolume <= 0.0f && slider->getValue() > 0.0f)
            GameManager::get()->playMenuMusic();
    }

    $override void sfxSliderChanged(CCObject* caller) {
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return OptionsLayer::sfxSliderChanged(caller);
        auto* slider = typeinfo_cast<SliderThumb*>(caller);
        if (!slider)
            return OptionsLayer::sfxSliderChanged(caller);
        FMODAudioEngine::get()->setEffectsVolume(mapSetVolume(slider->getValue()));
    }
};

#include <Geode/modify/PauseLayer.hpp>
class $modify(PauseLayer) {
    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("PauseLayer::musicSliderChanged", Priority::Last)) {
            log::warn("Failed to set hook priority");
        }
        if (!self.setHookPriorityPre("PauseLayer::sfxSliderChanged", Priority::Last)) {
            log::warn("Failed to set hook priority");
        }
    }

    $override void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return;
        auto* musicSlider = typeinfo_cast<Slider*>(getChildByID("music-slider"));
        auto* sfxSlider = typeinfo_cast<Slider*>(getChildByID("sfx-slider"));
        if (musicSlider)
            musicSlider->setValue(mapGetVolume(FMODAudioEngine::get()->getBackgroundMusicVolume()));
        if (sfxSlider)
            sfxSlider->setValue(mapGetVolume(FMODAudioEngine::get()->getEffectsVolume()));
    }

    $override void musicSliderChanged(CCObject* caller) {
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return PauseLayer::musicSliderChanged(caller);
        auto* slider = typeinfo_cast<SliderThumb*>(caller);
        if (!slider)
            return PauseLayer::musicSliderChanged(caller);
        FMODAudioEngine::get()->setBackgroundMusicVolume(mapSetVolume(slider->getValue()));
    }

    $override void sfxSliderChanged(CCObject* caller) {
        if (!Mod::get()->getSettingValue<bool>("logarithmic-volume-sliders"))
            return PauseLayer::sfxSliderChanged(caller);
        auto* slider = typeinfo_cast<SliderThumb*>(caller);
        if (!slider)
            return PauseLayer::sfxSliderChanged(caller);
        FMODAudioEngine::get()->setEffectsVolume(mapSetVolume(slider->getValue()));
    }
};
