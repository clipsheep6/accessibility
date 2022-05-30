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

#ifndef ACCESSIBLE_ABILITY_MANAGER_SERVICE_H
#define ACCESSIBLE_ABILITY_MANAGER_SERVICE_H

#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "accessibility_dumper.h"
#include "accessible_ability_manager_service_stub.h"
#include "accessible_ability_manager_service_event_handler.h"
#include "accessibility_account_data.h"
#include "accessibility_common_event.h"
#include "accessibility_display_manager.h"
#include "accessibility_input_interceptor.h"
#include "accessibility_keyevent_filter.h"
#include "accessibility_touchEvent_injector.h"
#include "accessibility_window_info.h"
#include "bundlemgr/bundle_mgr_interface.h"
#include "input_manager.h"
#include "singleton.h"
#include "system_ability.h"
#include "window_manager.h"

namespace OHOS {
namespace Accessibility {
class AccessibilityAccountData;
class TouchEventInjector;

class AccessibleAbilityManagerService : public SystemAbility, public AccessibleAbilityManagerServiceStub {
    DECLARE_SINGLETON(AccessibleAbilityManagerService)
    DECLEAR_SYSTEM_ABILITY(AccessibleAbilityManagerService)
public:
    /* For system ability */
    void OnStart() override;
    void OnStop() override;
    void OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
    void OnRemoveSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
    int Dump(int fd, const std::vector<std::u16string>& args) override;

public:
    /* For AccessibleAbilityManagerServiceStub */
    void SendEvent(const AccessibilityEventInfo &uiEvent) override;

    uint32_t RegisterStateObserver(const sptr<IAccessibleAbilityManagerStateObserver> &callback) override;

    uint32_t RegisterCaptionObserver(const sptr<IAccessibleAbilityManagerCaptionObserver> &callback) override;

    void RegisterEnableAbilityListsObserver(
        const sptr<IAccessibilityEnableAbilityListsObserver> &observer) override;

    bool GetAbilityList(const uint32_t abilityTypes, const int32_t stateType,
        std::vector<AccessibilityAbilityInfo> &infos) override;

    void RegisterElementOperator(const int32_t windowId,
        const sptr<IAccessibilityElementOperator> &operation) override;

    void DeregisterElementOperator(const int32_t windowId) override;

    AccessibilityConfig::CaptionProperty GetCaptionProperty() override;
    bool SetCaptionProperty(const AccessibilityConfig::CaptionProperty &caption) override;
    bool SetCaptionState(const bool state) override;

    bool GetEnabledState() override;
    bool GetCaptionState() override;
    bool GetTouchGuideState() override;
    bool GetGestureState() override;
    bool GetKeyEventObserverState() override;

    bool EnableAbility(const std::string &name, const uint32_t capabilities) override;
    bool GetEnabledAbilities(std::vector<std::string> &enabledAbilities) override;
    bool GetInstalledAbilities(std::vector<AccessibilityAbilityInfo> &installedAbilities) override;
    bool DisableAbility(const std::string &name) override;
    RetError EnableUITestAbility(const sptr<IRemoteObject>& obj) override;
    bool DisableUITestAbility() override;
    int32_t GetActiveWindow() override;

public:
    /* For inner modules */
    bool EnableShortKeyTargetAbility();
    bool DisableShortKeyTargetAbility();

    void SetTouchEventInjector(const sptr<TouchEventInjector> &touchEventInjector);

    inline sptr<TouchEventInjector> GetTouchEventInjector()
    {
        return touchEventInjector_;
    }

    inline sptr<KeyEventFilter> GetKeyEventFilter()
    {
        return keyEventFilter_;
    }

    void SetKeyEventFilter(const sptr<KeyEventFilter> &keyEventFilter);

    /* For DisplayResize */
    void NotifyDisplayResizeStateChanged(int32_t displayId, Rect& rect, float scale, float centerX, float centerY);

    inline int32_t GetCurrentAccountId()
    {
        return currentAccountId_;
    }

    inline uint32_t GetConnectCounter()
    {
        return connectCounter_;
    }

    inline std::shared_ptr<AAMSEventHandler> &GetMainHandler()
    {
        return handler_;
    }

    inline std::shared_ptr<AppExecFwk::EventRunner> &GetMainRunner()
    {
        return runner_;
    }

    sptr<AccessibilityAccountData> GetCurrentAccountData();
    sptr<AppExecFwk::IBundleMgr> GetBundleMgrProxy();

    /* For common event */
    void AddedUser(int32_t accountId);
    void RemovedUser(int32_t accountId);
    void SwitchedUser(int32_t accountId);
    void PackageChanged(const std::string &bundleName);
    void PackageRemoved(const std::string &bundleName);
    void PackageAdd(const std::string &bundleName);

    void UpdateAccessibilityManagerService();
    void UpdateAbilities();

    bool SetScreenMagnificationState(const bool state) override;
    bool SetShortKeyState(const bool state) override;
    bool SetMouseKeyState(const bool state) override;
    bool SetMouseAutoClick(const int32_t time) override;
    bool SetShortkeyTarget(const std::string &name) override;
    bool SetHighContrastTextState(const bool state) override;
    bool SetInvertColorState(const bool state) override;
    bool SetAnimationOffState(const bool state) override;
    bool SetAudioMonoState(const bool state) override;
    bool SetDaltonizationColorFilter(const uint32_t filter) override;
    bool SetContentTimeout(const uint32_t time) override;
    bool SetBrightnessDiscount(const float discount) override;
    bool SetAudioBalance(const float balance) override;

    bool GetScreenMagnificationState() override;
    bool GetShortKeyState() override;
    bool GetMouseKeyState() override;
    int32_t GetMouseAutoClick() override;
    std::string GetShortkeyTarget() override;
    bool GetHighContrastTextState() override;
    bool GetInvertColorState() override;
    bool GetAnimationOffState() override;
    bool GetAudioMonoState() override;
    uint32_t GetDaltonizationColorFilter() override;
    uint32_t GetContentTimeout() override;
    float GetBrightnessDiscount() override;
    float GetAudioBalance() override;

    uint32_t RegisterConfigObserver(const sptr<IAccessibleAbilityManagerConfigObserver> &callback) override;
    void UpdateConfigState();
    void UpdateAudioBalance();
    void UpdateBrightnessDiscount();
    void UpdateContentTimeout();
    void UpdateDaltonizationColorFilter();
    void UpdateMouseAutoClick();
    void UpdateShortkeyTarget();

private:
    int32_t GetOsAccountId();
    void AddUITestClient(const sptr<IRemoteObject> &obj);
    void RemoveUITestClient(sptr<AccessibleAbilityConnection> &connection);

    class StateCallbackDeathRecipient final : public IRemoteObject::DeathRecipient {
    public:
        StateCallbackDeathRecipient() = default;
        ~StateCallbackDeathRecipient() final = default;
        DISALLOW_COPY_AND_MOVE(StateCallbackDeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote) final;
    };

    class InteractionOperationDeathRecipient final : public IRemoteObject::DeathRecipient {
    public:
        InteractionOperationDeathRecipient(int32_t windowId) : windowId_(windowId) {};
        ~InteractionOperationDeathRecipient() final = default;
        DISALLOW_COPY_AND_MOVE(InteractionOperationDeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote) final;
        int32_t windowId_ = INVALID_WINDOW_ID;
    };

    class CaptionPropertyCallbackDeathRecipient final : public IRemoteObject::DeathRecipient {
    public:
        CaptionPropertyCallbackDeathRecipient() = default;
        ~CaptionPropertyCallbackDeathRecipient() final = default;
        DISALLOW_COPY_AND_MOVE(CaptionPropertyCallbackDeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote) final;
    };

    class EnableAbilityListsObserverDeathRecipient final : public IRemoteObject::DeathRecipient {
    public:
        EnableAbilityListsObserverDeathRecipient() = default;
        ~EnableAbilityListsObserverDeathRecipient() final = default;
        DISALLOW_COPY_AND_MOVE(EnableAbilityListsObserverDeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote) final;
    };

    bool Init();

    class ConfigCallbackDeathRecipient final : public IRemoteObject::DeathRecipient {
    public:
        ConfigCallbackDeathRecipient() = default;
        ~ConfigCallbackDeathRecipient() final = default;
        DISALLOW_COPY_AND_MOVE(ConfigCallbackDeathRecipient);

        void OnRemoteDied(const wptr<IRemoteObject> &remote) final;
    };
    
    void InitAccountDependence();
    void InitBundleDependence();
    void InitCommonEventDependence();
    void InitDisplayDependence();
    void InitWindowDependence();
    bool InnerEnableAbility(const std::string &name, const uint32_t capabilities);
    bool InnerDisableAbility(const std::string &name);

    sptr<AccessibilityWindowConnection> GetAccessibilityWindowConnection(int32_t windowId);
    void ClearFocus(int32_t windowId);
    void OutsideTouch(int32_t windowId);
    void UpdateAccessibilityWindowStateByEvent(const AccessibilityEventInfo &event);

    void UpdateAccessibilityState();
    void UpdateInputFilter();
    void UpdateCaptionProperty();

    bool isRunning_ = false;
    std::map<int32_t, bool> dependentServicesStatus_;
    int32_t currentAccountId_ = -1;
    uint32_t connectCounter_ = 1;
    std::map<int32_t, sptr<AccessibilityAccountData>> a11yAccountsData_;
    sptr<AppExecFwk::IBundleMgr> bundleManager_ = nullptr;

    sptr<AccessibilityInputInterceptor> inputInterceptor_ = nullptr;
    sptr<TouchEventInjector> touchEventInjector_ = nullptr;
    sptr<KeyEventFilter> keyEventFilter_ = nullptr;
    sptr<AccessibilityDumper> accessibilityDumper_ = nullptr;

    std::shared_ptr<AppExecFwk::EventRunner> runner_;
    std::shared_ptr<AAMSEventHandler> handler_;

    sptr<IRemoteObject::DeathRecipient> stateCallbackDeathRecipient_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> interactionOperationDeathRecipient_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> captionPropertyCallbackDeathRecipient_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> enableAbilityListsObserverDeathRecipient_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> configCallbackDeathRecipient_ = nullptr;
    static std::mutex mutex_;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBLE_ABILITY_MANAGER_SERVICE_H