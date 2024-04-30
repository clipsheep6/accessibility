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

#ifndef ACCESSIBILITY_WINDOW_CONNECTION_H
#define ACCESSIBILITY_WINDOW_CONNECTION_H

#include "i_accessibility_element_operator.h"

namespace OHOS {
namespace Accessibility {
#define ROOT_TREE_ID -3
class AccessibilityWindowConnection : public RefBase {
public:
    AccessibilityWindowConnection(const int32_t windowId, const sptr<IAccessibilityElementOperator> &connection,
        const int32_t accountId);
    AccessibilityWindowConnection(const int32_t windowId, const int32_t treeId,
        const sptr<IAccessibilityElementOperator> &connection, const int32_t accountId);
    ~AccessibilityWindowConnection();

    inline sptr<IAccessibilityElementOperator> GetProxy()
    {
        return proxy_;
    }

    sptr<IAccessibilityElementOperator> GetCardProxy(const int32_t treeId)
    {
        if (treeId != ROOT_TREE_ID) {
            auto iter = cardProxy_.find(treeId);
            if (iter != cardProxy_.end()) {
                return nullptr;
            }
            return cardProxy_[treeId];
        }
        if (cardProxy_.empty()) {
            return nullptr;
        }
        return cardProxy_.begin()->second;
    }

    RetError SetCardProxy(const int32_t treeId, sptr<IAccessibilityElementOperator> operation);

private:
    int32_t windowId_;
    int32_t accountId_;
    int32_t treeId_;
    std::map<int32_t, sptr<IAccessibilityElementOperator>> cardProxy_;
    sptr<IAccessibilityElementOperator> proxy_;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBILITY_WINDOW_CONNECTION_H