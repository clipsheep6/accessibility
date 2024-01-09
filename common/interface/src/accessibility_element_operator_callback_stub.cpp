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

#include "accessibility_element_operator_callback_stub.h"
#include "accessibility_element_info_parcel.h"
#include "accessibility_ipc_interface_code.h"
#include "hilog_wrapper.h"
#include "parcel_util.h"

namespace OHOS {
namespace Accessibility {

constexpr int32_t SINGLE_TRANSMIT = -2;
constexpr int32_t MULTI_TRANSMIT_FINISH = -1;

AccessibilityElementOperatorCallbackStub::AccessibilityElementOperatorCallbackStub()
{
    memberFuncMap_[static_cast<uint32_t>(
        AccessibilityInterfaceCode::SET_RESULT_BY_ACCESSIBILITY_ID)] =
        &AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByAccessibilityIdResult;
    memberFuncMap_[static_cast<uint32_t>(AccessibilityInterfaceCode::SET_RESULT_BY_TEXT)] =
        &AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByTextResult;
    memberFuncMap_[static_cast<uint32_t>(
        AccessibilityInterfaceCode::SET_RESULT_FOCUSED_INFO)] =
        &AccessibilityElementOperatorCallbackStub::HandleSetFindFocusedElementInfoResult;
    memberFuncMap_[static_cast<uint32_t>(AccessibilityInterfaceCode::SET_RESULT_FOCUS_MOVE)] =
        &AccessibilityElementOperatorCallbackStub::HandleSetFocusMoveSearchResult;
    memberFuncMap_[static_cast<uint32_t>(
        AccessibilityInterfaceCode::SET_RESULT_PERFORM_ACTION)] =
        &AccessibilityElementOperatorCallbackStub::HandleSetExecuteActionResult;
}

std::vector<AccessibilityElementInfo> AccessibilityElementOperatorCallbackStub::storeData = {};

AccessibilityElementOperatorCallbackStub::~AccessibilityElementOperatorCallbackStub()
{
    HILOG_DEBUG();
    memberFuncMap_.clear();
}

int AccessibilityElementOperatorCallbackStub::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    HILOG_DEBUG("cmd = %{public}u, flags= %{public}d", code, option.GetFlags());
    std::u16string descriptor = AccessibilityElementOperatorCallbackStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        HILOG_ERROR("AccessibilityElementOperatorCallbackStub::OnRemoteRequest"
            "local descriptor is not equal to remote");
        return ERR_INVALID_STATE;
    }

    auto memFunc = memberFuncMap_.find(code);
    if (memFunc != memberFuncMap_.end()) {
        auto func = memFunc->second;
        if (func != nullptr) {
            return (this->*func)(data, reply);
        }
    }
    HILOG_WARN("AccessibilityElementOperatorCallbackStub::OnRemoteRequest, default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByAccessibilityIdResult(
    MessageParcel &data, MessageParcel &reply)
{
    HILOG_DEBUG();
    int32_t flag = data.ReadInt32();
    if (flag == SINGLE_TRANSMIT || flag == 0) {
        storeData.clear();
    }

    int32_t accessibilityInfosize = data.ReadInt32();
    ContainerSecurityVerify(data, accessibilityInfosize, storeData.max_size());
    for (int32_t i = 0; i < accessibilityInfosize; i++) {
        sptr<AccessibilityElementInfoParcel> accessibilityInfo =
            data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (!accessibilityInfo) {
            HILOG_ERROR("ReadStrongParcelable<accessibilityInfo> failed");
            storeData.clear();
            reply.WriteInt32(RET_ERR_FAILED);
            return TRANSACTION_ERR;
        }
        storeData.push_back(*accessibilityInfo);
    }

    int32_t requestId = data.ReadInt32();
    if (flag == SINGLE_TRANSMIT || flag == MULTI_TRANSMIT_FINISH) {
        reply.WriteInt32(0);
        HILOG_DEBUG("size %{public}zu requestId %{public}d", storeData.size(), requestId);
        SetSearchElementInfoByAccessibilityIdResult(storeData, requestId);
        // clear storeData will crash here
        return NO_ERROR;
    }

    reply.WriteInt32(0);
    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetSearchElementInfoByTextResult(
    MessageParcel &data, MessageParcel &reply)
{
    HILOG_DEBUG();
    std::vector<AccessibilityElementInfo> infos {};
    int32_t accessibilityInfosize = data.ReadInt32();
    ContainerSecurityVerify(data, accessibilityInfosize, infos.max_size());
    for (int32_t i = 0; i < accessibilityInfosize; i++) {
        sptr<AccessibilityElementInfoParcel> accessibilityInfo =
            data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
        if (!accessibilityInfo) {
            HILOG_ERROR("ReadStrongParcelable<accessibilityInfo> failed");
            return TRANSACTION_ERR;
        }
        infos.emplace_back(*accessibilityInfo);
    }
    int32_t requestId = data.ReadInt32();

    SetSearchElementInfoByTextResult(infos, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetFindFocusedElementInfoResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();
    sptr<AccessibilityElementInfoParcel> info = data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
    if (!info) {
        HILOG_ERROR("ReadStrongParcelable<AccessibilityElementInfo> failed");
        return TRANSACTION_ERR;
    }

    int32_t requestId = data.ReadInt32();

    SetFindFocusedElementInfoResult(*info, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetFocusMoveSearchResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();
    sptr<AccessibilityElementInfoParcel> info = data.ReadStrongParcelable<AccessibilityElementInfoParcel>();
    if (!info) {
        HILOG_ERROR("ReadStrongParcelable<AccessibilityElementInfo> failed");
        return TRANSACTION_ERR;
    }

    int32_t requestId = data.ReadInt32();

    SetFocusMoveSearchResult(*info, requestId);

    return NO_ERROR;
}

ErrCode AccessibilityElementOperatorCallbackStub::HandleSetExecuteActionResult(MessageParcel &data,
    MessageParcel &reply)
{
    HILOG_DEBUG();

    bool succeeded = data.ReadBool();
    int32_t requestId = data.ReadInt32();

    SetExecuteActionResult(succeeded, requestId);

    return NO_ERROR;
}
} // namespace Accessibility
} // namespace OHOS