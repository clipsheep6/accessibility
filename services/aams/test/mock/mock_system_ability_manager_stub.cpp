/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "system_ability_manager_stub.h"

#include "errors.h"
#include "ipc_skeleton.h"
#include "ipc_types.h"
#include "sam_log.h"
#include "string_ex.h"

namespace OHOS {
SystemAbilityManagerStub::SystemAbilityManagerStub()
{}

int32_t SystemAbilityManagerStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    return 0;
}

bool SystemAbilityManagerStub::EnforceInterceToken(MessageParcel &data)
{
    return false;
}

int32_t SystemAbilityManagerStub::GetSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::CheckSystemAbilityInner(MessageParcel& data, MessageParcel& reply)
{
    return ERR_NONE;
}

int32_t SystemAbilityManagerStub::AddSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::RemoveSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::ListSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::SubsSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::UnSubsSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::CheckRemtSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::AddOndemandSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::CheckSystemAbilityImmeInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::LoadSystemAbilityInner(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int32_t SystemAbilityManagerStub::UnmarshalingSaExtraProp(MessageParcel &data, SAExtraProp &extraProp)
{
    return 0;
}

int32_t SystemAbilityManagerStub::GetHapIdMultiuser(int32_t uid)
{
    return 0;
}

bool SystemAbilityManagerStub::CanRequest()
{
    return true;
}
}  // namespace OHOS
