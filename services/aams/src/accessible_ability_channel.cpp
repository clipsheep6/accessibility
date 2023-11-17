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

#include "accessible_ability_channel.h"
#include "accessible_ability_manager_service.h"
#include "accessibility_window_connection.h"
#include "accessibility_window_manager.h"
#include "accessible_ability_connection.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr uint32_t TIME_OUT_OPERATOR = 5000;
} // namespace

AccessibleAbilityChannel::AccessibleAbilityChannel(const int32_t accountId, const std::string &clientName)
    : clientName_(clientName), accountId_(accountId)
{
    eventHandler_ = std::make_shared<AppExecFwk::EventHandler>(
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetMainRunner());
}

RetError AccessibleAbilityChannel::SearchElementInfoByAccessibilityId(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
    const int32_t mode)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([syncPromise, accessibilityWindowId, elementId, requestId, callback, mode](
        int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("search element accountId[%{public}d], name[%{public}s]", accountId, name.c_str());
        sptr<IAccessibilityElementOperator> elementOperator = nullptr;
        RetError ret = GetElementOperator(accountId, accessibilityWindowId, FOCUS_TYPE_INVALID, name, elementOperator);
        if (ret != RET_OK) {
            HILOG_ERROR("Get elementOperator failed! accessibilityWindowId[%{public}d]", accessibilityWindowId);
            syncPromise->set_value(ret);
            return;
        }

        auto& awm = Singleton<AccessibilityWindowManager>::GetInstance();
        int32_t realElementId = awm.GetSceneBoardElementId(accessibilityWindowId, elementId);
        elementOperator->SearchElementInfoByAccessibilityId(realElementId, requestId, callback, mode);
        HILOG_DEBUG("AccessibleAbilityChannel::SearchElementInfoByAccessibilityId successfully");
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "SearchElementInfoByAccessibilityId");
    
    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait SearchElementInfoByAccessibilityId result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::SearchElementInfosByText(const int32_t accessibilityWindowId,
    const int32_t elementId, const std::string &text, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([syncPromise, accessibilityWindowId, elementId, text, requestId, callback](
        int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("accountId[%{public}d], name[%{public}s]", accountId, name.c_str());
        sptr<IAccessibilityElementOperator> elementOperator = nullptr;
        RetError ret = GetElementOperator(accountId, accessibilityWindowId, FOCUS_TYPE_INVALID, name, elementOperator);
        if (ret != RET_OK) {
            HILOG_ERROR("Get elementOperator failed! accessibilityWindowId[%{public}d]", accessibilityWindowId);
            syncPromise->set_value(ret);
            return;
        }

        auto& awm = Singleton<AccessibilityWindowManager>::GetInstance();
        int32_t realElementId = awm.GetSceneBoardElementId(accessibilityWindowId, elementId);
        elementOperator->SearchElementInfosByText(realElementId, text, requestId, callback);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "SearchElementInfosByText");

    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait SearchElementInfosByText result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::FindFocusedElementInfo(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t focusType, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([syncPromise, accessibilityWindowId, elementId,
        focusType, requestId, callback](int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("accountId[%{public}d], name[%{public}s]", accountId, name.c_str());
        sptr<IAccessibilityElementOperator> elementOperator = nullptr;
        RetError ret = GetElementOperator(accountId, accessibilityWindowId, focusType, name, elementOperator);
        if (ret != RET_OK) {
            HILOG_ERROR("Get elementOperator failed! accessibilityWindowId[%{public}d]", accessibilityWindowId);
            syncPromise->set_value(ret);
            return;
        }

        auto& awm = Singleton<AccessibilityWindowManager>::GetInstance();
        int32_t realElementId = awm.GetSceneBoardElementId(accessibilityWindowId, elementId);
        elementOperator->FindFocusedElementInfo(realElementId, focusType, requestId, callback);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "FindFocusedElementInfo");
    
    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait FindFocusedElementInfo result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::FocusMoveSearch(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t direction, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([syncPromise, accessibilityWindowId,
        elementId, direction, requestId, callback](int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("accountId[%{public}d], name[%{public}s]", accountId, name.c_str());
        sptr<IAccessibilityElementOperator> elementOperator = nullptr;
        RetError ret = GetElementOperator(accountId, accessibilityWindowId, FOCUS_TYPE_INVALID, name, elementOperator);
        if (ret != RET_OK) {
            HILOG_ERROR("Get elementOperator failed! accessibilityWindowId[%{public}d]", accessibilityWindowId);
            syncPromise->set_value(ret);
            return;
        }

        auto& awm = Singleton<AccessibilityWindowManager>::GetInstance();
        int32_t realElementId = awm.GetSceneBoardElementId(accessibilityWindowId, elementId);
        elementOperator->FocusMoveSearch(realElementId, direction, requestId, callback);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "FocusMoveSearch");
    
    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait FocusMoveSearch result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::ExecuteAction(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t action, const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([syncPromise, accessibilityWindowId, elementId, action,
        actionArguments, requestId, callback](int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("accountId[%{public}d], name[%{public}s]", accountId, name.c_str());
        sptr<IAccessibilityElementOperator> elementOperator = nullptr;
        RetError ret = GetElementOperator(accountId, accessibilityWindowId, FOCUS_TYPE_INVALID, name, elementOperator);
        if (ret != RET_OK) {
            HILOG_ERROR("Get elementOperator failed! accessibilityWindowId[%{public}d]", accessibilityWindowId);
            syncPromise->set_value(ret);
            return;
        }

        auto& awm = Singleton<AccessibilityWindowManager>::GetInstance();
        int32_t realElementId = awm.GetSceneBoardElementId(accessibilityWindowId, elementId);
        elementOperator->ExecuteAction(realElementId, action, actionArguments, requestId, callback);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "ExecuteAction");
    
    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait ExecuteAction result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo)
{
    HILOG_DEBUG("windowId:%{public}d", windowId);

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }
 
    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::shared_ptr<AccessibilityWindowInfo> tmpWindowInfo = std::make_shared<AccessibilityWindowInfo>(windowInfo);
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([windowId, tmpWindowInfo, syncPromise](
        int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG("windowId:%{public}d", windowId);
        sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, name);
        if (!clientConnection) {
            HILOG_ERROR("There is no client connection");
            syncPromise->set_value(RET_ERR_NO_CONNECTION);
            return;
        }
        if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_RETRIEVE)) {
            HILOG_ERROR("AccessibleAbilityChannel::GetWindow failed: no capability");
            syncPromise->set_value(RET_ERR_NO_CAPABILITY);
            return;
        }

        if (Singleton<AccessibilityWindowManager>::GetInstance().GetAccessibilityWindow(windowId, *tmpWindowInfo)) {
            syncPromise->set_value(RET_OK);
        } else {
            syncPromise->set_value(RET_ERR_NO_WINDOW_CONNECTION);
        }
        }, accountId_, clientName_), "GetWindow");

    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait GetWindow result");
        return RET_ERR_TIME_OUT;
    }

    windowInfo = *tmpWindowInfo;
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::GetWindows(std::vector<AccessibilityWindowInfo> &windows)
{
    HILOG_DEBUG();
    uint64_t displayId = Singleton<AccessibilityDisplayManager>::GetInstance().GetDefaultDisplayId();
    HILOG_DEBUG("default display id is %{public}" PRIu64 "", displayId);
    return GetWindows(displayId, windows);
}

RetError AccessibleAbilityChannel::GetWindowsByDisplayId(const uint64_t displayId,
    std::vector<AccessibilityWindowInfo> &windows)
{
    HILOG_DEBUG();
    return GetWindows(displayId, windows);
}

RetError AccessibleAbilityChannel::GetWindows(uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows) const
{
    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    auto tmpWindows = std::make_shared<std::vector<AccessibilityWindowInfo>>(windows);
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([displayId, tmpWindows, syncPromise](
        int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG();
        sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, name);
        if (!clientConnection) {
            HILOG_ERROR("There is no client connection");
            syncPromise->set_value(RET_ERR_NO_CONNECTION);
            return;
        }

        if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_RETRIEVE)) {
            HILOG_ERROR("GetWindows failed: no capability");
            syncPromise->set_value(RET_ERR_NO_CAPABILITY);
            return;
        }

        std::vector<AccessibilityWindowInfo> windowInfos =
            Singleton<AccessibilityWindowManager>::GetInstance().GetAccessibilityWindows();
        for (auto &window : windowInfos) {
            HILOG_ERROR("wj675913  window_id[%{public}d], ltopx[%{public}d], ltopy[%{public}d], rbotx[%{public}d], rboty[%{public}d], isDecorEnable_[%{public}d].",
                window.GetWindowId(), window.GetRectInScreen().GetLeftTopXScreenPostion(),
                window.GetRectInScreen().GetLeftTopYScreenPostion(), window.GetRectInScreen().GetRightBottomXScreenPostion(),
                window.GetRectInScreen().GetRightBottomYScreenPostion(), window.IsDecorEnable());           

            if (window.GetDisplayId() == displayId) {
                tmpWindows->emplace_back(window);
            }
        }
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "GetWindows");
    
    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait GetWindows result");
        return RET_ERR_TIME_OUT;
    }

    windows = *tmpWindows;
    return syncFuture.get();
}

void AccessibleAbilityChannel::SetOnKeyPressEventResult(const bool handled, const int32_t sequence)
{
    HILOG_DEBUG();

    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr.");
        return;
    }

    eventHandler_->PostTask(std::bind([=](int32_t accountId, const std::string &name) -> void {
        sptr<KeyEventFilter> keyEventFilter =
            Singleton<AccessibleAbilityManagerService>::GetInstance().GetKeyEventFilter();
        if (!keyEventFilter) {
            return;
        }

        sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, name);
        if (!clientConnection) {
            HILOG_ERROR("There is no client connection");
            return;
        }
        keyEventFilter->SetServiceOnKeyEventResult(*clientConnection, handled, sequence);
        }, accountId_, clientName_), "SetOnKeyPressEventResult");
}

RetError AccessibleAbilityChannel::SendSimulateGesture(
    const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath)
{
    HILOG_INFO();
    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([gesturePath, syncPromise](int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG();
        sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, name);
        if (!clientConnection) {
            HILOG_ERROR("There is no client connection");
            syncPromise->set_value(RET_ERR_NO_CONNECTION);
            return;
        }

        if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_GESTURE)) {
            HILOG_ERROR("AccessibleAbilityChannel::SendSimulateGesture failed: no capability");
            syncPromise->set_value(RET_ERR_NO_CAPABILITY);
            return;
        }

        sptr<TouchEventInjector> touchEventInjector =
            Singleton<AccessibleAbilityManagerService>::GetInstance().GetTouchEventInjector();
        if (!touchEventInjector) {
            HILOG_ERROR("touchEventInjector is null");
            syncPromise->set_value(RET_ERR_NO_INJECTOR);
            return;
        }
        touchEventInjector->InjectEvents(gesturePath);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "SendSimulateGesture");

    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait SendSimulateGesture result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

RetError AccessibleAbilityChannel::SetTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    HILOG_DEBUG();
    if (!eventHandler_) {
        HILOG_ERROR("eventHandler_ is nullptr");
        return RET_ERR_NULLPTR;
    }

    std::shared_ptr<std::promise<RetError>> syncPromise = std::make_shared<std::promise<RetError>>();
    std::future syncFuture = syncPromise->get_future();
    eventHandler_->PostTask(std::bind([targetBundleNames, syncPromise](
        int32_t accountId, const std::string &name) -> void {
        HILOG_DEBUG();
        sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, name);
        if (!clientConnection) {
            HILOG_ERROR("There is no client connection");
            syncPromise->set_value(RET_ERR_NO_CONNECTION);
            return;
        }

        clientConnection->SetAbilityInfoTargetBundleName(targetBundleNames);
        syncPromise->set_value(RET_OK);
        }, accountId_, clientName_), "SetTargetBundleName");

    std::future_status wait = syncFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait SetTargetBundleName result");
        return RET_ERR_TIME_OUT;
    }
    return syncFuture.get();
}

sptr<AccessibleAbilityConnection> AccessibleAbilityChannel::GetConnection(
    int32_t accountId, const std::string &clientName)
{
    HILOG_DEBUG();
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr");
        return nullptr;
    }

    HILOG_DEBUG("accountId[%{public}d] clientName[%{public}s]", accountId, clientName.c_str());
    return accountData->GetAccessibleAbilityConnection(clientName);
}

RetError AccessibleAbilityChannel::GetElementOperator(
    int32_t accountId, int32_t windowId, int32_t focusType, const std::string &clientName,
    sptr<IAccessibilityElementOperator> &elementOperator)
{
    HILOG_DEBUG();
    elementOperator = nullptr;
    sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId, clientName);
    if (!clientConnection) {
        HILOG_ERROR("There is no client connection");
        return RET_ERR_NO_CONNECTION;
    }
    if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_RETRIEVE)) {
        HILOG_ERROR("the client has no retieve capability");
        return RET_ERR_NO_CAPABILITY;
    }

    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr");
        return RET_ERR_NULLPTR;
    }
    int32_t realId = Singleton<AccessibilityWindowManager>::GetInstance().ConvertToRealWindowId(windowId, focusType);
    sptr<AccessibilityWindowConnection> connection = accountData->GetAccessibilityWindowConnection(realId);
    if (!connection) {
        HILOG_ERROR("windowId[%{public}d] has no connection", realId);
        return RET_ERR_NO_WINDOW_CONNECTION;
    }
    elementOperator = connection->GetProxy();
    if (!elementOperator) {
        HILOG_ERROR("The proxy of window connection is nullptr");
        return RET_ERR_NULLPTR;
    }
    return RET_OK;
}
} // namespace Accessibility
} // namespace OHOS