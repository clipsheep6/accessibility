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

#include <hitrace_meter.h>

#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"
#include "utils.h"

namespace OHOS {
namespace Accessibility {
AccessibilityWindowManager::AccessibilityWindowManager()
{
}

bool AccessibilityWindowManager::Init()
{
    HILOG_DEBUG("start");
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "QueryWindowInfo");
    std::vector<sptr<Rosen::AccessibilityWindowInfo>> windowInfos;
    Rosen::WMError err = OHOS::Rosen::WindowManager::GetInstance().GetAccessibilityWindowInfo(windowInfos);
    if (err != Rosen::WMError::WM_OK) {
        Utils::RecordUnavailableEvent(A11yUnavailableEvent::QUERY_EVENT, A11yError::ERROR_QUERY_WINDOW_INFO_FAILED);
        HILOG_ERROR("get window info from wms failed. err[%{public}d]", err);
        return false;
    }
    HILOG_DEBUG("windowInfos size is %{public}zu", windowInfos.size());
    for (auto &window : windowInfos) {
        if (!window) {
            HILOG_ERROR("window is nullptr");
            continue;
        }

        if ((window->wid_ == SCENE_BOARD_WINDOW_ID && !subWindows_.count(window->innerWid_)) ||
            (window->wid_ != SCENE_BOARD_WINDOW_ID && !a11yWindows_.count(window->wid_))) {
            auto a11yWindowInfo = CreateAccessibilityWindowInfo(window);
            a11yWindows_[a11yWindowInfo->wid_] = a11yWindowInfo;
            if (a11yWindows_[a11yWindowInfo->wid_].IsFocused()) {
                SetActiveWindow(a11yWindowInfo->wid_);
            }
        }
    }
    return true;
}

void AccessibilityWindowManager::DeInit()
{
    HILOG_DEBUG("start");
    a11yWindows_.clear();
    subWindows_.clear();
    activeWindowId_ = INVALID_WINDOW_ID;
    a11yFocusedWindowId_ = INVALID_WINDOW_ID;
}

AccessibilityWindowManager::~AccessibilityWindowManager()
{
    DeregisterWindowListener();
}

void AccessibilityWindowManager::RegisterWindowListener(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    if (windowListener_) {
        HILOG_DEBUG("Window listener is already registered!");
        return;
    }

    eventHandler_ = handler;
    windowListener_ = new(std::nothrow) AccessibilityWindowListener(*this);
    if (!windowListener_) {
        HILOG_ERROR("Create window listener fail!");
        return;
    }
    OHOS::Rosen::WindowManager::GetInstance().RegisterWindowUpdateListener(windowListener_);
}

void AccessibilityWindowManager::DeregisterWindowListener()
{
    if (windowListener_) {
        OHOS::Rosen::WindowManager::GetInstance().UnregisterWindowUpdateListener(windowListener_);
        windowListener_ = nullptr;
        eventHandler_ = nullptr;
    }
}

void AccessibilityWindowManager::OnWindowUpdate(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos,
    Rosen::WindowUpdateType type)
{
    HILOG_DEBUG("start");
    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return;
    }
    if (infos.size() == 0) {
        HILOG_ERROR("window info is err");
        return;
    }
    eventHandler_->PostTask(std::bind([=]() -> void {
        switch (type) {
            case Rosen::WindowUpdateType::WINDOW_UPDATE_ADDED:
                WindowUpdateAdded(infos);
                break;
            case Rosen::WindowUpdateType::WINDOW_UPDATE_REMOVED:
                WindowUpdateRemoved(infos);
                break;
            case Rosen::WindowUpdateType::WINDOW_UPDATE_BOUNDS:
                WindowUpdateBounds(infos);
                break;
            case Rosen::WindowUpdateType::WINDOW_UPDATE_ACTIVE:
                WindowUpdateActive(infos);
                break;
            case Rosen::WindowUpdateType::WINDOW_UPDATE_FOCUSED:
                WindowUpdateFocused(infos);
                break;
            case Rosen::WindowUpdateType::WINDOW_UPDATE_PROPERTY:
                WindowUpdateProperty(infos);
                break;
            default:
                break;
        }
        HILOG_DEBUG("a11yWindows[%{public}zu]", a11yWindows_.size());
        }), "TASK_ON_WINDOW_UPDATE");
}

int32_t AccessibilityWindowManager::ConvertToRealWindowId(int32_t windowId, int32_t focusType)
{
    int32_t winId;
    HILOG_DEBUG("ConvertToRealWindowId called, windowId[%{public}d], focusType[%{public}d]", windowId, focusType);
    if (windowId == ACTIVE_WINDOW_ID) {
        HILOG_DEBUG("After convert active windowId[%{public}d]", activeWindowId_);
        winId = activeWindowId_;
    }

    if (windowId == ANY_WINDOW_ID) {
        if (focusType == FOCUS_TYPE_ACCESSIBILITY) {
            HILOG_DEBUG("After convert a11yFocused windowId[%{public}d] by accessibility type", a11yFocusedWindowId_);
            winId = a11yFocusedWindowId_;
        } else if (focusType == FOCUS_TYPE_INPUT) {
            HILOG_DEBUG("After convert active windowId[%{public}d] by input type", activeWindowId_);
            winId = activeWindowId_;
        }
    }

    if (subWindows_.count(winId)) {
        HILOG_DEBUG("After convert normal windowId[%{public}d]", SCENE_BOARD_WINDOW_ID);
        return SCENE_BOARD_WINDOW_ID;
    }
    HILOG_DEBUG("After convert windowId[%{public}d] and activeId[%{public}d]", winId, activeWindowId_);
    return winId;
}

AccessibilityWindowType ConvertWindowType(Rosen::WindowType type)
{
    AccessibilityWindowType winType = TYPE_WINDOW_INVALID;

    if (type < Rosen::WindowType::SYSTEM_WINDOW_BASE) {
        winType = TYPE_APPLICATION;
    } else if ((type >= Rosen::WindowType::SYSTEM_WINDOW_BASE) && (type <= Rosen::WindowType::SYSTEM_WINDOW_END)) {
        winType = TYPE_SYSTEM;
    } else {
        HILOG_ERROR("Unknown windowType[%{public}d]", type);
    }
    return winType;
}

void AccessibilityWindowManager::UpdateAccessibilityWindowInfo(AccessibilityWindowInfo &accWindowInfo,
    const sptr<Rosen::AccessibilityWindowInfo> windowInfo)
{
    accWindowInfo.SetWindowId(windowInfo->wid_);
    accWindowInfo.SetWindowType(static_cast<uint32_t>(windowInfo->type_));
    accWindowInfo.SetWindowMode(static_cast<uint32_t>(windowInfo->mode_));
    accWindowInfo.SetAccessibilityWindowType(ConvertWindowType(windowInfo->type_));
    accWindowInfo.SetFocused(windowInfo->focused_);
    accWindowInfo.SetWindowLayer(windowInfo->layer_);
    Rect bound;
    bound.SetLeftTopScreenPostion(windowInfo->windowRect_.posX_, windowInfo->windowRect_.posY_);
    bound.SetRightBottomScreenPostion(
        windowInfo->windowRect_.posX_ + static_cast<int32_t>(windowInfo->windowRect_.width_),
        windowInfo->windowRect_.posY_ + static_cast<int32_t>(windowInfo->windowRect_.height_));
    accWindowInfo.SetRectInScreen(bound);
    accWindowInfo.SetDisplayId(windowInfo->displayId_);
    accWindowInfo.SetDecorEnable(windowInfo->isDecorEnable_);
    accWindowInfo.SetUiNodeId(windowInfo->uiNodeId_);
    accWindowInfo.SetInnerWid(windowInfo->innerWid_);
    if (accWindowInfo.GetWindowId() == SCENE_BOARD_WINDOW_ID) {
        accWindowInfo.SetWindowId(windowInfo->innerWid_);
        HILOG_INFO("scene board window id 1 convert inner window id[%{public}d]", windowInfo->innerWid_);
    }
}

AccessibilityWindowInfo AccessibilityWindowManager::CreateAccessibilityWindowInfo(
    const sptr<Rosen::AccessibilityWindowInfo> windowInfo)
{
    AccessibilityWindowInfo info;
    UpdateAccessibilityWindowInfo(info, windowInfo);
    HILOG_DEBUG("Create WindowInfo Id(%{public}d) type(%{public}d) posX(%{public}d) posY(%{public}d)"
        "witdth(%{public}d) height(%{public}d) display id(%{public}" PRIu64 ") isDecorEnable(%{public}d)"
        "innerWid(%{public}d), uiNodeId(%{public}d)",
        windowInfo->wid_, windowInfo->type_, windowInfo->windowRect_.posX_, windowInfo->windowRect_.posY_,
        windowInfo->windowRect_.width_, windowInfo->windowRect_.height_, windowInfo->displayId_,
        windowInfo->isDecorEnable_, windowInfo->innerWid_, windowInfo->uiNodeId_);
    return info;
}

void AccessibilityWindowManager::SetActiveWindow(int32_t windowId)
{
    HILOG_DEBUG("windowId(%{public}d)", windowId);
    if (windowId == INVALID_WINDOW_ID) {
        ClearOldActiveWindow();
        activeWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_WARN("Window id is not found");
        return;
    }

    if (activeWindowId_ == windowId) {
        HILOG_WARN("Old and new active window is same");
        return;
    }

    ClearOldActiveWindow();
    activeWindowId_ = windowId;
    if (a11yWindows_.count(activeWindowId_)) {
        a11yWindows_[activeWindowId_].SetActive(true);
        AccessibilityEventInfo evtInf(activeWindowId_, WINDOW_UPDATE_ACTIVE);
        auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
        aams.SendEvent(evtInf);
        HILOG_DEBUG("ActiveWindowId is %{public}d", activeWindowId_);
    }
}

void AccessibilityWindowManager::SetAccessibilityFocusedWindow(int32_t windowId)
{
    HILOG_DEBUG("start");
    if (windowId == INVALID_WINDOW_ID) {
        ClearAccessibilityFocused();
        a11yFocusedWindowId_ = INVALID_WINDOW_ID;
        return;
    }

    if (!a11yWindows_.count(windowId)) {
        HILOG_ERROR("Window id[%{public}d] is not found", windowId);
        return;
    }

    if (a11yFocusedWindowId_ == windowId) {
        HILOG_WARN("Old and new focus window is same");
        return;
    }

    ClearAccessibilityFocused();
    a11yFocusedWindowId_ = windowId;
    if (a11yWindows_.count(a11yFocusedWindowId_)) {
        a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(true);
        HILOG_DEBUG("a11yFocusedWindowId_ is %{public}d", a11yFocusedWindowId_);
    }
}

std::vector<AccessibilityWindowInfo> AccessibilityWindowManager::GetAccessibilityWindows()
{
    HILOG_DEBUG("a11yWindows_ size[%{public}zu]", a11yWindows_.size());
    std::vector<sptr<Rosen::AccessibilityWindowInfo>> windowInfos;
    std::vector<AccessibilityWindowInfo> windows;
    Rosen::WMError err = OHOS::Rosen::WindowManager::GetInstance().GetAccessibilityWindowInfo(windowInfos);
    if (err != Rosen::WMError::WM_OK) {
        HILOG_ERROR("get window info from wms failed. err[%{public}d]", err);
        return windows;
    }
    for (auto &info : windowInfos) {
        if (info == nullptr) {
            HILOG_INFO("info is nullptr");
            continue;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (a11yWindows_.count(winInfo->wid_)) {
            a11yWindows_[winInfo->wid_] = winInfo;
        }
    }
    std::transform(a11yWindows_.begin(), a11yWindows_.end(), std::back_inserter(windows),
        [](const std::map<int32_t, AccessibilityWindowInfo>::value_type &window) { return window.second; });

    HILOG_DEBUG("window size[%{public}zu]", windows.size());
    for (auto &logWindow : windows) {
        HILOG_DEBUG("logWindow id[%{public}d]", logWindow.GetWindowId());
    }
    return windows;
}

bool AccessibilityWindowManager::GetAccessibilityWindow(int32_t windowId, AccessibilityWindowInfo &window)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);
    std::vector<sptr<Rosen::AccessibilityWindowInfo>> windowInfos;
    Rosen::WMError err = OHOS::Rosen::WindowManager::GetInstance().GetAccessibilityWindowInfo(windowInfos);
    if (err != Rosen::WMError::WM_OK) {
        HILOG_ERROR("get window info from wms failed. err[%{public}d]", err);
        return false;
    }
    for (auto &info : windowInfos) {
        if (info == nullptr) {
            HILOG_ERROR("info is nullptr");
            continue;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (a11yWindows_.count(winInfo->wid_)) {
            a11yWindows_[winInfo->wid_] = winInfo;
        }
    }

    if (a11yWindows_.count(windowId)) {
        window = a11yWindows_[windowId];
        return true;
    }
    return false;
}

bool AccessibilityWindowManager::IsValidWindow(int32_t windowId)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);

    auto it = std::find_if(a11yWindows_.begin(), a11yWindows_.end(),
        [windowId](const std::map<int32_t, AccessibilityWindowInfo>::value_type &window) {
            return window.first == windowId;
        });
    if (it == a11yWindows_.end()) {
        return false;
    }
    return true;
}

void AccessibilityWindowManager::SetWindowSize(int32_t windowId, Rect rect)
{
    HILOG_DEBUG("start windowId(%{public}d)", windowId);
    auto it = std::find_if(a11yWindows_.begin(), a11yWindows_.end(),
        [windowId](const std::map<int32_t, AccessibilityWindowInfo>::value_type &window) {
            return window.first == windowId;
        });
    if (it != a11yWindows_.end()) {
        it->second.SetRectInScreen(rect);
    }
}

void AccessibilityWindowManager::WindowUpdateAdded(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        a11yWindows_[winInfo->wid_] = winInfo;
        if (windowInfo->wid_ == SCENE_BOARD_WINDOW_ID) {
            subWindows_.insert(winInfo->wid_);
        }
        AccessibilityEventInfo evtInfAdded(winInfo->wid_, WINDOW_UPDATE_ADDED);
        ingleton<AccessibleAbilityManagerService>::GetInstance().SendEvent(evtInfAdded);
        if (a11yWindows_[winInfo->wid_].IsFocused()) {
            SetActiveWindow(winInfo->wid_);
        }
    }
}

void AccessibilityWindowManager::WindowUpdateRemoved(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (!a11yWindows_.count(winInfo->wid_)) {
            return;
        }

        if (winInfo->wid_ == activeWindowId_) {
            SetActiveWindow(INVALID_WINDOW_ID);
        }

        if (winInfo->wid_ == a11yFocusedWindowId_) {
            SetAccessibilityFocusedWindow(INVALID_WINDOW_ID);
        }

        subWindows.erase(winInfo->wid_);
        a11yWindows_.erase(winInfo->wid_);
        AccessibilityEventInfo evtInfRemoved(winInfo->wid_, WINDOW_UPDATE_REMOVED);
        aams.SendEvent(evtInfRemoved);
    }
}

void AccessibilityWindowManager::WindowUpdateFocused(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (!a11yWindows_.count(winInfo->wid_)) {
            a11yWindows_[winInfo->wid_] = winInfo;
        }

        if (windowInfo->wid_ == SCENE_BOARD_WINDOW_ID) {
            subWindows_.insert(winInfo->wid_);
        }

        SetActiveWindow(winInfo->wid_);
        AccessibilityEventInfo evtInfFocused(winInfo->wid_, WINDOW_UPDATE_FOCUSED);
        aams.SendEvent(evtInfFocused);
    }
}

void AccessibilityWindowManager::WindowUpdateBounds(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        AccessibilityEventInfo evtInfBounds(windowInfo->wid_, WINDOW_UPDATE_BOUNDS);
        aams.SendEvent(evtInfBounds);
    }
}

void AccessibilityWindowManager::WindowUpdateActive(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (!a11yWindows_.count(winInfo->wid_)) {
            a11yWindows_[winInfo->wid_] = winInfo;
        }

        if (windowInfo->wid_ == SCENE_BOARD_WINDOW_ID) {
            subWindows_.insert(winInfo->wid_);
        }

        SetActiveWindow(winInfo->wid_);
    }
}

void AccessibilityWindowManager::WindowUpdateProperty(const std::vector<sptr<Rosen::AccessibilityWindowInfo>>& infos)
{
    HILOG_DEBUG("start");
    auto &aams = Singleton<AccessibleAbilityManagerService>::GetInstance();
    for (auto &windowInfo : infos) {
        if (!windowInfo) {
            HILOG_ERROR("invalid windowInfo");
            return;
        }

        auto winInfo = CreateAccessibilityWindowInfo(windowInfo);
        if (a11yWindows_.count(winInfo->wid_)) {
            a11yWindows_[winInfo->wid_] = winInfo;
        }
        
        AccessibilityEventInfo evtInfProperty(winInfo->wid_, WINDOW_UPDATE_PROPERTY);
        aams.SendEvent(evtInfProperty);
    }
}

void AccessibilityWindowManager::ClearOldActiveWindow()
{
    HILOG_DEBUG("active window id is %{public}d", activeWindowId_);
    if (activeWindowId_ == INVALID_WINDOW_ID) {
        HILOG_DEBUG("active window id is invalid");
        return;
    }

    if (a11yWindows_.count(activeWindowId_)) {
        a11yWindows_[activeWindowId_].SetActive(false);
    }

    if (activeWindowId_ == a11yFocusedWindowId_) {
        HILOG_DEBUG("Old active window is a11yFocused window.");
        SetAccessibilityFocusedWindow(INVALID_WINDOW_ID);
    }
}

void AccessibilityWindowManager::ClearAccessibilityFocused()
{
    HILOG_DEBUG("a11yFocused window id is %{public}d", a11yFocusedWindowId_);
    if (a11yFocusedWindowId_ == INVALID_WINDOW_ID) {
        HILOG_DEBUG("a11yFocused window id is invalid");
        return;
    }

    if (a11yWindows_.count(a11yFocusedWindowId_)) {
        a11yWindows_[a11yFocusedWindowId_].SetAccessibilityFocused(false);
    }

    int32_t windowId = a11yFocusedWindowId_;
    if (subWindows_.count(a11yFocusedWindowId_)) {
        windowId = SCENE_BOARD_WINDOW_ID;
    }
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountData();
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr");
        return;
    }
    sptr<AccessibilityWindowConnection> connection =
        accountData->GetAccessibilityWindowConnection(windowId);
    if (!connection) {
        HILOG_ERROR("windowId[%{public}d] has no connection", windowId);
        return;
    }
    if (!connection->GetProxy()) {
        HILOG_ERROR("windowId[%{public}d] has no proxy", windowId);
        return;
    }
    connection->GetProxy()->ClearFocus();

    // Send event
    AccessibilityEventInfo eventInfo(TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED_EVENT);
    eventInfo.SetWindowId(a11yFocusedWindowId_);
    Singleton<AccessibleAbilityManagerService>::GetInstance().SendEvent(eventInfo);
}

int32_t AccessibilityWindowManager::GetSceneBoardElementId(const int32_t windowId, const int32_t elementId)
{
    if (subWindow_.count(windowId)) {
        auto iter = a11yWindows_.find(windowId);
        if (iter != a11yWindows_.end()) {
            return iter->second.GetUiNodeId();
        }
    }
    return elementId;
}
} // namespace Accessibility
} // namespace OHOS