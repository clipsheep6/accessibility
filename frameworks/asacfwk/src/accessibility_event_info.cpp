/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "accessibility_event_info.h"
#include "accessibility_operator.h"
#include "hilog_wrapper.h"

namespace OHOS {
namespace Accessibility {
bool AccessibilityMemo::ReadFromParcel(Parcel &parcel)
{
    HILOG_DEBUG("start");
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, channelId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, componentId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, currentIndex_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, beginIndex_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, endIndex_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(StringVector, parcel, &contents_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, componentType_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, description_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, beforeText_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, latestConent_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, elementId_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemCounts_);

    return true;
}

bool AccessibilityMemo::Marshalling(Parcel &parcel) const
{
    HILOG_DEBUG("start");
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, channelId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, componentId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, currentIndex_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, beginIndex_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, endIndex_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(StringVector, parcel, contents_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, componentType_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, description_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, beforeText_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, latestConent_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, elementId_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemCounts_);

    return true;
}

AccessibilityMemo* AccessibilityMemo::Unmarshalling(Parcel& parcel)
{
    HILOG_DEBUG("start");
    AccessibilityMemo* accessibilityRecord = new AccessibilityMemo();
    if (!accessibilityRecord->ReadFromParcel(parcel)) {
        HILOG_ERROR("read from parcel failed");
        delete accessibilityRecord;
        accessibilityRecord = nullptr;
    }
    return accessibilityRecord;
}

void AccessibilityMemo::SetChannelId(const int channelId)
{
    channelId_ = channelId;
    HILOG_DEBUG("channelId_[%{public}d]", channelId_);
}

int AccessibilityMemo::GetChannelId() const
{
    HILOG_DEBUG("channelId_[%{public}d]", channelId_);
    return channelId_;
}

void AccessibilityMemo::SetSource(const int componentId)
{
    HILOG_DEBUG("componentId[%{public}d]", componentId);
    componentId_ = componentId;
    elementId_ = componentId;
}

bool AccessibilityMemo::GetSource(AccessibilityElementInfo &elementInfo) const
{
    HILOG_INFO("called] channelId_[%{public}d], windowId_[%{public}d], elementId_[%{public}d]",
        channelId_, windowId_, elementId_);
    AccessibilityOperator *instance = &AccessibilityOperator::GetInstance();
    AccessibilityElementInfo element {};
    std::vector<AccessibilityElementInfo> elementInfos {};
    bool result = false;
    if (instance != nullptr) {
        result = instance->SearchElementInfosByAccessibilityId(channelId_, windowId_, elementId_, 0, elementInfos);
        for (auto& info : elementInfos) {
            HILOG_INFO("called] GetSource OK");
            result = true;
            elementInfo = info;
            break;
        }
        HILOG_INFO("called] GetSource is null");
    } else {
        HILOG_INFO("called] Can't get AccessibilityOperator instance");
    }
    return result;
}

int AccessibilityMemo::GetViewId() const
{
    HILOG_DEBUG("componentId_[%{public}d]", componentId_);
    return componentId_;
}

int AccessibilityMemo::GetAccessibilityId() const
{
    HILOG_INFO("called] elementId_[%{public}d]", elementId_);
    return elementId_;
}

void AccessibilityMemo::SetWindowId(const int windowId)
{
    HILOG_DEBUG("windowId[%{public}d]", windowId);
    windowId_ = windowId;
}

int AccessibilityMemo::GetWindowId() const
{
    HILOG_DEBUG("windowId_[%{public}d]", windowId_);
    return windowId_;
}

int AccessibilityMemo::GetCurrentIndex() const
{
    HILOG_DEBUG("currentIndex_[%{public}d]", currentIndex_);
    return currentIndex_;
}

void AccessibilityMemo::SetCurrentIndex(const int index)
{
    currentIndex_ = index;
    HILOG_DEBUG("currentIndex_[%{public}d]", currentIndex_);
}

int AccessibilityMemo::GetBeginIndex() const
{
    HILOG_DEBUG("beginIndex_[%{public}d]", beginIndex_);
    return beginIndex_;
}

void AccessibilityMemo::SetBeginIndex(const int index)
{
    beginIndex_ = index;
    HILOG_DEBUG("beginIndex_[%{public}d]", beginIndex_);
}

int AccessibilityMemo::GetEndIndex() const
{
    HILOG_DEBUG("endIndex_[%{public}d]", endIndex_);
    return endIndex_;
}

void AccessibilityMemo::SetEndIndex(const int index)
{
    endIndex_ = index;
    HILOG_DEBUG("endIndex_[%{public}d]", endIndex_);
}

std::string AccessibilityMemo::GetComponentType() const
{
    HILOG_DEBUG("componentType_[%{public}s]", componentType_.c_str());
    return componentType_;
}

void AccessibilityMemo::SetComponentType(const std::string &className)
{
    componentType_ = className;
    HILOG_DEBUG("componentType_[%{public}s]", componentType_.c_str());
}

std::string AccessibilityMemo::GetBeforeText() const
{
    HILOG_DEBUG("beforeText_[%{public}s]", beforeText_.c_str());
    return beforeText_;
}

void AccessibilityMemo::SetBeforeText(const std::string &beforeText)
{
    beforeText_ = beforeText;
    HILOG_DEBUG("beforeText_[%{public}s]", beforeText_.c_str());
}

void AccessibilityMemo::AddContent(const std::string &content)
{
    contents_.push_back(content);
    HILOG_DEBUG("content[%{public}s]", content.c_str());
}

std::vector<std::string> AccessibilityMemo::GetContentList() const
{
    return contents_;
}

std::string AccessibilityMemo::GetLatestContent() const
{
    HILOG_DEBUG("latestConent_[%{public}s]", latestConent_.c_str());
    return latestConent_;
}

void AccessibilityMemo::SetLatestContent(const std::string &content)
{
    latestConent_ = content;
    HILOG_DEBUG("latestConent_[%{public}s]", latestConent_.c_str());
}

std::string AccessibilityMemo::GetDescription() const
{
    HILOG_DEBUG("description_[%{public}s]", description_.c_str());
    return description_;
}

void AccessibilityMemo::SetDescription(const std::string &contentDescription)
{
    description_ = contentDescription;
    HILOG_DEBUG("description_[%{public}s]", description_.c_str());
}

void AccessibilityMemo::SetItemCounts(const int itemCounts)
{
    itemCounts_ = itemCounts;
    HILOG_DEBUG("itemCounts_[%{public}d]", itemCounts_);
}

int AccessibilityMemo::GetItemCounts() const
{
    HILOG_DEBUG("itemCounts_[%{public}d]", itemCounts_);
    return itemCounts_;
}

bool AccessibilityEventInfo::ReadFromParcel(Parcel &parcel)
{
    HILOG_DEBUG("start");
    int eventType = TYPE_VIEW_INVALID;
    int gestureType = GESTURE_INVALID;
    int triggerAction = ACCESSIBILITY_ACTION_INVALID;
    int textMoveStep = STEP_CHARACTER;
    int windowContentChangeTypes = CONTENT_CHANGE_TYPE_INVALID;
    int windowChangeTypes = WINDOW_UPDATE_INVALID;
    int category = CATEGORY_INVALID;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, eventType);
    eventType_ = static_cast<EventType>(eventType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, gestureType);
    gestureType_ = static_cast<GestureType>(gestureType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, bundleName_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, triggerAction);
    triggerAction_ = static_cast<ActionType>(triggerAction);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, timeStamp_);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, textMoveStep);
    textMoveStep_ = static_cast<TextMoveUnit>(textMoveStep);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowContentChangeTypes);
    windowContentChangeTypes_ = static_cast<WindowsContentChangeTypes>(windowContentChangeTypes);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowChangeTypes);
    windowChangeTypes_ = static_cast<WindowUpdateType>(windowChangeTypes);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, recordsCount_);
    int32_t recordSize = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, recordSize);
    for (auto i = 0; i < recordSize; i++) {
        std::shared_ptr<AccessibilityEventInfo> accessibilityRecord(parcel.ReadParcelable<AccessibilityEventInfo>());
        if (!accessibilityRecord) {
            HILOG_ERROR("ReadParcelable<accessibilityInfo> failed");
            return false;
        }
        records_.emplace_back(*accessibilityRecord);
    }
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, category);
    category_ = static_cast<NotificationCategory>(category);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pageId_);

    int32_t channelId = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, channelId);
    SetChannelId(channelId);
    int32_t componentId = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, componentId);
    SetSource(componentId);
    int32_t windowId = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, windowId);
    SetWindowId(windowId);
    int32_t currentIndex = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, currentIndex);
    SetCurrentIndex(currentIndex);
    int32_t beginIndex = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, beginIndex);
    SetBeginIndex(beginIndex);
    int32_t endIndex = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, endIndex);
    SetEndIndex(endIndex);
    int32_t contentSize = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, contentSize);
    std::string content;
    for (auto i = 0 ; i < contentSize; i++) {
        READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, content);
        AddContent(content);
    }
    std::string componentType;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, componentType);
    SetComponentType(componentType);
    std::string description;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, description);
    SetDescription(description);
    std::string beforeText;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, beforeText);
    SetBeforeText(beforeText);
    std::string latestConent;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, latestConent);
    SetLatestContent(latestConent);
    int32_t elementId = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, elementId);

    int32_t itemCounts = 0;
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, itemCounts);
    SetItemCounts(itemCounts);
    return true;
}

bool AccessibilityEventInfo::Marshalling(Parcel &parcel) const
{
    HILOG_DEBUG("start");
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(eventType_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(gestureType_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, bundleName_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(triggerAction_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, timeStamp_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(textMoveStep_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(windowContentChangeTypes_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(windowChangeTypes_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, recordsCount_);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, records_.size());
    for (auto &record : records_) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Parcelable, parcel, &record);
    }
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, static_cast<int>(category_));
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pageId_);

    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetChannelId());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetViewId());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetWindowId());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetCurrentIndex());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetBeginIndex());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetEndIndex());
    auto contentList = GetContentList();
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, contentList.size());
    for (auto &content : contentList) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, content);
    }
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, GetComponentType());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, GetDescription());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, GetBeforeText());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, GetLatestContent());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetAccessibilityId());
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, GetItemCounts());
    return true;
}

AccessibilityEventInfo *AccessibilityEventInfo::Unmarshalling(Parcel& parcel)
{
    HILOG_DEBUG("start");
    AccessibilityEventInfo *accessibilityEventInfo = new AccessibilityEventInfo();
    if (!accessibilityEventInfo->ReadFromParcel(parcel)) {
        HILOG_ERROR("read from parcel failed");
        delete accessibilityEventInfo;
        accessibilityEventInfo = nullptr;
    }
    return accessibilityEventInfo;
}

int AccessibilityEventInfo::GetRecordCount() const
{
    HILOG_DEBUG("recordsCount_[%{public}d]", recordsCount_);
    return recordsCount_;
}

void AccessibilityEventInfo::SetRecordCount(const int recordCount)
{
    recordsCount_ = recordCount;
    HILOG_DEBUG("recordsCount_[%{public}d]", recordsCount_);
}

void AccessibilityEventInfo::AddRecord(const AccessibilityEventInfo &record)
{
    HILOG_DEBUG("start");
    records_.push_back(record);
}

AccessibilityEventInfo AccessibilityEventInfo::GetRecord(const int index)
{
    HILOG_DEBUG("start");
    if (index >= recordsCount_ || index < 0) {
        AccessibilityEventInfo record {};
        HILOG_ERROR("[called] index[%{public}d] is invalid", index);
        return record;
    }
    return records_[index];
}

std::vector<AccessibilityEventInfo> AccessibilityEventInfo::GetRecords()
{
    HILOG_DEBUG("start");
    return records_;
}

EventType AccessibilityEventInfo::GetEventType() const
{
    HILOG_DEBUG("eventType_[%{public}d]", eventType_);
    return eventType_;
}

WindowsContentChangeTypes AccessibilityEventInfo::GetWindowContentChangeTypes() const
{
    HILOG_DEBUG("windowType_[%{public}d]", windowContentChangeTypes_);
    return windowContentChangeTypes_;
}

void AccessibilityEventInfo::SetWindowContentChangeTypes(const WindowsContentChangeTypes changeTypes)
{
    windowContentChangeTypes_ = changeTypes;
    HILOG_DEBUG("windowType_[%{public}d]", windowContentChangeTypes_);
}

WindowUpdateType AccessibilityEventInfo::GetWindowChangeTypes() const
{
    HILOG_DEBUG("windowType_[%{public}d]", windowChangeTypes_);
    return windowChangeTypes_;
}

void AccessibilityEventInfo::SetWindowChangeTypes(const WindowUpdateType changeTypes)
{
    windowChangeTypes_ = changeTypes;
    HILOG_DEBUG("windowType_[%{public}d]", windowChangeTypes_);
}

void AccessibilityEventInfo::SetEventType(const EventType eventType)
{
    eventType_ = EventType(eventType);
    HILOG_DEBUG("eventType_[%{public}d]", eventType_);
}

long long AccessibilityEventInfo::GetTimeStamp() const
{
    HILOG_DEBUG("start");
    return timeStamp_;
}

void AccessibilityEventInfo::SetTimeStamp(const long long eventTime)
{
    HILOG_DEBUG("start");
    timeStamp_ = eventTime;
}

std::string AccessibilityEventInfo::GetBundleName() const
{
    HILOG_DEBUG("bundleName_[%{public}s]", bundleName_.c_str());
    return bundleName_;
}

void AccessibilityEventInfo::SetBundleName(const std::string &bundleName)
{
    bundleName_ = bundleName;
    HILOG_DEBUG("bundleName_[%{public}s]", bundleName_.c_str());
}

void AccessibilityEventInfo::SetTextMovementStep(const TextMoveUnit granularity)
{
    textMoveStep_ = granularity;
    HILOG_DEBUG("textMoveStep_[%{public}d]", textMoveStep_);
}

TextMoveUnit AccessibilityEventInfo::GetTextMovementStep() const
{
    HILOG_DEBUG("textMoveStep_[%{public}d]", textMoveStep_);
    return textMoveStep_;
}

void AccessibilityEventInfo::SetTriggerAction(const ActionType action)
{
    triggerAction_ = action;
    HILOG_DEBUG("triggerAction_[%{public}d]", triggerAction_);
}

ActionType AccessibilityEventInfo::GetTriggerAction() const
{
    HILOG_DEBUG("triggerAction_[%{public}d]", triggerAction_);
    return triggerAction_;
}

AccessibilityEventInfo::AccessibilityEventInfo(int windowId, WindowUpdateType windowChangeTypes)
{
    HILOG_DEBUG("start");
    eventType_ = TYPE_WINDOW_UPDATE;
    windowChangeTypes_ = windowChangeTypes;
    SetWindowId(windowId);
}

AccessibilityEventInfo::AccessibilityEventInfo(EventType eventType)
{
    eventType_ = eventType;
    HILOG_DEBUG("eventType_[%{public}d]", eventType_);
}

void AccessibilityEventInfo::SetNotificationInfo(const NotificationCategory category)
{
    category_ = category;
    HILOG_DEBUG("category_[%{public}d]", category_);
}

void AccessibilityEventInfo::SetGestureType(const GestureType gestureType)
{
    gestureType_ = gestureType;
    HILOG_DEBUG("gestureType_[%{public}d]", gestureType_);
}

GestureType AccessibilityEventInfo::GetGestureType() const
{
    HILOG_DEBUG("gestureType_[%{public}d]", gestureType_);
    return gestureType_;
}

NotificationCategory AccessibilityEventInfo::GetNotificationInfo() const
{
    HILOG_DEBUG("category_[%{public}d]", category_);
    return category_;
}

void AccessibilityEventInfo::SetPageId(const int pageId)
{
    HILOG_DEBUG("pageId[%{public}d]", pageId);
    pageId_ = pageId;
}

int AccessibilityEventInfo::GetPageId() const
{
    HILOG_DEBUG("pageId_[%{public}d]", pageId_);
    return pageId_;
}
} // namespace Accessibility
} // namespace OHOS