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

#include "accessibility_graphic_proxy.h"

#include <string>
#include <dlfcn.h>

namespace OHOS {
namespace Accessibility {

namespace {
    const std::string LIB_GRAPHIC_NAME = "libaccessibility_graphics.z.so";

    const std::string RSSurfaceNodeCreateFuncName = "RSSurfaceNodeCreate";
    const std::string RSSurfaceNodeSetFrameGravityFuncName = "RSSurfaceNodeSetFrameGravity";
    const std::string RSSurfaceNodeSetPositionZFuncName = "RSSurfaceNodeSetPositionZ";
    const std::string RSSurfaceNodeSetBoundsFuncName = "RSSurfaceNodeSetBounds";
    const std::string RSSurfaceNodeSetBoundsExFuncName = "RSSurfaceNodeSetBoundsEx";
    const std::string RSSurfaceNodeSetBackgroundColorFuncName = "RSSurfaceNodeSetBackgroundColor";
    const std::string RSSurfaceNodeSetRotationFuncName = "RSSurfaceNodeSetRotation";
    const std::string RSSurfaceNodeSetDrawRegionFuncName = "RSSurfaceNodeSetDrawRegion";
    const std::string RSSurfaceNodeAttachToDisplayFuncName = "RSSurfaceNodeAttachToDisplay";
    const std::string RSSurfaceNodeDetachToDisplayFuncName = "RSSurfaceNodeDetachToDisplay";
    const std::string RSSurfaceNodeClearChildrenFuncName = "RSSurfaceNodeClearChildren";
    const std::string RSSurfaceNodeSetVisibleFuncName = "RSSurfaceNodeSetVisible";

    const std::string RSCanvasNodeCreateFuncName = "RSCanvasNodeCreate";
    const std::string RSCanvasNodeSetBoundsFuncName = "RSCanvasNodeSetBounds";
    const std::string RSCanvasNodeSetFrameFuncName = "RSCanvasNodeSetFrame";
    const std::string RSCanvasNodeSetBackgroundColorFuncName = "RSCanvasNodeSetBackgroundColor";
    const std::string RSCanvasNodeSetCornerRadiusFuncName = "RSCanvasNodeSetCornerRadius";
    const std::string RSCanvasNodeSetPositionZFuncName = "RSCanvasNodeSetPositionZ";
    const std::string RSCanvasNodeSetRotationFuncName = "RSCanvasNodeSetRotation";
    const std::string RSCanvasNodeDrawingProgressByOpenSourceFuncName = "RSCanvasNodeDrawingProgressByOpenSource";
    const std::string RSCanvasNodeDrawingProgressByRosenDrawingFuncName = "RSCanvasNodeDrawingProgressByRosenDrawing";

    const std::string RSNodeCreateFuncName = "RSNodeCreate";
    const std::string RSNodeSetBoundsFuncName = "RSNodeSetBounds";
    const std::string RSNodeSetFrameFuncName = "RSNodeSetFrame";
    const std::string RSNodeSetBackgroundColorFuncName = "RSNodeSetBackgroundColor";
    const std::string RSNodeSetCornerRadiusFuncName = "RSNodeSetCornerRadius";
    const std::string RSNodeSetPositionZFuncName = "RSNodeSetPositionZ";
    const std::string RSNodeSetRotationFuncName = "RSNodeSetRotation";
    const std::string RSNodeAddChildFuncName = "RSNodeAddChild";
    const std::string RSNodeGetIdFuncName = "RSNodeGetId";

    const std::string RSUIDirectorCreateFuncName = "RSUIDirectorCreate";
    const std::string RSUIDirectorSetRootFuncName = "RSUIDirectorSetRoot";
    const std::string RSUIDirectorInitFuncName = "RSUIDirectorInit";
    const std::string RSUIDirectorSetRSSurfaceNodeFuncName = "RSUIDirectorSetRSSurfaceNode";

    const std::string RSTransactionFlushImplicitTransactionFuncName = "RSTransactionFlushImplicitTransaction";
}

typedef A11yRSSurfaceNode *(*RSSurfaceNodeCreateFunc)();
typedef void (*RSSurfaceNodeSetFrameGravityFunc)(A11yRSSurfaceNode *node, int32_t gravity);
typedef void (*RSSurfaceNodeSetPositionZFunc)(A11yRSSurfaceNode *node, float position);
typedef void (*RSSurfaceNodeSetBoundsFunc)(A11yRSSurfaceNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSSurfaceNodeSetBoundsExFunc)(A11yRSSurfaceNode *node, float positionX, float positionY);
typedef void (*RSSurfaceNodeSetBackgroundColorFunc)(A11yRSSurfaceNode *node);
typedef void (*RSSurfaceNodeSetRotationFunc)(A11yRSSurfaceNode *node, float degree);
typedef void (*RSSurfaceNodeSetDrawRegionFunc)(A11yRSSurfaceNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSSurfaceNodeAttachToDisplayFunc)(A11yRSSurfaceNode *node, uint64_t screenId);
typedef void (*RSSurfaceNodeDetachToDisplayFunc)(A11yRSSurfaceNode *node, uint64_t screenId);
typedef void (*RSSurfaceNodeClearChildrenFunc)(A11yRSSurfaceNode *node);
typedef void (*RSSurfaceNodeSetVisibleFunc)(A11yRSSurfaceNode *node, bool state);

typedef A11yRSCanvasNode *(*RSCanvasNodeCreateFunc)();
typedef void (*RSCanvasNodeSetBoundsFunc)(A11yRSCanvasNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSCanvasNodeSetBoundsExFunc)(A11yRSCanvasNode *node, float positionX, float positionY);
typedef void (*RSCanvasNodeSetFrameFunc)(A11yRSCanvasNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSCanvasNodeSetBackgroundColorFunc)(A11yRSCanvasNode *node);
typedef void (*RSCanvasNodeSetCornerRadiusFunc)(A11yRSCanvasNode *node, float cornerRadius);
typedef void (*RSCanvasNodeSetPositionZFunc)(A11yRSCanvasNode *node, float position);
typedef void (*RSCanvasNodeSetRotationFunc)(A11yRSCanvasNode *node, float degree);
#ifndef USE_ROSEN_DRAWING
typedef void (*RSCanvasNodeDrawingProgressByOpenSourceFunc)(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#else
typedef void (*RSCanvasNodeDrawingProgressByRosenDrawingFunc)(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#endif

typedef A11yRSNode *(*RSNodeCreateFunc)(); 
typedef void (*RSNodeSetBoundsFunc)(A11yRSNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSNodeSetFrameFunc)(A11yRSNode *node, float positionX, float positionY, float width, float height);
typedef void (*RSNodeSetBackgroundColorFunc)(A11yRSNode *node);
typedef void (*RSNodeSetCornerRadiusFunc)(A11yRSNode *node, float cornerRadius);
typedef void (*RSNodeSetPositionZFunc)(A11yRSNode *node, float position);
typedef void (*RSNodeSetRotationFunc)(A11yRSNode *node, float degree);
typedef void (*RSNodeAddChildFunc)(A11yRSNode *node, A11yRSCanvasNode *canvasNode, float degree);
typedef uint32_t (*RSNodeGetIdFunc)(A11yRSNode *node);

typedef A11yRSUIDirector *(*RSUIDirectorCreateFunc)(); 
typedef void (*RSUIDirectorSetRootFunc)(A11yRSUIDirector *node, uint32_t root);
typedef void (*RSUIDirectorInitFunc)(A11yRSUIDirector *node);
typedef void (*RSUIDirectorSetRSSurfaceNodeFunc)(A11yRSUIDirector *node, A11yRSSurfaceNode *surfaceNode);

typedef void (*RSTransactionFlushImplicitTransactionFunc)();

#define CHECK_NULL_NODE(nodePtr) { \
    if (nodePtr == nullptr) { \
        return ; \
    } \
}

A11yGraphicFuncAdapter::A11yGraphicFuncAdapter()
{
    handler_ = dlopen(LIB_GRAPHIC_NAME.c_str(), RTLD_LAZY);
}

A11yGraphicFuncAdapter::~A11yGraphicFuncAdapter()
{
    if (handler_) {
        dlclose(handler_);
        handler_ = nullptr;
    }
}

void *A11yGraphicFuncAdapter::GetFunc(const std::string funcName)
{
    if (handler_) {
        return (void *)dlsym(handler_, funcName.c_str());
    }
    return nullptr;
}

std::shared_ptr<A11yRSSurfaceNodeProxy> A11yRSSurfaceNodeProxy::Create()
{
    std::shared_ptr<A11yRSSurfaceNodeProxy> nodePtr = std::make_shared<A11yRSSurfaceNodeProxy>();
    if (nodePtr) {
        A11yGraphicFuncAdapter Funcs;
        auto func = (RSSurfaceNodeCreateFunc)Funcs.GetFunc(RSSurfaceNodeCreateFuncName);
        if (func) {
            nodePtr->node = func();
        }
    }
    if(nodePtr && nodePtr->node) {
        return nodePtr;
    } else {
        return nullptr;
    }
}

void A11yRSSurfaceNodeProxy::SetFrameGravity(int32_t gravity)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetFrameGravityFunc)Funcs.GetFunc(RSSurfaceNodeSetFrameGravityFuncName);
    if (func) {
        func(node, gravity);
    }
}

void A11yRSSurfaceNodeProxy::SetPositionZ(float position)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetPositionZFunc)Funcs.GetFunc(RSSurfaceNodeSetPositionZFuncName);
    if (func) {
        func(node, position);
    }
}

void A11yRSSurfaceNodeProxy::SetBounds(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetBoundsFunc)Funcs.GetFunc(RSSurfaceNodeSetBoundsFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSSurfaceNodeProxy::SetBoundsEx(float positionX, float positionY)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetBoundsExFunc)Funcs.GetFunc(RSSurfaceNodeSetBoundsExFuncName);
    if (func) {
        func(node, positionX, positionY);
    }
}

void A11yRSSurfaceNodeProxy::SetBackgroundColor()
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetBackgroundColorFunc)Funcs.GetFunc(RSSurfaceNodeSetBackgroundColorFuncName);
    if (func) {
        func(node);
    }
}

void A11yRSSurfaceNodeProxy::SetRotation(float degree)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetRotationFunc)Funcs.GetFunc(RSSurfaceNodeSetRotationFuncName);
    if (func) {
        func(node, degree);
    }
}
void A11yRSSurfaceNodeProxy::SetDrawRegion(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetDrawRegionFunc)Funcs.GetFunc(RSSurfaceNodeSetDrawRegionFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSSurfaceNodeProxy::AttachToDisplay(uint64_t screenId)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeAttachToDisplayFunc)Funcs.GetFunc(RSSurfaceNodeAttachToDisplayFuncName);
    if (func) {
        func(node, screenId);
    }
}

void A11yRSSurfaceNodeProxy::DetachToDisplay(uint64_t screenId)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeDetachToDisplayFunc)Funcs.GetFunc(RSSurfaceNodeDetachToDisplayFuncName);
    if (func) {
        func(node, screenId);
    }
}

void A11yRSSurfaceNodeProxy::ClearChildren()
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeClearChildrenFunc)Funcs.GetFunc(RSSurfaceNodeClearChildrenFuncName);
    if (func) {
        func(node);
    }
}

void A11yRSSurfaceNodeProxy::SetVisible(bool state)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSSurfaceNodeSetVisibleFunc)Funcs.GetFunc(RSSurfaceNodeSetVisibleFuncName);
    if (func) {
        func(node, state);
    }
}

std::shared_ptr<A11yRSCanvasNodeProxy> A11yRSCanvasNodeProxy::Create()
{
    std::shared_ptr<A11yRSCanvasNodeProxy> nodePtr = std::make_shared<A11yRSCanvasNodeProxy>();
    if (nodePtr) {
        A11yGraphicFuncAdapter Funcs;
        auto func = (RSCanvasNodeCreateFunc)Funcs.GetFunc(RSCanvasNodeCreateFuncName);
        if (func) {
            nodePtr->node = func();
        }
    }
    if(nodePtr && nodePtr->node) {
        return nodePtr;
    } else {
        return nullptr;
    }
}

void A11yRSCanvasNodeProxy::SetBounds(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetBoundsFunc)Funcs.GetFunc(RSCanvasNodeSetBoundsFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSCanvasNodeProxy::SetFrame(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetFrameFunc)Funcs.GetFunc(RSCanvasNodeSetFrameFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSCanvasNodeProxy::SetBackgroundColor()
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetBackgroundColorFunc)Funcs.GetFunc(RSCanvasNodeSetBackgroundColorFuncName);
    if (func) {
        func(node);
    }
}

void A11yRSCanvasNodeProxy::SetCornerRadius(float cornerRadius)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetCornerRadiusFunc)Funcs.GetFunc(RSCanvasNodeSetCornerRadiusFuncName);
    if (func) {
        func(node, cornerRadius);
    }
}

void A11yRSCanvasNodeProxy::SetPositionZ(float position)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetPositionZFunc)Funcs.GetFunc(RSCanvasNodeSetPositionZFuncName);
    if (func) {
        func(node, position);
    }
}

void A11yRSCanvasNodeProxy::SetRotation(float degree)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeSetRotationFunc)Funcs.GetFunc(RSCanvasNodeSetRotationFuncName);
    if (func) {
        func(node, degree);
    }
}

#ifndef USE_ROSEN_DRAWING
void A11yRSCanvasNodeProxy::DrawingProgressByOpenSource(int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeDrawingProgressByOpenSourceFunc)Funcs.GetFunc(RSCanvasNodeDrawingProgressByOpenSourceFuncName);
    if (func) {
        func(node, physicalX, physicalY, angle, imageWidth, imageHeight, half, dispalyDensity);
    }
}
#else
void A11yRSCanvasNodeProxy::DrawingProgressByRosenDrawing(int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSCanvasNodeDrawingProgressByRosenDrawingFunc)Funcs.GetFunc(RSCanvasNodeDrawingProgressByRosenDrawingFuncName);
    if (func) {
        func(node, physicalX, physicalY, angle, imageWidth, imageHeight, half, dispalyDensity);
    }
}
#endif

std::shared_ptr<A11yRSNodeProxy> A11yRSNodeProxy::Create()
{
    std::shared_ptr<A11yRSNodeProxy> nodePtr = std::make_shared<A11yRSNodeProxy>();
    if (nodePtr) {
        A11yGraphicFuncAdapter Funcs;
        auto func = (RSNodeCreateFunc)Funcs.GetFunc(RSNodeCreateFuncName);
        if (func) {
            nodePtr->node = func();
        }
    }
    if(nodePtr && nodePtr->node) {
        return nodePtr;
    } else {
        return nullptr;
    }
}

void A11yRSNodeProxy::SetBounds(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetBoundsFunc)Funcs.GetFunc(RSNodeSetBoundsFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSNodeProxy::SetFrame(float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetFrameFunc)Funcs.GetFunc(RSNodeSetFrameFuncName);
    if (func) {
        func(node, positionX, positionY, width, height);
    }
}

void A11yRSNodeProxy::SetBackgroundColor()
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetBackgroundColorFunc)Funcs.GetFunc(RSNodeSetBackgroundColorFuncName);
    if (func) {
        func(node);
    }
}

void A11yRSNodeProxy::SetCornerRadius(float cornerRadius)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetCornerRadiusFunc)Funcs.GetFunc(RSNodeSetCornerRadiusFuncName);
    if (func) {
        func(node, cornerRadius);
    }
}

void A11yRSNodeProxy::SetPositionZ(float position)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetPositionZFunc)Funcs.GetFunc(RSNodeSetPositionZFuncName);
    if (func) {
        func(node, position);
    }
}

void A11yRSNodeProxy::SetRotation(float degree)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeSetRotationFunc)Funcs.GetFunc(RSNodeSetRotationFuncName);
    if (func) {
        func(node, degree);
    }
}

void A11yRSNodeProxy::AddChild(std::shared_ptr<A11yRSCanvasNodeProxy> canvasNode, int32_t index)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeAddChildFunc)Funcs.GetFunc(RSNodeAddChildFuncName);
    if (func && canvasNode && canvasNode->node) {
        func(node, canvasNode->node, index);
    }
}

uint32_t A11yRSNodeProxy::GetId()
{
    if (node == nullptr)
        return 0;
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSNodeGetIdFunc)Funcs.GetFunc(RSNodeGetIdFuncName);
    if (func) {
        return func(node);
    }
    return 0;
}

std::shared_ptr<A11yRSUIDirectorProxy> A11yRSUIDirectorProxy::Create()
{
    std::shared_ptr<A11yRSUIDirectorProxy> nodePtr = std::make_shared<A11yRSUIDirectorProxy>();
    if (nodePtr) {
        A11yGraphicFuncAdapter Funcs;
        auto func = (RSUIDirectorCreateFunc)Funcs.GetFunc(RSUIDirectorCreateFuncName);
        if (func) {
            nodePtr->node = func();
        }
    }
    if(nodePtr && nodePtr->node) {
        return nodePtr;
    } else {
        return nullptr;
    }
}

void A11yRSUIDirectorProxy::SetRoot(uint32_t root)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSUIDirectorSetRootFunc)Funcs.GetFunc(RSUIDirectorSetRootFuncName);
    if (func) {
        return func(node, root);
    }
}
void A11yRSUIDirectorProxy::Init()
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSUIDirectorInitFunc)Funcs.GetFunc(RSUIDirectorInitFuncName);
    if (func) {
        return func(node);
    }
}

void A11yRSUIDirectorProxy::SetRSSurfaceNode(std::shared_ptr<A11yRSSurfaceNodeProxy> surfaceNode)
{
    CHECK_NULL_NODE(node);
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSUIDirectorSetRSSurfaceNodeFunc)Funcs.GetFunc(RSUIDirectorSetRSSurfaceNodeFuncName);
    if (func && surfaceNode && surfaceNode->node) {
        return func(node, surfaceNode->node);
    }
}

void A11yRSTransactionProxy::FlushImplicitTransaction()
{
    A11yGraphicFuncAdapter Funcs;
    auto func = (RSTransactionFlushImplicitTransactionFunc)Funcs.GetFunc(RSTransactionFlushImplicitTransactionFuncName);
    if (func) {
        return func();
    }
}
} // Accessibility
} // OHOS
