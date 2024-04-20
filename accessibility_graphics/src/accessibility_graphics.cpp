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

#include "accessibility_graphics.h"

#include <draw/canvas.h>
#include <ui/rs_canvas_node.h>
#include <ui/rs_surface_node.h>
#include <ui/rs_ui_director.h>
#include <ui/rs_root_node.h>
#include <transaction/rs_transaction.h>

#include "pipeline/rs_recording_canvas.h"
#include "recording/recording_canvas.h"

struct A11yRSUIDirector {
    std::shared_ptr<OHOS::Rosen::RSUIDirector> rsUiDirector_;
    A11yRSUIDirector()
    {
        rsUiDirector_ = nullptr;
    }
};

struct A11yRSSurfaceNode {
    std::shared_ptr<OHOS::Rosen::RSSurfaceNode> surfaceNode_;
    A11yRSSurfaceNode()
    {
        surfaceNode_ = nullptr;
    }
};
struct A11yRSCanvasNode {
    std::shared_ptr<OHOS::Rosen::RSCanvasNode> canvasNode_;
    A11yRSCanvasNode()
    {
        canvasNode_ = nullptr;
    }
};
struct A11yRSNode {
    std::shared_ptr<OHOS::Rosen::RSNode> rsNode_;
    A11yRSNode()
    {
        rsNode_ = nullptr;
    }
};

#define CHECK_NULL_NODE(nodePtr) { \
    if ((nodePtr) == nullptr) { \
        return ; \
    } \
}

constexpr int32_t NUMBER_0 = 0;
constexpr int32_t NUMBER_1 = 1;
constexpr int32_t NUMBER_2 = 2;

const std::string PROGRESS_BACKGROUND_COLOR_TYPE = "#0A59F7"; // progress circle background
const std::string PROGRESS_COLOR_TYPE = "#FFFFFF"; // center circle
const std::string OUT_CIRCLE_BACKGROUND_COLOR_TYPE = "#1756C4"; // outer circle

constexpr int32_t OUT_CIRCLE_BACKGROUND_ALPHA = 64;
constexpr int32_t PROGRESS_BACKGROUND_ALPHA = 153;
constexpr int32_t CENTER_CIRCLE_BACKGROUND_ALPHA = 230;
constexpr int32_t PROGRESS_BAR_BACKGROUND_ALPHA = 230;

constexpr int32_t OUT_CIRCLE_RADIUS = 49; // outer circle
constexpr int32_t PROGRESS_RADIUS = 46; // progress circle background
constexpr int32_t CENTER_CIRCLE_RADIUS = 6; // center circle
constexpr int32_t PROGRESS_STROKE_WIDTH = 2; // progress stroke
constexpr int32_t START_ANGLE = -90; // start angle

constexpr int32_t HEXADECIMAL = 16;
constexpr int32_t DECIMAL = 10;
constexpr char CHAR_0 = '0';
constexpr char CHAR_9 = '9';
constexpr char CHAR_A = 'A';
constexpr char CHAR_B = 'B';
constexpr char CHAR_C = 'C';
constexpr char CHAR_D = 'D';
constexpr char CHAR_E = 'E';
constexpr char CHAR_F = 'F';

constexpr int32_t COLOR_STRING_LENGTH = 2;
constexpr int32_t RED_START_INDEX = 1;
constexpr int32_t GREEN_START_INDEX = 3;
constexpr int32_t BLUE_START_INDEX = 5;


static int32_t ParseStringToInteger(const std::string& color)
{
    int32_t val = 0;
    int32_t base = 1;
    for (int32_t index = static_cast<int32_t>(color.size() - 1); index >= 0; index--) {
        if (color[index] >= CHAR_0 && color[index] <= CHAR_9) {
            val += base * static_cast<int32_t>(color[index] - CHAR_0);
        }

        if (color[index] == CHAR_A) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        if (color[index] == CHAR_B) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        if (color[index] == CHAR_C) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        if (color[index] == CHAR_D) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        if (color[index] == CHAR_E) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        if (color[index] == CHAR_F) {
            val += base * static_cast<int32_t>(color[index] - CHAR_A + DECIMAL);
        }

        base = base * HEXADECIMAL;
    }

    return val;
}

static std::vector<int32_t> ParseColorString(const std::string& color)
{
    std::string r = color.substr(RED_START_INDEX, COLOR_STRING_LENGTH);
    std::string g = color.substr(GREEN_START_INDEX, COLOR_STRING_LENGTH);
    std::string b = color.substr(BLUE_START_INDEX, COLOR_STRING_LENGTH);

    int32_t rValue = ParseStringToInteger(r);
    int32_t gValue = ParseStringToInteger(g);
    int32_t bValue = ParseStringToInteger(b);

    std::vector<int32_t> rgbValue = {rValue, gValue, bValue};
    return rgbValue;
}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

A11Y_GRAPHICS_API A11yRSSurfaceNode *RSSurfaceNodeCreate()
{
    OHOS::Rosen::RSSurfaceNodeConfig surfaceNodeConfig;
    surfaceNodeConfig.SurfaceNodeName = "screen touch progress";
    OHOS::Rosen::RSSurfaceNodeType surfaceNodeType = OHOS::Rosen::RSSurfaceNodeType::SELF_DRAWING_WINDOW_NODE;
    A11yRSSurfaceNode * node = new A11yRSSurfaceNode();
    if (node) {
        node->surfaceNode_ = OHOS::Rosen::RSSurfaceNode::Create(surfaceNodeConfig, surfaceNodeType);
    }
    return node;
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetFrameGravity(A11yRSSurfaceNode *node, int32_t gravity)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    (void)gravity;
    node->surfaceNode_->SetFrameGravity(OHOS::Rosen::Gravity::RESIZE_ASPECT_FILL);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetPositionZ(A11yRSSurfaceNode *node, float position)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    (void)position;
    node->surfaceNode_->SetPositionZ(OHOS::Rosen::RSSurfaceNode::POINTER_WINDOW_POSITION_Z);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetBounds(A11yRSSurfaceNode *node, float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->SetBounds(positionX, positionY, width, height);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetBoundsEx(A11yRSSurfaceNode *node, float positionX, float positionY)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->SetBounds(positionX, positionY,
        node->surfaceNode_->GetStagingProperties().GetBounds().z_,
        node->surfaceNode_->GetStagingProperties().GetBounds().w_);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetBackgroundColor(A11yRSSurfaceNode *node)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->SetBackgroundColor(SK_ColorTRANSPARENT);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetRotation(A11yRSSurfaceNode *node, float degree)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->SetRotation(degree);
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetDrawRegion(A11yRSSurfaceNode *node, float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    std::shared_ptr<OHOS::Rosen::RectF> drawRect = std::make_shared<OHOS::Rosen::RectF>(positionX, positionY, width, height);
    node->surfaceNode_->SetDrawRegion(drawRect);
}

A11Y_GRAPHICS_API void RSSurfaceNodeAttachToDisplay(A11yRSSurfaceNode *node, uint64_t screenId)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->AttachToDisplay(screenId);
}

A11Y_GRAPHICS_API void RSSurfaceNodeDetachToDisplay(A11yRSSurfaceNode *node, uint64_t screenId)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->DetachToDisplay(screenId);
}

A11Y_GRAPHICS_API void RSSurfaceNodeClearChildren(A11yRSSurfaceNode *node)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->ClearChildren();
}

A11Y_GRAPHICS_API void RSSurfaceNodeSetVisible(A11yRSSurfaceNode *node, bool state)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->surfaceNode_);
    node->surfaceNode_->SetVisible(state);
}

A11Y_GRAPHICS_API A11yRSCanvasNode* RSCanvasNodeCreate()
{
    A11yRSCanvasNode * node = new A11yRSCanvasNode();
    if (node) {
        node->canvasNode_ = OHOS::Rosen::RSCanvasNode::Create();
    }
    return node;
}

A11Y_GRAPHICS_API void RSCanvasNodeSetBounds(A11yRSCanvasNode *node,
    float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetBounds(positionX, positionY, width, height);
}

A11Y_GRAPHICS_API void RSCanvasNodeSetFrame(A11yRSCanvasNode *node,
    float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetFrame(positionX, positionY, width, height);
}

A11Y_GRAPHICS_API void RSCanvasNodeSetBackgroundColor(A11yRSCanvasNode *node)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetBackgroundColor(SK_ColorTRANSPARENT);
}

A11Y_GRAPHICS_API void RSCanvasNodeSetCornerRadius(A11yRSCanvasNode *node, float cornerRadius)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetCornerRadius(cornerRadius);
}

A11Y_GRAPHICS_API void RSCanvasNodeSetPositionZ(A11yRSCanvasNode *node, float position)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetPositionZ(position);
}

A11Y_GRAPHICS_API void RSCanvasNodeSetRotation(A11yRSCanvasNode *node, float degree)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);
    node->canvasNode_->SetRotation(degree);
}

#ifndef USE_ROSEN_DRAWING
A11Y_GRAPHICS_API void RSCanvasNodeDrawingProgressByOpenSource(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);

    auto canvas = static_cast<OHOS::Rosen::RSRecordingCanvas *>(node->canvasNode_->BeginRecording(imageWidth, imageHeight));

    // outer circle
    SkPaint outCirclePaint;
    outCirclePaint.setAntiAlias(true);
    outCirclePaint.setAlphaf(OUT_CIRCLE_BACKGROUND_ALPHA);

    std::vector<int32_t> outCircleColor = ParseColorString(OUT_CIRCLE_BACKGROUND_COLOR_TYPE);
    outCirclePaint.setARGB(OUT_CIRCLE_BACKGROUND_ALPHA, outCircleColor[NUMBER_0], outCircleColor[NUMBER_1],
        outCircleColor[NUMBER_2]);
    outCirclePaint.setStyle(SkPaint::kFill_Style);
    canvas->drawCircle(half, half, dispalyDensity * OUT_CIRCLE_RADIUS, outCirclePaint);

    // center circle
    SkPaint centerCirclePaint;
    centerCirclePaint.setAntiAlias(true);
    centerCirclePaint.setAlphaf(CENTER_CIRCLE_BACKGROUND_ALPHA);
    std::vector<int32_t> centerCircleColor = ParseColorString(PROGRESS_COLOR_TYPE);
    centerCirclePaint.setARGB(CENTER_CIRCLE_BACKGROUND_ALPHA, centerCircleColor[NUMBER_0],
        centerCircleColor[NUMBER_1], centerCircleColor[NUMBER_2]);
    centerCirclePaint.setStyle(SkPaint::kFill_Style);
    canvas->drawCircle(half, half, dispalyDensity * CENTER_CIRCLE_RADIUS, centerCirclePaint);

    // progress circle
    SkPaint progressCirclePaint;
    progressCirclePaint.setAntiAlias(true);
    progressCirclePaint.setAlphaf(PROGRESS_BACKGROUND_ALPHA);
    std::vector<int32_t> progressCircleColor = ParseColorString(PROGRESS_BACKGROUND_COLOR_TYPE);
    progressCirclePaint.setARGB(PROGRESS_BACKGROUND_ALPHA, progressCircleColor[NUMBER_0],
        progressCircleColor[NUMBER_1], progressCircleColor[NUMBER_2]);
    progressCirclePaint.setStrokeWidth(PROGRESS_STROKE_WIDTH * dispalyDensity);
    progressCirclePaint.setStyle(SkPaint::kStroke_Style);
    canvas->drawCircle(half, half, dispalyDensity * PROGRESS_RADIUS, progressCirclePaint);

    // progress arc
    SkPaint progressArcPaint;
    progressArcPaint.setAntiAlias(true);
    progressArcPaint.setAlphaf(PROGRESS_BAR_BACKGROUND_ALPHA);
    std::vector<int32_t> progressArcColor = ParseColorString(PROGRESS_COLOR_TYPE);
    progressArcPaint.setARGB(PROGRESS_BAR_BACKGROUND_ALPHA, progressArcColor[NUMBER_0],
        progressArcColor[NUMBER_1], progressArcColor[NUMBER_2]);
    progressArcPaint.setStrokeWidth(PROGRESS_STROKE_WIDTH * dispalyDensity);
    progressArcPaint.setStyle(SkPaint::kStroke_Style);

    SkRect arcRect = SkRect::MakeLTRB(half - dispalyDensity * PROGRESS_RADIUS,
        half - dispalyDensity * PROGRESS_RADIUS,
        half + dispalyDensity * PROGRESS_RADIUS,
        half + dispalyDensity * PROGRESS_RADIUS);
    canvas->drawArc(arcRect, START_ANGLE, angle, false, progressArcPaint);

    canvasNode->canvasNode_->FinishRecording();
    OHOS::Rosen::RSTransaction::FlushImplicitTransaction();
}
#else
A11Y_GRAPHICS_API void RSCanvasNodeDrawingProgressByRosenDrawing(A11yRSCanvasNode *node,
    int32_t physicalX, int32_t physicalY, int32_t angle,
    int32_t imageWidth, int32_t imageHeight, int32_t half, float dispalyDensity)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->canvasNode_);

    auto canvas = node->canvasNode_->BeginRecording(imageWidth, imageHeight);

    // outer circle
    OHOS::Rosen::Drawing::Brush outCircleBrush;
    outCircleBrush.SetAntiAlias(true);
    outCircleBrush.SetAlphaF(OUT_CIRCLE_BACKGROUND_ALPHA);
    std::vector<int32_t> outCircleColor = ParseColorString(OUT_CIRCLE_BACKGROUND_COLOR_TYPE);
    outCircleBrush.SetARGB(OUT_CIRCLE_BACKGROUND_ALPHA, outCircleColor[NUMBER_0], outCircleColor[NUMBER_1],
        outCircleColor[NUMBER_2]);

    canvas->AttachBrush(outCircleBrush);
    canvas->DrawCircle(OHOS::Rosen::Drawing::Point(half, half), dispalyDensity * OUT_CIRCLE_RADIUS);
    canvas->DetachBrush();

    // center circle
    OHOS::Rosen::Drawing::Brush centerCircleBrush;
    centerCircleBrush.SetAntiAlias(true);
    centerCircleBrush.SetAlphaF(CENTER_CIRCLE_BACKGROUND_ALPHA);
    std::vector<int32_t> centerCircleColor = ParseColorString(PROGRESS_COLOR_TYPE);
    centerCircleBrush.SetARGB(CENTER_CIRCLE_BACKGROUND_ALPHA, centerCircleColor[NUMBER_0],
        centerCircleColor[NUMBER_1], centerCircleColor[NUMBER_2]);

    canvas->AttachBrush(centerCircleBrush);
    canvas->DrawCircle(OHOS::Rosen::Drawing::Point(half, half), dispalyDensity * CENTER_CIRCLE_RADIUS);
    canvas->DetachBrush();

    // progress circle
    OHOS::Rosen::Drawing::Pen progressPen;
    progressPen.SetAntiAlias(true);
    progressPen.SetAlphaF(PROGRESS_BACKGROUND_ALPHA);
    std::vector<int32_t> progressCircleColor = ParseColorString(PROGRESS_BACKGROUND_COLOR_TYPE);
    progressPen.SetARGB(PROGRESS_BACKGROUND_ALPHA, progressCircleColor[NUMBER_0],
        progressCircleColor[NUMBER_1], progressCircleColor[NUMBER_2]);
    progressPen.SetWidth(PROGRESS_STROKE_WIDTH * dispalyDensity);

    canvas->AttachPen(progressPen);
    canvas->DrawCircle(OHOS::Rosen::Drawing::Point(half, half), dispalyDensity * PROGRESS_RADIUS);
    canvas->DetachPen();

    // progress arc
    OHOS::Rosen::Drawing::Pen progressArcPen;
    progressArcPen.SetAntiAlias(true);
    progressArcPen.SetAlphaF(PROGRESS_BAR_BACKGROUND_ALPHA);
    std::vector<int32_t> progressArcColor = ParseColorString(PROGRESS_COLOR_TYPE);
    progressArcPen.SetARGB(PROGRESS_BAR_BACKGROUND_ALPHA, progressArcColor[NUMBER_0],
        progressArcColor[NUMBER_1], progressArcColor[NUMBER_2]);
    progressArcPen.SetWidth(PROGRESS_STROKE_WIDTH * dispalyDensity);

    OHOS::Rosen::Drawing::Rect rect(half - dispalyDensity * PROGRESS_RADIUS,
        half - dispalyDensity * PROGRESS_RADIUS,
        half + dispalyDensity * PROGRESS_RADIUS,
        half + dispalyDensity * PROGRESS_RADIUS);
    canvas->AttachPen(progressArcPen);
    canvas->DrawArc(rect, START_ANGLE, angle);
    canvas->DetachPen();

    node->canvasNode_->FinishRecording();
    OHOS::Rosen::RSTransaction::FlushImplicitTransaction();
}
#endif

// rs_root_node.h
A11Y_GRAPHICS_API A11yRSNode* RSNodeCreate()
{
    A11yRSNode * node = new A11yRSNode();
    if (node) {
        node->rsNode_ = OHOS::Rosen::RSRootNode::Create();
    }
    return node;
}

A11Y_GRAPHICS_API void RSNodeSetBounds(A11yRSNode *node,
    float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetBounds(positionX, positionY, width, height);
}

A11Y_GRAPHICS_API void RSNodeSetFrame(A11yRSNode *node,
    float positionX, float positionY, float width, float height)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetFrame(positionX, positionY, width, height);
}

A11Y_GRAPHICS_API void RSNodeSetBackgroundColor(A11yRSNode *node)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetBackgroundColor(SK_ColorTRANSPARENT);
}

A11Y_GRAPHICS_API void RSNodeSetCornerRadius(A11yRSNode *node, float cornerRadius)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetCornerRadius(cornerRadius);
}

A11Y_GRAPHICS_API void RSNodeSetPositionZ(A11yRSNode *node, float position)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetPositionZ(position);
}

A11Y_GRAPHICS_API void RSNodeSetRotation(A11yRSNode *node, float degree)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    node->rsNode_->SetRotation(degree);
}

A11Y_GRAPHICS_API void RSNodeAddChild(A11yRSNode *node, A11yRSCanvasNode *canvasNode, float degree)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsNode_);
    CHECK_NULL_NODE(canvasNode);
    CHECK_NULL_NODE(canvasNode->canvasNode_);
    node->rsNode_->AddChild(canvasNode->canvasNode_, degree);
}

A11Y_GRAPHICS_API uint32_t RSNodeGetId(A11yRSNode *node)
{
    if (node == nullptr || node->rsNode_ == nullptr) {
        return 0;
    }
    return  node->rsNode_->GetId();
}

//rs_ui_director.h
A11Y_GRAPHICS_API A11yRSUIDirector *RSUIDirectorCreate()
{
    A11yRSUIDirector * node = new A11yRSUIDirector();
    if (node) {
        node->rsUiDirector_ = OHOS::Rosen::RSUIDirector::Create();
    }
    return node;
}

A11Y_GRAPHICS_API void RSUIDirectorSetRoot(A11yRSUIDirector *node, uint32_t root)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsUiDirector_);
    node->rsUiDirector_->SetRoot(root);
}

A11Y_GRAPHICS_API void RSUIDirectorInit(A11yRSUIDirector *node)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsUiDirector_);
    node->rsUiDirector_->Init();
}

A11Y_GRAPHICS_API void RSUIDirectorSetRSSurfaceNode(A11yRSUIDirector *node, A11yRSSurfaceNode *surfaceNode)
{
    CHECK_NULL_NODE(node);
    CHECK_NULL_NODE(node->rsUiDirector_);
    CHECK_NULL_NODE(surfaceNode);
    CHECK_NULL_NODE(surfaceNode->surfaceNode_);
    node->rsUiDirector_->SetRSSurfaceNode(surfaceNode->surfaceNode_);
}

A11Y_GRAPHICS_API void RSTransactionFlushImplicitTransaction()
{
    OHOS::Rosen::RSTransaction::FlushImplicitTransaction();
}
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
