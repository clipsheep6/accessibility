/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "accessibleabilitychannelstub_fuzzer.h"
#include "accessible_ability_channel_stub.h"
#include "securec.h"

namespace OHOS {
namespace Accessibility {
namespace {
    constexpr size_t DATA_MIN_SIZE = 100;
    constexpr char END_CHAR = '\0';
    constexpr size_t LEN = 10;
}

class AbilityChannelImplFuzzTest : public AccessibleAbilityChannelStub {
public:
    AbilityChannelImplFuzzTest() = default;
    ~AbilityChannelImplFuzzTest() = default;
    
    RetError SearchElementInfoByAccessibilityId(const int32_t accessibilityWindowId,
        const int64_t elementId, const int32_t requestId, const sptr<IAccessibilityElementOperatorCallback> &callback,
        const int32_t mode) override {}
    RetError SearchElementInfosByText(const int32_t accessibilityWindowId, const int64_t elementId,
        const std::string &text, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override {}
    RetError FindFocusedElementInfo(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t focusType, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override {}
    RetError FocusMoveSearch(const int32_t accessibilityWindowId, const int64_t elementId,
        const int32_t direction, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override {}
    RetError ExecuteAction(const int32_t accessibilityWindowId, const int64_t elementId, const int32_t action,
        const std::map<std::string, std::string> &actionArguments, const int32_t requestId,
        const sptr<IAccessibilityElementOperatorCallback> &callback) override {}
    RetError GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo) override {}
    RetError GetWindows(std::vector<AccessibilityWindowInfo> &windows) override {}
    RetError GetWindowsByDisplayId(const uint64_t displayId,
        std::vector<AccessibilityWindowInfo> &windows) override {}
    void SetOnKeyPressEventResult(const bool handled, const int32_t sequence) override {}
    RetError SendSimulateGesture(const std::shared_ptr<AccessibilityGestureInjectPath>& gesturePath) override {}
    RetError SetTargetBundleName(const std::vector<std::string> &targetBundleNames) override {}
};

template<class T>
size_t GetObject(T &object, const uint8_t *data, size_t size)
{
    size_t objectSize = sizeof(object);
    if (objectSize > size) {
        return 0;
    }
    return memcpy_s(&object, objectSize, databuffer, objectSize) == EOK ? objectSize : 0;
}

bool AbilityChannelOnRemoteRequestFuzzTest(const uint8_t* data, size_t size)
{
    if (data == nullptr || size < DATA_MIN_SIZE) {
        return false;
    }

    size_t position = 0;
    uint32_t code = 0;
    MessageParcel mdata;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    position += GetObject<uint32_t>(code, &data[position], size - position);
    AbilityChannelImplFuzzTest channelImp;
    mdata.WriteInterfaceToken(AbilityChannelImplFuzzTest::GetDescriptor());
    channelImp.OnRemoteRequest(code, mdata, reply, option);

    return true;
}
} // namespace Accessibility
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::Accessibility::AbilityChannelOnRemoteRequestFuzzTest(data, size);
    return 0;
}