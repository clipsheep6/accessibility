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
#include <vector>
#include "accessibility_element_operator_callback_stub.h"
#include "hilog_wrapper.h"
#include "ipc_skeleton.h"
#include "ipc_types.h"
#include "iremote_object.h"
#include "accessibility_errorcode.h"

namespace OHOS {
namespace Accessibility {
AccessibilityElementOperatorCallbackStub::AccessibilityElementOperatorCallbackStub()
{}

AccessibilityElementOperatorCallbackStub::~AccessibilityElementOperatorCallbackStub()
{}

int AccessibilityElementOperatorCallbackStub::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    return 0;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByAccessibilityIdResult(
    MessageParcel &data,
    MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByTextResult(
    MessageParcel &data,
    MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetFindFocusedElementInfoResult(MessageParcel &data,
    MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetFocusMoveSearchResult(MessageParcel &data,
    MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetExecuteActionResult(MessageParcel &data,
    MessageParcel &reply)
{
    return NO_ERROR;
}
} // namespace Accessibility
} // namespace OHOS