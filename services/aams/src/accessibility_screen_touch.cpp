/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#include <map>
#include "accessibility_screen_touch.h"
#include "accessible_ability_manager_service.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {

constexpr int32_t POINTER_COUNT_1 = 1;
constexpr int32_t DISPLAY_WIDTH_RATIO = 150;

constexpr uint32_t CLICK_RESPONSE_DELAY_SHORT = 0;
constexpr uint32_t CLICK_RESPONSE_DELAY_MEDIUM = 1;
constexpr uint32_t CLICK_RESPONSE_DELAY_LONG = 2;

constexpr uint32_t CLICK_RESPONSE_TIME_SHORT = 0; // ms
constexpr uint32_t CLICK_RESPONSE_TIME_MEDIUM = 300; // ms
constexpr uint32_t CLICK_RESPONSE_TIME_LONG = 600; // ms

constexpr uint32_t IGNORE_REPEAT_CLICK_SHORTEST = 0;
constexpr uint32_t IGNORE_REPEAT_CLICK_SHORT = 1;
constexpr uint32_t IGNORE_REPEAT_CLICK_MEDIUM = 2;
constexpr uint32_t IGNORE_REPEAT_CLICK_LONG = 3;
constexpr uint32_t IGNORE_REPEAT_CLICK_LONGEST = 4;

constexpr uint32_t IGNORE_REPEAT_CLICK_TIME_SHORTEST = 100; // ms
constexpr uint32_t IGNORE_REPEAT_CLICK_TIME_SHORT = 400; // ms
constexpr uint32_t IGNORE_REPEAT_CLICK_TIME_MEDIUM = 700; // ms
constexpr uint32_t IGNORE_REPEAT_CLICK_TIME_LONG = 1000; // ms
constexpr uint32_t IGNORE_REPEAT_CLICK_TIME_LONGEST = 1300; // ms

const std::map<uint32_t, uint32_t> CLICK_RESPONSE_TIME_MAP = {
    {CLICK_RESPONSE_DELAY_SHORT, CLICK_RESPONSE_TIME_SHORT},
    {CLICK_RESPONSE_DELAY_MEDIUM, CLICK_RESPONSE_TIME_MEDIUM},
    {CLICK_RESPONSE_DELAY_LONG, CLICK_RESPONSE_TIME_LONG}
};

const std::map<uint32_t, uint32_t> IGNORE_REPEAT_CLICK_TIME_MAP = {
    {IGNORE_REPEAT_CLICK_SHORTEST, IGNORE_REPEAT_CLICK_TIME_SHORTEST},
    {IGNORE_REPEAT_CLICK_SHORT, IGNORE_REPEAT_CLICK_TIME_SHORT},
    {IGNORE_REPEAT_CLICK_MEDIUM, IGNORE_REPEAT_CLICK_TIME_MEDIUM},
    {IGNORE_REPEAT_CLICK_LONG, IGNORE_REPEAT_CLICK_TIME_LONG},
    {IGNORE_REPEAT_CLICK_LONGEST, IGNORE_REPEAT_CLICK_TIME_LONGEST}
};

AccessibilityScreenTouch::AccessibilityScreenTouch()
{
    HILOG_DEBUG();
    // get from account data directly
    sptr<AccessibilityAccountData> accountData =
        Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountData();
    clickResponseTime_ = accountData->GetConfig()->GetClickResponseTime();
    ignoreRepeatClickState_ = accountData->GetConfig()->GetIgnoreRepeatClickState();
    ignoreRepeatClickTime_ = accountData->GetConfig()->GetIgnoreRepeatClickTime();

    if (clickResponseTime_ > 0 && ignoreRepeatClickState_ == true) {
        currentState_ = BOTH_RESPONSE_DELAY_IGNORE_REPEAT_CLICK;
    } else if (clickResponseTime_ > 0) {
        currentState_ = CLICK_RESPONSE_DELAY_STATE;
    } else if (ignoreRepeatClickState_ == true) {
        currentState_ = IGNORE_REPEAT_CLICK_STATE;
    } else {
        currentState_ = DEFAULT_STATE;
    }

    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    auto display = displayMgr.GetDefaultDisplay();
    if (!display) {
        HILOG_ERROR("get display is nullptr");
        return;
    }

    threshold_ = CALCULATION_DIMENSION(display->GetWidth()) / DISPLAY_WIDTH_RATIO;
}

uint32_t AccessibilityScreenTouch::GetRealClickResponseTime()
{
    auto iter = CLICK_RESPONSE_TIME_MAP.find(clickResponseTime_);
    if (iter != CLICK_RESPONSE_TIME_MAP.end()) {
        return iter->second;
    }

    return CLICK_RESPONSE_TIME_MAP.at(CLICK_RESPONSE_DELAY_SHORT);
}

uint32_t AccessibilityScreenTouch::GetRealIgnoreRepeatClickTime()
{
    auto iter = IGNORE_REPEAT_CLICK_TIME_MAP.find(ignoreRepeatClickTime_);
    if (iter != IGNORE_REPEAT_CLICK_TIME_MAP.end()) {
        return iter->second;
    }

    return IGNORE_REPEAT_CLICK_TIME_MAP.at(IGNORE_REPEAT_CLICK_SHORTEST);
}

bool AccessibilityScreenTouch::GetRealIgnoreRepeatClickState()
{
    return ignoreRepeatClickState_;
}

void AccessibilityScreenTouch::HandleResponseDelayStateInnerDown(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    MMI::PointerEvent::PointerItem pointerItem;
    if (!event.GetPointerItem(event.GetPointerId(), pointerItem)) {
        HILOG_WARN("get GetPointerItem %{public}d failed", event.GetPointerId());
    }

    startTime_ = event.GetActionTime();
    startPointer_ = pointerItem;
    isMoveBeyondThreshold_ = false;
    isFirstDownEvent_ = true;
}

void AccessibilityScreenTouch::HandleResponseDelayStateInnerMove(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isMoveBeyondThreshold_ == true) {
        EventTransmission::OnPointerEvent(event);
        return;
    }

    MMI::PointerEvent::PointerItem pointerItem;
    if (!event.GetPointerItem(event.GetPointerId(), pointerItem)) {
        HILOG_WARN("get GetPointerItem %{public}d failed", event.GetPointerId());
    }

    float offsetX = startPointer_.GetDisplayX() - pointerItem.GetDisplayX();
    float offsetY = startPointer_.GetDisplayY() - pointerItem.GetDisplayY();
    double duration = hypot(offsetX, offsetY);
    if (duration > threshold_) {
        event.SetPointerAction(MMI::PointerEvent::POINTER_ACTION_DOWN);
        EventTransmission::OnPointerEvent(event);
        isMoveBeyondThreshold_ = true;
        return;
    }

    if ((startTime_ + GetRealClickResponseTime() * US_TO_MS) > event.GetActionTime()) {
        return;
    }

    if (isFirstDownEvent_ == true) {
        event.SetPointerAction(MMI::PointerEvent::POINTER_ACTION_DOWN);
        EventTransmission::OnPointerEvent(event);
        isFirstDownEvent_ = false;
        return;
    }

    EventTransmission::OnPointerEvent(event);
}

void AccessibilityScreenTouch::HandleResponseDelayStateInnerUp(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isMoveBeyondThreshold_ == true) {
        EventTransmission::OnPointerEvent(event);
        return;
    }

    if ((startTime_ + GetRealClickResponseTime() * US_TO_MS) > event.GetActionTime()) {
        return;
    }

    if (isFirstDownEvent_ == true) {
        return;
    }

    EventTransmission::OnPointerEvent(event);
}

void AccessibilityScreenTouch::HandleResponseDelayState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    switch (event.GetPointerAction()) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            HandleResponseDelayStateInnerDown(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            HandleResponseDelayStateInnerMove(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            HandleResponseDelayStateInnerUp(event);
            break;
        default:
            break;
    }
}

void AccessibilityScreenTouch::HandleIgnoreRepeatClickStateInnerDown(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    int64_t downTime = event.GetActionTime();
    if ((downTime - lastUpTime_) / US_TO_MS < GetRealIgnoreRepeatClickTime()) {
        isInterceptClick_ = true;
    }

    EventTransmission::OnPointerEvent(event);
    isInterceptClick_ = false;
}

void AccessibilityScreenTouch::HandleIgnoreRepeatClickStateInnerMove(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isInterceptClick_ == false) {
        EventTransmission::OnPointerEvent(event);
    }
}

void AccessibilityScreenTouch::HandleIgnoreRepeatClickStateInnerUp(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isInterceptClick_ == false) {
        EventTransmission::OnPointerEvent(event);
        lastUpTime_ = event.GetActionTime();
    }
}

void AccessibilityScreenTouch::HandleIgnoreRepeatClickState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    switch (event.GetPointerAction()) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            HandleIgnoreRepeatClickStateInnerDown(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            HandleIgnoreRepeatClickStateInnerMove(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            HandleIgnoreRepeatClickStateInnerUp(event);
            break;
        default:
            break;
    }
}

void AccessibilityScreenTouch::HandleBothStateInnerDown(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    int64_t downTime = event.GetActionTime();
    if ((downTime - lastUpTime_) / US_TO_MS < GetRealIgnoreRepeatClickTime()) {
        isInterceptClick_ = true;
    }

    HandleResponseDelayStateInnerDown(event);
    isInterceptClick_ = false;
}

void AccessibilityScreenTouch::HandleBothStateInnerMove(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isInterceptClick_ == true) {
        return;
    }

    HandleResponseDelayStateInnerMove(event);
}

void AccessibilityScreenTouch::HandleBothStateInnerUp(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetPointerIds().size() > POINTER_COUNT_1) {
        return;
    }

    if (isInterceptClick_ == true) {
        return;
    }

    lastUpTime_ = event.GetActionTime();
    HandleResponseDelayStateInnerUp(event);
}

void AccessibilityScreenTouch::HandleBothState(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    switch (event.GetPointerAction()) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            HandleBothStateInnerDown(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            HandleBothStateInnerMove(event);
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            HandleBothStateInnerUp(event);
            break;
        default:
            break;
    }
}

void AccessibilityScreenTouch::Clear()
{
    isMoveBeyondThreshold_ = false;
    isInterceptClick_ = false;
    startPointer_ = {};
}

bool AccessibilityScreenTouch::OnPointerEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (event.GetSourceType() != MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN) {
        EventTransmission::OnPointerEvent(event);
        return false;
    }

    if (event.GetPointerAction() == MMI::PointerEvent::POINTER_ACTION_CANCEL) {
        Clear();
        return true;
    }

    switch (currentState_) {
        case ScreenTouchState::CLICK_RESPONSE_DELAY_STATE:
            HandleResponseDelayState(event);
            break;
        case ScreenTouchState::IGNORE_REPEAT_CLICK_STATE:
            HandleIgnoreRepeatClickState(event);
            break;
        case ScreenTouchState::BOTH_RESPONSE_DELAY_IGNORE_REPEAT_CLICK:
            HandleBothState(event);
            break;
        case ScreenTouchState::DEFAULT_STATE:
        default:
            EventTransmission::OnPointerEvent(event);
    }

    return true;
}
} // namespace Accessibility
} // namespace OHOS