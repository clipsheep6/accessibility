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

#ifndef ACCESSIBILITY_STATE_EVENT_H
#define ACCESSIBILITY_STATE_EVENT_H

#include <cstdint>

namespace OHOS {
namespace Accessibility {
enum AccessibilityStateEventType : int32_t {
    EVENT_ACCESSIBILITY_STATE_CHANGED = 0x00000001,
    EVENT_TOUCH_GUIDE_STATE_CHANGED = 0x00000002,
    EVENT_KEVEVENT_STATE_CHANGED = 0x00000004,
    EVENT_GESTURE_STATE_CHANGED = 0x00000008,
    EVENT_SCREEN_MAGNIFIER_CHANGED = 0x00000010,
    EVENT_AUTO_CLICK_CHANGED = 0x00000020,
    EVENT_SHORT_KEY_CHANGED = 0x00000040,
};

class AccessibilityStateObserver {
public:
    /**
     * @brief Destruct
     * @param
     * @return
     * @since 3
     * @sysCap Accessibility
     */
    virtual ~AccessibilityStateObserver() = default;

    /**
     * @brief Receives notifications on accessibility status changes.
     * @param state Indicates the status change.
     * @return
     * @since 3
     * @sysCap Accessibility
     */
    virtual void OnStateChanged(const bool state) = 0;
};
} // namespace Accessibility
} // namespace OHOS
#endif