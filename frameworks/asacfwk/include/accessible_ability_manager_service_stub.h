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

#ifndef ACCESSIBILITY_ABILITY_MANAGER_SERVICE_CLIENT_STUB_H
#define ACCESSIBILITY_ABILITY_MANAGER_SERVICE_CLIENT_STUB_H

#include <cstdint>
#include "accessibility_ability_info.h"
#include "accessible_ability_manager_service_state_interface.h"
#include "accessible_ability_manager_service_interface.h"
#include "accessibility_event_info.h"
#include "accessibility_errorcode.h"
#include "iremote_object.h"
#include "iremote_stub.h"
#include "nocopyable.h"

namespace OHOS {
namespace Accessibility {
/*
* The class define the interface to call ABMS API.
*/
class AccessibleAbilityManagerServiceClientStub : public IRemoteStub<IAccessibleAbilityManagerServiceClient> {
public:
    /**
     * @brief construct function
     * @param object The object of IPC
     * @return
     */
    AccessibleAbilityManagerServiceClientStub();

    /**
     * @brief destruct function
     * @param
     * @return
     */
    virtual ~AccessibleAbilityManagerServiceClientStub();

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
     * @brief Handle the IPC request for the function:HandleSendEvent.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleSendEvent(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleRegisterStateCallback.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleRegisterStateCallback(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetAbilityList.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetAbilityList(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleRegisterAccessibilityInteractionOperation.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleRegisterAccessibilityInteractionOperation(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleDeregisterAccessibilityInteractionOperation.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleDeregisterAccessibilityInteractionOperation(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleInterrupt.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleInterrupt(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleGetSuggestedInterval.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleGetSuggestedInterval(MessageParcel &data, MessageParcel &reply);

    /**
     * @brief Handle the IPC request for the function:HandleRegisterAbilityConnectionClientTmp.
     * @param data The data of prcess communication
     * @param reply The response of IPC request
     * @return NO_ERROR: successful; otherwise is failed.
     */
    ErrCode HandleRegisterAbilityConnectionClientTmp(MessageParcel &data, MessageParcel &reply);

    using AccessibilityAbilityManagerServiceFunc = ErrCode (AccessibleAbilityManagerServiceClientStub::*)
    (MessageParcel &data, MessageParcel &reply);
    std::map<uint32_t, AccessibilityAbilityManagerServiceFunc> memberFuncMap_;

    DISALLOW_COPY_AND_MOVE(AccessibleAbilityManagerServiceClientStub);
};

} //namespace Accessibility
} //namespace OHOS
#endif