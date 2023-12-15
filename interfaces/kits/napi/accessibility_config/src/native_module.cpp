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

#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

#include "hilog_wrapper.h"
#include "napi_accessibility_config.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "accessibility_config.h"

EXTERN_C_START
/*
 * function for module exports
 */
static napi_property_descriptor configDesc[] = {
    DECLARE_NAPI_FUNCTION("set", NAccessibilityConfig::SetConfig),
    DECLARE_NAPI_FUNCTION("get", NAccessibilityConfig::GetConfig),
    DECLARE_NAPI_FUNCTION("on", NAccessibilityConfig::SubscribeConfigObserver),
    DECLARE_NAPI_FUNCTION("off", NAccessibilityConfig::UnSubscribeConfigObserver),
};

static napi_value InitHighContrastText(napi_env env)
{
    napi_value highContrastTextValue;
    napi_create_object(env, &highContrastTextValue);
    NAPI_CALL(env, napi_define_properties(env, highContrastTextValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_HIGH_CONTRAST_TEXT);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, highContrastTextValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return highContrastTextValue;
}

static napi_value InitInvertColor(napi_env env)
{
    napi_value invertColorValue;
    napi_create_object(env, &invertColorValue);
    NAPI_CALL(env, napi_define_properties(env, invertColorValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_INVERT_COLOR);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, invertColorValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return invertColorValue;
}

static napi_value InitDaltonizationState(napi_env env)
{
    napi_value daltonizationStateValue;
    napi_create_object(env, &daltonizationStateValue);
    NAPI_CALL(env, napi_define_properties(env, daltonizationStateValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_DALTONIZATION_STATE);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, daltonizationStateValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return daltonizationStateValue;
}

static napi_value InitDaltonizationColorFilter(napi_env env)
{
    napi_value daltonizationColorFilterValue;
    napi_create_object(env, &daltonizationColorFilterValue);
    NAPI_CALL(env, napi_define_properties(env, daltonizationColorFilterValue,
                                          sizeof(configDesc) /
                                          sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_DALTONIZATION_COLOR_FILTER);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, daltonizationColorFilterValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return daltonizationColorFilterValue;
}

static napi_value InitContentTimeout(napi_env env)
{
    napi_value contentTimeoutValue;
    napi_create_object(env, &contentTimeoutValue);
    NAPI_CALL(env, napi_define_properties(env, contentTimeoutValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_CONTENT_TIMEOUT);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, contentTimeoutValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return contentTimeoutValue;
}

static napi_value InitAnimationOff(napi_env env)
{
    napi_value animationOffValue;
    napi_create_object(env, &animationOffValue);
    NAPI_CALL(env, napi_define_properties(env, animationOffValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_ANIMATION_OFF);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, animationOffValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return animationOffValue;
}

static napi_value InitBrightnessDiscount(napi_env env)
{
    napi_value brightnessDiscountValue;
    napi_create_object(env, &brightnessDiscountValue);
    NAPI_CALL(env, napi_define_properties(env, brightnessDiscountValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_BRIGHTNESS_DISCOUNT);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, brightnessDiscountValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return brightnessDiscountValue;
}

static napi_value InitScreenMagnifier(napi_env env)
{
    napi_value screenMagnifierValue;
    napi_create_object(env, &screenMagnifierValue);
    NAPI_CALL(env, napi_define_properties(env, screenMagnifierValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_SCREEN_MAGNIFICATION);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, screenMagnifierValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return screenMagnifierValue;
}

static napi_value InitAudioMono(napi_env env)
{
    napi_value audioMonoValue;
    napi_create_object(env, &audioMonoValue);
    NAPI_CALL(env, napi_define_properties(env, audioMonoValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_AUDIO_MONO);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, audioMonoValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return audioMonoValue;
}

static napi_value InitAudioBalance(napi_env env)
{
    napi_value audioBalanceValue;
    napi_create_object(env, &audioBalanceValue);
    NAPI_CALL(env, napi_define_properties(env, audioBalanceValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_AUDIO_BALANCE);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, audioBalanceValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return audioBalanceValue;
}

static napi_value InitMouseKey(napi_env env)
{
    napi_value mouseKeyValue;
    napi_create_object(env, &mouseKeyValue);
    NAPI_CALL(env, napi_define_properties(env, mouseKeyValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_MOUSE_KEY);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, mouseKeyValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return mouseKeyValue;
}

static napi_value InitMouseAutoClick(napi_env env)
{
    napi_value mouseAutoClickValue;
    napi_create_object(env, &mouseAutoClickValue);
    NAPI_CALL(env, napi_define_properties(env, mouseAutoClickValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_MOUSE_AUTOCLICK);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, mouseAutoClickValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return mouseAutoClickValue;
}

static napi_value InitShortKey(napi_env env)
{
    napi_value shortKeyValue;
    napi_create_object(env, &shortKeyValue);
    NAPI_CALL(env, napi_define_properties(env, shortKeyValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_SHORT_KEY);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, shortKeyValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return shortKeyValue;
}

static napi_value InitShortKeyTarget(napi_env env)
{
    napi_value shortKeyTargetValue;
    napi_create_object(env, &shortKeyTargetValue);
    NAPI_CALL(env, napi_define_properties(env, shortKeyTargetValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_SHORT_KEY_TARGET);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, shortKeyTargetValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return shortKeyTargetValue;
}

static napi_value InitShortKeyMultiTarget(napi_env env)
{
    napi_value shortKeyMultiTargetValue;
    napi_create_object(env, &shortKeyMultiTargetValue);
    NAPI_CALL(env, napi_define_properties(env, shortKeyMultiTargetValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_SHORT_KEY_MULTI_TARGET);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, shortKeyMultiTargetValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return shortKeyMultiTargetValue;
}

static napi_value InitCaptionsState(napi_env env)
{
    napi_value captionsValue;
    napi_create_object(env, &captionsValue);
    NAPI_CALL(env, napi_define_properties(env, captionsValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_CAPTION_STATE);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, captionsValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return captionsValue;
}

static napi_value InitCaptionsStyle(napi_env env)
{
    napi_value captionsStyleValue;
    napi_create_object(env, &captionsStyleValue);
    NAPI_CALL(env, napi_define_properties(env, captionsStyleValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_CAPTION_STYLE);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, captionsStyleValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return captionsStyleValue;
}

static napi_value InitClickResponseTime(napi_env env)
{
    napi_value clickResponseTimeValue;
    napi_create_object(env, &clickResponseTimeValue);
    NAPI_CALL(env, napi_define_properties(env, clickResponseTimeValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONIFG_CLICK_RESPONSE_TIME);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, clickResponseTimeValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return clickResponseTimeValue;
}

static napi_value InitIgnoreRepeatClickState(napi_env env)
{
    napi_value ignoreRepeatClickStateValue;
    napi_create_object(env, &ignoreRepeatClickStateValue);
    NAPI_CALL(env, napi_define_properties(env, ignoreRepeatClickStateValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_IGNORE_REPEAT_CLICK_STATE);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, ignoreRepeatClickStateValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return ignoreRepeatClickStateValue;
}

static napi_value InitIgnoreRepeatClickTime(napi_env env)
{
    napi_value ignoreRepeatClickTimeValue;
    napi_create_object(env, &ignoreRepeatClickTimeValue);
    NAPI_CALL(env, napi_define_properties(env, ignoreRepeatClickTimeValue,
                                          sizeof(configDesc) / sizeof(configDesc[0]),
                                          configDesc));
    NAccessibilityConfigClass* nativeObj =
        new(std::nothrow) NAccessibilityConfigClass(OHOS::AccessibilityConfig::CONFIG_IGNORE_REPEAT_CLICK_TIME);
    if (!nativeObj) {
        HILOG_ERROR("Failed to create nativeObj.");
        return nullptr;
    }
    nativeObj->SetEnv(env);
    NAPI_CALL(env, napi_wrap(env, ignoreRepeatClickTimeValue, reinterpret_cast<void*>(nativeObj),
                             NAccessibilityConfigClass::Destructor, nullptr, nativeObj->GetWrapper()));
    return ignoreRepeatClickTimeValue;
}

static napi_value InitConfigModule(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("on", NAccessibilityConfig::SubscribeState),
        DECLARE_NAPI_FUNCTION("off", NAccessibilityConfig::UnsubscribeState),
        DECLARE_NAPI_FUNCTION("enableAbility", NAccessibilityConfig::EnableAbility),
        DECLARE_NAPI_FUNCTION("disableAbility", NAccessibilityConfig::DisableAbility),
        DECLARE_NAPI_STATIC_PROPERTY("highContrastText", InitHighContrastText(env)),
        DECLARE_NAPI_STATIC_PROPERTY("invertColor", InitInvertColor(env)),
        DECLARE_NAPI_STATIC_PROPERTY("daltonizationState", InitDaltonizationState(env)),
        DECLARE_NAPI_STATIC_PROPERTY("daltonizationColorFilter", InitDaltonizationColorFilter(env)),
        DECLARE_NAPI_STATIC_PROPERTY("contentTimeout", InitContentTimeout(env)),
        DECLARE_NAPI_STATIC_PROPERTY("animationOff", InitAnimationOff(env)),
        DECLARE_NAPI_STATIC_PROPERTY("brightnessDiscount", InitBrightnessDiscount(env)),
        DECLARE_NAPI_STATIC_PROPERTY("screenMagnifier", InitScreenMagnifier(env)),
        DECLARE_NAPI_STATIC_PROPERTY("audioMono", InitAudioMono(env)),
        DECLARE_NAPI_STATIC_PROPERTY("audioBalance", InitAudioBalance(env)),
        DECLARE_NAPI_STATIC_PROPERTY("mouseKey", InitMouseKey(env)),
        DECLARE_NAPI_STATIC_PROPERTY("mouseAutoClick", InitMouseAutoClick(env)),
        DECLARE_NAPI_STATIC_PROPERTY("shortkey", InitShortKey(env)),
        DECLARE_NAPI_STATIC_PROPERTY("shortkeyTarget", InitShortKeyTarget(env)),
        DECLARE_NAPI_STATIC_PROPERTY("shortkeyMultiTargets", InitShortKeyMultiTarget(env)),
        DECLARE_NAPI_STATIC_PROPERTY("captions", InitCaptionsState(env)),
        DECLARE_NAPI_STATIC_PROPERTY("captionsStyle", InitCaptionsStyle(env)),
        DECLARE_NAPI_STATIC_PROPERTY("clickResponseTime", InitClickResponseTime(env)),
        DECLARE_NAPI_STATIC_PROPERTY("ignoreRepeatClick", InitIgnoreRepeatClickState(env)),
        DECLARE_NAPI_STATIC_PROPERTY("repeatClickInterval", InitIgnoreRepeatClickTime(env)),
    };

    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

    auto &instance = OHOS::AccessibilityConfig::AccessibilityConfig::GetInstance();
    (void)instance.InitializeContext();
    NAccessibilityConfig::configObservers_->SubscribeToFramework();
    NAccessibilityConfig::enableAbilityListsObservers_->SubscribeToFramework();
    HILOG_INFO("-----Init config module end------");
    return exports;
}
EXTERN_C_END

/*
 * Module define
 */
static napi_module _config_module = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = InitConfigModule,
    .nm_modname = "accessibility.config",
    .nm_priv = ((void*)0),
    .reserved = {0},
};
/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void RegisterNapiAccessibilityConfigModule(void)
{
    napi_module_register(&_config_module);
}
