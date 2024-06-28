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

#include "accessibility_window_connection.h"
#include <ipc_skeleton.h>

using namespace std;

namespace OHOS {
namespace Accessibility {
AccessibilityWindowConnection::AccessibilityWindowConnection(const int32_t windowId,
    const sptr<IAccessibilityElementOperator> &connection, const int32_t accountId)
{
    windowId_ = windowId;
    proxy_ = connection;
    accountId_ = accountId;
}

AccessibilityWindowConnection::AccessibilityWindowConnection(const int32_t windowId, const int32_t treeId,
    const sptr<IAccessibilityElementOperator> &connection, const int32_t accountId)
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    windowId_ = windowId;
    treeId_ = treeId;
    cardProxy_[treeId] = connection;
    tokenIdMap_[treeId] = tokenId;
    proxy_ = connection;
    accountId_ = accountId;
}

AccessibilityWindowConnection::~AccessibilityWindowConnection()
{
}

RetError AccessibilityWindowConnection::SetCardProxy(const int32_t treeId,
    sptr<IAccessibilityElementOperator> operation)
{
    if (!operation) {
        HILOG_DEBUG("SetCardProxy : operation is nullptr");
        return RET_ERR_FAILED;
    }
    cardProxy_[treeId] = operation;
    return RET_OK;
}

RetError AccessibilityWindowConnection::SetTokenIdMap(const int32_t treeId,
    const uint32_t tokenId)
{
    HILOG_DEBUG("tokenId : %{public}d, treeId : %{public}d", tokenId, treeId);
    tokenIdMap_[treeId] = tokenId;
    return RET_OK;
}

sptr<IAccessibilityElementOperator> AccessibilityWindowConnection::GetCardProxy(const int32_t treeId)
{
    auto iter = cardProxy_.find(treeId);
    if (iter != cardProxy_.end()) {
        HILOG_DEBUG("GetCardProxy : operation is ok");
        return cardProxy_[treeId];
    }
    HILOG_DEBUG("GetCardProxy : operation is no");
    return proxy_;
}

uint32_t AccessibilityWindowConnection::GetTokenIdMap(const int32_t treeId)
{
    HILOG_DEBUG("tokenId : %{public}d", tokenIdMap_[treeId]);
    return tokenIdMap_[treeId];
}
} // namespace Accessibility
} // namespace OHOS
