/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef MOCK_ACCESSIBLE_ABILITY_CHANNEL_PROXY_H
#define MOCK_ACCESSIBLE_ABILITY_CHANNEL_PROXY_H

#include "accessible_ability_channel_proxy.h"
#include "gmock/gmock.h"

namespace OHOS {
namespace Accessibility {
class MockAccessibleAbilityChannelProxy : public AccessibleAbilityChannelProxy {
public:
    MockAccessibleAbilityChannelProxy(const sptr<IRemoteObject>& object);
    virtual ~MockAccessibleAbilityChannelProxy() = default;

    MOCK_METHOD5(SearchElementInfoByAccessibilityId,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t requestId,
            const sptr<IAccessibilityElementOperatorCallback>& callback, const int32_t mode));
    MOCK_METHOD5(SearchElementInfosByText,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const std::string& text,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD5(FindFocusedElementInfo,
        bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t focusType,
            const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));
    MOCK_METHOD5(
        FocusMoveSearch, bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t direction,
                             const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback));

    MOCK_METHOD6(ExecuteAction, bool(const int32_t accessibilityWindowId, const int32_t elementId, const int32_t action,
                                    std::map<std::string, std::string>& actionArguments, const int32_t requestId,
                                    const sptr<IAccessibilityElementOperatorCallback>& callback));

    MOCK_METHOD1(GetWindows, std::vector<AccessibilityWindowInfo>(const uint64_t displayId));
    MOCK_METHOD1(ExecuteCommonAction, bool(const int32_t action));

    MOCK_METHOD2(SetOnKeyPressEventResult, void(const bool handled, const int32_t sequence));
    MOCK_METHOD2(
        SendSimulateGesture, void(const int32_t sequenceNum,
        const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath));
};
} // namespace Accessibility
} // namespace OHOS
#endif // MOCK_ACCESSIBLE_ABILITY_CHANNEL_PROXY_H