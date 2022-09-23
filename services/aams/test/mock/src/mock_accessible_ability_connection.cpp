/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mock_accessible_ability_connection.h"
#include <algorithm>
#include <map>
#include <vector>
#include "accessibility_account_data.h"
#include "accessibility_ut_helper.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
MockAccessibleAbilityConnection::MockAccessibleAbilityConnection(const sptr<AccessibilityAccountData>& accountData,
    const int32_t connectionId, AccessibilityAbilityInfo& abilityInfo)
    : AccessibleAbilityConnection(accountData, connectionId, abilityInfo)
{
    (void)accountData;
    (void)connectionId;
    (void)abilityInfo;
}
MockAccessibleAbilityConnection::~MockAccessibleAbilityConnection()
{}

AccessibleAbilityChannel::AccessibleAbilityChannel(const int32_t accountId, const std::string &clientName)
    : clientName_(clientName), accountId_(accountId)
{
}

bool AccessibleAbilityChannel::SearchElementInfoByAccessibilityId(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback,
    const int32_t mode)
{
    (void)accessibilityWindowId;
    (void)elementId;
    (void)requestId;
    (void)callback;
    (void)mode;
    return true;
}

bool AccessibleAbilityChannel::SearchElementInfosByText(const int32_t accessibilityWindowId,
    const int32_t elementId, const std::string& text, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback)
{
    (void)accessibilityWindowId;
    (void)elementId;
    (void)text;
    (void)requestId;
    (void)callback;
    return true;
}

bool AccessibleAbilityChannel::FindFocusedElementInfo(const int32_t accessibilityWindowId,
    const int32_t elementId, const int32_t focusType, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback)
{
    (void)accessibilityWindowId;
    (void)elementId;
    (void)focusType;
    (void)requestId;
    (void)callback;
    return true;
}

bool AccessibleAbilityChannel::FocusMoveSearch(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t direction, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback>& callback)
{
    (void)accessibilityWindowId;
    (void)elementId;
    (void)direction;
    (void)requestId;
    (void)callback;
    return true;
}

bool AccessibleAbilityChannel::ExecuteAction(const int32_t accessibilityWindowId, const int32_t elementId,
    const int32_t action, const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback>& callback)
{
    (void)accessibilityWindowId;
    (void)elementId;
    (void)action;
    (void)actionArguments;
    (void)requestId;
    (void)callback;
    return true;
}

bool AccessibleAbilityChannel::GetWindows(uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows) const
{
    (void)displayId;
    (void)windows;
    sptr<AccessibleAbilityConnection> clientConnection = GetConnection(accountId_, clientName_);
    if (!clientConnection) {
        HILOG_ERROR("There is no client connection");
        return false;
    }

    if (!(clientConnection->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_RETRIEVE)) {
        HILOG_ERROR("AccessibleAbilityChannel::GetWindows failed: no capability");
        return false;
    }
    return true;
}

bool AccessibleAbilityChannel::GetWindows(std::vector<AccessibilityWindowInfo> &windows)
{
    uint64_t displayId = 0;
    return GetWindows(displayId, windows);
}

bool AccessibleAbilityChannel::GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo)
{
    (void)windowId;
    (void)windowInfo;
    return true;
}

bool AccessibleAbilityChannel::SetEventTypeFilter(const uint32_t filter)
{
    (void)filter;
    return true;
}

bool AccessibleAbilityChannel::SetTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    (void)targetBundleNames;
    return true;
}

bool AccessibleAbilityChannel::GetWindowsByDisplayId(const uint64_t displayId,
    std::vector<AccessibilityWindowInfo> &windows)
{
    return GetWindows(displayId, windows);
}

bool AccessibleAbilityChannel::ExecuteCommonAction(int32_t action)
{
    (void)action;
    // temp deal: need external dependence
    return true;
}

void AccessibleAbilityChannel::SetOnKeyPressEventResult(const bool handled, const int32_t sequence)
{
    (void)handled;
    (void)sequence;
}

bool AccessibleAbilityChannel::SendSimulateGesture(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath)
{
    (void)gesturePath;
    return true;
}

sptr<AccessibleAbilityConnection> AccessibleAbilityChannel::GetConnection(int32_t accountId,
    const std::string &clientName)
{
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetAccountData(accountId);
    if (!accountData) {
        HILOG_ERROR("accountData is nullptr");
        return nullptr;
    }

    return accountData->GetAccessibleAbilityConnection(clientName);
}

AccessibleAbilityConnection::AccessibleAbilityConnection(
    const sptr<AccessibilityAccountData>& accountData, int32_t connectionId, AccessibilityAbilityInfo& abilityInfo)
{
    connectionId_ = connectionId;
    abilityInfo_ = abilityInfo;
    accountData_ = accountData;
}

AccessibleAbilityConnection::~AccessibleAbilityConnection()
{
    HILOG_DEBUG("start");
}

void AccessibleAbilityConnection::OnAbilityConnectDone(
    const AppExecFwk::ElementName& element, const sptr<IRemoteObject>& remoteObject, int32_t resultCode)
{
    (void)element;
    (void)remoteObject;
    (void)resultCode;
    sptr<AccessibleAbilityConnection> pointer = this;
    if (accountData_ != nullptr) {
        accountData_->AddConnectedAbility(pointer);
    }
}

void AccessibleAbilityConnection::OnAbilityDisconnectDone(const AppExecFwk::ElementName& element, int32_t resultCode)
{
    (void)element;
    (void)resultCode;
    return;
}

bool AccessibleAbilityConnection::OnKeyPressEvent(const MMI::KeyEvent& keyEvent, const int32_t sequence)
{
    (void)keyEvent;
    (void)sequence;
    return false;
}

bool AccessibleAbilityConnection::IsWantedEvent(int32_t eventType)
{
    (void)eventType;
    return true;
}

AAFwk::Want CreateWant(AppExecFwk::ElementName& element)
{
    (void)element;
    AAFwk::Want want;
    return want;
}

void AccessibleAbilityConnection::Disconnect()
{
    HILOG_DEBUG("start");
    AccessibilityAbilityHelper::GetInstance().SetTestChannelId(-1);
}

void AccessibleAbilityConnection::Connect(const AppExecFwk::ElementName& element)
{
    HILOG_DEBUG("start");
    (void)element;
}

int32_t AccessibleAbilityConnection::GetChannelId()
{
    HILOG_DEBUG("connectionId_ is %{public}d", connectionId_);
    return connectionId_;
}

void AccessibleAbilityConnection::AccessibleAbilityConnectionDeathRecipient::OnRemoteDied(
    const wptr<IRemoteObject>& remote)
{
    (void)remote;
    // Temp deal: notify setting
}

void AccessibleAbilityConnection::OnAccessibilityEvent(AccessibilityEventInfo &eventInfo)
{
    (void)eventInfo;
}
} // namespace Accessibility
} // namespace OHOS