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

#include "mock_power_mgr_client.h"

namespace OHOS {
namespace PowerMgr {
PowerMgrClient::PowerMgrClient()
{}
PowerMgrClient::~PowerMgrClient()
{}

ErrCode PowerMgrClient::Connect()
{
    return ERR_OK;
}

void PowerMgrClient::ResetProxy(const wptr<IRemoteObject>& remote)
{}

void PowerMgrClient::PowerMgrDeathRecipient::OnRemoteDied(const wptr<IRemoteObject>& remote)
{}

Error<void> PowerMgrClient::RebootDevice(const std::string& reason)
{
    return Error<void>();
}

Error<void> PowerMgrClient::ShutDownDevice(const std::string& reason)
{
    return Error<void>();
}

Error<void> PowerMgrClient::SuspendDevice(SuspendDeviceType reason, bool suspendImmed)
{
    return Error<void>();
}

Error<void> PowerMgrClient::WakeupDevice(WakeupDeviceType reason, const std::string& detail)
{
    return Error<void>();
}

Error<bool> PowerMgrClient::RefreshActivity(UserActivityType type)
{
    return Error<bool>();
}

Error<bool> PowerMgrClient::IsRunningLockTypeSupported(uint32_t type)
{
    return Error<bool>();
}

Error<bool> PowerMgrClient::ForceSuspendDevice()
{
    return Error<bool>();
}

Error<bool> PowerMgrClient::IsScreenOn()
{
    return Error<bool>();
}

Error<PowerState> PowerMgrClient::GetState()
{
    return Error<PowerState>();
}

Error<std::shared_ptr<RunningLock>> PowerMgrClient::CreateRunningLock(const std::string& name, RunningLockType type)
{
    return Error<std::shared_ptr<RunningLock>>();
}

Error<void> PowerMgrClient::RegisterPowerStateCallback(const sptr<IPowerStateCallback>& callback)
{
    return Error<void>();
}

Error<void> PowerMgrClient::UnRegisterPowerStateCallback(const sptr<IPowerStateCallback>& callback)
{
    return Error<void>();
}

Error<void> PowerMgrClient::RegisterShutdownCallback(
    const sptr<IShutdownCallback>& callback, IShutdownCallback::ShutdownPriority priority)
{
    return Error<void>();
}

Error<void> PowerMgrClient::UnRegisterShutdownCallback(const sptr<IShutdownCallback>& callback)
{
    return Error<void>();
}

Error<void> PowerMgrClient::RegisterPowerModeCallback(const sptr<IPowerModeCallback>& callback)
{
    return Error<void>();
}

Error<void> PowerMgrClient::UnRegisterPowerModeCallback(const sptr<IPowerModeCallback>& callback)
{
    return Error<void>();
}

Error<void> PowerMgrClient::SetDisplaySuspend(bool enable)
{
    return Error<void>();
}

Error<void> PowerMgrClient::SetDeviceMode(const PowerMode mode)
{
    return Error<void>();
}

Error<PowerMode> PowerMgrClient::GetDeviceMode()
{
    return Error<PowerMode>();
}

std::string PowerMgrClient::Dump(const std::vector<std::string>& args)
{
    return "ERR_OK";
}
} // namespace PowerMgr
} // namespace OHOS