/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "accessibility_settings_config.h"
#include "hilog_wrapper.h"
#include "system_ability_definition.h"
#include "utils.h"

#define CHECK_DATASHARE_NULLPTR(ptr) { \
    if ((ptr) == nullptr) { \
        HILOG_ERROR("datashare_ is nullptr"); \
        return RET_ERR_NULLPTR; \
    } \
}

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr uint32_t DEFAULT_COLOR = 0xff000000;
    const std::string ACCESSIBILITY = "accessible";
    const std::string TOUCHGUIDE = "touchGuide";
    const std::string GESTURE = "gesture";
    const std::string CAPTION = "CaptionState";
    const std::string KEYEVENT = "keyEventObserver";
    const std::string SCREENMAGNIFIER = "ScreenMagnification";
    const std::string SHORTKEY = "ShortKey";
    const std::string MOUSEKEY = "MouseKey";
    const std::string HIGHCONTRASTTEXT = "highContrastText";
    const std::string DALTONIZATIONSTATE = "daltonizationState";
    const std::string INVERTCOLOR = "invertColor";
    const std::string ANIMATIONOFF = "animationOff";
    const std::string AUDIOMONO = "audioMono";
    const std::string IGNOREREPEATCLICKSTATE = "ignoreRepeatClickState";
} // namespace
AccessibilitySettingsConfig::AccessibilitySettingsConfig(int32_t id)
{
    HILOG_DEBUG("id = [%{public}d]", id);
    accountId_ = id;
}

RetError AccessibilitySettingsConfig::SetEnabled(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    enabled_ = state;
    return SetConfigState(ACCESSIBILITY, state);
}

RetError AccessibilitySettingsConfig::SetTouchGuideState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    eventTouchGuideState_ = state;
    return SetConfigState(TOUCHGUIDE, state);
}

RetError AccessibilitySettingsConfig::SetGestureState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    gesturesSimulation_ = state;
    return SetConfigState(GESTURE, state);
}

RetError AccessibilitySettingsConfig::SetKeyEventObserverState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    filteringKeyEvents_ = state;
    return SetConfigState(KEYEVENT, state);
}

RetError AccessibilitySettingsConfig::SetCaptionState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    isCaptionState_ = state;
    return SetConfigState(CAPTION, state);
}

RetError AccessibilitySettingsConfig::SetScreenMagnificationState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    isScreenMagnificationState_ = state;
    return SetConfigState(SCREENMAGNIFIER, state);
}

RetError AccessibilitySettingsConfig::SetShortKeyState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    isShortKeyState_ = state;
    return SetConfigState(SHORTKEY, state);
}


RetError AccessibilitySettingsConfig::SetStartFromAtoHosState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    CHECK_DATASHARE_NULLPTR(datashare_);
    return datashare_->PutBoolValue("AccessibilityStartFromAtoHos", state);
}

RetError AccessibilitySettingsConfig::SetMouseKeyState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    isMouseKeyState_ = state;
    return SetConfigState(MOUSEKEY, state);
}

RetError AccessibilitySettingsConfig::SetMouseAutoClick(const int32_t time)
{
    HILOG_DEBUG("time = [%{public}d]", time);
    CHECK_DATASHARE_NULLPTR(datashare_);
    mouseAutoClick_ = time;
    return datashare_->PutIntValue("MouseAutoClick", time);
}

RetError AccessibilitySettingsConfig::SetShortkeyTarget(const std::string &name)
{
    HILOG_DEBUG("name = [%{public}s]", name.c_str());
    shortkeyTarget_ = name;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutStringValue("ShortkeyTarget", name);
}

RetError AccessibilitySettingsConfig::SetShortkeyMultiTarget(const std::vector<std::string> &name)
{
    HILOG_DEBUG();
    shortkeyMultiTarget_ = name;
    CHECK_DATASHARE_NULLPTR(datashare_);

    std::string stringOut = "";
    Utils::VectorToString(name, stringOut);
    return datashare_->PutStringValue("ShortkeyMultiTarget", stringOut);
}

RetError AccessibilitySettingsConfig::SetShortkeyMultiTargetInPkgRemove(const std::string &name)
{
    HILOG_DEBUG();
    CHECK_DATASHARE_NULLPTR(datashare_);
    RetError rtn = RET_OK;

    for (auto iter = shortkeyMultiTarget_.begin(); iter != shortkeyMultiTarget_.end(); ++iter) {
        if (*iter == name) {
            shortkeyMultiTarget_.erase(iter);
            std::string stringOut = "";
            Utils::VectorToString(shortkeyMultiTarget_, stringOut);
            rtn = datashare_->PutStringValue("ShortkeyMultiTarget", stringOut);
            break;
        }
    }
    return rtn;
}

RetError AccessibilitySettingsConfig::SetHighContrastTextState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    highContrastTextState_ = state;
    return SetConfigState(HIGHCONTRASTTEXT, state);
}

RetError AccessibilitySettingsConfig::SetInvertColorState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    invertColorState_ = state;
    return SetConfigState(INVERTCOLOR, state);
}

RetError AccessibilitySettingsConfig::SetAnimationOffState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    animationOffState_ = state;
    return SetConfigState(ANIMATIONOFF, state);
}

RetError AccessibilitySettingsConfig::SetAudioMonoState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    audioMonoState_ = state;
    return SetConfigState(AUDIOMONO, state);
}

RetError AccessibilitySettingsConfig::SetDaltonizationState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    daltonizationState_ = state;
    return SetConfigState(DALTONIZATIONSTATE, state);
}

RetError AccessibilitySettingsConfig::SetDaltonizationColorFilter(const uint32_t filter)
{
    HILOG_DEBUG("filter = [%{public}u]", filter);
    daltonizationColorFilter_ = filter;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutIntValue("daltonizationColorFilter", static_cast<int32_t>(filter));
}


RetError AccessibilitySettingsConfig::SetContentTimeout(const uint32_t time)
{
    HILOG_DEBUG("time = [%{public}u]", time);
    contentTimeout_ = time;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutIntValue("contentTimeout", static_cast<int32_t>(time));
}

RetError AccessibilitySettingsConfig::SetBrightnessDiscount(const float discount)
{
    HILOG_DEBUG("discount = [%{public}f]", discount);
    brightnessDiscount_ = discount;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutFloatValue("brightnessDiscount", discount);
}

RetError AccessibilitySettingsConfig::SetAudioBalance(const float balance)
{
    HILOG_DEBUG("balance = [%{public}f]", balance);
    audioBalance_ = balance;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutFloatValue("audioBalance", balance);
}

RetError AccessibilitySettingsConfig::SetClickResponseTime(const uint32_t time)
{
    HILOG_DEBUG("clickResponseTime = [%{public}u]", time);
    clickResponseTime_ = time;
    CHECK_DATASHARE_NULLPTR(datashare_);

    return datashare_->PutIntValue("clickResponseTime", time);
}

RetError AccessibilitySettingsConfig::SetIgnoreRepeatClickState(const bool state)
{
    HILOG_DEBUG("state = [%{public}s]", state ? "True" : "False");
    ignoreRepeatClickState_ = state;
    return SetConfigState(IGNOREREPEATCLICKSTATE, state);
}

RetError AccessibilitySettingsConfig::SetIgnoreRepeatClickTime(const uint32_t time)
{
    HILOG_DEBUG("ignoreRepeatClickTime = [%{public}u]", time);
    ignoreRepeatClickTime_ = time;
    CHECK_DATASHARE_NULLPTR(datashare_);
    return datashare_->PutIntValue("ignoreRepeatClickTime", time);
}

RetError AccessibilitySettingsConfig::SetCaptionProperty(const AccessibilityConfig::CaptionProperty& caption)
{
    HILOG_DEBUG();
    captionProperty_ = caption;
    CHECK_DATASHARE_NULLPTR(datashare_);

    const std::string& FONTFAMILY = captionProperty_.GetFontFamily();
    int32_t FONTSCALE = captionProperty_.GetFontScale();
    uint32_t FONTCOLOR = captionProperty_.GetFontColor();
    const std::string& FONTEDGETYPE = captionProperty_.GetFontEdgeType();
    uint32_t BACKGROUNDCOLOR = captionProperty_.GetBackgroundColor();
    uint32_t WINDOWCOLOR = captionProperty_.GetWindowColor();

    datashare_->PutStringValue("fontFamily", FONTFAMILY);
    datashare_->PutIntValue("fontColor", static_cast<int32_t>(FONTCOLOR));
    datashare_->PutStringValue("fontEdgeType", FONTEDGETYPE);
    datashare_->PutIntValue("backgroundColor", static_cast<int32_t>(BACKGROUNDCOLOR));
    datashare_->PutIntValue("windowColor", static_cast<int32_t>(WINDOWCOLOR));
    datashare_->PutIntValue("fontScale", FONTSCALE);
    return RET_OK;
}

bool AccessibilitySettingsConfig::GetCaptionState() const
{
    return isCaptionState_;
}

bool AccessibilitySettingsConfig::GetScreenMagnificationState() const
{
    return isScreenMagnificationState_;
}

bool AccessibilitySettingsConfig::GetShortKeyState() const
{
    return isShortKeyState_;
}

bool AccessibilitySettingsConfig::GetMouseKeyState() const
{
    return isMouseKeyState_;
}

int32_t AccessibilitySettingsConfig::GetMouseAutoClick() const
{
    return mouseAutoClick_;
}

const std::string &AccessibilitySettingsConfig::GetShortkeyTarget() const
{
    return shortkeyTarget_;
}

const std::vector<std::string> &AccessibilitySettingsConfig::GetShortkeyMultiTarget() const
{
    return shortkeyMultiTarget_;
}

bool AccessibilitySettingsConfig::GetHighContrastTextState() const
{
    return highContrastTextState_;
}

bool AccessibilitySettingsConfig::GetInvertColorState() const
{
    return invertColorState_;
}

bool AccessibilitySettingsConfig::GetAnimationOffState() const
{
    return animationOffState_;
}

bool AccessibilitySettingsConfig::GetAudioMonoState() const
{
    return audioMonoState_;
}

bool AccessibilitySettingsConfig::GetDaltonizationState() const
{
    return daltonizationState_;
}

uint32_t AccessibilitySettingsConfig::GetDaltonizationColorFilter() const
{
    return daltonizationColorFilter_;
}

uint32_t AccessibilitySettingsConfig::GetContentTimeout() const
{
    return contentTimeout_;
}

float AccessibilitySettingsConfig::GetBrightnessDiscount() const
{
    return brightnessDiscount_;
}

float AccessibilitySettingsConfig::GetAudioBalance() const
{
    return audioBalance_;
}

bool AccessibilitySettingsConfig::GetEnabledState() const
{
    return enabled_;
}

bool AccessibilitySettingsConfig::GetTouchGuideState() const
{
    return eventTouchGuideState_;
}

bool AccessibilitySettingsConfig::GetGestureState() const
{
    return gesturesSimulation_;
}

bool AccessibilitySettingsConfig::GetKeyEventObserverState() const
{
    return filteringKeyEvents_;
}

const AccessibilityConfig::CaptionProperty &AccessibilitySettingsConfig::GetCaptionProperty() const
{
    return captionProperty_;
};

uint32_t AccessibilitySettingsConfig::GetClickResponseTime() const
{
    return clickResponseTime_;
}

bool AccessibilitySettingsConfig::GetIgnoreRepeatClickState() const
{
    return ignoreRepeatClickState_;
}

uint32_t AccessibilitySettingsConfig::GetIgnoreRepeatClickTime() const
{
    return ignoreRepeatClickTime_;
}

bool AccessibilitySettingsConfig::GetStartFromAtoHosState()
{
    HILOG_DEBUG();
    CHECK_DATASHARE_NULLPTR(datashare_);

    bool value = false;
    datashare_->GetBoolValue("AccessibilityStartFromAtoHos", value);
    return value;
}

uint32_t AccessibilitySettingsConfig::GetConfigState()
{
    HILOG_DEBUG();
    uint32_t state = 0;
    if (isCaptionState_) {
        state |= STATE_CAPTION_ENABLED;
    }

    if (isScreenMagnificationState_) {
        state |= STATE_SCREENMAGNIFIER_ENABLED;
    }

    if (isMouseKeyState_) {
        state |= STATE_MOUSEKEY_ENABLED;
    }

    if (isShortKeyState_) {
        state |= STATE_SHORTKEY_ENABLED;
    }

    if (highContrastTextState_) {
        state |= STATE_HIGHCONTRAST_ENABLED;
    }

    if (daltonizationState_) {
        state |= STATE_DALTONIZATION_STATE_ENABLED;
    }

    if (invertColorState_) {
        state |= STATE_INVETRTCOLOR_ENABLED;
    }

    if (animationOffState_) {
        state |= STATE_ANIMATIONOFF_ENABLED;
    }

    if (audioMonoState_) {
        state |= STATE_AUDIOMONO_ENABLED;
    }

    if (ignoreRepeatClickState_) {
        state |= STATE_IGNORE_REPEAT_CLICK_ENABLED;
    }
    return state;
}

void AccessibilitySettingsConfig::InitCaption()
{
    HILOG_DEBUG();
    if (datashare_ == nullptr) {
        return;
    }

    std::string strValue = datashare_->GetStringValue("CaptionState", "");
    HILOG_DEBUG(" pref_->GetString() = %{public}s.", strValue.c_str());
    if (!std::strcmp(strValue.c_str(), "on")) {
        isCaptionState_ = true;
    } else {
        isCaptionState_ = false;
    }

    std::string FONTFAMILY = datashare_->GetStringValue("fontFamily", "default");
    HILOG_DEBUG("fontFamily = %{public}s.", FONTFAMILY.c_str());

    int32_t FONTSCALE =  static_cast<int32_t>(datashare_->GetIntValue("fontScale", 100));
    HILOG_DEBUG("fontScale = %{public}d.", FONTSCALE);

    uint32_t FONTCOLOR = static_cast<uint32_t>(datashare_->GetIntValue("fontColor", DEFAULT_COLOR));
    HILOG_DEBUG("fontColor = 0x%{public}x.", FONTCOLOR);

    std::string FONTEDGETYPE = datashare_->GetStringValue("fontEdgeType", "none");
    HILOG_DEBUG("FONTEDGETYPE = 0x%{public}s.", FONTEDGETYPE.c_str());

    uint32_t BACKGROUNDCOLOR = static_cast<uint32_t>(datashare_->GetIntValue("backgroundColor", DEFAULT_COLOR));
    HILOG_DEBUG("BACKGROUNDCOLOR = 0x%{public}x.", BACKGROUNDCOLOR);

    uint32_t WINDOWCOLOR = static_cast<uint32_t>(datashare_->GetIntValue("windowColor", DEFAULT_COLOR));
    HILOG_DEBUG("WINDOWCOLOR = 0x%{public}x.", WINDOWCOLOR);

    captionProperty_.SetFontFamily(FONTFAMILY);
    captionProperty_.SetFontScale(FONTSCALE);
    captionProperty_.SetFontColor(FONTCOLOR);
    captionProperty_.SetFontEdgeType(FONTEDGETYPE);
    captionProperty_.SetBackgroundColor(BACKGROUNDCOLOR);
    captionProperty_.SetWindowColor(WINDOWCOLOR);
}

void AccessibilitySettingsConfig::InitSetting()
{
    HILOG_DEBUG();
    if (datashare_ == nullptr) {
        return;
    }

    isScreenMagnificationState_ = datashare_->GetBoolValue(SCREENMAGNIFIER, false);
    isMouseKeyState_= datashare_->GetBoolValue(MOUSEKEY, false);
    isShortKeyState_ = datashare_->GetBoolValue(SHORTKEY, false);
    animationOffState_ = datashare_->GetBoolValue(ANIMATIONOFF, false);
    invertColorState_ = datashare_->GetBoolValue(INVERTCOLOR, false);
    highContrastTextState_ = datashare_->GetBoolValue(HIGHCONTRASTTEXT, false);
    daltonizationState_ = datashare_->GetBoolValue(DALTONIZATIONSTATE, false);
    audioMonoState_ = datashare_->GetBoolValue(AUDIOMONO, false);
    ignoreRepeatClickState_ = datashare_->GetBoolValue(IGNOREREPEATCLICKSTATE, false);

    shortkeyTarget_ = datashare_->GetStringValue("ShortkeyTarget", "none");

    std::string tmpString = datashare_->GetStringValue("ShortkeyMultiTarget", "");
    Utils::StringToVector(tmpString, shortkeyMultiTarget_);

    mouseAutoClick_ = static_cast<int32_t>(datashare_->GetIntValue("MouseAutoClick", -1));
    daltonizationColorFilter_ = static_cast<uint32_t>(datashare_->GetIntValue("daltonizationColorFilter", 0));
    contentTimeout_ = static_cast<uint32_t>(datashare_->GetIntValue("contentTimeout", 0));
    brightnessDiscount_ = static_cast<float>(datashare_->GetFloatValue("brightnessDiscount", 1.0));
    audioBalance_ = static_cast<float>(datashare_->GetFloatValue("audioBalance", 0));
    clickResponseTime_ = static_cast<uint32_t>(datashare_->GetIntValue("clickResponseTime", 0));
    ignoreRepeatClickTime_ = static_cast<uint32_t>(datashare_->GetIntValue("ignoreRepeatClickTime", 0));
}

void AccessibilitySettingsConfig::InitCapability()
{
    HILOG_DEBUG();
    if (datashare_ == nullptr) {
        return;
    }

    enabled_ = datashare_->GetBoolValue(ACCESSIBILITY, false);
    eventTouchGuideState_ = datashare_->GetBoolValue(TOUCHGUIDE, false);
    gesturesSimulation_ = datashare_->GetBoolValue(GESTURE, false);
    filteringKeyEvents_ = datashare_->GetBoolValue(KEYEVENT, false);
}

void AccessibilitySettingsConfig::InitEnabledList()
{
    HILOG_DEBUG();
    if (datashare_ == nullptr) {
        return;
    }

    std::string strValue = datashare_->GetStringValue("BundleName/AbilityName/Capabilities", "");
    HILOG_DEBUG("Capabilities = %{public}s", strValue.c_str());
    Utils::StringToVector(strValue, enabledAbilityInfos_);
}

RetError AccessibilitySettingsConfig::SetConfigState(const std::string& key, bool value)
{
    CHECK_DATASHARE_NULLPTR(datashare_);
    return datashare_->PutBoolValue(key, value);
}

void AccessibilitySettingsConfig::Init()
{
    HILOG_DEBUG();
    datashare_ = std::make_shared<AccessibilityDatashareHelper>(DATASHARE_TYPE::SYSTEM, accountId_);
    if (datashare_ == nullptr) {
        return;
    }
    datashare_->Initialize(POWER_MANAGER_SERVICE_ID);
    InitCaption();
    InitSetting();
}

const std::vector<std::string> &AccessibilitySettingsConfig::GetEnabledAbilityInfos()
{
    HILOG_DEBUG();
    return enabledAbilityInfos_;
}

void AccessibilitySettingsConfig::UpdateEnabledAbilities(const std::vector<std::string> &vecvalue)
{
    HILOG_DEBUG();
    enabledAbilityInfos_ = vecvalue;
    if (datashare_ == nullptr) {
        return;
    }

    std::string stringOut = "";
    Utils::VectorToString(enabledAbilityInfos_, stringOut);
    datashare_->PutStringValue("BundleName/AbilityName/Capabilities", stringOut);
}

void AccessibilitySettingsConfig::ClearData()
{
    HILOG_DEBUG();
}
} // namespace Accessibility
} // namespace OHOS