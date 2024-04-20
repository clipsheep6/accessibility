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

#include "accessibility_circle_drawing_manager.h"
#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
#include "accessibility_display_manager.h"
#endif
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {

constexpr int32_t DEFAULT_VALUE = -1;

const std::string PROGRESS_BACKGROUND_COLOR_TYPE = "#0A59F7"; // progress circle background
const std::string PROGRESS_COLOR_TYPE = "#FFFFFF"; // center circle
const std::string OUT_CIRCLE_BACKGROUND_COLOR_TYPE = "#1756C4"; // outer circle

constexpr int32_t DEFAULT_WIDTH = 500;
constexpr int32_t DEFAULT_HEIGHT = 500;
constexpr int32_t DEFAULT_HALF = 2;

constexpr int32_t DEFAULT_PIXEL_DENSITY = 160;

constexpr int32_t INVALID_NODE_ID = -1;

// dlopen define
constexpr int32_t RESIZE_ASPECT_FILL = 13;
constexpr int32_t POINTER_WINDOW_POSITION_Z = 9999;

std::shared_ptr<AccessibilityCircleDrawingManager> AccessibilityCircleDrawingManager::pointDrawMgr_ = nullptr;

std::shared_ptr<AccessibilityCircleDrawingManager> AccessibilityCircleDrawingManager::GetInstance()
{
    HILOG_DEBUG();
    if (pointDrawMgr_ == nullptr) {
        pointDrawMgr_ = std::make_shared<AccessibilityCircleDrawingManager>();
    }
    return pointDrawMgr_;
}

void AccessibilityCircleDrawingManager::DeleteInstance()
{
    HILOG_DEBUG();
    if (pointDrawMgr_ == nullptr) {
        HILOG_ERROR("surfaceNode_ is nullptr");
        return;
    }
    pointDrawMgr_ = nullptr;
}

AccessibilityCircleDrawingManager::AccessibilityCircleDrawingManager()
{
    HILOG_DEBUG();
    imageWidth_ = DEFAULT_WIDTH;
    imageHeight_ = DEFAULT_HEIGHT;
    half_ = DEFAULT_WIDTH / DEFAULT_HALF;

#ifdef OHOS_BUILD_ENABLE_DISPLAY_MANAGER
    AccessibilityDisplayManager &displayMgr = Singleton<AccessibilityDisplayManager>::GetInstance();
    screenId_ = displayMgr.GetDefaultDisplayId(); // default screenId 0
    auto dpi = displayMgr.GetDefaultDisplayDpi();
    dispalyDensity_ = static_cast<float>(dpi) / DEFAULT_PIXEL_DENSITY;
#else
    HILOG_DEBUG("not support display manager");
    screenId_ = 0;
    dispalyDensity_ = 1;
#endif
}

AccessibilityCircleDrawingManager::~AccessibilityCircleDrawingManager()
{
    HILOG_DEBUG();
    if (surfaceNode_ == nullptr) {
        HILOG_ERROR("surfaceNode_ is nullptr");
        return;
    }

    surfaceNode_->ClearChildren();
    surfaceNode_->DetachToDisplay(screenId_);
    surfaceNode_ = nullptr;
    canvasNode_ = nullptr;
    if (rsUiDirector_ != nullptr) {
        rsUiDirector_->SetRoot(INVALID_NODE_ID);
    }

    rootNode_ = nullptr;
    rsUiDirector_ = nullptr;
    A11yRSTransactionProxy::FlushImplicitTransaction();
}

void AccessibilityCircleDrawingManager::UpdatePointerVisible(bool state)
{
    HILOG_DEBUG("state %{public}s", state ? "true" : "false");
    if (surfaceNode_ == nullptr) {
        HILOG_ERROR("surfaceNode_ is nullptr");
        return;
    }

    surfaceNode_->SetVisible(state);
    A11yRSTransactionProxy::FlushImplicitTransaction();
}

void AccessibilityCircleDrawingManager::CreatePointerWindow(int32_t physicalX, int32_t physicalY, uint64_t screenId)
{
    rsUiDirector_ = A11yRSUIDirectorProxy::Create();
    if (rsUiDirector_ == nullptr) {
        return;
    }

    surfaceNode_ = A11yRSSurfaceNodeProxy::Create();
    if (surfaceNode_ == nullptr) {
        HILOG_ERROR("create surfaceNode_ fail");
        return;
    }

    rsUiDirector_->Init();
    surfaceNode_->SetFrameGravity(RESIZE_ASPECT_FILL);
    surfaceNode_->SetPositionZ(POINTER_WINDOW_POSITION_Z);
    surfaceNode_->SetBounds(physicalX - half_, physicalY - half_, imageWidth_, imageHeight_);
    surfaceNode_->SetBackgroundColor(); // USE_ROSEN_DRAWING SK_ColorTRANSPARENT
    screenId_ = screenId;
    surfaceNode_->AttachToDisplay(screenId);
    surfaceNode_->SetRotation(0);

    surfaceNode_->SetDrawRegion(physicalX - half_, physicalY - half_, imageWidth_, imageHeight_);

    rootNode_ = A11yRSNodeProxy::Create();
    if (rootNode_ == nullptr) {
        return;
    }

    rootNode_->SetBounds(0, 0, imageWidth_, imageHeight_);
    rootNode_->SetFrame(0, 0, imageWidth_, imageHeight_); 
    rootNode_->SetBackgroundColor(); // USE_ROSEN_DRAWING SK_ColorTRANSPARENT
    rootNode_->SetCornerRadius(1);
    rootNode_->SetPositionZ(POINTER_WINDOW_POSITION_Z);
    rootNode_->SetRotation(0);

    canvasNode_ = A11yRSCanvasNodeProxy::Create();
    if (canvasNode_ == nullptr) {
        HILOG_ERROR("create canvasNode_ fail");
        return;
    }

    rootNode_->AddChild(canvasNode_, DEFAULT_VALUE);
    canvasNode_->SetBounds(0, 0, imageWidth_, imageHeight_);
    canvasNode_->SetFrame(0, 0, imageWidth_, imageHeight_);
    canvasNode_->SetBackgroundColor(); // USE_ROSEN_DRAWING SK_ColorTRANSPARENT
    canvasNode_->SetCornerRadius(1);
    canvasNode_->SetPositionZ(POINTER_WINDOW_POSITION_Z);
    canvasNode_->SetRotation(0);

    rsUiDirector_->SetRSSurfaceNode(surfaceNode_);
    rsUiDirector_->SetRoot(rootNode_->GetId());
    A11yRSTransactionProxy::FlushImplicitTransaction();
}

void AccessibilityCircleDrawingManager::DrawingProgress(int32_t physicalX, int32_t physicalY, int32_t angle)
{
#ifndef USE_ROSEN_DRAWING
    canvasNode_->DrawingProgressByOpenSource(physicalX, physicalY, angle,
        imageWidth_, imageHeight_, half_, dispalyDensity_);
#else
    canvasNode_->DrawingProgressByRosenDrawing(physicalX, physicalY, angle,
        imageWidth_, imageHeight_, half_, dispalyDensity_);
#endif
}

void AccessibilityCircleDrawingManager::SetPointerLocation(int32_t physicalX, int32_t physicalY, uint64_t screenId)
{
    HILOG_DEBUG("Pointer window move, x:%{public}d, y:%{public}d", physicalX, physicalY);
    if (surfaceNode_ != nullptr) {
        surfaceNode_->SetBoundsEx(physicalX - half_, physicalY - half_);
        screenId_ = screenId;
        surfaceNode_->AttachToDisplay(screenId);
        A11yRSTransactionProxy::FlushImplicitTransaction();
    }
}

void AccessibilityCircleDrawingManager::DrawPointer(int32_t physicalX, int32_t physicalY, int32_t angle,
    uint64_t screenId)
{
    HILOG_DEBUG();
    if (surfaceNode_ != nullptr) {
        SetPointerLocation(physicalX, physicalY, screenId);
        DrawingProgress(physicalX, physicalY, angle);
        UpdatePointerVisible(true);
        HILOG_DEBUG("surfaceNode_ is existed");
        return;
    }

    CreatePointerWindow(physicalX, physicalY, screenId);
    if (surfaceNode_ == nullptr) {
        HILOG_ERROR("surfaceNode_ is nullptr");
        return;
    }

    DrawingProgress(physicalX, physicalY, angle);
    UpdatePointerVisible(true);
}

} // namespace Accessibility
} // namespace OHOS