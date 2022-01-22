/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed On an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "accessibility_window_manager.h"
#include "accessible_ability_manager_service.h"

namespace OHOS {
namespace Accessibility {

AccessibilityWindowInfoManager &AccessibilityWindowInfoManager::GetInstance() {
    static AccessibilityWindowInfoManager windowManager;
    return windowManager;
}

int AccessibilityWindowInfoManager::ConvertToRealWindowId(int windowId, int focusType) {
    HILOG_DEBUG("ConvertToRealWindowId called, windowId[%{public}d], focusType[%{public}d]", windowId, focusType);
    if (windowId == ACTIVE_WINDOW_ID) {
        HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", activeWindowId_);
        return activeWindowId_;
    }

    if (windowId == ANY_WINDOW_ID) {
        if (focusType == FOCUS_TYPE_ACCESSIBILITY) {
            HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", a11yFocusedWindowId_);
            return a11yFocusedWindowId_;
        } else if (focusType == FOCUS_TYPE_INPUT) {
            HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", inputFocusedWindowId_);
            return inputFocusedWindowId_;
        }
    }
    HILOG_DEBUG("ConvertToRealWindowId called, after convert windowId[%{public}d]", windowId);
    return windowId;
}

WindowType ConvertWindowType(Rosen::WindowType type) {
    WindowType winType = TYPE_WINDOW_INVALID;
    if (type < Rosen::WindowType::SYSTEM_WINDOW_BASE) {
        winType = TYPE_APPLICATION;
    } else {
        winType = TYPE_SYSTEM;
    }
    return winType;
}

AccessibilityWindowInfo AccessibilityWindowInfoManager::CreateAccessibilityWindowInfo(Rosen::WindowInfo &windowInfo) {
    AccessibilityWindowInfo info;
    info.SetWindowId(windowInfo.wid);
    info.SetWindowType(ConvertWindowType(windowInfo.type));
    // TODO: Set anchorId or anchorPosition
    info.SetFocused(windowInfo.isFocused);
    Rect bound;
    bound.SetLeftTopScreenPostion(windowInfo.positionX, windowInfo.positionY);
    bound.SetRightBottomScreenPostion(windowInfo.positionX + windowInfo.width,
                                      windowInfo.positionY + windowInfo.height);
    info.SetRectInScreen(bound);

    return info;
}

void AccessibilityWindowInfoManager::RegisterWindowChangeListener() {
    if (!registerFlag_) {
        auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
        Rosen::WindowManager::GetInstance().RegisterWindowUpdateListener(this);
        registerFlag_ = true;
        return;
    }
}

void AccessibilityWindowInfoManager::DeregisterWindowChangeListener() {
    if (registerFlag_) {
        auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
        Rosen::WindowManager::GetInstance().UnregisterWindowUpdateListener(this);
        registerFlag_ = false;
        return;
    }
}

void AccessibilityWindowInfoManager::OnWindowChanged(int windowId, bool add) {
    auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
    HILOG_DEBUG("%{public}s: windowId[%{public}d] add[%{public}d]", __func__, windowId, add);
    if (add) {
        auto accountData = aams->GetCurrentAccountData();
        if (accountData->GetAccessibilityInteractionConnection(windowId)) {
            sptr<Rosen::WindowInfo> windowInfo = Rosen::WindowManager::GetInstance().GetWindowInfo(windowId);
            if (!windowInfo) {
                HILOG_ERROR("GetWindowInfo failed by window id!!");
                return;
            }

            auto a11yWindowInfo = CreateAccessibilityWindowInfo(*windowInfo);
            a11yWindows_.emplace(windowId, a11yWindowInfo);

            AccessibilityEventInfo evtInf(windowId, WINDOW_UPDATE_ADDED);
            aams->SendEvent(evtInf, aams->GetCurrentAccountId());
            if (a11yWindowInfo.IsFocused()) {
                SetActiveWindow(windowId);
            }
        }
    } else {
        if (!a11yWindows_.count(windowId)) {
            return;
        }

        if (windowId == activeWindowId_) {
            SetActiveWindow(INVALID_WINDOW_ID);
        }

        if (windowId == a11yFocusedWindowId_) {
            SetAccessibilityFocusedWindow(INVALID_WINDOW_ID);
        }

        if (windowId == inputFocusedWindowId_) {
            SetInputFocusedWindow(INVALID_WINDOW_ID);
        }

        a11yWindows_.erase(windowId);

        AccessibilityEventInfo evtInf(windowId, WINDOW_UPDATE_REMOVED);
        aams->SendEvent(evtInf, aams->GetCurrentAccountId());
    }
}

void AccessibilityWindowInfoManager::OnWindowUpdate(uint32_t windowId, Rosen::WindowUpdateType type) {
    HILOG_DEBUG("ycz OnWindowUpdate, windowId and updateType: %{public}d, %{public}x", windowId, type);
    sptr<Rosen::WindowInfo> info = Rosen::WindowManager::GetInstance().GetWindowInfo(windowId);
    if (info->wret == Rosen::WMError::WM_ERROR_NULLPTR) {
        HILOG_ERROR("ycz wret error: nullptr");
        return;
    }
    switch(type) {
        case Rosen::WindowUpdateType::WINDOW_UPDATE_ADDED:{
            HILOG_DEBUG("ycz window added:wret[%{public}x], wid[%{public}d],width[%{public}d], \
            height[%{public}d],positionX[%{public}d],positionY[%{public}d],\
            isFocused[%{public}d],mode[%{public}d],type[%{public}d]",
            info->wret, info->wid, info->width,info->height,info->positionX,info->positionY,
            info->isFocused,info->mode,info->type);
            OnWindowChanged(windowId, true);
            break;
        }
        case Rosen::WindowUpdateType::WINDOW_UPDATE_REMOVED: {
            HILOG_DEBUG("ycz window removed:wret[%{public}x], wid[%{public}d],width[%{public}d], \
            height[%{public}d],positionX[%{public}d],positionY[%{public}d],\
            isFocused[%{public}d],mode[%{public}d],type[%{public}d]",
            info->wret, info->wid, info->width,info->height,info->positionX,info->positionY,
            info->isFocused,info->mode,info->type);
            OnWindowChanged(windowId, false);
            break;
        }
        case Rosen::WindowUpdateType::WINDOW_UPDATE_FOCUSED:
            HILOG_DEBUG("ycz window focused");
            break;
        default:
            HILOG_DEBUG("other update type");
    }
}

void AccessibilityWindowInfoManager::SetActiveWindow(int windowId) {
    HILOG_DEBUG("%{public}s start", __func__);
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(activeWindowId_)) {
            a11yWindows_[activeWindowId_].SetActive(false);
        }
        activeWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (activeWindowId_ != windowId) {
        if (a11yWindows_.count(activeWindowId_)) {
            a11yWindows_[activeWindowId_].SetActive(false);
        }
        activeWindowId_ = windowId;
        a11yWindows_[activeWindowId_].SetActive(true);
        auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
        AccessibilityEventInfo evtInf(activeWindowId_, WINDOW_UPDATE_ACTIVE);
        aams->SendEvent(evtInf, aams->GetCurrentAccountId());
    }
    HILOG_DEBUG("%{public}s %{public}d", __func__, activeWindowId_);
}

void AccessibilityWindowInfoManager::SetAccessibilityFocusedWindow(int windowId) {
    HILOG_DEBUG("%{public}s start", __func__);
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(a11yFocusedWindowId_)) {
            a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(false);
        }
        a11yFocusedWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (a11yFocusedWindowId_ != windowId) {
        if (a11yWindows_.count(a11yFocusedWindowId_)) {
            a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(false);
        }
        a11yFocusedWindowId_ = windowId;
        a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(true);
    }
    HILOG_DEBUG("%{public}s %{public}d", __func__, a11yFocusedWindowId_);
}

void AccessibilityWindowInfoManager::SetInputFocusedWindow(int windowId) {
    if (windowId == INVALID_WINDOW_ID) {
        if (a11yWindows_.count(inputFocusedWindowId_)) {
            a11yWindows_[inputFocusedWindowId_].SetFocused(false);
        }
        inputFocusedWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (inputFocusedWindowId_ != windowId) {
        if (a11yWindows_.count(inputFocusedWindowId_)) {
            a11yWindows_[inputFocusedWindowId_].SetFocused(false);
        }
        inputFocusedWindowId_ = windowId;
        a11yWindows_[inputFocusedWindowId_].SetFocused(true);
    }
}

std::vector<AccessibilityWindowInfo> AccessibilityWindowInfoManager::GetAccessibilityWindows() {
    HILOG_INFO("%{public}s", __func__);
    HILOG_DEBUG("a11yWindows_ size[%{public}d]", a11yWindows_.size());
    std::vector<AccessibilityWindowInfo> windows;
    for (auto window : a11yWindows_) {
        windows.push_back(window.second);
    }
    HILOG_DEBUG("window size[%{public}d]", windows.size());
    for (auto logWindow : windows) {
        HILOG_DEBUG("logWindow id[%{public}d]", logWindow.GetWindowId());
    }
    return windows;
}

bool AccessibilityWindowInfoManager::GetAccessibilityWindow(int windowId, AccessibilityWindowInfo &window)
{
    HILOG_INFO("%{public}s", __func__);
    if (a11yWindows_.count(windowId)) {
        window = a11yWindows_[windowId];
        return true;
    }
    return false;
}

bool AccessibilityWindowInfoManager::IsValidWindow(int windowId)
{
    HILOG_INFO("%{public}s", __func__);
    if (a11yWindows_.count(windowId)) {
        return true;
    }
    return false;
}

void AccessibilityWindowInfoManager::SetWindowSize(int windowId, Rect rect)
{
    HILOG_INFO("%{public}s", __func__);
    for (auto &window : a11yWindows_) {
        if (window.first == windowId) {
            window.second.SetRectInScreen(rect);
            return;
        }
    }
}
}  // namespace Accessibility
}  // namespace OHOS