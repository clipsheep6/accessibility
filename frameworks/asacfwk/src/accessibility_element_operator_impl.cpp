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

#include "accessibility_element_operator_impl.h"
#include "accessibility_element_operator.h"
#include "accessibility_system_ability_client.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t REQUEST_WINDOW_ID_MAX = 0x00007FFF;
    constexpr uint32_t REQUEST_ID_MASK = 0x0000FFFF;
    constexpr int32_t REQUEST_ID_MASK_BIT = 16;
    constexpr int32_t ELEMENT_INFO_END_INDEX = -1;
    constexpr int32_t MAX_ELEMENT_INFO_NUMBER = 1000;
} // namespaces

AccessibilityElementOperatorImpl::AccessibilityElementOperatorImpl(int32_t windowId,
    const std::shared_ptr<AccessibilityElementOperator> &operation,
    AccessibilityElementOperatorCallback &callback)
    : windowId_(windowId), operatorCallback_(callback), operator_(operation)
{
    HILOG_DEBUG();
}

AccessibilityElementOperatorImpl::~AccessibilityElementOperatorImpl()
{
    HILOG_DEBUG();
}

void AccessibilityElementOperatorImpl::SearchElementInfoByAccessibilityId(const int32_t elementId,
    const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback, const int32_t mode)
{
    HILOG_DEBUG();
    int32_t mRequestId = AddRequest(requestId, callback);
    HILOG_DEBUG("search element add requestId[%{public}d]", mRequestId);
    if (operator_) {
        operator_->SearchElementInfoByAccessibilityId(elementId, mRequestId, operatorCallback_, mode);
    } else {
        HILOG_ERROR("Operator is nullptr");
    }
}

void AccessibilityElementOperatorImpl::SearchElementInfosByText(const int32_t elementId,
    const std::string &text, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();
    int32_t mRequestId = AddRequest(requestId, callback);
    HILOG_DEBUG("add requestId[%{public}d]", mRequestId);
    if (operator_) {
        operator_->SearchElementInfosByText(elementId, text, mRequestId, operatorCallback_);
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

void AccessibilityElementOperatorImpl::FindFocusedElementInfo(const int32_t elementId,
    const int32_t focusType, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();
    int32_t mRequestId = AddRequest(requestId, callback);
    HILOG_DEBUG("add requestId[%{public}d]", mRequestId);
    if (operator_) {
        operator_->FindFocusedElementInfo(elementId, focusType, mRequestId, operatorCallback_);
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

void AccessibilityElementOperatorImpl::FocusMoveSearch(const int32_t elementId,
    const int32_t direction, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();
    int32_t mRequestId = AddRequest(requestId, callback);
    HILOG_DEBUG("add requestId[%{public}d]", mRequestId);
    if (operator_) {
        operator_->FocusMoveSearch(elementId, direction, mRequestId, operatorCallback_);
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

void AccessibilityElementOperatorImpl::ExecuteAction(const int32_t elementId,
    const int32_t action, const std::map<std::string, std::string> &actionArguments,
    int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    HILOG_DEBUG();
    int32_t mRequestId = AddRequest(requestId, callback);
    HILOG_DEBUG("add requestId[%{public}d]", mRequestId);
    if (operator_) {
        operator_->ExecuteAction(elementId, action, actionArguments, mRequestId, operatorCallback_);
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

void AccessibilityElementOperatorImpl::ClearFocus()
{
    HILOG_DEBUG();
    if (operator_) {
        operator_->ClearFocus();
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

void AccessibilityElementOperatorImpl::OutsideTouch()
{
    HILOG_DEBUG();
    if (operator_ != nullptr) {
        operator_->OutsideTouch();
    } else {
        HILOG_DEBUG("Can not find interaction object");
    }
}

int32_t AccessibilityElementOperatorImpl::GetWindowId()
{
    return windowId_;
}

int32_t AccessibilityElementOperatorImpl::AddRequest(int32_t requestId,
    const sptr<IAccessibilityElementOperatorCallback> &callback)
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t compositionRequestId = static_cast<uint32_t>(requestId) & REQUEST_ID_MASK;

    if (windowId_ < REQUEST_WINDOW_ID_MAX && windowId_ > 0) {
        compositionRequestId |= static_cast<uint32_t>(windowId_) << REQUEST_ID_MASK_BIT;
    } else {
        HILOG_ERROR("window id[%{public}d] is wrong", windowId_);
        return -1;
    }

    requestId = static_cast<int32_t>(compositionRequestId);
    auto iter = requests_.find(requestId);
    if (iter == requests_.end()) {
        requests_[requestId] = callback;
    }
    return requestId;
}

void AccessibilityElementOperatorImpl::SetSearchElementInfoByAccessibilityIdResult(
    const std::list<AccessibilityElementInfo> &infos, const int32_t requestId)
{
    HILOG_DEBUG();
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<AccessibilityElementInfo> myInfos = TranslateListToVector(infos);
    auto iter = requests_.find(requestId);
    if (iter == requests_.end()) {
        HILOG_DEBUG("Can't find the callback [requestId:%d]", requestId);
        return;
    }

    if (iter->second == nullptr) {
        HILOG_DEBUG("Can't find the callback object [requestId:%d]", requestId);
        requests_.erase(iter);
        return;
    }

    if (myInfos.size() <= MAX_ELEMENT_INFO_NUMBER) {
        iter->second->SetSearchElementInfoByAccessibilityIdResult(myInfos, requestId, ELEMENT_INFO_END_INDEX);
        requests_.erase(iter);
        return;
    }

    int32_t index = 0;
    while (index < myInfos.size()) {
        std::vector<AccessibilityElementInfo> tmpInfos;
        int32_t count = 0;
        while (index < myInfos.size() && count < MAX_ELEMENT_INFO_NUMBER) {
            tmpInfos.push_back(myInfos[index++]);
            count++;
        }

        if (index >= myInfos.size()) {
            iter->second->SetSearchElementInfoByAccessibilityIdResult(tmpInfos, requestId, ELEMENT_INFO_END_INDEX);
        } else {
            iter->second->SetSearchElementInfoByAccessibilityIdResult(tmpInfos, requestId,
                (index - 1) / MAX_ELEMENT_INFO_NUMBER);
        }
    }

    requests_.erase(iter);
    return;
}

void AccessibilityElementOperatorImpl::SetSearchElementInfoByTextResult(
    const std::list<AccessibilityElementInfo> &infos, const int32_t requestId)
{
    HILOG_DEBUG();
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<AccessibilityElementInfo> myInfos = TranslateListToVector(infos);
    auto iter = requests_.find(requestId);
    if (iter != requests_.end()) {
        if (iter->second != nullptr) {
            iter->second->SetSearchElementInfoByTextResult(myInfos, requestId);
        }
        requests_.erase(iter);
    } else {
        HILOG_DEBUG("Can't find the callback [requestId:%d]", requestId);
    }
}

void AccessibilityElementOperatorImpl::SetFindFocusedElementInfoResult(
    const AccessibilityElementInfo &info, const int32_t requestId)
{
    HILOG_DEBUG();
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = requests_.find(requestId);
    if (iter != requests_.end()) {
        if (iter->second != nullptr) {
            iter->second->SetFindFocusedElementInfoResult(info, requestId);
        }
        requests_.erase(iter);
    } else {
        HILOG_DEBUG("Can't find the callback [requestId:%d]", requestId);
    }
}

void AccessibilityElementOperatorImpl::SetFocusMoveSearchResult(
    const AccessibilityElementInfo &info, const int32_t requestId)
{
    HILOG_DEBUG();
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = requests_.find(requestId);
    if (iter != requests_.end()) {
        if (iter->second != nullptr) {
            iter->second->SetFocusMoveSearchResult(info, requestId);
        }
        requests_.erase(iter);
    } else {
        HILOG_DEBUG("Can't find the callback [requestId:%d]", requestId);
    }
}

void AccessibilityElementOperatorImpl::SetExecuteActionResult(
    const bool succeeded, const int32_t requestId)
{
    HILOG_DEBUG();
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = requests_.find(requestId);
    if (iter != requests_.end()) {
        if (iter->second != nullptr) {
            iter->second->SetExecuteActionResult(succeeded, requestId);
        }
        requests_.erase(iter);
    } else {
        HILOG_DEBUG("Can't find the callback [requestId:%d]", requestId);
    }
}
} // namespace Accessibility
} // namespace OHOS