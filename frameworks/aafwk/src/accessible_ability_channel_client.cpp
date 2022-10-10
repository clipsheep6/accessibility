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

#include "accessible_ability_channel_client.h"

#include <cinttypes>
#include <hitrace_meter.h>

#include "accessibility_element_operator_callback_impl.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr uint32_t TIME_OUT_OPERATOR = 5000;
    constexpr int32_t REQUEST_ID_MAX = 0x0000FFFF;
} // namespace

int32_t AccessibleAbilityChannelClient::GenerateRequestId()
{
    int32_t requestId = requestId_++;
    requestId = requestId % REQUEST_ID_MAX;

    return requestId;
}

sptr<IRemoteObject> AccessibleAbilityChannelClient::GetRemote()
{
    return proxy_->AsObject();
}

void AccessibleAbilityChannelClient::SetOnKeyPressEventResult(const bool handled, const int32_t sequence)
{
    HILOG_INFO("[channelId:%{public}d]", channelId_);
    if (proxy_) {
        proxy_->SetOnKeyPressEventResult(handled, sequence);
    } else {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
    }
}

bool AccessibleAbilityChannelClient::FindFocusedElementInfo(int32_t accessibilityWindowId,
    int32_t elementId, int32_t focusType, AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG("[channelId:%{public}d]", channelId_);
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "FindFocusedElement");
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }

    int32_t requestId = GenerateRequestId();
    sptr<AccessibilityElementOperatorCallbackImpl> elementOperator =
        new(std::nothrow) AccessibilityElementOperatorCallbackImpl();
    if (!elementOperator) {
        HILOG_ERROR("Failed to create elementOperator.");
        return false;
    }
    std::future<void> promiseFuture = elementOperator->promise_.get_future();

    if (!proxy_->FindFocusedElementInfo(accessibilityWindowId, elementId, focusType, requestId, elementOperator)) {
        return false;
    }
    HILOG_DEBUG("channelId:%{public}d, accessibilityWindowId:%{public}d, elementId:%{public}d, focusType:%{public}d",
        channelId_, accessibilityWindowId, elementId, focusType);

    std::future_status wait = promiseFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait result");
        return false;
    }

    if (elementOperator->accessibilityInfoResult_.GetAccessibilityId() ==
        AccessibilityElementInfo::UNDEFINED_ACCESSIBILITY_ID) {
        HILOG_ERROR("The elementInfo from ace is wrong");
        return false;
    }
    HILOG_INFO("Get result successfully from ace.");

    elementInfo = elementOperator->accessibilityInfoResult_;
    return true;
}

bool AccessibleAbilityChannelClient::SendSimulateGesture(
    const std::shared_ptr<AccessibilityGestureInjectPath> &gesturePath)
{
    HILOG_INFO("[channelId:%{public}d]", channelId_);
    if (proxy_) {
        return proxy_->SendSimulateGesture(gesturePath);
    } else {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }
}

bool AccessibleAbilityChannelClient::ExecuteAction(int32_t accessibilityWindowId,
    int32_t elementId, int32_t action, const std::map<std::string, std::string> &actionArguments)
{
    HILOG_DEBUG("[channelId:%{public}d]", channelId_);
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "ExecuteAction");
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }

    int32_t requestId = GenerateRequestId();
    sptr<AccessibilityElementOperatorCallbackImpl> elementOperator =
        new(std::nothrow) AccessibilityElementOperatorCallbackImpl();
    if (!elementOperator) {
        HILOG_ERROR("Failed to create elementOperator.");
        return false;
    }
    std::future<void> promiseFuture = elementOperator->promise_.get_future();

    if (!proxy_->ExecuteAction(accessibilityWindowId, elementId, action, actionArguments, requestId, elementOperator)) {
        return false;
    }

    std::future_status wait = promiseFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait result");
        return false;
    }
    HILOG_INFO("Get result successfully from ace. executeActionResult_[%{public}d]",
        elementOperator->executeActionResult_);
    return elementOperator->executeActionResult_;
}

bool AccessibleAbilityChannelClient::SearchElementInfosByAccessibilityId(int32_t accessibilityWindowId,
    int32_t elementId, int32_t mode, std::vector<AccessibilityElementInfo> &elementInfos)
{
    HILOG_DEBUG("[channelId:%{public}d] [windowId:%{public}d]", channelId_, accessibilityWindowId);
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "SearchElementById");
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }

    int32_t requestId = GenerateRequestId();
    sptr<AccessibilityElementOperatorCallbackImpl> elementOperator =
        new(std::nothrow) AccessibilityElementOperatorCallbackImpl();
    if (!elementOperator) {
        HILOG_ERROR("Failed to create elementOperator.");
        return false;
    }
    std::future<void> promiseFuture = elementOperator->promise_.get_future();

    if (!proxy_->SearchElementInfoByAccessibilityId(accessibilityWindowId, elementId, requestId,
        elementOperator, mode)) {
        return false;
    }

    std::future_status wait = promiseFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait result");
        return false;
    }

    for (auto &info : elementOperator->elementInfosResult_) {
        if (info.GetAccessibilityId() == AccessibilityElementInfo::UNDEFINED_ACCESSIBILITY_ID) {
            HILOG_ERROR("The elementInfo from ace is wrong");
            return false;
        }
    }
    HILOG_INFO("Get result successfully from ace. size[%{public}zu]", elementOperator->elementInfosResult_.size());
    elementInfos = elementOperator->elementInfosResult_;
    return true;
}

bool AccessibleAbilityChannelClient::GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo)
{
    HILOG_DEBUG("[channelId:%{public}d] [windowId:%{public}d]", channelId_, windowId);
    HITRACE_METER(HITRACE_TAG_ACCESSIBILITY_MANAGER);
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }
    return proxy_->GetWindow(windowId, windowInfo);
}

bool AccessibleAbilityChannelClient::GetWindows(std::vector<AccessibilityWindowInfo> &windows)
{
    HILOG_DEBUG("[channelId:%{public}d]", channelId_);
    HITRACE_METER(HITRACE_TAG_ACCESSIBILITY_MANAGER);
    if (proxy_) {
        return proxy_->GetWindows(windows);
    } else {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }
}

bool AccessibleAbilityChannelClient::GetWindows(const uint64_t displayId,
    std::vector<AccessibilityWindowInfo> &windows) const
{
    HILOG_DEBUG("[channelId:%{public}d] [displayId:%{public}" PRIu64 "]", channelId_, displayId);
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "GetWindowsByDisplayId");
    if (proxy_) {
        return proxy_->GetWindowsByDisplayId(displayId, windows);
    } else {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }
}

bool AccessibleAbilityChannelClient::SearchElementInfosByText(int32_t accessibilityWindowId,
    int32_t elementId, const std::string &text, std::vector<AccessibilityElementInfo> &elementInfos)
{
    HILOG_DEBUG("[channelId:%{public}d]", channelId_);
    HITRACE_METER_NAME(HITRACE_TAG_ACCESSIBILITY_MANAGER, "SearchElementByText");
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }

    int32_t requestId = GenerateRequestId();
    sptr<AccessibilityElementOperatorCallbackImpl> elementOperator =
        new(std::nothrow) AccessibilityElementOperatorCallbackImpl();
    if (!elementOperator) {
        HILOG_ERROR("Failed to create elementOperator.");
        return false;
    }
    std::future<void> promiseFuture = elementOperator->promise_.get_future();

    if (!proxy_->SearchElementInfosByText(accessibilityWindowId, elementId, text, requestId, elementOperator)) {
        return false;
    }

    std::future_status wait = promiseFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait result");
        return false;
    }

    for (auto &info : elementOperator->elementInfosResult_) {
        if (info.GetAccessibilityId() == AccessibilityElementInfo::UNDEFINED_ACCESSIBILITY_ID) {
            HILOG_ERROR("The elementInfo from ace is wrong");
            return false;
        }
    }
    HILOG_INFO("Get result successfully from ace. size[%{public}zu]", elementOperator->elementInfosResult_.size());
    elementInfos = elementOperator->elementInfosResult_;
    return true;
}

bool AccessibleAbilityChannelClient::FocusMoveSearch(int32_t accessibilityWindowId,
    int32_t elementId, int32_t direction, AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG("[channelId:%{public}d]", channelId_);
    if (!proxy_) {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }

    int32_t requestId = GenerateRequestId();
    sptr<AccessibilityElementOperatorCallbackImpl> elementOperator =
        new(std::nothrow) AccessibilityElementOperatorCallbackImpl();
    if (!elementOperator) {
        HILOG_ERROR("Failed to create elementOperator.");
        return false;
    }
    std::future<void> promiseFuture = elementOperator->promise_.get_future();

    if (!proxy_->FocusMoveSearch(accessibilityWindowId, elementId, direction, requestId, elementOperator)) {
        return false;
    }

    std::future_status wait = promiseFuture.wait_for(std::chrono::milliseconds(TIME_OUT_OPERATOR));
    if (wait != std::future_status::ready) {
        HILOG_ERROR("Failed to wait result");
        return false;
    }

    if (elementOperator->accessibilityInfoResult_.GetAccessibilityId() ==
        AccessibilityElementInfo::UNDEFINED_ACCESSIBILITY_ID) {
        HILOG_ERROR("The elementInfo from ace is wrong");
        return false;
    }

    HILOG_INFO("Get result successfully from ace");
    elementInfo = elementOperator->accessibilityInfoResult_;
    return true;
}

bool AccessibleAbilityChannelClient::SetTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    HILOG_INFO("[channelId:%{public}d]", channelId_);
    if (proxy_) {
        return proxy_->SetTargetBundleName(targetBundleNames);
    } else {
        HILOG_ERROR("Failed to connect to aams [channelId:%{public}d]", channelId_);
        return false;
    }
}
} // namespace Accessibility
} // namespace OHOS