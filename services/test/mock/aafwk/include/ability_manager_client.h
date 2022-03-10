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

#ifndef OHOS_AAFWK_ABILITY_MANAGER_CLIENT_H
#define OHOS_AAFWK_ABILITY_MANAGER_CLIENT_H

#include <mutex>

#include "ability_connect_callback_interface.h"
#include "iremote_object.h"
#include "want.h"

namespace OHOS {
namespace AAFwk {
/**
 * @class AbilityManagerClient
 * AbilityManagerClient is used to access ability manager services.
 */
class AbilityManagerClient {
public:
    AbilityManagerClient() {}
    virtual ~AbilityManagerClient() {}
    ErrCode ConnectAbility(const Want &want, const sptr<IAbilityConnection> &connect,
        const sptr<IRemoteObject> &callerToken, int32_t userId)
    {
        return ERR_OK;
    }

    ErrCode DisconnectAbility(const sptr<IAbilityConnection> &connect)
    {
        return ERR_OK;
    }
    static std::shared_ptr<AbilityManagerClient> GetInstance();
private:
    static std::shared_ptr<AbilityManagerClient> instance_;
    static std::mutex mutex_;
};
} // namespace AAFwk
} // namespace OHOS
#endif  // OHOS_AAFWK_ABILITY_MANAGER_H
