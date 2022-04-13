/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
AccessibilityWindowInfoManager::AccessibilityWindowInfoManager()
{
    windowListener_ = new AccessibilityWindowListener(*this);
    eventHandler_ = nullptr;
}

AccessibilityWindowInfoManager& AccessibilityWindowInfoManager::GetInstance()
{
    static AccessibilityWindowInfoManager windowManager;
    return windowManager;
}

void AccessibilityWindowInfoManager::OnWindowUpdate(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo,
    Rosen::WindowUpdateType type)
{
    HILOG_DEBUG("start");
    (void)windowInfo;
    (void)type;
}

void AccessibilityWindowInfoManager::WindowUpdate(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo,
    Rosen::WindowUpdateType type)
{
    HILOG_DEBUG("windowId[%{public}d] type[%{public}d]", windowInfo->currentWindowInfo_->wid_, type);
    (void)windowInfo;
    (void)type;
}

int32_t AccessibilityWindowInfoManager::ConvertToRealWindowId(int32_t windowId, int32_t focusType)
{
    (void)windowId;
    (void)focusType;
    return 0;
}

WindowType ConvertWindowType(Rosen::WindowType type)
{
    (void)type;
    WindowType winType = TYPE_WINDOW_INVALID;

    if (type < Rosen::WindowType::SYSTEM_WINDOW_BASE) {
        winType = TYPE_APPLICATION;
    } else if ((type >= Rosen::WindowType::SYSTEM_WINDOW_BASE) && (type <= Rosen::WindowType::SYSTEM_WINDOW_END)) {
        winType = TYPE_SYSTEM;
    } else {
        HILOG_ERROR("Unknown windowType[%{public}d]", type);
    }
    return winType;
}

AccessibilityWindowInfo AccessibilityWindowInfoManager::CreateAccessibilityWindowInfo(
    Rosen::AccessibilityWindowInfo& windowInfo)
{
    (void)windowInfo;
    AccessibilityWindowInfo info;
    info.SetWindowId(0);
    info.SetWindowType(ConvertWindowType(Rosen::WindowType::SYSTEM_WINDOW_BASE));
    info.SetFocused(true);
    Rect bound(0, 100, 800, 400);
    info.SetRectInScreen(bound);
    return info;
}

void AccessibilityWindowInfoManager::RegisterWindowChangeListener()
{}

void AccessibilityWindowInfoManager::DeregisterWindowChangeListener()
{}

void AccessibilityWindowInfoManager::SetActiveWindow(int32_t windowId)
{
    (void)windowId;
}

void AccessibilityWindowInfoManager::SetAccessibilityFocusedWindow(int32_t windowId)
{
    (void)windowId;
}

void AccessibilityWindowInfoManager::SetInputFocusedWindow(int32_t windowId)
{
    (void)windowId;
}

std::vector<AccessibilityWindowInfo> AccessibilityWindowInfoManager::GetAccessibilityWindows()
{
    std::vector<AccessibilityWindowInfo> windows;
    return windows;
}

bool AccessibilityWindowInfoManager::GetAccessibilityWindow(int32_t windowId, AccessibilityWindowInfo& window)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);
    if (a11yWindows_.count(windowId)) {
        window = a11yWindows_[windowId];
        return true;
    }
    return false;
}

bool AccessibilityWindowInfoManager::IsValidWindow(int32_t windowId)
{
    (void)windowId;
    return true;
}

void AccessibilityWindowInfoManager::SetWindowSize(int32_t windowId, Rect rect)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);
    for (auto& window : a11yWindows_) {
        if (window.first == windowId) {
            window.second.SetRectInScreen(rect);
            return;
        }
    }
}

void AccessibilityWindowInfoManager::UpdateWindowLayer(const sptr<Rosen::AccessibilityWindowInfo>& windowInfo)
{
    HILOG_DEBUG("start");
    int32_t layer = 0;
    for (auto& window : windowInfo->windowList_) {
        auto it = a11yWindows_.find(window->wid_);
        if (it == a11yWindows_.end()) {
            HILOG_ERROR("The window(%{public}d) not in a11yWindows_", window->wid_);
            layer++;
        } else {
            it->second.SetWindowLayer(layer);
            layer++;
        }
    }
}
} // namespace Accessibility
} // namespace OHOS