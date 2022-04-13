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

#include "accessibility_element_operator_callback_impl.h"
#include <memory>
#include <gtest/gtest.h>

using namespace testing;
using namespace testing::ext;

const int32_t SEQUENCE_NUM = 1;

namespace OHOS {
namespace Accessibility {
class AccessibilityElementOperatorCallbackImplUnitTest : public ::testing::Test {
public:
    AccessibilityElementOperatorCallbackImplUnitTest()
    {}
    ~AccessibilityElementOperatorCallbackImplUnitTest()
    {}
    std::promise<void> promise_;

    std::shared_ptr<AccessibilityElementOperatorCallbackImpl> instance_ = nullptr;

    static void SetUpTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplUnitTest Start";
    }
    static void TearDownTestCase()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplUnitTest End";
    }
    void SetUp()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplUnitTest SetUp()";
        instance_ = std::make_shared<AccessibilityElementOperatorCallbackImpl>(promise_);
    };
    void TearDown()
    {
        GTEST_LOG_(INFO) << "AccessibilityElementOperatorCallbackImplUnitTest TearDown()";
        instance_ = nullptr;
    }
};

/**
 * @tc.number: SetSearchElementInfoByAccessibilityIdResult_001
 * @tc.name: SetSearchElementInfoByAccessibilityIdResult
 * @tc.desc: Test function SetSearchElementInfoByAccessibilityIdResult
 */
HWTEST_F(
    AccessibilityElementOperatorCallbackImplUnitTest, SetSearchElementInfoByAccessibilityIdResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 start";

    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info{};
    infos.push_back(info);
    instance_->SetSearchElementInfoByAccessibilityIdResult(infos, SEQUENCE_NUM);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByAccessibilityIdResult_001 end";
}

/**
 * @tc.number: SetSearchElementInfoByTextResult_001
 * @tc.name: SetSearchElementInfoByTextResult
 * @tc.desc: Test function SetSearchElementInfoByTextResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplUnitTest, SetSearchElementInfoByTextResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info{};
    infos.push_back(info);
    instance_->SetSearchElementInfoByTextResult(infos, SEQUENCE_NUM);
    GTEST_LOG_(INFO) << "SetSearchElementInfoByTextResult_001 end";
}

/**
 * @tc.number: SetFindFocusedElementInfoResult_001
 * @tc.name: SetFindFocusedElementInfoResult
 * @tc.desc: Test function SetFindFocusedElementInfoResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplUnitTest, SetFindFocusedElementInfoResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info{};
    infos.push_back(info);
    instance_->SetFindFocusedElementInfoResult(info, SEQUENCE_NUM);
    GTEST_LOG_(INFO) << "SetFindFocusedElementInfoResult_001 end";
}

/**
 * @tc.number: SetFocusMoveSearchResult_001
 * @tc.name: SetFocusMoveSearchResult
 * @tc.desc: Test function SetFocusMoveSearchResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplUnitTest, SetFocusMoveSearchResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info{};
    infos.push_back(info);
    instance_->SetFocusMoveSearchResult(info, SEQUENCE_NUM);
    GTEST_LOG_(INFO) << "SetFocusMoveSearchResult_001 end";
}

/**
 * @tc.number: SetExecuteActionResult_001
 * @tc.name: SetExecuteActionResult
 * @tc.desc: Test function SetExecuteActionResult
 */
HWTEST_F(AccessibilityElementOperatorCallbackImplUnitTest, SetExecuteActionResult_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 start";
    if (!instance_) {
        GTEST_LOG_(INFO) << "Cann't get AccessibilityElementOperatorCallbackImpl instance_";
        return;
    }

    std::vector<AccessibilityElementInfo> infos;
    AccessibilityElementInfo info{};
    infos.push_back(info);
    instance_->SetExecuteActionResult(true, SEQUENCE_NUM);
    GTEST_LOG_(INFO) << "SetExecuteActionResult_001 end";
}
} // namespace Accessibility
} // namespace OHOS