/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed On an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "accessibility_gesture_recognizer.h"
#include "hilog_wrapper.h"
#include <cinttypes>

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr int32_t LIMIT_SIZE_TWO = 2;
    constexpr int32_t LIMIT_SIZE_THREE = 3;
    constexpr int32_t POINTER_COUNT_1 = 1;
    constexpr float EPSINON = 0.0001f;
    constexpr float TOUCH_SLOP = 8.0f;
} // namespace

GestureHandler::GestureHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner,
    AccessibilityGestureRecognizer &server) : AppExecFwk::EventHandler(runner), server_(server)
{
}

void GestureHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    HILOG_DEBUG();
    if (!event) {
        HILOG_ERROR("event is null");
        return;
    }

    switch (event->GetInnerEventId()) {
        case AccessibilityGestureRecognizer::LONG_PRESS_MSG:
            RemoveEvent(AccessibilityGestureRecognizer::SINGLE_TAP_MSG);
            server_.SetIsLongpress(true);
            server_.MaybeRecognizeLongPress(*server_.GetCurDown());
            break;
        case AccessibilityGestureRecognizer::SINGLE_TAP_MSG:
            if (!server_.GetContinueDown()) {
                server_.SingleTapDetected();
            }
            break;
        default:
            break;
    }
}

float AccessibilityGestureRecognizer::GetDoubleTapMoveThreshold(float densityDpi)
{
    return densityDpi * (1.0f / 25.4f) * MM_PER_CM;
}

AccessibilityGestureRecognizer::AccessibilityGestureRecognizer()
{
    HILOG_DEBUG();
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    auto display = displayMgr.GetDefaultDisplay();
    if (!display) {
        HILOG_ERROR("get display is nullptr");
        return;
    }

    threshold_ = GetDoubleTapMoveThreshold(display->GetDpi());
    xMinPixels_ = MIN_PIXELS(display->GetWidth());
    yMinPixels_ = MIN_PIXELS(display->GetHeight());

    float densityPixels = display->GetVirtualPixelRatio();
    int32_t slop = static_cast<int32_t>(densityPixels * DOUBLE_TAP_SLOP + 0.5f);
    doubleTapScaledSlop_ = slop * slop;
#else
    HILOG_DEBUG("not support display manager");
    threshold_ = 1;
    xMinPixels_ = 1;
    yMinPixels_ = 1;
    int32_t slop = static_cast<int32_t>(1 * DOUBLE_TAP_SLOP + 0.5f);
    doubleTapScaledSlop_ = slop * slop;
#endif

    runner_ = Singleton<AccessibleAbilityManagerService>::GetInstance().GetMainRunner();
    if (!runner_) {
        HILOG_ERROR("get runner failed");
        return;
    }
    handler_ = std::make_shared<GestureHandler>(runner_, *this);
    if (!handler_) {
        HILOG_ERROR("create event handler failed");
        return;
    }
}

void AccessibilityGestureRecognizer::RegisterListener(AccessibilityGestureRecognizeListener& listener)
{
    HILOG_DEBUG();

    listener_ = &listener;
}

void AccessibilityGestureRecognizer::UnregisterListener()
{
    HILOG_DEBUG();

    listener_ = nullptr;
}

bool AccessibilityGestureRecognizer::OnPointerEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    switch (event.GetPointerAction()) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            if (isDoubleTap_ && isLongpress_) {
                HILOG_INFO("isDoubleTap and longpress, on down event");
                return false;
            }
            if (event.GetPointerIds().size() == POINTER_COUNT_1) {
                HandleTouchDownEvent(event);
            } else {
                Clear();
                isRecognizingGesture_ = false;
                isGestureStarted_ = false;
                pointerRoute_.clear();
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_MOVE:
            if (isDoubleTap_ && isLongpress_) {
                HILOG_DEBUG("isDoubleTap and isLongpress, send move event to Multimodel.");
                return false;
            }
            return HandleTouchMoveEvent(event);
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (event.GetPointerIds().size() == POINTER_COUNT_1) {
                return HandleTouchUpEvent(event);
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_CANCEL:
            Clear();
            break;
        default:
            break;
    }
    if (!isRecognizingGesture_) {
        return false;
    }
    return StandardGestureRecognizer(event);
}

void AccessibilityGestureRecognizer::Clear()
{
    HILOG_DEBUG();

    isFirstTapUp_ = false;
    isDoubleTap_ = false;
    isGestureStarted_ = false;
    isRecognizingGesture_ = false;
    pointerRoute_.clear();
    continueDown_ = false;
    StandardGestureCanceled();
}

void AccessibilityGestureRecognizer::HandleTouchDownEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    Pointer mp;
    MMI::PointerEvent::PointerItem pointerIterm;
    if (!event.GetPointerItem(event.GetPointerId(), pointerIterm)) {
        HILOG_WARN("get GetPointerItem(%{public}d) failed", event.GetPointerId());
    }
    mp.px_ = static_cast<float>(pointerIterm.GetDisplayX());
    mp.py_ = static_cast<float>(pointerIterm.GetDisplayY());
    isDoubleTap_ = false;
    isRecognizingGesture_ = true;
    isGestureStarted_ = false;
    pointerRoute_.clear();
    pointerRoute_.push_back(mp);
    prePointer_ = pointerIterm;
    startPointer_ = pointerIterm;
    startTime_ = event.GetActionTime();
}

void AccessibilityGestureRecognizer::AddSwipePosition(MMI::PointerEvent::PointerItem &pointerIterm)
{
    HILOG_DEBUG();

    Pointer mp;
    prePointer_ = pointerIterm;
    mp.px_ = pointerIterm.GetDisplayX();
    mp.py_ = pointerIterm.GetDisplayY();
    pointerRoute_.push_back(mp);
}

bool AccessibilityGestureRecognizer::HandleTouchMoveEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    MMI::PointerEvent::PointerItem pointerIterm;
    if (!event.GetPointerItem(event.GetPointerId(), pointerIterm)) {
        HILOG_ERROR("get GetPointerItem(%{public}d) failed", event.GetPointerId());
        return false;
    }
    int64_t eventTime = event.GetActionTime();
    float offsetX = startPointer_.GetDisplayX() - pointerIterm.GetDisplayX();
    float offsetY = startPointer_.GetDisplayY() - pointerIterm.GetDisplayY();
    double duration = hypot(offsetX, offsetY);
    if (isRecognizingGesture_) {
        if (isDoubleTap_ && duration > TOUCH_SLOP) {
            HILOG_DEBUG("Cancel double tap event because the finger moves beyond preset slop.");
            isRecognizingGesture_ = false;
            isDoubleTap_ = false;
            return listener_->OnCancelled(event);
        } else if (duration > threshold_) {
            startPointer_ = pointerIterm;
            startTime_ = eventTime;
            isFirstTapUp_ = false;
            isDoubleTap_ = false;
            if (!isGestureStarted_) {
                isGestureStarted_ = true;
                listener_->OnStarted();
                return false;
            }
        } else if (!isFirstTapUp_) {
            int64_t durationTime = eventTime - startTime_;
            int64_t thresholdTime = isGestureStarted_ ?
                GESTURE_STARTED_TIME_THRESHOLD : GESTURE_NOT_STARTED_TIME_THRESHOLD;
            if (durationTime > thresholdTime) {
                isRecognizingGesture_ = false;
                isGestureStarted_ = false;
                pointerRoute_.clear();
                return listener_->OnCancelled(event);
            }
        }
        if ((abs(pointerIterm.GetDisplayX() - prePointer_.GetDisplayX())) >= xMinPixels_ ||
            (abs(pointerIterm.GetDisplayY() - prePointer_.GetDisplayY())) >= yMinPixels_) {
            AddSwipePosition(pointerIterm);
        }
    }
    if (!isRecognizingGesture_) {
        return false;
    }

    return StandardGestureRecognizer(event);
}

bool AccessibilityGestureRecognizer::HandleTouchUpEvent(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    Pointer mp;
    MMI::PointerEvent::PointerItem pointerIterm;
    if (!event.GetPointerItem(event.GetPointerId(), pointerIterm)) {
        HILOG_WARN("get GetPointerItem(%{public}d) failed", event.GetPointerId());
    }

    if (isDoubleTap_) {
        if (isLongpress_) {
            HILOG_DEBUG("up event, isDoubleTap and longpress.");
            return false;
        } else {
            HILOG_DEBUG();
            return DoubleTapRecognized(event);
        }
    }
    if (isGestureStarted_) {
        if ((abs(pointerIterm.GetDisplayX() - prePointer_.GetDisplayX())) >= xMinPixels_ ||
            (abs(pointerIterm.GetDisplayY() - prePointer_.GetDisplayY())) >= yMinPixels_) {
            HILOG_DEBUG("Add position to pointer route.");
            mp.px_ = pointerIterm.GetDisplayX();
            mp.py_ = pointerIterm.GetDisplayY();
            pointerRoute_.push_back(mp);
        }
        return recognizeDirectionGesture(event);
    }
    if (!isRecognizingGesture_) {
        return false;
    }
    return StandardGestureRecognizer(event);
}

bool AccessibilityGestureRecognizer::StandardGestureRecognizer(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    switch (event.GetPointerAction()) {
        case MMI::PointerEvent::POINTER_ACTION_DOWN:
            if (event.GetPointerIds().size() == POINTER_COUNT_1) {
                if (pCurDown_ && pPreUp_ && isDoubleTap(event)) {
                    HILOG_DEBUG("Double tap is recognized");
                    isDoubleTapdetecting_ = true;
                    isDoubleTap_ = true;
                } else {
                    handler_->SendEvent(SINGLE_TAP_MSG, 0, DOUBLE_TAP_TIMEOUT / US_TO_MS);
                }
                pCurDown_ = std::make_shared<MMI::PointerEvent>(event);
                isTapDown_ = true;
                continueDown_ = true;
                isLongpress_ = false;
                handler_->RemoveEvent(LONG_PRESS_MSG);
                handler_->SendEvent(LONG_PRESS_MSG, 0, LONG_PRESS_TIMEOUT / US_TO_MS);
            } else {
                StandardGestureCanceled();
            }
            break;
        case MMI::PointerEvent::POINTER_ACTION_UP:
            if (event.GetPointerIds().size() == POINTER_COUNT_1) {
                continueDown_ = false;
                if (isLongpress_) {
                    handler_->RemoveEvent(SINGLE_TAP_MSG);
                    isLongpress_ = false;
                } else if (!isDoubleTapdetecting_ && isTapDown_) {
                    isFirstTapUp_ = true;
                }
                pPreUp_ = std::make_unique<MMI::PointerEvent>(event);
                isDoubleTapdetecting_ = false;
                handler_->RemoveEvent(LONG_PRESS_MSG);
            }
            break;
        default:
            break;
    }
    return false;
}

void AccessibilityGestureRecognizer::StandardGestureCanceled()
{
    HILOG_DEBUG();

    handler_->RemoveEvent(LONG_PRESS_MSG);
    handler_->RemoveEvent(SINGLE_TAP_MSG);
    isLongpress_ = false;
    isDoubleTapdetecting_ = false;
    isTapDown_ = false;
    isDoubleTap_ = false;
}

void AccessibilityGestureRecognizer::SingleTapDetected()
{
    HILOG_DEBUG();

    Clear();
}

void AccessibilityGestureRecognizer::MaybeRecognizeLongPress(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
}

bool AccessibilityGestureRecognizer::DoubleTapRecognized(MMI::PointerEvent &event)
{
    HILOG_DEBUG();

    Clear();
    return listener_->OnDoubleTap(event);
}

bool AccessibilityGestureRecognizer::recognizeDirectionGesture(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    if (!listener_) {
        HILOG_ERROR("listener_ is nullptr.");
        return false;
    }

    if (pointerRoute_.size() < LIMIT_SIZE_TWO) {
        return listener_->OnCancelled(event);
    }

    // Check the angle of the most recent motion vector versus the preceding motion vector,
    // segment the line if the angle is about 90 degrees.
    std::vector<Pointer> pointerPath = GetPointerPath(pointerRoute_);

    if (pointerPath.size() == LIMIT_SIZE_TWO) {
        int32_t swipeDirection = GetSwipeDirection(pointerPath[0], pointerPath[1]);
        return listener_->OnCompleted(GESTURE_DIRECTION[swipeDirection]);
    } else if (pointerPath.size() == LIMIT_SIZE_THREE) {
        int32_t swipeDirectionH = GetSwipeDirection(pointerPath[0], pointerPath[1]);
        int32_t swipeDirectionHV = GetSwipeDirection(pointerPath[1], pointerPath[2]);
        return listener_->OnCompleted(GESTURE_DIRECTION_TO_ID[swipeDirectionH][swipeDirectionHV]);
    }
    return listener_->OnCancelled(event);
}

int32_t AccessibilityGestureRecognizer::GetSwipeDirection(Pointer firstP, Pointer secondP)
{
    float offsetX = secondP.px_ - firstP.px_;
    float offsetY = secondP.py_ - firstP.py_;
    if (abs(offsetX) > abs(offsetY)) {
        return offsetX > EPSINON ? SWIPE_RIGHT : SWIPE_LEFT;
    } else {
        return offsetY < EPSINON ? SWIPE_UP : SWIPE_DOWN;
    }
}

std::vector<Pointer> AccessibilityGestureRecognizer::GetPointerPath(std::vector<Pointer> &route)
{
    HILOG_DEBUG();

    std::vector<Pointer> pointerPath;
    Pointer firstSeparation = route[0];
    Pointer nextPoint;
    Pointer newSeparation;
    float xUnitVector = 0;
    float yUnitVector = 0;
    float xVector = 0;
    float yVector = 0;
    float vectorLength = 0;
    int32_t numSinceFirstSep = 0;

    pointerPath.push_back(firstSeparation);
    for (size_t i = 1; i < route.size(); i++) {
        nextPoint = route[i];
        if (numSinceFirstSep > 0) {
            xVector = xUnitVector / numSinceFirstSep;
            yVector = yUnitVector / numSinceFirstSep;
            newSeparation.px_ = vectorLength * xVector + firstSeparation.px_;
            newSeparation.py_ = vectorLength * yVector + firstSeparation.py_;

            float xNextUnitVector = nextPoint.px_ - newSeparation.px_;
            float yNextUnitVector = nextPoint.py_ - newSeparation.py_;
            float nextVectorLength = hypot(xNextUnitVector, yNextUnitVector);
            if (nextVectorLength > EPSINON) {
                xNextUnitVector /= nextVectorLength;
                yNextUnitVector /= nextVectorLength;
            }

            if ((xVector * xNextUnitVector + yVector * yNextUnitVector) < DEGREES_THRESHOLD) {
                pointerPath.push_back(newSeparation);
                firstSeparation = newSeparation;
                xUnitVector = 0;
                yUnitVector = 0;
                numSinceFirstSep = 0;
            }
        }
        xVector = nextPoint.px_ - firstSeparation.px_;
        yVector = nextPoint.py_ - firstSeparation.py_;
        vectorLength = hypot(xVector, yVector);
        numSinceFirstSep += 1;
        if (vectorLength > EPSINON) {
            xUnitVector += xVector / vectorLength;
            yUnitVector += yVector / vectorLength;
        }
    }
    pointerPath.push_back(nextPoint);
    return pointerPath;
}

bool AccessibilityGestureRecognizer::isDoubleTap(MMI::PointerEvent &event)
{
    HILOG_DEBUG();
    int64_t durationTime = event.GetActionTime() - pPreUp_->GetActionTime();
    if (!(durationTime <= DOUBLE_TAP_TIMEOUT && durationTime >= MIN_DOUBLE_TAP_TIME)) {
        HILOG_WARN("durationTime[%{public}" PRId64 "] is wrong", durationTime);
        return false;
    }

    MMI::PointerEvent::PointerItem curPI;
    if (!event.GetPointerItem(event.GetPointerId(), curPI)) {
        HILOG_WARN("get GetPointerItem(%{public}d) failed", event.GetPointerId());
    }

    MMI::PointerEvent::PointerItem firstPI;
    pCurDown_->GetPointerItem(pCurDown_->GetPointerId(), firstPI);
    int32_t durationX = firstPI.GetDisplayX() - curPI.GetDisplayX();
    int32_t durationY = firstPI.GetDisplayY() - curPI.GetDisplayY();

    return (durationX * durationX + durationY * durationY < doubleTapScaledSlop_);
}
} // namespace Accessibility
} // namespace OHOS
