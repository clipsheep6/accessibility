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

#ifndef ACCESSIBILITY_SETTING_PROVIDER_H
#define ACCESSIBILITY_SETTING_PROVIDER_H

#include "datashare_helper.h"
#include "errors.h"
#include "mutex"
#include "accessibility_setting_observer.h"
#include "accessibility_caption.h"

namespace OHOS {
namespace Accessibility {
class AccessibilitySettingProvider : public NoCopyable {
public:
    static AccessibilitySettingProvider& GetInstance(int32_t systemAbilityId);
    ErrCode GetStringValue(const std::string& key, std::string& value);
    ErrCode GetIntValue(const std::string& key, int32_t& value);
    ErrCode GetLongValue(const std::string& key, int64_t& value);
    ErrCode GetBoolValue(const std::string& key, bool& value);
    ErrCode GetFloatValue(const std::string& key, float& value);
    ErrCode PutStringValue(const std::string& key, const std::string& value, bool needNotify = true);
    ErrCode PutIntValue(const std::string& key, int32_t value, bool needNotify = true);
    ErrCode PutLongValue(const std::string& key, int64_t value, bool needNotify = true);
    ErrCode PutBoolValue(const std::string& key, bool value, bool needNotify = true);
    bool IsValidKey(const std::string& key);
    sptr<AccessibilitySettingObserver> CreateObserver(const std::string& key,
        AccessibilitySettingObserver::UpdateFunc& func);
    ErrCode RegisterObserver(const sptr<AccessibilitySettingObserver>& observer);
    ErrCode UnregisterObserver(const sptr<AccessibilitySettingObserver>& observer);

protected:
    ~AccessibilitySettingProvider() override;

private:
    static AccessibilitySettingProvider* instance_;
    static std::mutex mutex_;
    static sptr<IRemoteObject> remoteObj_;

    static void Initialize(int32_t systemAbilityId);
    static std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelper();
    static bool ReleaseDataShareHelper(std::shared_ptr<DataShare::DataShareHelper>& helper);
    static Uri AssembleUri(const std::string& key);
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_SETTING_PROVIDER_H