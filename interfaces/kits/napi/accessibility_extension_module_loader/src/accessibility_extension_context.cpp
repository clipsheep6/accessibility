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

#include "accessibility_extension_context.h"
#include "accessible_ability_client.h"
#include "hilog_wrapper.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "napi_common.h"

using namespace std;

namespace OHOS {
namespace Accessibility {
RetError AccessibilityExtensionContext::GetFocus(const int32_t focusType, AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetFocus(focusType, elementInfo);
}

RetError AccessibilityExtensionContext::GetFocusByElementInfo(const AccessibilityElementInfo &sourceInfo,
    const int32_t focusType, AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetFocusByElementInfo(sourceInfo, focusType, elementInfo);
}

RetError AccessibilityExtensionContext::InjectGesture(
    const std::shared_ptr<AccessibilityGestureInjectPath> &gesturePath)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->InjectGesture(gesturePath);
}

RetError AccessibilityExtensionContext::GetRoot(AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetRoot(elementInfo);
}

RetError AccessibilityExtensionContext::GetRootByWindow(const AccessibilityWindowInfo &windowInfo,
    AccessibilityElementInfo &elementInfo)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetRootByWindow(windowInfo, elementInfo);
}

RetError AccessibilityExtensionContext::GetWindows(std::vector<AccessibilityWindowInfo> &windows)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetWindows(windows);
}

RetError AccessibilityExtensionContext::GetWindows(const uint64_t displayId,
    std::vector<AccessibilityWindowInfo> &windows)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetWindows(displayId, windows);
}

RetError AccessibilityExtensionContext::GetNext(const AccessibilityElementInfo &elementInfo,
    const FocusMoveDirection direction, AccessibilityElementInfo &nextElementInfo)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetNext(elementInfo, direction, nextElementInfo);
}

RetError AccessibilityExtensionContext::GetChildElementInfo(const int32_t index,
    const AccessibilityElementInfo &parent, AccessibilityElementInfo &child)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetChildElementInfo(index, parent, child);
}

RetError AccessibilityExtensionContext::GetByContent(const AccessibilityElementInfo &elementInfo,
    const std::string &text, std::vector<AccessibilityElementInfo> &elementInfos)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetByContent(elementInfo, text, elementInfos);
}

RetError AccessibilityExtensionContext::GetParentElementInfo(const AccessibilityElementInfo &child,
    AccessibilityElementInfo &parent)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->GetParentElementInfo(child, parent);
}

RetError AccessibilityExtensionContext::ExecuteAction(const AccessibilityElementInfo &elementInfo,
    const ActionType action, const std::map<std::string, std::string> &actionArguments)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->ExecuteAction(elementInfo, action, actionArguments);
}

RetError AccessibilityExtensionContext::SetTargetBundleName(const std::vector<std::string> &targetBundleNames)
{
    HILOG_DEBUG();
    sptr<AccessibleAbilityClient> aaClient = AccessibleAbilityClient::GetInstance();
    if (!aaClient) {
        HILOG_ERROR("aaClient is nullptr");
        return RET_ERR_NULLPTR;
    }
    return aaClient->SetTargetBundleName(targetBundleNames);
}

void AccessibilityExtensionContext::StartUIExtension(std::shared_ptr<UIExtensionRequestContext> asyncContext)
{
    HILOG_DEBUG("begin StartUIExtensionAbility");
    if (asyncContext == nullptr) {
        HILOG_ERROR("asyncContext is null");
        return;
    }
    auto abilityContext = asyncContext->context;
    if (abilityContext == nullptr) {
        HILOG_ERROR("abilityContext is null");
        return;
    }
    auto uiContent = abilityContext->GetUIContent();
    if (uiContent == nullptr) {
        HILOG_ERROR("uiContent is null");
        return;
    }

    auto uiExtCallback = std::make_shared<UIExtensionCallback>(asyncContext);
    OHOS::Ace::ModalUIExtensionCallbacks extensionCallbacks = {
        std::bind(&UIExtensionCallback::OnRelease, uiExtCallback, std::placeholders::_1),
        std::bind(&UIExtensionCallback::OnResult, uiExtCallback, std::placeholders::_1, std::placeholders::_2),
        std::bind(&UIExtensionCallback::OnReceive, uiExtCallback, std::placeholders::_1),
        std::bind(&UIExtensionCallback::OnError, uiExtCallback, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3),
        std::bind(&UIExtensionCallback::OnRemoteReady, uiExtCallback, std::placeholders::_1),
        std::bind(&UIExtensionCallback::OnDestroy, uiExtCallback)
    };

    OHOS::Ace::ModalUIExtensionConfig uiExtConfig;
    uiExtConfig.isProhibitBack = false;
    HILOG_DEBUG("Ready to CreateModalUIExtension");
    int32_t sessionId = uiContent->CreateModalUIExtension(asyncContext->requestWant, extensionCallbacks, uiExtConfig);
    HILOG_INFO("end CreateModalUIExtension sessionId = %{public}d", sessionId);
    if (sessionId == 0) {
        HILOG_ERROR("CreateModalUIExtension failed, sessionId is %{public}d", sessionId);
    }
    uiExtCallback->SetSessionId(sessionId);
    return;
}

CommonAsyncContext::CommonAsyncContext(napi_env napiEnv)
{
    env = napiEnv;
}

CommonAsyncContext::~CommonAsyncContext()
{
    if (callbackRef) {
        HILOG_DEBUG("~CommonAsyncContext delete callbackRef");
        napi_delete_reference(env, callbackRef);
        callbackRef = nullptr;
    }
    if (work) {
        HILOG_DEBUG("~CommonAsyncContext delete work");
        napi_delete_async_work(env, work);
        work = nullptr;
    }
}

UIExtensionCallback::UIExtensionCallback(std::shared_ptr<UIExtensionRequestContext>& reqContext)
{
    this->reqContext_ = reqContext;
}

void UIExtensionCallback::SetSessionId(int32_t sessionId)
{
    this->sessionId_ = sessionId;
}

bool UIExtensionCallback::SetErrorCode(int32_t code)
{
    if (this->reqContext_ == nullptr) {
        HILOG_ERROR("OnError reqContext is nullptr");
        return false;
    }
    if (this->alreadyCallback_) {
        HILOG_DEBUG("alreadyCallback");
        return false;
    }
    this->alreadyCallback_ = true;
    this->reqContext_->errCode = code;
    return true;
}

void UIExtensionCallback::OnRelease(int32_t releaseCode)
{
    HILOG_DEBUG("UIExtensionComponent OnRelease(), releaseCode = %{public}d", releaseCode);
    if (SetErrorCode(releaseCode)) {
        SendMessageBack();
    }
}

void UIExtensionCallback::OnResult(int32_t resultCode, const OHOS::AAFwk::Want& result)
{
    HILOG_DEBUG("UIExtensionComponent OnResult(), resultCode = %{public}d", resultCode);
    this->resultCode_ = resultCode;
    this->resultWant_ = result;
    if (SetErrorCode(0)) {
        SendMessageBack();
    }
}

void UIExtensionCallback::OnReceive(const OHOS::AAFwk::WantParams& request)
{
    HILOG_DEBUG("UIExtensionComponent OnReceive()");
}

void UIExtensionCallback::OnError(int32_t errorCode, const std::string& name, const std::string& message)
{
    HILOG_ERROR(
        "UIExtensionComponent OnError(), errorCode = %{public}d, name = %{public}s, message = %{public}s",
        errorCode, name.c_str(), message.c_str());
    if (SetErrorCode(errorCode)) {
        SendMessageBack();
    }
}

void UIExtensionCallback::OnRemoteReady(const std::shared_ptr<OHOS::Ace::ModalUIExtensionProxy>& uiProxy)
{
    HILOG_DEBUG("UIExtensionComponent OnRemoteReady()");
}

void UIExtensionCallback::OnDestroy()
{
    HILOG_DEBUG("UIExtensionComponent OnDestroy()");
    if (SetErrorCode(0)) {
        SendMessageBack();
    }
}

void UIExtensionCallback::ProcessCallbackOrPromise(napi_env env,
    const CommonAsyncContext* asyncContext, napi_value data)
{
    napi_value args[ARGS_SIZE_TWO] = {nullptr};

    if (asyncContext->errCode == RET_OK) {
        NAPI_CALL_RETURN_VOID(env, napi_get_null(env, &args[PARAM0]));
        args[PARAM1] = data;
    } else {
        napi_value errObj = nullptr;
        napi_create_uint32(env, asyncContext->errCode, &errObj);

        args[PARAM0] = errObj;
        NAPI_CALL_RETURN_VOID(env, napi_get_null(env, &args[PARAM1]));
    }

    HILOG_DEBUG("Promise");
    if (asyncContext->errCode == RET_OK) {
        NAPI_CALL_RETURN_VOID(env, napi_resolve_deferred(env, asyncContext->deferred, args[PARAM1]));
    } else {
        HILOG_ERROR("Promise reject, errCode=%{public}d", asyncContext->errCode);
        NAPI_CALL_RETURN_VOID(env, napi_reject_deferred(env, asyncContext->deferred, args[PARAM0]));
    }
}

void UIExtensionCallback::SendMessageBack()
{
    HILOG_INFO("start SendMessageBack");
    if (this->reqContext_ == nullptr) {
        HILOG_ERROR("reqContext is nullptr");
        return;
    }

    auto abilityContext = this->reqContext_->context;
    if (abilityContext != nullptr) {
        auto uiContent = abilityContext->GetUIContent();
        if (uiContent != nullptr) {
            HILOG_DEBUG("CloseModalUIExtension");
            uiContent->CloseModalUIExtension(this->sessionId_);
        }
    }

    napi_value nativeObjJs = nullptr;
    NAPI_CALL_RETURN_VOID(this->reqContext_->env, napi_create_object(this->reqContext_->env, &nativeObjJs));
    napi_value resultCode = nullptr;
    NAPI_CALL_RETURN_VOID(this->reqContext_->env,
        napi_create_int32(this->reqContext_->env, this->resultCode_, &resultCode));
    NAPI_CALL_RETURN_VOID(this->reqContext_->env,
        napi_set_named_property(this->reqContext_->env, nativeObjJs, "resultCode", resultCode));

    HILOG_DEBUG("ProcessCallbackOrPromise");
    ProcessCallbackOrPromise(this->reqContext_->env, this->reqContext_.get(), nativeObjJs);
}
} // namespace Accessibility
} // namespace OHOS