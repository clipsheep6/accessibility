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

#include "accessibility_account_data.h"
#include "accessibility_display_manager.h"
#include "accessible_ability_manager_service.h"
#include "extension_ability_info.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
const static std::string PREF_TEST_PATH =
    "/data/service/el1/public/barrierfree/accessibility_ability_manager_service/";

AccessibilityAccountData::AccessibilityAccountData(int accountId)
{
    id_ = accountId;
}

AccessibilityAccountData::~AccessibilityAccountData()
{}

int AccessibilityAccountData::GetAccountId()
{
    HILOG_DEBUG("start.");
    return id_;
}

// get client state.
uint32_t AccessibilityAccountData::GetAccessibilityState()
{
    HILOG_DEBUG("start.");
    uint32_t state = 0;
    if (!connectedA11yAbilities_.empty() || !connectingA11yAbilities_.empty()) {
        state |= AccessibilitySystemAbilityClient::STATE_ACCESSIBILITY_ENABLED;
        if (!isEnabled_) {
            SetEnabled(true);
        }
    } else {
        if (isEnabled_) {
            SetEnabled(false);
        }
    }

    if (isEventTouchGuideState_) {
        state |= AccessibilitySystemAbilityClient::STATE_EXPLORATION_ENABLED;
    }

    if (isCaptionState_) {
        state |= AccessibilitySystemAbilityClient::STATE_CAPTION_ENABLED;
    }

    if (isFilteringKeyEvents_) {
        state |= AccessibilitySystemAbilityClient::STATE_KEYEVENT_ENABLED;
    }

    if (isGesturesSimulation_) {
        state |= AccessibilitySystemAbilityClient::STATE_GESTURE_ENABLED;
    }

    return state;
}

// switch the user causes state changed.
void AccessibilityAccountData::OnAccountSwitched()
{
    HILOG_DEBUG("start.");
    // reset AccessibleAbilityConnection
    for (auto itr = connectedA11yAbilities_.begin(); itr != connectedA11yAbilities_.end(); itr++) {
        itr->second->Disconnect();
    }

    // Clear all abilities.
    connectedA11yAbilities_.clear();
    connectingA11yAbilities_.clear();
    enabledAbilities_.clear();
    installedAbilities_.clear();

    // Clear Setting State.
    isEventTouchGuideState_ = false;
    isScreenMagnification_ = false;
    isFilteringKeyEvents_ = false;
    isGesturesSimulation_ = false;
}

// add connect ability.
void AccessibilityAccountData::AddConnectedAbility(sptr<AccessibleAbilityConnection>& connection)
{
    HILOG_DEBUG("URI is %{public}s", connection->GetElementName().GetURI().c_str());
    if (!connectedA11yAbilities_.count(connection->GetElementName().GetURI())) {
        connectedA11yAbilities_.insert(make_pair(connection->GetElementName().GetURI(), connection));
    }

    HILOG_DEBUG("Add ConnectedAbility: %{public}d", connectedA11yAbilities_.size());
}

// remove connect ability.
void AccessibilityAccountData::RemoveConnectedAbility(sptr<AccessibleAbilityConnection>& connection)
{
    HILOG_DEBUG("URI is %{public}s", connection->GetElementName().GetURI().c_str());
    std::map<std::string, sptr<AccessibleAbilityConnection>>::iterator it =
        connectedA11yAbilities_.find(connection->GetElementName().GetURI());
    if (it != connectedA11yAbilities_.end()) {
        connectedA11yAbilities_.erase(it);
    }

    HILOG_DEBUG("Remove ConnectedAbility: %{public}d", connectedA11yAbilities_.size());
}

void AccessibilityAccountData::AddStateCallback(const sptr<IAccessibleAbilityManagerServiceState>& callback)
{
    HILOG_DEBUG("start.");
    stateCallbacks_.push_back(callback);
}

// remove IAccessibleAbilityManagerServiceState
void AccessibilityAccountData::RemoveStateCallback(const wptr<IRemoteObject>& callback)
{
    HILOG_DEBUG("start.");
    for (auto itr = stateCallbacks_.begin(); itr != stateCallbacks_.end(); itr++) {
        if ((*itr)->AsObject() == callback) {
            stateCallbacks_.erase(itr);
            break;
        }
    }
}

void AccessibilityAccountData::AddCaptionPropertyCallback(
    const sptr<IAccessibleAbilityManagerServiceCaptionProperty>& callback)
{
    HILOG_DEBUG("start.");
    captionPropertyCallbacks_.push_back(callback);
}

void AccessibilityAccountData::RemoveCaptionPropertyCallback(const wptr<IRemoteObject>& callback)
{
    HILOG_DEBUG("start.");
    for (auto itr = captionPropertyCallbacks_.begin(); itr != captionPropertyCallbacks_.end(); itr++) {
        if ((*itr)->AsObject() == callback) {
            captionPropertyCallbacks_.erase(itr);
            break;
        }
    }
}
// add AccessibilityInteractionConnection
void AccessibilityAccountData::AddAccessibilityWindowConnection(
    const int windowId, const sptr<AccessibilityWindowConnection>& interactionConnection)
{
    HILOG_DEBUG("windowId(%{public}d)", windowId);
    if (!asacConnections_.count(windowId)) {
        asacConnections_.insert(std::make_pair(windowId, interactionConnection));
    }
}

// remove AccessibilityWindowConnection
void AccessibilityAccountData::RemoveAccessibilityWindowConnection(const int windowId)
{
    HILOG_DEBUG("windowId(%{public}d)", windowId);
    std::map<int, sptr<AccessibilityWindowConnection>>::iterator it = asacConnections_.find(windowId);
    if (it != asacConnections_.end()) {
        asacConnections_.erase(it);
    }
}

void AccessibilityAccountData::AddConnectingA11yAbility(const AppExecFwk::ElementName& elementName)
{
    HILOG_DEBUG("start.");
    if (!connectingA11yAbilities_.count(elementName.GetURI())) {
        connectingA11yAbilities_.insert(make_pair(elementName.GetURI(), elementName));
    }

    HILOG_DEBUG("Add ConnectingA11yAbility: %{public}d", connectingA11yAbilities_.size());
}

void AccessibilityAccountData::RemoveConnectingA11yAbility(const AppExecFwk::ElementName& elementName)
{
    HILOG_DEBUG("start.");
    std::map<std::string, AppExecFwk::ElementName>::iterator it = connectingA11yAbilities_.find(elementName.GetURI());
    if (it != connectingA11yAbilities_.end()) {
        connectingA11yAbilities_.erase(it);
    }

    HILOG_DEBUG("Remove ConnectingA11yAbility: %{public}d", connectingA11yAbilities_.size());
}

// For UT
void AccessibilityAccountData::AddEnabledAbility(const AppExecFwk::ElementName& elementName)
{
    HILOG_DEBUG("start.");
    if (!enabledAbilities_.count(elementName.GetURI())) {
        enabledAbilities_.insert(make_pair(elementName.GetURI(), elementName));
    }

    HILOG_DEBUG("Add EnabledAbility: %{public}d", enabledAbilities_.size());
}

void AccessibilityAccountData::RemoveEnabledAbility(const AppExecFwk::ElementName& elementName)
{
    HILOG_DEBUG("start.");
    std::map<std::string, AppExecFwk::ElementName>::iterator it = enabledAbilities_.find(elementName.GetURI());
    HILOG_DEBUG("Remove EnabledAbility: %{public}d", enabledAbilities_.size());
    if (it != enabledAbilities_.end()) {
        enabledAbilities_.erase(it);
    }

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);

    std::string strValue = pref->GetString("BundleName", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());

    std::vector<std::string> vecvalue;
    StringToVector(strValue, vecvalue);
    std::string BundleName = elementName.GetBundleName();
    for (std::vector<std::string>::iterator val = vecvalue.begin();val != vecvalue.end();) {
        if (std::strcmp(val->c_str(), BundleName.c_str()) == 0) {
            val = vecvalue.erase(val);
            HILOG_DEBUG("remove val = %{public}s", val->c_str());
        } else {
            ++val;
        }
    }
    std::string stringOut = "";
    VectorToString(vecvalue, stringOut);
    int err = pref->PutString("BundleName", stringOut);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);

    HILOG_DEBUG("Remove EnabledAbility: %{public}d", enabledAbilities_.size());

    DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->UpdateAbilities();
}

// For UT
void AccessibilityAccountData::AddInstalledAbility(AccessibilityAbilityInfo& abilityInfo)
{
    HILOG_DEBUG("abilityInfo's bundle name is %{public}s", abilityInfo.GetPackageName().c_str());
    for (size_t i = 0; i < installedAbilities_.size(); i++) {
        if (installedAbilities_[i].GetPackageName() == abilityInfo.GetPackageName()) {
            HILOG_DEBUG("the ability is already exist.");
            return;
        }
    }
    installedAbilities_.push_back(abilityInfo);
    HILOG_DEBUG("push back installed ability successfully and installedAbilities_'s size is %{public}d",
        installedAbilities_.size());
}

void AccessibilityAccountData::RemoveInstalledAbility(AccessibilityAbilityInfo& abilityInfo)
{
    HILOG_DEBUG("start.");
    for (auto it = installedAbilities_.begin(); it != installedAbilities_.end();) {
        if (it->GetPackageName() == abilityInfo.GetPackageName()) {
            installedAbilities_.erase(it);
        } else {
            ++it;
        }
    }

    DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->UpdateAbilities();
}

void AccessibilityAccountData::ClearInstalledAbility()
{
    HILOG_DEBUG("start.");
    installedAbilities_.clear();
}

// get AccessibleAbilityConnection
const sptr<AccessibleAbilityConnection> AccessibilityAccountData::GetAccessibleAbilityConnection(
    const std::string elementName)
{
    HILOG_DEBUG("URI is %{public}s", elementName.c_str());
    for (auto& connected : connectedA11yAbilities_) {
        std::string::size_type  idx = connected.first.find(elementName);
        if (idx == std::string::npos) {
            continue;
        } else {
            HILOG_DEBUG("founded URI = %{public}s ", connected.first.c_str());
            return connected.second;
        }
    }
    HILOG_DEBUG("URI %{public}s not found ", elementName.c_str());
    return nullptr;
}

// get AccessibilityWindowConnection.
const sptr<AccessibilityWindowConnection> AccessibilityAccountData::GetAccessibilityWindowConnection(
    const int windowId)
{
    HILOG_DEBUG("start.");
    if (asacConnections_.count(windowId) > 0) {
        return asacConnections_[windowId];
    }

    return nullptr;
}

// get map<std::string, sptr<AccessibleAbilityConnection>> connectedA11yAbilities_
const std::map<std::string, sptr<AccessibleAbilityConnection>> AccessibilityAccountData::GetConnectedA11yAbilities()
{
    HILOG_DEBUG("start.");
    return connectedA11yAbilities_;
}

const std::vector<sptr<IAccessibleAbilityManagerServiceState>> AccessibilityAccountData::GetStateCallbacks()
{
    HILOG_DEBUG("start.");
    return stateCallbacks_;
}

// get map<int, sptr<AccessibilityWindowConnection>> asacConnections_
const std::map<int, sptr<AccessibilityWindowConnection>> AccessibilityAccountData::GetAsacConnections()
{
    HILOG_DEBUG("start.");
    return asacConnections_;
}

const CaptionPropertyCallbacks AccessibilityAccountData::GetCaptionPropertyCallbacks()
{
    HILOG_DEBUG("start.");
    return captionPropertyCallbacks_;
}

// get connectingA11yAbilities_.
const std::map<std::string, AppExecFwk::ElementName> AccessibilityAccountData::GetConnectingA11yAbilities()
{
    HILOG_DEBUG("start.");
    return connectingA11yAbilities_;
}

// get enabledAbilities_.
const std::map<std::string, AppExecFwk::ElementName> AccessibilityAccountData::GetEnabledAbilities()
{
    HILOG_DEBUG("enabledAbilities_ size is (%{public}d).", enabledAbilities_.size());
    for (auto& abilitie : enabledAbilities_) {
        std::string bundleName = abilitie.second.GetBundleName();
        HILOG_DEBUG("bundleName = %{public}s ", bundleName.c_str());
    }
    return enabledAbilities_;
}

// get installedAbilities_.
const std::vector<AccessibilityAbilityInfo> AccessibilityAccountData::GetInstalledAbilities()
{
    HILOG_DEBUG("start.");
    return installedAbilities_;
}

const std::vector<AccessibilityAbilityInfo> AccessibilityAccountData::GetAbilitiesByState(AbilityStateType state)
{
    HILOG_DEBUG("state(%{public}d) start.", state);
    if (state == ABILITY_STATE_ENABLE) {
        std::vector<AccessibilityAbilityInfo> enabledAbilities;
        for (auto ability : connectedA11yAbilities_) {
            enabledAbilities.push_back(ability.second->GetAbilityInfo());
        }
        HILOG_DEBUG("connectedA11yAbilities_ is (%{public}d) enabledAbilities is (%{public}d).",
            connectedA11yAbilities_.size(), enabledAbilities.size());
        return enabledAbilities;
    } else if (state == ABILITY_STATE_DISABLE) {
        std::vector<AccessibilityAbilityInfo> disabledAbilities = installedAbilities_;
        for (auto enabledAbility : connectedA11yAbilities_) {
            for (auto itr = disabledAbilities.begin(); itr != disabledAbilities.end();) {
                if (itr->GetId() == enabledAbility.second->GetAbilityInfo().GetId()) {
                    disabledAbilities.erase(itr);
                } else {
                    itr++;
                }
            }
        }
        return disabledAbilities;
    } else {
        return installedAbilities_;
    }
}

void AccessibilityAccountData::UpdateAccountCapabilities()
{
    HILOG_DEBUG("start.");
    UpdateFilteringKeyEventsCapability();
    UpdateEventTouchGuideCapability();
    UpdateGesturesSimulationCapability();
    UpdateMagnificationCapability();

    SetTouchGuideState(isEventTouchGuideState_);
    SetGestureState(isGesturesSimulation_);
    SetKeyEventObserverState(isFilteringKeyEvents_);
}

void AccessibilityAccountData::UpdateEventTouchGuideCapability()
{
    HILOG_DEBUG("start.");
    for (auto itr = connectedA11yAbilities_.begin(); itr != connectedA11yAbilities_.end(); itr++) {
        if (itr->second->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_TOUCH_GUIDE) {
            isEventTouchGuideState_ = true;
            return;
        }
    }
    isEventTouchGuideState_ = false; // temp deal
}

void AccessibilityAccountData::UpdateGesturesSimulationCapability()
{
    HILOG_DEBUG("start.");
    for (auto itr = connectedA11yAbilities_.begin(); itr != connectedA11yAbilities_.end(); itr++) {
        if (itr->second->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_GESTURE) {
            isGesturesSimulation_ = true;
            return;
        }
    }
    isGesturesSimulation_ = false;
}

void AccessibilityAccountData::UpdateFilteringKeyEventsCapability()
{
    HILOG_DEBUG("start.");
    for (auto itr = connectedA11yAbilities_.begin(); itr != connectedA11yAbilities_.end(); itr++) {
        if (itr->second->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_KEY_EVENT_OBSERVER) {
            isFilteringKeyEvents_ = true;
            return;
        }
    }
    isFilteringKeyEvents_ = false;
}

void AccessibilityAccountData::UpdateMagnificationCapability()
{
    HILOG_DEBUG("start.");
    for (auto itr = connectedA11yAbilities_.begin(); itr != connectedA11yAbilities_.end(); itr++) {
        if (itr->second->GetAbilityInfo().GetCapabilityValues() & Capability::CAPABILITY_ZOOM) {
            isScreenMagnification_ = true;
            return;
        }
    }
    isScreenMagnification_ = false;
}

bool AccessibilityAccountData::SetCaptionState(const bool state)
{
    HILOG_DEBUG("start.");
    isCaptionState_ = state;

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    if (errCode != 0) {
        return false;
    }

    std::string STATE_TRUE = "on";
    std::string STATE_FALSE = "off";
    std::string strValue = "";
    if (isCaptionState_) {
        strValue = STATE_TRUE;
    } else {
        strValue = STATE_FALSE;
    }

    int err = pref->PutString("CaptionState", strValue);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);

    return true;
}

bool AccessibilityAccountData::SetCaptionProperty(const CaptionProperty& caption)
{
    HILOG_DEBUG("start.");
    captionProperty_ = caption;

    std::string FONTFAMILY = captionProperty_.GetFontFamily();
    int FONTSCALE = captionProperty_.GetFontScale();
    std::string FONTCOLOR = captionProperty_.GetFontColor();
    std::string FONTEDGETYPE = captionProperty_.GetFontEdgeType();
    std::string BACKGROUNDCOLOR = captionProperty_.GetBackgroundColor();
    std::string WINDOWCOLOR = captionProperty_.GetWindowColor();

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);

    int err = pref->PutString("fontFamily", FONTFAMILY);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->PutString("fontColor", FONTCOLOR);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->PutString("fontEdgeType", FONTEDGETYPE);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->PutString("backgroundColor", BACKGROUNDCOLOR);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->PutString("windowColor", WINDOWCOLOR);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->PutInt("fontScale", FONTSCALE);
    HILOG_DEBUG("pref->PutInt() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);

    return true;
}

bool AccessibilityAccountData::SetEnabled(const bool state)
{
    HILOG_DEBUG("start.");
    isEnabled_ = state;

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    if (errCode != 0) {
        return false;
    }

    std::string STATE_TRUE = "on";
    std::string STATE_FALSE = "off";
    std::string strValue = "";
    HILOG_DEBUG("isEnabled_ = %{public}s", isEnabled_ ? "true" : "false");
    if (isEnabled_) {
        strValue = STATE_TRUE;
    } else {
        strValue = STATE_FALSE;
    }
    int err = pref->PutString("accessible", strValue);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);
    return true;
}

bool AccessibilityAccountData::SetTouchGuideState(const bool state)
{
    HILOG_DEBUG("start.");
    isEventTouchGuideState_ = state;

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    if (errCode != 0) {
        return false;
    }

    std::string STATE_TRUE = "on";
    std::string STATE_FALSE = "off";
    std::string strValue = "";
    if (isEventTouchGuideState_) {
        strValue = STATE_TRUE;
    } else {
        strValue = STATE_FALSE;
    }

    int err = pref->PutString("touchGuide", strValue);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);
    return true;
}

bool AccessibilityAccountData::SetGestureState(const bool state)
{
    HILOG_DEBUG("start.");
    isGesturesSimulation_ = state;

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    if (errCode != 0) {
        return false;
    }

    std::string STATE_TRUE = "on";
    std::string STATE_FALSE = "off";
    std::string strValue = "";
    if (isGesturesSimulation_) {
        strValue = STATE_TRUE;
    } else {
        strValue = STATE_FALSE;
    }

    int err = pref->PutString("gesture", strValue);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);
    return true;
}

bool AccessibilityAccountData::SetKeyEventObserverState(const bool state)
{
    HILOG_DEBUG("start.");
    isFilteringKeyEvents_ = state;

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    if (errCode != 0) {
        return false;
    }

    std::string STATE_TRUE = "on";
    std::string STATE_FALSE = "off";
    std::string strValue = "";
    if (isFilteringKeyEvents_) {
        strValue = STATE_TRUE;
    } else {
        strValue = STATE_FALSE;
    }

    int err = pref->PutString("keyEventObserver", strValue);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);
    return true;
}

bool AccessibilityAccountData::GetEnabledState()
{
    return isEnabled_;
}

bool AccessibilityAccountData::GetTouchGuideState()
{
    HILOG_DEBUG("start. isEventTouchGuideState_ = %{public}s", isEventTouchGuideState_ ? "true" : "false");
    return isEventTouchGuideState_;
}

bool AccessibilityAccountData::GetGestureState()
{
    return isGesturesSimulation_;
}

bool AccessibilityAccountData::GetKeyEventObserverState()
{
    return isFilteringKeyEvents_;
}

bool AccessibilityAccountData::GetCaptionState()
{
    return isCaptionState_;
}

bool AccessibilityAccountData::SetEnabledObj(std::map<std::string, AppExecFwk::ElementName> it)
{
    HILOG_DEBUG("start.");

    // add ability to the last of enabledAbilities_
    for (auto& ability : it) {
        enabledAbilities_.insert(std::pair<std::string, AppExecFwk::ElementName>(ability.first, ability.second));
    }

    std::string BundleName = "";
    std::vector<std::string> vecvalue;
    for (auto& abilitie : enabledAbilities_) {
        BundleName = abilitie.second.GetBundleName();
        vecvalue.push_back(BundleName);
        HILOG_DEBUG("bundleName = %{public}s ", BundleName.c_str());
    }

    std::string stringOut = "";
    VectorToString(vecvalue, stringOut);

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);
    int err = pref->PutString("BundleName", stringOut);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);

    return true;
}

bool AccessibilityAccountData::ReadConfigurationForAccountData()
{
    HILOG_DEBUG("start.");
    // temp deal: read the user confige data.
    return true;
}

// get installedAbilities_.
bool AccessibilityAccountData::GetInstalledAbilitiesFromBMS()
{
    HILOG_DEBUG("start.");

    std::vector<AppExecFwk::ExtensionAbilityInfo> extensionInfos;
    auto aams = DelayedSingleton<AccessibleAbilityManagerService>::GetInstance();
    sptr<AppExecFwk::IBundleMgr> bms = nullptr;
    bms = aams->GetBundleMgrProxy();
    if (bms == nullptr) {
        HILOG_ERROR("GetBundleMgrProxy failed.");
        return false;
    }
    bms->QueryExtensionAbilityInfos(AppExecFwk::ExtensionAbilityType::ACCESSIBILITY, id_, extensionInfos);
    HILOG_DEBUG("query extensionAbilityInfos' size is %{public}d.", extensionInfos.size());
    for (auto& info : extensionInfos) {
        AccessibilityAbilityInfo* accessibilityInfo = new AccessibilityAbilityInfo(info);
        AddInstalledAbility(*accessibilityInfo);
    }
    return true;
}

void AccessibilityAccountData::CaptionInit(const std::shared_ptr<NativePreferences::Preferences> &pref)
{
    HILOG_DEBUG("start.");

    std::string strValue = pref->GetString("CaptionState", "");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", strValue.c_str());
    if (std::strcmp(strValue.c_str(), "on") == 0) {
        isCaptionState_ = true;
    } else {
        isCaptionState_ = false;
    }

    std::string FONTFAMILY = pref->GetString("fontFamily", "default");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", FONTFAMILY.c_str());

    int FONTSCALE = pref->GetInt("fontScale", 0);
    HILOG_DEBUG(" pref->GetString() = %{public}d.", FONTSCALE);

    std::string FONTCOLOR = pref->GetString("fontColor", "");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", FONTCOLOR.c_str());

    std::string FONTEDGETYPE = pref->GetString("fontEdgeType", "none");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", FONTEDGETYPE.c_str());

    std::string BACKGROUNDCOLOR = pref->GetString("backgroundColor", "");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", BACKGROUNDCOLOR.c_str());

    std::string WINDOWCOLOR = pref->GetString("windowColor", "");
    HILOG_DEBUG(" pref->GetString() = %{public}s.", WINDOWCOLOR.c_str());

    captionProperty_.SetFontFamily(FONTFAMILY);
    captionProperty_.SetFontScale(FONTSCALE);
    captionProperty_.SetFontColor(FONTCOLOR);
    captionProperty_.SetFontEdgeType(FONTEDGETYPE);
    captionProperty_.SetBackgroundColor(BACKGROUNDCOLOR);
    captionProperty_.SetWindowColor(WINDOWCOLOR);
}

void AccessibilityAccountData::CapbilityInit(const std::shared_ptr<NativePreferences::Preferences> &pref)
{
    HILOG_DEBUG("start.");

    std::string strValue = pref->GetString("accessible", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());
    if (std::strcmp(strValue.c_str(), "on") == 0) {
        isEnabled_ = true;
    } else {
        isEnabled_ = false;
    }

    strValue = pref->GetString("touchGuide", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());
    if (std::strcmp(strValue.c_str(), "on") == 0) {
        HILOG_DEBUG("isEventTouchGuideState_ = true");
        isEventTouchGuideState_ = true;
    } else {
        HILOG_DEBUG("isEventTouchGuideState_ = false");
        isEventTouchGuideState_ = false;
    }

    strValue = pref->GetString("gesture", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());
    if (std::strcmp(strValue.c_str(), "on") == 0) {
        isFilteringKeyEvents_ = true;
    } else {
        isFilteringKeyEvents_ = false;
    }

    strValue = pref->GetString("keyEventObserver", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());
    if (std::strcmp(strValue.c_str(), "on") == 0) {
        isGesturesSimulation_ = true;
    } else {
        isGesturesSimulation_ = false;
    }
}

void AccessibilityAccountData::EnabledListInit(const std::shared_ptr<NativePreferences::Preferences> &pref)
{
    HILOG_DEBUG("start.");
    std::string BundleName = "";
    AccessibilityAbilityInfo abilityInfo;

    std::string strValue = pref->GetString("BundleName", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());

    std::vector<std::string> vecvalue;
    StringToVector(strValue, vecvalue);
    for (auto i = vecvalue.begin(); i != vecvalue.end(); i++) {
        BundleName = *i;
        HILOG_DEBUG("BundleName = %{public}s", BundleName.c_str());
        AppExecFwk::ElementName elementName("", BundleName, "");
        enabledAbilities_.insert(std::pair<std::string, AppExecFwk::ElementName>(elementName.GetURI(), elementName));
    }
}

void AccessibilityAccountData::StringToVector(std::string &stringIn, std::vector<std::string> &vectorResult)
{
    HILOG_DEBUG("start.");
    int strLength = stringIn.size();
    std::vector<int> position;
    int wrodCount = 0;

    if (strLength == 0) {
        return;
    }

    for (int i = 0; i < strLength; i++) {
        if (stringIn[i] == ',') {
            position.push_back(i);
        }
    }

    wrodCount = position.size();
    if ((wrodCount == 0) && (strLength > 0)) {
        vectorResult.push_back(stringIn);
    } else {
        int startWrod = 0;
        int length = 0;
        for (int i = 0; i <= wrodCount; i++) {
            if (i == 0) {
                startWrod = 0;
                length = position[i];
                vectorResult.push_back(stringIn.substr(startWrod, length)); // first string
            } else if (i < wrodCount) {
                startWrod = position[i - 1] + 1;
                length = position[i] - position[i - 1] - 1;
                vectorResult.push_back(stringIn.substr(startWrod, length)); // second string to last-1 string
            } else {
                startWrod = position[i - 1] + 1;
                length = strLength - position[i - 1] - 1;
                vectorResult.push_back(stringIn.substr(startWrod, length)); // last string
            }
        }
    }
    HILOG_DEBUG("strLength = %{public}d, wrodCount = %{public}d, stringIn : %{public}s",
        strLength, wrodCount, stringIn.c_str());
    for (auto& var : vectorResult) {
        HILOG_DEBUG("vectorResult = %{public}s ", var.c_str());
    }
    HILOG_DEBUG("end.");
}

void AccessibilityAccountData::init()
{
    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);

    CaptionInit(pref);
    CapbilityInit(pref);
    EnabledListInit(pref);
}

bool AccessibilityAccountData::DisableAbilities(std::map<std::string, AppExecFwk::ElementName> it)
{
    HILOG_DEBUG("start.");
    for (auto &disAbilities : it) {
        enabledAbilities_.erase(disAbilities.first);
    }

    int errCode = 0;
    std::shared_ptr<NativePreferences::Preferences> pref =
        NativePreferences::PreferencesHelper::GetPreferences(PREF_TEST_PATH + "test.xml", errCode);
    HILOG_DEBUG("errCode = %{public}d.", errCode);

    std::string strValue = pref->GetString("BundleName", "");
    HILOG_DEBUG("strValue = %{public}s", strValue.c_str());

    std::vector<std::string> vecvalue;
    StringToVector(strValue, vecvalue);
    std::string BundleName = "";
    for (auto& disableAbility : it) {
        BundleName = disableAbility.second.GetBundleName();
        HILOG_DEBUG("BundleName = %{public}s", BundleName.c_str());
        for (std::vector<std::string>::iterator val = vecvalue.begin(); val != vecvalue.end();) {
            if (std::strcmp(val->c_str(), BundleName.c_str()) == 0) {
                val = vecvalue.erase(val);
                HILOG_DEBUG("remove val = %{public}s", val->c_str());
            } else {
                ++val;
            }
        }
    }

    std::string stringOut = "";
    VectorToString(vecvalue, stringOut);
    int err = pref->PutString("BundleName", stringOut);
    HILOG_DEBUG("pref->PutString() = %{public}d.", err);

    err = pref->FlushSync();
    HILOG_DEBUG("pref->FlushSync() = %{public}d.", err);

    DelayedSingleton<AccessibleAbilityManagerService>::GetInstance()->UpdateAbilities();
    return true;
}

void AccessibilityAccountData::AddUITestConnectedAbility(sptr<AccessibleAbilityConnection>& connection)
{
    HILOG_DEBUG("start.");
    if (!uiTestConnectedA11yAbility_) {
        HILOG_DEBUG("Add uiTestConnectedA11yAbility success");
        uiTestConnectedA11yAbility_ = connection;
    }
}

void AccessibilityAccountData::RemoveUITestConnectedAbility(sptr<AccessibleAbilityConnection>& connection)
{
    HILOG_DEBUG("start.");
    uiTestConnectedA11yAbility_ = nullptr;
}

const sptr<AccessibleAbilityConnection> AccessibilityAccountData::GetUITestConnectedAbilityConnection()
{
    HILOG_DEBUG("start.");
    return uiTestConnectedA11yAbility_;
}

void AccessibilityAccountData::VectorToString(std::vector<std::string> &vectorVal, std::string &stringOut)
{
    HILOG_DEBUG("start.");
    int i = 0;
    for (auto& var : vectorVal) {
        if (i > 0) {
            stringOut = stringOut + ',';
        }
        stringOut = stringOut + var.c_str();
        i++;
    }
    HILOG_DEBUG("end stringOUT = %{public}s .", stringOut.c_str());
}
}  // namespace Accessibility
}  // namespace OHOS
