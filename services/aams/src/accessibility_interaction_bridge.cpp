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
#include "accessibility_interaction_bridge.h"
#include <algorithm>
#include "accessibility_ability_info.h"
#include "accessibility_account_data.h"
#include "accessibility_display_manager.h"
#include "accessibility_window_manager.h"
#include "accessible_ability_connection.h"
#include "dummy.h"

namespace OHOS {
namespace Accessibility {
const int INTERACTION_BRIDGE_CHANNEL_ID = 0;
const int NONE_ID = -1;

AccessibilityInteractionBridge& AccessibilityInteractionBridge::GetInstance()
{
    HILOG_DEBUG("[%{public}s]", __func__);
    static AccessibilityInteractionBridge instance_;

    return instance_;
}

AccessibilityInteractionBridge::AccessibilityInteractionBridge()
{
    HILOG_DEBUG("[%{public}s]", __func__);
    AppExecFwk::AbilityInfo info;
    sptr<AccessibilityAbilityInfo> abilityInfo = new AccessibilityAbilityInfo(info);
    abilityInfo->SetCapabilityValues(Capability::CAPABILITY_RETRIEVE);
    accountData_ = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->GetCurrentAccountData();
    connection_ = new AccessibleAbilityConnection(accountData_, INTERACTION_BRIDGE_CHANNEL_ID, *abilityInfo);
    channel_ = new AccessibleAbilityChannelStubImpl(*connection_);
    if (!channel_) {
        HILOG_DEBUG("channel is nullptr.");
        AccessibilityOperator::GetInstance().RemoveChannel(INTERACTION_BRIDGE_CHANNEL_ID);
    } else {
        HILOG_DEBUG("AddChannel start.");
        AccessibilityOperator::GetInstance().AddChannel(INTERACTION_BRIDGE_CHANNEL_ID, channel_);
    }
    HILOG_INFO("AccessibleAbilityManagerService::AccessibilityInteractionBridge successfully");
}

AccessibilityElementInfo AccessibilityInteractionBridge::FindFocusedElementInfo()
{
    HILOG_DEBUG("[%{public}s without window]", __func__);

    return FindFocusedElementInfo(ANY_WINDOW_ID);
}

bool AccessibilityInteractionBridge::PerformActionOnAccessibilityFocusedItem(const ActionType& action)
{
    HILOG_DEBUG("[%{public}s]", __func__);
    auto focus = FindFocusedElementInfo();
    std::map<std::string, std::string> actionArguments{};
    HILOG_DEBUG("[%{public}s ExecuteAction]", __func__);
    return focus.ExecuteAction(action, actionArguments);
}

bool Intersect(Rect& focus, Rect source) {
    HILOG_DEBUG("[%{public}s]", __func__);
    int minx = std::max(focus.GetLeftTopXScreenPostion(), source.GetLeftTopXScreenPostion());
    int miny = std::max(focus.GetLeftTopYScreenPostion(), source.GetLeftTopYScreenPostion());
    int maxx = std::min(focus.GetRightBottomXScreenPostion(), source.GetRightBottomXScreenPostion());
    int maxy = std::min(focus.GetRightBottomYScreenPostion(), source.GetRightBottomYScreenPostion());
    if ((minx > maxx) || (miny > maxy)) {
        HILOG_DEBUG("The two Rects do not intersect");
        return false;
    }
    focus.SetLeftTopScreenPostion(minx,miny);
    focus.SetRightBottomScreenPostion(maxx,maxy);
    return true;
}

bool Intersect(Rect& rect, AccessibilityWindowInfo window) {
    HILOG_DEBUG("[%{public}s]", __func__);
    if (!Intersect(rect, window.GetRectInScreen())) {
        return false;
    }
    return true;
}

bool Intersect(Rect& rect, WMDisplayInfo display) {
    HILOG_DEBUG("[%{public}s]", __func__);
    Rect source(0, 0, display.width, display.height);
    if (!Intersect(rect,source)) {
        return false;
    }
    return true;
}


bool AccessibilityInteractionBridge::GetAccessibilityFocusClickPointInScreen(MmiPoint &point)
{
    HILOG_DEBUG("[%{public}s]", __func__);
    auto focus = FindFocusedElementInfo();
    auto focusRect = focus.GetRectInScreen();
    /* Apply magnification if needed.**/
    //TODO
    /* Apply magnification if needed.**/
    //Intersect with window
    auto windowManager = AccessibilityWindowInfoManager::GetInstance();
    AccessibilityWindowInfo window;
    auto result = windowManager.GetAccessibilityWindow(windowManager.activeWindowId_, window);
    if (!result) {
        return false;
    }
    if (!Intersect(focusRect, window)) {
        return false;
    }
    //Intersect with display  dummy
    auto displayInfo = AccessibilityDisplayManager::GetInstance().GetDefaultDisplay();
    if (!Intersect(focusRect, displayInfo)) {
        return false;
    }

    float px = (focusRect.GetLeftTopXScreenPostion() + focusRect.GetRightBottomXScreenPostion())/2;
    float py = (focusRect.GetLeftTopYScreenPostion() + focusRect.GetRightBottomYScreenPostion())/2;
    point.Setxy(px, py);
    return true;
}
AccessibilityElementInfo AccessibilityInteractionBridge::FindFocusedElementInfo(const int &windowId)
{
    HILOG_DEBUG("[%{public}s with window]", __func__);
    AccessibilityElementInfo info {};
    bool result = AccessibilityOperator::GetInstance().FindFocusedElementInfo(INTERACTION_BRIDGE_CHANNEL_ID,
        windowId,NONE_ID,FOCUS_TYPE_ACCESSIBILITY, info);
    if (!result) {
        HILOG_ERROR("result is false");
    }
    return info;
}

} // namespace Accessibility
} // namespace OHOS
