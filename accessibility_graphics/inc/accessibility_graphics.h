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
#ifndef ACCESSIBILITY_GRAPHICS_H
#define ACCESSIBILITY_GRAPHICS_H

#ifndef A11Y_GRAPHICS_API
#define A11Y_GRAPHICS_API __attribute__((visibility("default")))
#endif // A11Y_GRAPHICS_API

#include <stdint.h>

struct A11yRSUIDirector;
struct A11yRSSurfaceNode;
struct A11yRSCanvasNode;
struct A11yRSNode;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// rs_surface_node.h
A11Y_GRAPHICS_API A11yRSSurfaceNode *RSSurfaceNodeCreate();

A11Y_GRAPHICS_API void RSSurfaceNodeSetFrameGravity(A11yRSSurfaceNode *node, int32_t gravity);

A11Y_GRAPHICS_API void RSSurfaceNodeSetPositionZ(A11yRSSurfaceNode *node, float position);

A11Y_GRAPHICS_API void RSSurfaceNodeSetBounds(A11yRSSurfaceNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSSurfaceNodeSetBoundsEx(A11yRSSurfaceNode *node, float positionX, float positionY);


A11Y_GRAPHICS_API void RSSurfaceNodeSetBackgroundColor(A11yRSSurfaceNode *node);

A11Y_GRAPHICS_API void RSSurfaceNodeSetRotation(A11yRSSurfaceNode *node, float degree);

A11Y_GRAPHICS_API void RSSurfaceNodeSetDrawRegion(A11yRSSurfaceNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSSurfaceNodeAttachToDisplay(A11yRSSurfaceNode *node, uint64_t screenId);

A11Y_GRAPHICS_API void RSSurfaceNodeDetachToDisplay(A11yRSSurfaceNode *node, uint64_t screenId);

A11Y_GRAPHICS_API void RSSurfaceNodeClearChildren(A11yRSSurfaceNode *node);

A11Y_GRAPHICS_API void RSSurfaceNodeSetVisible(A11yRSSurfaceNode *node, bool state);

// rs_canvas_node.h
A11Y_GRAPHICS_API A11yRSCanvasNode *RSCanvasNodeCreate();

A11Y_GRAPHICS_API void RSCanvasNodeSetBounds(A11yRSCanvasNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSCanvasNodeSetFrame(A11yRSCanvasNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSCanvasNodeSetBackgroundColor(A11yRSCanvasNode *node);

A11Y_GRAPHICS_API void RSCanvasNodeSetCornerRadius(A11yRSCanvasNode *node, float cornerRadius);

A11Y_GRAPHICS_API void RSCanvasNodeSetPositionZ(A11yRSCanvasNode *node, float position);

A11Y_GRAPHICS_API void RSCanvasNodeSetRotation(A11yRSCanvasNode *node, float degree);

#ifndef USE_ROSEN_DRAWING
A11Y_GRAPHICS_API void RSCanvasNodeDrawingProgressByOpenSource(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#else
A11Y_GRAPHICS_API void RSCanvasNodeDrawingProgressByRosenDrawing(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity);
#endif

// rs_root_node.h
A11Y_GRAPHICS_API A11yRSNode *RSNodeCreate();

A11Y_GRAPHICS_API void RSNodeSetBounds(A11yRSNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSNodeSetFrame(A11yRSNode *node,
    float positionX, float positionY, float width, float height);

A11Y_GRAPHICS_API void RSNodeSetBackgroundColor(A11yRSNode *node);

A11Y_GRAPHICS_API void RSNodeSetCornerRadius(A11yRSNode *node, float cornerRadius);

A11Y_GRAPHICS_API void RSNodeSetPositionZ(A11yRSNode *node, float position);

A11Y_GRAPHICS_API void RSNodeSetRotation(A11yRSNode *node, float degree);

A11Y_GRAPHICS_API void RSNodeAddChild(A11yRSNode *node, A11yRSCanvasNode *canvasNode, float degree);

A11Y_GRAPHICS_API uint32_t RSNodeGetId(A11yRSNode *node);

//rs_ui_director.h
A11Y_GRAPHICS_API A11yRSUIDirector *RSUIDirectorCreate(); 

A11Y_GRAPHICS_API void RSUIDirectorSetRoot(A11yRSUIDirector *node, uint32_t root);

A11Y_GRAPHICS_API void RSUIDirectorInit(A11yRSUIDirector *node);

A11Y_GRAPHICS_API void RSUIDirectorSetRSSurfaceNode(A11yRSUIDirector *node, A11yRSSurfaceNode *surfaceNode);

A11Y_GRAPHICS_API void RSTransactionFlushImplicitTransaction();
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // ACCESSIBILITY_GRAPHICS_H