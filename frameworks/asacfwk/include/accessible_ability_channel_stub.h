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

#ifndef ACCESSIBLE_ABILITY_CHANNEL_STUB_H
#define ACCESSIBLE_ABILITY_CHANNEL_STUB_H

#include <map>
#include <stdint.h>
#include "accessible_ability_channel_interface.h"
#include "accessibility_errorcode.h"
#include "hilog_wrapper.h"
#include "iremote_stub.h"
#include "nocopyable.h"
#include "message_parcel.h"
#include "message_option.h"

namespace OHOS {
namespace Accessibility {

class AccessibleAbilityChannelStub : public IRemoteStub<IAccessibleAbilityChannel> {
public:
    /**
     * @brief construct function
     * @param object The object of IPC
     * @return
     */
    AccessibleAbilityChannelStub();

    /**
     * @brief destruct function
     * @param
     * @return
     */
    virtual ~AccessibleAbilityChannelStub();

    /**
     * @brief Receive the event from proxy by IPC mechanism.
     * @param code The code is matched with the process function.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @param option The option paramter of IPC,such as: async,sync
     * @return
     */
    virtual int OnRemoteRequest(uint32_t code, MessageParcel &data,
                                    MessageParcel &reply, MessageOption &option) override;
private:
    /**
     * @brief Handle the IPC request for the function:HandleSearchElementInfoByAccessibilityId.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchElementInfoByAccessibilityId(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSearchElementInfosByText.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSearchElementInfosByText(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleFindFocusedElementInfo.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleFindFocusedElementInfo(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleFocusMoveSearch.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleFocusMoveSearch(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandlePerformAction.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandlePerformAction(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetWindows.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetWindows(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandlePerformCommonAction.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandlePerformCommonAction(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleDisableAbility.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleDisableAbility(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSetOnKeyPressEventResult.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSetOnKeyPressEventResult(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetDisplayResizeScale.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetDisplayResizeScale(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetDisplayResizeCenterX.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetDisplayResizeCenterX(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetDisplayResizeCenterY.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetDisplayResizeCenterY(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetDisplayResizeRect.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetDisplayResizeRect(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleResetDisplayResize.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleResetDisplayResize(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSetDisplayResizeScaleAndCenter.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSetDisplayResizeScaleAndCenter(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleSendSimulateGesture.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSendSimulateGesture(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleIsFingerprintGestureDetectionValid.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleIsFingerprintGestureDetectionValid(MessageParcel &data, MessageParcel &reply);

    using AccessibleAbilityConnectionFunc = ErrCode (AccessibleAbilityChannelStub::*)(MessageParcel &data,
                                                        MessageParcel &reply);
    std::map<uint32_t, AccessibleAbilityConnectionFunc> memberFuncMap_;

    DISALLOW_COPY_AND_MOVE(AccessibleAbilityChannelStub);
};

} // namespace Accessibility
} // namespace OHOS

#endif // ACCESSIBLE_ABILITY_CHANNEL_STUB_H