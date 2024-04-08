/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "accessibility_setting_provider.h"
#include "accessibility_datashare_helper.h"
#include <gtest/gtest.h>

namespace OHOS {
namespace Accessibility {
AccessibilitySettingProvider* AccessibilitySettingProvider::instance_;
std::mutex AccessibilitySettingProvider::mutex_;
namespace {
    constexpr int32_t DEFAULT_ACCOUNT_ID = 100;
} // namespace

AccessibilitySettingProvider::AccessibilitySettingProvider()
    : AccessibilityDatashareHelper(DATASHARE_TYPE::GLOBAL, DEFAULT_ACCOUNT_ID)
{
}

AccessibilitySettingProvider::~AccessibilitySettingProvider()
{
    instance_ = nullptr;
}

AccessibilitySettingProvider& AccessibilitySettingProvider::GetInstance(int32_t systemAbilityId)
{
    if (instance_ == nullptr) {
        instance_ = new AccessibilitySettingProvider();
    }
    return *instance_;
}

void AccessibilitySettingProvider::DeleteInstance()
{
}

RetError AccessibilitySettingProvider::GetIntValue(const std::string& key, int32_t& value)
{
    (void)key;
    (void)value;
    return RET_OK;
}

RetError AccessibilitySettingProvider::GetLongValue(const std::string& key, int64_t& value)
{
    (void)key;
    (void)value;
    return RET_OK;
}

RetError AccessibilitySettingProvider::GetBoolValue(const std::string& key, bool& value)
{
    (void)key;
    (void)value;
    return RET_OK;
}

RetError AccessibilitySettingProvider::GetFloatValue(const std::string& key, float& value)
{
    (void)key;
    (void)value;
    return RET_OK;
}

RetError AccessibilitySettingProvider::PutIntValue(const std::string& key, int32_t value, bool needNotify)
{
    (void)key;
    (void)value;
    (void)needNotify;
    return RET_OK;
}

RetError AccessibilitySettingProvider::PutLongValue(const std::string& key, int64_t value, bool needNotify)
{
    (void)key;
    (void)value;
    (void)needNotify;
    return RET_OK;
}

RetError AccessibilitySettingProvider::PutBoolValue(const std::string& key, bool value, bool needNotify)
{
    (void)key;
    (void)value;
    (void)needNotify;
    return RET_OK;
}

sptr<AccessibilitySettingObserver> AccessibilitySettingProvider::CreateObserver(const std::string& key,
    AccessibilitySettingObserver::UpdateFunc& func)
{
    (void)key;
    (void)func;
    return nullptr;
}

RetError AccessibilitySettingProvider::RegisterObserver(const sptr<AccessibilitySettingObserver>& observer)
{
    (void)observer;
    return RET_OK;
}

RetError AccessibilitySettingProvider::UnregisterObserver(const sptr<AccessibilitySettingObserver>& observer)
{
    (void)observer;
    return RET_OK;
}

RetError AccessibilitySettingProvider::RegisterObserver(const std::string& key, AccessibilitySettingObserver::UpdateFunc& func)
{
    (void)key;
    (void)func;
    return RET_OK;
}

RetError AccessibilitySettingProvider::UnregisterObserver(const std::string& key)
{
    (void)key;
    return RET_OK;
}

RetError AccessibilitySettingProvider::GetStringValue(const std::string& key, std::string& value)
{
    (void)key;
    (void)value;
    return RET_OK;
}

RetError AccessibilitySettingProvider::PutStringValue
    (const std::string& key, const std::string& value, bool needNotify)
{
    (void)key;
    (void)value;
    (void)needNotify;
    return RET_OK;
}
} // namespace Accessibility
} // namespace OHOS