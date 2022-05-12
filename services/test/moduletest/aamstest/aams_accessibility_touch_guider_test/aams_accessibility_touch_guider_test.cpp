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

#include <gtest/gtest.h>
#include <unistd.h>
#include "accessibility_touch_guider.h"
#include "accessibility_account_data.h"
#include "accessibility_display_manager.h"
#include "accessibility_element_operator_stub.h"
#include "accessibility_helper.h"
#include "accessibility_input_interceptor.h"
#include "accessible_ability_channel.h"
#include "accessible_ability_connection.h"
#include "accessible_ability_manager_service.h"
#include "iservice_registry.h"
#include "mock_accessibility_element_operator_impl.h"
#include "mock_accessibility_element_operator_proxy.h"
#include "mock_input_manager.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace Accessibility {
const static int32_t sleepTime = 3;

class AamsTouchGuideTest : public testing::Test {
public:
    AamsTouchGuideTest()
    {}
    ~AamsTouchGuideTest()
    {}
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

protected:
    std::shared_ptr<MMI::PointerEvent> CreateMoveEvent(int32_t pointerCount, int32_t pointId);
    std::shared_ptr<MMI::PointerEvent> CreateTouchEvent(int32_t action, int32_t pointId);
    std::shared_ptr<MMI::PointerEvent> CreateTouchEvent(int32_t action,
        std::vector<MMI::PointerEvent::PointerItem> point, int64_t occurredTime, int64_t startTime,
        int32_t pointId);

    sptr<AccessibleAbilityChannel> aastub_ = nullptr;
    std::shared_ptr<MMI::IInputEventConsumer> interceptorId_ = nullptr;
    void WritefileAll(const char* fname, const char* data);
    void AddAccessibilityWindowConnection();
};

void AamsTouchGuideTest::WritefileAll(const char* fname, const char* data)
{
    FILE* fp;
    if (!(fp = fopen(fname, "w"))) {
        printf("open file %s fail \n", fname);
        return;
    }

    (void)fprintf(fp, "%s", data);
    (void)fclose(fp);
}

void AamsTouchGuideTest::SetUpTestCase()
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest SetUpTestCase";
}

void AamsTouchGuideTest::TearDownTestCase()
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest TearDownTestCase";
}

void AamsTouchGuideTest::SetUp()
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest SetUp";

    Singleton<AccessibleAbilityManagerService>::GetInstance().OnStart();
    AccessibilityHelper::GetInstance().WaitForServicePublish();
    GTEST_LOG_(INFO) << "AccessibleAbilityManagerService is published";

    interceptorId_ = std::make_shared<AccessibilityInputEventConsumer>();
    MMI::InputManager::GetInstance()->AddInterceptor(interceptorId_);

    // add an ability connection client
    AccessibilityAbilityInitParams initParams;
    std::shared_ptr<AccessibilityAbilityInfo> abilityInfo = std::make_shared<AccessibilityAbilityInfo>(initParams);
    abilityInfo->SetAccessibilityAbilityType(AccessibilityAbilityTypes::ACCESSIBILITY_ABILITY_TYPE_ALL);
    abilityInfo->SetCapabilityValues(Capability::CAPABILITY_TOUCH_GUIDE);
    AppExecFwk::ElementName elementName("deviceId", "bundleName", "name");
    auto accountData = Singleton<AccessibleAbilityManagerService>::GetInstance().GetCurrentAccountData();
    accountData->AddInstalledAbility(*abilityInfo);
    sptr<AccessibleAbilityConnection> connection = new AccessibleAbilityConnection(accountData, 0, *abilityInfo);
    aastub_ = new AccessibleAbilityChannel(*connection);
    connection->OnAbilityConnectDoneSync(elementName, aastub_, 0);

    AddAccessibilityWindowConnection();
}

void AamsTouchGuideTest::TearDown()
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest TearDown";

    Singleton<AccessibleAbilityManagerService>::GetInstance().DeregisterElementOperator(0);
    sleep(sleepTime);
    aastub_ = nullptr;
    interceptorId_ = nullptr;
    AccessibilityHelper::GetInstance().GetEventType().clear();
    MMI::MockInputManager::ClearTouchActions();
}

std::shared_ptr<MMI::PointerEvent> AamsTouchGuideTest::CreateTouchEvent(int32_t action, int32_t pointId)
{
    std::shared_ptr<MMI::PointerEvent> touchEvent = MMI::PointerEvent::Create();
    MMI::PointerEvent::PointerItem item = {};

    item.SetPointerId(1);
    touchEvent->AddPointerItem(item);
    touchEvent->SetSourceType(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    touchEvent->SetPointerAction(action);
    touchEvent->SetPointerId(pointId);

    return touchEvent;
}

std::shared_ptr<MMI::PointerEvent> AamsTouchGuideTest::CreateMoveEvent(int32_t pointerCount, int32_t pointId)
{
    std::shared_ptr<MMI::PointerEvent> touchEvent = MMI::PointerEvent::Create();
    MMI::PointerEvent::PointerItem item = {};

    for (int32_t i = 0; i < pointerCount; i++) {
        item.SetPointerId(i + 1);
        touchEvent->AddPointerItem(item);
    }
    touchEvent->SetSourceType(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    touchEvent->SetPointerAction(MMI::PointerEvent::POINTER_ACTION_MOVE);
    touchEvent->SetPointerId(pointId);

    return touchEvent;
}

std::shared_ptr<MMI::PointerEvent> AamsTouchGuideTest::CreateTouchEvent(int32_t action,
    std::vector<MMI::PointerEvent::PointerItem> points, int64_t occurredTime, int64_t startTime,
    int32_t pointId)
{
    std::shared_ptr<MMI::PointerEvent> pointerEvent = MMI::PointerEvent::Create();

    for (auto point : points) {
        pointerEvent->AddPointerItem(point);
    }
    pointerEvent->SetPointerId(pointId);
    pointerEvent->SetSourceType(MMI::PointerEvent::SOURCE_TYPE_TOUCHSCREEN);
    pointerEvent->SetPointerAction(action);
    pointerEvent->SetActionStartTime(startTime);
    pointerEvent->SetActionTime(occurredTime);

    return pointerEvent;
}

void AamsTouchGuideTest::AddAccessibilityWindowConnection()
{
    GTEST_LOG_(INFO) << "aamsAccessibleAbilityChannelTest AddAccessibilityWindowConnection";
    // accessibility interaction connection
    int32_t windowId = 0;
    std::shared_ptr<AccessibilityElementOperator> operation = nullptr;
    sptr<AccessibilityElementOperatorStub> stub = new MockAccessibilityElementOperatorImpl(windowId, operation);
    sptr<IAccessibilityElementOperator> proxy = new MockAccessibilityElementOperatorProxy(stub);
    GTEST_LOG_(INFO) << "aams  RegisterElementOperator";
    Singleton<AccessibleAbilityManagerService>::GetInstance().RegisterElementOperator(windowId, proxy);
}

/**
 * @tc.number: OnTouchEvent001
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the event that two fingers moving in same directions in dragging state.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent001 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(500);
    point1.SetGlobalY(500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(1000);
    point2.SetGlobalY(500);
    point2.SetPointerId(2);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(2000);
    point3.SetGlobalY(500);
    point3.SetPointerId(1);
    MMI::PointerEvent::PointerItem point4 = {};
    point4.SetGlobalX(2500);
    point4.SetGlobalY(500);
    point4.SetPointerId(2);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent001 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 2);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 2);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 2);
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_END);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_DOWN;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    EXPECT_EQ(mtTouchAction[1], expectValue);
    expectValue = MMI::PointerEvent::POINTER_ACTION_UP;
    EXPECT_EQ(mtTouchAction[2], expectValue);
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent001 ENDs";
}

/**
 * @tc.number: OnTouchEvent002
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the event that move slowly with one finger.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent002 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::shared_ptr<MMI::PointerEvent> event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent002 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    event = CreateMoveEvent(1, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);

    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent002 ENDs";
}

/**
 * @tc.number: OnTouchEvent004
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the GESTURE_SWIPE_LEFT_THEN_RIGHT gesture.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent004 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(2500);
    point1.SetGlobalY(2500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(1500);
    point2.SetGlobalY(2500);
    point2.SetPointerId(1);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(0);
    point3.SetGlobalY(2500);
    point3.SetPointerId(1);
    MMI::PointerEvent::PointerItem point4 = {};
    point4.SetGlobalX(2500);
    point4.SetGlobalY(2250);
    point4.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent004 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_GESTURE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(mtTouchAction[2], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_GESTURE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetGestureId(),
        static_cast<int32_t>(GestureType::GESTURE_SWIPE_LEFT_THEN_RIGHT));
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent004 ENDs";
}

/**
 * @tc.number: OnTouchEvent005
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the GESTURE_SWIPE_DOWN_THEN_UP gesture.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent005 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(2500);
    point1.SetGlobalY(2500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(2500);
    point2.SetGlobalY(3500);
    point2.SetPointerId(1);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(2500);
    point3.SetGlobalY(5000);
    point3.SetPointerId(1);
    MMI::PointerEvent::PointerItem point4 = {};
    point4.SetGlobalX(2250);
    point4.SetGlobalY(2500);
    point4.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent005 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_GESTURE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(mtTouchAction[2], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_GESTURE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetGestureId(),
        static_cast<int32_t>(GestureType::GESTURE_SWIPE_DOWN_THEN_UP));
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent005 ENDs";
}

/**
 * @tc.number: OnTouchEvent006
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the GESTURE_SWIPE_RIGHT_THEN_LEFT gesture.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent006, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent006 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(2500);
    point1.SetGlobalY(2500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(3500);
    point2.SetGlobalY(2500);
    point2.SetPointerId(1);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(5000);
    point3.SetGlobalY(2500);
    point3.SetPointerId(1);
    MMI::PointerEvent::PointerItem point4 = {};
    point4.SetGlobalX(2500);
    point4.SetGlobalY(2250);
    point4.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent006 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_GESTURE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(mtTouchAction[2], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_GESTURE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetGestureId(),
        static_cast<int32_t>(GestureType::GESTURE_SWIPE_RIGHT_THEN_LEFT));
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent006 ENDs";
}

/**
 * @tc.number: OnTouchEvent007
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the GESTURE_SWIPE_UP_THEN_DOWN gesture.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent007, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent007 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(2500);
    point1.SetGlobalY(2500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(2500);
    point2.SetGlobalY(1500);
    point2.SetPointerId(1);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(2500);
    point3.SetGlobalY(0);
    point3.SetPointerId(1);
    MMI::PointerEvent::PointerItem point4 = {};
    point4.SetGlobalX(2250);
    point4.SetGlobalY(2500);
    point4.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent007 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point4);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(3);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_GESTURE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(mtTouchAction[2], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_GESTURE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetGestureId(),
        static_cast<int32_t>(GestureType::GESTURE_SWIPE_UP_THEN_DOWN));
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent007 ENDs";
}

/**
 * @tc.number: OnTouchEvent008
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the UP gesture.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent008, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent008 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(2500);
    point1.SetGlobalY(2500);
    point1.SetPointerId(1);
    MMI::PointerEvent::PointerItem point2 = {};
    point2.SetGlobalX(2500);
    point2.SetGlobalY(1500);
    point2.SetPointerId(1);
    MMI::PointerEvent::PointerItem point3 = {};
    point3.SetGlobalX(2500);
    point3.SetGlobalY(0);
    point3.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent008 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point2);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    points.clear();
    points.emplace_back(point3);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_MOVE, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_GESTURE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_GESTURE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetGestureId(), static_cast<int32_t>(GestureType::GESTURE_SWIPE_UP));
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent008 ENDs";
}

/**
 * @tc.number: OnTouchEvent009
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the single tap event.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent009, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent009 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::shared_ptr<MMI::PointerEvent> event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent009 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(sleepTime);

    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[1], EventType::TYPE_TOUCH_GUIDE_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(mtTouchAction[1], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[2], EventType::TYPE_TOUCH_GUIDE_END);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);

    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent009 ends";
}

/**
 * @tc.number: OnTouchEvent010
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the double tap and long press event.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent010, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent010 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(300);
    point1.SetGlobalY(500);
    point1.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent010 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 40, 0, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(sleepTime);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 200, 0, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(sleepTime);

    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(sleepTime);

    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);

    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent010 ends";
}

/**
 * @tc.number: OnTouchEvent011
 * @tc.name:OnTouchEvent
 * @tc.desc: Check the double-tap event.
 */
HWTEST_F(AamsTouchGuideTest, AamsTouchGuideTest_Moduletest_OnTouchEvent011, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent011 starts";

    AccessibilityHelper::GetInstance().GetEventType() = {};
    MMI::MockInputManager::ClearTouchActions();
    std::vector<MMI::PointerEvent::PointerItem> points = {};
    MMI::PointerEvent::PointerItem point1 = {};
    point1.SetGlobalX(300);
    point1.SetGlobalY(500);
    point1.SetPointerId(1);

    points.emplace_back(point1);
    std::shared_ptr<MMI::PointerEvent> event =
        CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 0, 0, 1);
    auto inputEventConsumer = MMI::MockInputManager::GetInputEventConsumer();
    if (!inputEventConsumer) {
        GTEST_LOG_(INFO) << "AamsTouchGuideTest OnTouchEvent011 inputEventConsumer is null";
        return;
    }
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_DOWN, points, 100, 0, 1);
    inputEventConsumer->OnInputEvent(event);
    sleep(1);
    event = CreateTouchEvent(MMI::PointerEvent::POINTER_ACTION_UP, points, 0, 0, 1);
    inputEventConsumer->OnInputEvent(event);

    sleep(sleepTime);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[0], EventType::TYPE_TOUCH_BEGIN);
    EXPECT_EQ(AccessibilityHelper::GetInstance().GetEventType()[3], EventType::TYPE_TOUCH_END);
    int32_t expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    auto mtTouchAction = MMI::MockInputManager::GetTouchActions();
    EXPECT_EQ(mtTouchAction[0], expectValue);
    expectValue = MMI::PointerEvent::POINTER_ACTION_MOVE;
    EXPECT_EQ(mtTouchAction[1], expectValue);

    GTEST_LOG_(INFO) << "AamsTouchGuideTest AamsTouchGuideTest_Moduletest_OnTouchEvent011 ends";
}
} // namespace Accessibility
} // namespace OHOS