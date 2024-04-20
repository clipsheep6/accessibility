/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#ifndef ACCESSIBILITY_GRAPHIC_PROXY_H
#define ACCESSIBILITY_GRAPHIC_PROXY_H

#include "accessibility_graphics.h"

#include <memory>

namespace OHOS {
namespace Accessibility {

class A11yRSTransactionProxy {
public:
    static void FlushImplicitTransaction();
};

class A11yRSSurfaceNodeProxy {
public:
    A11yRSSurfaceNodeProxy(){};
    ~A11yRSSurfaceNodeProxy(){};

    static std::shared_ptr<A11yRSSurfaceNodeProxy> Create();
    void SetFrameGravity(int32_t gravity);
    void SetPositionZ(float position);
    void SetBounds(float positionX, float positionY, float width, float height);
    void SetBoundsEx(float positionX, float positionY);
    void SetBackgroundColor();
    void SetRotation(float degree);
    void SetDrawRegion(float positionX, float positionY, float width, float height);
    void AttachToDisplay(uint64_t screenId);
    void DetachToDisplay(uint64_t screenId);
    void ClearChildren();
    void SetVisible(bool state);
public:
    A11yRSSurfaceNode *node;
};

class A11yRSCanvasNodeProxy {
public:
    A11yRSCanvasNodeProxy(){};
    ~A11yRSCanvasNodeProxy(){};

    static std::shared_ptr<A11yRSCanvasNodeProxy> Create();
    void SetBounds(float positionX, float positionY, float width, float height);
    void SetBoundsEx(float positionX, float positionY);
    void SetFrame(float positionX, float positionY, float width, float height);
    void SetBackgroundColor();
    void SetCornerRadius(float cornerRadius);
    void SetPositionZ(float position);
    void SetRotation(float degree);
#ifndef USE_ROSEN_DRAWING
    void DrawingProgressByOpenSource(int32_t physicalX, int32_t physicalY, int32_t angle,
        int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#else
    void DrawingProgressByRosenDrawing(int32_t physicalX, int32_t physicalY, int32_t angle,
        int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#endif

public:
    A11yRSCanvasNode *node;
};

class A11yRSNodeProxy {
public:
    A11yRSNodeProxy(){};
    ~A11yRSNodeProxy(){};

    static std::shared_ptr<A11yRSNodeProxy> Create();
    void SetBounds(float positionX, float positionY, float width, float height);
    void SetFrame(float positionX, float positionY, float width, float height);
    void SetBackgroundColor();
    void SetCornerRadius(float cornerRadius);
    void SetPositionZ(float position);
    void SetRotation(float degree);
    void AddChild(std::shared_ptr<A11yRSCanvasNodeProxy> canvasNode, int32_t index);
    uint32_t GetId();
public:
    A11yRSNode *node;
};

class A11yRSUIDirectorProxy {
public:
    A11yRSUIDirectorProxy(){};
    ~A11yRSUIDirectorProxy(){};

    static std::shared_ptr<A11yRSUIDirectorProxy> Create();
    void SetRoot(uint32_t root);
    void Init();
    void SetRSSurfaceNode(std::shared_ptr<A11yRSSurfaceNodeProxy> surfaceNode);


public:
    A11yRSUIDirector *node;
};

class A11yGraphicFuncAdapter {
public:
    A11yGraphicFuncAdapter();
    ~A11yGraphicFuncAdapter();
    void *GetFunc(const std::string funcName);
private:
    void *handler_;
};
} // namespace Accessibility
} // namesopace OHOS

#endif // ACCESSIBILITY_GRAPHIC_PROXY_H
