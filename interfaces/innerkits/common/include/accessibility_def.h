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

#ifndef ACCESSIBILITY_DEFINE_H
#define ACCESSIBILITY_DEFINE_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include "accessibility_constants.h"

namespace OHOS {
namespace AccessibilityConfig {

enum DALTONIZATION_TYPE : uint32_t {
    Normal = 0,
    Protanomaly,
    Deuteranomaly,
    Tritanomaly,
};

enum CLICK_RESPONSE_TIME : uint32_t {
    ResponseDelayShort = 0,
    ResponseDelayMedium,
    ResponseDelayLong,
};

enum IGNORE_REPEAT_CLICK_TIME : uint32_t {
    RepeatClickTimeoutShortest = 0,
    RepeatClickTimeoutShort,
    RepeatClickTimeoutMedium,
    RepeatClickTimeoutLong,
    RepeatClickTimeoutLongest,
};
}
}

namespace OHOS {
namespace Accessibility {
enum RetError : int32_t {
    RET_OK = 0,
    RET_ERR_FAILED = -1,
    RET_ERR_INVALID_PARAM = 1001,
    RET_ERR_NULLPTR,
    RET_ERR_IPC_FAILED,
    RET_ERR_SAMGR,
    RET_ERR_NO_PERMISSION,
    RET_ERR_NOT_SYSTEM_APP,
    RET_ERR_TIME_OUT,

    RET_ERR_REGISTER_EXIST = 4001,
    RET_ERR_NO_REGISTER,
    RET_ERR_CONNECTION_EXIST,
    RET_ERR_NO_CONNECTION,
    RET_ERR_NO_WINDOW_CONNECTION,
    RET_ERR_NO_CAPABILITY,
    RET_ERR_INVALID_ELEMENT_INFO_FROM_ACE,
    RET_ERR_PERFORM_ACTION_FAILED_BY_ACE,
    RET_ERR_NO_INJECTOR,
    RET_ERR_NOT_INSTALLED,
    RET_ERR_NOT_ENABLED,
    RET_ERR_PROPERTY_NOT_EXIST,
    RET_ERR_ACTION_NOT_SUPPORT,
};

enum SET_AA_CALLBACK_RESULT : int32_t {
    FIND_ACCESSIBILITY_NODE_BY_ACCESSIBILITY_ID = 2,
    FIND_ACCESSIBILITY_NODE_BY_VIEW_ID,
    FIND_ACCESSIBILITY_NODE_BY_TEXT,
    FIND_FOCUS,
    FIND_FOCUS_SEARCH,
};

enum TARGET_NAME : int32_t {
    HIGH_CONTRAST_TEXT = 0,
    INVERT_COLOR,
    DALTONIZATION_COLOR_FILTER,
    CONTENT_TIMEOUT,
    ANIMATION_OFF,
    BRIGHTNESS_DISCOUNT,
    AUDIO_MONO,
    AUDIO_BALANCE,
    MOUSE_KEY,
    CAPTION_STATE,
    CAPTION_STYLE,
    SCREEN_MAGNIFICATION,
    MOUSE_AUTOCLICK,
};

enum GlobalAction : uint32_t {
    GLOBAL_ACTION_BACK = 0,
    GLOBAL_ACTION_HOME = 1,
    GLOBAL_ACTION_RECENT = 2,
    GLOBAL_ACTION_NOTIFICATION = 3,
    GLOBAL_ACTION_LOCK_SCREEN = 4,
    GLOBAL_ACTION_MAX,
    GLOBAL_ACTION_INVALID = 0xFFFFFFFF,
};

enum AbilityStateType : int32_t {
    ABILITY_STATE_INVALID = 0,
    ABILITY_STATE_ENABLE,
    ABILITY_STATE_DISABLE,
    ABILITY_STATE_INSTALLED,
};

// The capability types of the accessible ability.
enum Capability : uint32_t {
    CAPABILITY_RETRIEVE = 0x0001,
    CAPABILITY_TOUCH_GUIDE = 0x0002,
    CAPABILITY_GESTURE = 0x0004,
    CAPABILITY_KEY_EVENT_OBSERVER = 0x0008,
    CAPABILITY_ZOOM = 0x0010,
};

// The accessibility ability types for feedbacks.
enum AccessibilityAbilityTypes : uint32_t {
    ACCESSIBILITY_ABILITY_TYPE_INVALID = 0x00000000,
    ACCESSIBILITY_ABILITY_TYPE_SPOKEN = 0x00000001,
    ACCESSIBILITY_ABILITY_TYPE_HAPTIC = 0x00000002,
    ACCESSIBILITY_ABILITY_TYPE_AUDIBLE = 0x00000004,
    ACCESSIBILITY_ABILITY_TYPE_VISUAL = 0x00000008,
    ACCESSIBILITY_ABILITY_TYPE_GENERIC = 0x00000010,
    ACCESSIBILITY_ABILITY_TYPE_ALL = 0xFFFFFFFF,
};

enum CaptionMode : int32_t {
    CAPTION_BLACK_WHITE = 0,  // CAPTION_background_foreground
    CAPTION_WHITE_BLACK,
    CAPTION_BLACK_YELLOW,
    CAPTION_BLUE_YELLOW,
    CAPTION_CUSTOM,
    CAPTION_MODE_MAX,
};

enum CaptionEdge : int32_t {
    CAPTION_EDGE_NONEB = 0,
    CAPTION_EDGE_OUTLINE,
    CAPTION_EDGE_DROP_SHADOW,
    CAPTION_EDGE_RAISED,
    CAPTION_EDGE_DEPRESSED,
};

enum ActionType : int32_t {
    ACCESSIBILITY_ACTION_INVALID = 0,
    ACCESSIBILITY_ACTION_FOCUS =  0x00000001,
    ACCESSIBILITY_ACTION_CLEAR_FOCUS = 0x00000002,
    ACCESSIBILITY_ACTION_SELECT = 0x00000004,
    ACCESSIBILITY_ACTION_CLEAR_SELECTION = 0x00000008,
    ACCESSIBILITY_ACTION_CLICK = 0x00000010,
    ACCESSIBILITY_ACTION_LONG_CLICK = 0x00000020,
    ACCESSIBILITY_ACTION_ACCESSIBILITY_FOCUS = 0x00000040,
    ACCESSIBILITY_ACTION_CLEAR_ACCESSIBILITY_FOCUS = 0x00000080,
    ACCESSIBILITY_ACTION_SCROLL_FORWARD = 0x00000100,
    ACCESSIBILITY_ACTION_SCROLL_BACKWARD = 0x00000200,
    ACCESSIBILITY_ACTION_COPY = 0x00000400,
    ACCESSIBILITY_ACTION_PASTE = 0x00000800,
    ACCESSIBILITY_ACTION_CUT = 0x00001000,
    ACCESSIBILITY_ACTION_SET_SELECTION = 0x00002000,
    ACCESSIBILITY_ACTION_SET_TEXT = 0x00004000,
    ACCESSIBILITY_ACTION_SET_CURSOR_POSITION = 0x00008000,
    ACCESSIBILITY_ACTION_NEXT_TEXT = 0x00200000,
    ACCESSIBILITY_ACTION_PREVIOUS_TEXT = 0x00400000,
    ACCESSIBILITY_ACTION_UNFOLD = 0x00800000,
    ACCESSIBILITY_ACTION_FOLD = 0x01000000,
    ACCESSIBILITY_ACTION_NEXT_HTML_ITEM = 0x02000000,
    ACCESSIBILITY_ACTION_PREVIOUS_HTML_ITEM = 0x04000000,
    ACCESSIBILITY_ACTION_DELETED = 0x08000000,
    ACCESSIBILITY_ACTION_COMMON = 0x10000000,
    ACCESSIBILITY_ACTION_HOME = 0x00010000,
    ACCESSIBILITY_ACTION_BACK = 0x00020000,
    ACCESSIBILITY_ACTION_RECENTTASK = 0x00040000,
    ACCESSIBILITY_ACTION_NOTIFICATIONCENTER = 0x00080000,
    ACCESSIBILITY_ACTION_CONTROLCENTER = 0x00008000,
    ACCESSIBILITY_ACTION_TYPE_MASK = 0x1FFFFFFF,
};

// text move step
enum TextMoveUnit : int32_t {
    STEP_INVALID = 0,
    STEP_CHARACTER = 0x00000001,
    STEP_WORD = 0x00000002,
    STEP_LINE = 0x00000004,
    STEP_PAGE = 0x00000008,
    STEP_PARAGRAPH = 0x00000010,
};

enum FocusMoveDirection : int32_t {
    DIRECTION_INVALID = 0,
    UP = 0x00000001,
    DOWN = 0x00000002,
    LEFT = 0x00000004,
    RIGHT = 0x00000008,
    FORWARD = 0x00000010,
    BACKWARD = 0x00000020,
};

enum WindowUpdateType : int32_t {
    WINDOW_UPDATE_INVALID = 0,
    WINDOW_UPDATE_ACCESSIBILITY_FOCUSED = 0x00000001,
    WINDOW_UPDATE_FOCUSED = 0x00000002,
    WINDOW_UPDATE_ACTIVE = 0x00000004,
    WINDOW_UPDATE_ADDED = 0x00000008,
    WINDOW_UPDATE_REMOVED = 0x00000010,
    WINDOW_UPDATE_BOUNDS = 0x00000020,
    WINDOW_UPDATE_TITLE = 0x00000040,
    WINDOW_UPDATE_LAYER = 0x00000080,
    WINDOW_UPDATE_PARENT = 0x00000100,
    WINDOW_UPDATE_CHILDREN = 0x00000200,
    WINDOW_UPDATE_PIP = 0x00000400,
    WINDOW_UPDATE_PROPERTY = 0x00000800,
};

enum WindowsContentChangeTypes : int32_t {
    CONTENT_CHANGE_TYPE_INVALID = 0,
    CONTENT_CHANGE_TYPE_SUBTREE = 0x00000001,
    CONTENT_CHANGE_TYPE_TEXT = 0x00000002,
    CONTENT_CHANGE_TYPE_CONTENT_DESCRIPTION = 0x00000004,
};

enum EventType : uint32_t {
    TYPE_VIEW_INVALID = 0,
    TYPE_VIEW_CLICKED_EVENT = 0x00000001,
    TYPE_VIEW_LONG_CLICKED_EVENT = 0x00000002,
    TYPE_VIEW_SELECTED_EVENT = 0x00000004,
    TYPE_VIEW_FOCUSED_EVENT = 0x00000008,
    TYPE_VIEW_TEXT_UPDATE_EVENT = 0x00000010,
    TYPE_PAGE_STATE_UPDATE = 0x00000020,
    TYPE_NOTIFICATION_UPDATE_EVENT = 0x00000040,
    TYPE_VIEW_HOVER_ENTER_EVENT = 0x00000080,
    TYPE_VIEW_HOVER_EXIT_EVENT = 0x00000100,
    TYPE_TOUCH_GUIDE_GESTURE_BEGIN = 0x00000200,
    TYPE_TOUCH_GUIDE_GESTURE_END = 0x00000400,
    TYPE_PAGE_CONTENT_UPDATE = 0x00000800,
    TYPE_VIEW_SCROLLED_EVENT = 0x000001000,
    TYPE_VIEW_TEXT_SELECTION_UPDATE_EVENT = 0x000002000,
    TYPE_PUBLIC_NOTICE_EVENT = 0x00004000,
    TYPE_VIEW_ACCESSIBILITY_FOCUSED_EVENT = 0x00008000,
    TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED_EVENT = 0x00010000,
    TYPE_VIEW_TEXT_MOVE_UNIT_EVENT = 0x00020000,
    TYPE_TOUCH_GUIDE_BEGIN = 0x00040000,
    TYPE_TOUCH_GUIDE_END = 0x00080000,
    TYPE_TOUCH_BEGIN = 0x00100000,
    TYPE_TOUCH_END = 0x00200000,
    TYPE_WINDOW_UPDATE = 0x00400000,
    TYPE_INTERRUPT_EVENT = 0x00800000,
    TYPE_GESTURE_EVENT = 0x01000000,
    TYPE_VIEW_SCROLLED_START = 0x04000000,
    TYPE_VIEW_REQUEST_FOCUS_FOR_ACCESSIBILITY = 0x02000000,
    TYPE_VIEW_ANNOUNCE_FOR_ACCESSIBILITY = 0x10000000,
    TYPE_MAX_NUM = 0x80000000,
    TYPES_ALL_MASK = 0xFFFFFFFF,
};

enum NotificationCategory : int32_t {
    CATEGORY_INVALID = 0,
    CATEGORY_CALL = 0x00000001,
    CATEGORY_MSG = 0x00000002,
    CATEGORY_EMAIL = 0x00000004,
    CATEGORY_EVENT = 0x00000008,
    CATEGORY_PROMO = 0x00000010,
    CATEGORY_ALARM = 0x00000020,
    CATEGORY_PROGRESS = 0x00000040,
    CATEGORY_SOCIAL = 0x00000080,
    CATEGORY_ERR = 0x00000100,
    CATEGORY_TRANSPORT = 0x00000200,
    CATEGORY_SYS = 0x00000400,
    CATEGORY_SERVICE = 0x00000800,
    CATEGORY_OTHERS = 0x00001000,
};

// The types of swipe gestures which are performed on the touch screen.
enum GestureType : uint32_t {
    GESTURE_INVALID = 0,
    GESTURE_SWIPE_UP = 0x0001,
    GESTURE_SWIPE_DOWN = 0x0002,
    GESTURE_SWIPE_LEFT = 0x0003,
    GESTURE_SWIPE_RIGHT = 0x0004,
    GESTURE_SWIPE_LEFT_THEN_RIGHT = 0x0005,
    GESTURE_SWIPE_RIGHT_THEN_LEFT = 0x0006,
    GESTURE_SWIPE_UP_THEN_DOWN = 0x0007,
    GESTURE_SWIPE_DOWN_THEN_UP = 0x0008,
    GESTURE_SWIPE_LEFT_THEN_UP = 0x0009,
    GESTURE_SWIPE_LEFT_THEN_DOWN = 0x000A,
    GESTURE_SWIPE_RIGHT_THEN_UP = 0x000B,
    GESTURE_SWIPE_RIGHT_THEN_DOWN = 0x000C,
    GESTURE_SWIPE_UP_THEN_LEFT = 0x000D,
    GESTURE_SWIPE_UP_THEN_RIGHT = 0x000E,
    GESTURE_SWIPE_DOWN_THEN_LEFT = 0x000F,
    GESTURE_SWIPE_DOWN_THEN_RIGHT = 0x0010,

    GESTURE_TAP = 0x0011,
    GESTURE_DOUBLETAP = 0x0012,
    GESTURE_DOUBLETAP_HOLD = 0x0013,
    GESTURE_DRAG = 0x0014,
    GESTURE_SWIPE_SLOWLY = 0x0015,
    GESTURE_TRIPLETAP = 0x0016,
    GESTURE_SCALE = 0x0017,
    GESTURE_SCROLL = 0x0018,
    GESTURE_TWO_FINGER_SINGLE_TAP = 0x0019,
    GESTURE_TWO_FINGER_DOUBLE_TAP = 0x001A,
    GESTURE_TWO_FINGER_DOUBLE_TAP_AND_HOLD = 0x001B,
    GESTURE_TWO_FINGER_TRIPLE_TAP = 0x001C,
    GESTURE_TWO_FINGER_TRIPLE_TAP_AND_HOLD = 0x001D,
    GESTURE_THREE_FINGER_SINGLE_TAP = 0x001E,
    GESTURE_THREE_FINGER_DOUBLE_TAP = 0x001F,
    GESTURE_THREE_FINGER_DOUBLE_TAP_AND_HOLD = 0x0020,
    GESTURE_THREE_FINGER_TRIPLE_TAP = 0x0021,
    GESTURE_THREE_FINGER_TRIPLE_TAP_AND_HOLD = 0x0022,
    GESTURE_FOUR_FINGER_SINGLE_TAP = 0x0023,
    GESTURE_FOUR_FINGER_DOUBLE_TAP = 0x0024,
    GESTURE_FOUR_FINGER_DOUBLE_TAP_AND_HOLD = 0x0025,
    GESTURE_FOUR_FINGER_TRIPLE_TAP = 0x0026,
    GESTURE_FOUR_FINGER_TRIPLE_TAP_AND_HOLD = 0x0027,
    GESTURE_THREE_FINGER_SWIPE_UP = 0x0028,
    GESTURE_THREE_FINGER_SWIPE_DOWN = 0x0029,
    GESTURE_THREE_FINGER_SWIPE_LEFT = 0x002A,
    GESTURE_THREE_FINGER_SWIPE_RIGHT = 0x002B,
    GESTURE_FOUR_FINGER_SWIPE_UP = 0x002C,
    GESTURE_FOUR_FINGER_SWIPE_DOWN = 0x002D,
    GESTURE_FOUR_FINGER_SWIPE_LEFT = 0x002E,
    GESTURE_FOUR_FINGER_SWIPE_RIGHT = 0x002F,
    GESTURE_GESTURE_ALL = 0xFFFFFFFF
};

enum AccessibilityWindowType : int32_t {
    TYPE_WINDOW_INVALID = 0,
    TYPE_ACCESSIBILITY_OVERLAY = 0x00000001,
    TYPE_APPLICATION = 0x00000002,
    TYPE_INPUT_METHOD = 0x00000004,
    TYPE_SPLIT_SCREEN_DIVIDER = 0x00000008,
    TYPE_SYSTEM = 0x00000010,
};

constexpr int32_t PARAM0 = 0;
constexpr int32_t PARAM1 = 1;
constexpr int32_t PARAM2 = 2;
} // namespace Accessibility
} // namespace OHOS

constexpr size_t CALLBACK_SIZE = 1;
constexpr size_t ARGS_SIZE_ZERO = 0;
constexpr size_t ARGS_SIZE_ONE = 1;
constexpr size_t ARGS_SIZE_TWO = 2;
constexpr size_t ARGS_SIZE_THREE = 3;
constexpr size_t ARGS_SIZE_FOUR = 4;
constexpr int32_t DEFAULT_INT32 = 0;
constexpr int32_t CODE_SUCCESS = 0;
constexpr int32_t CODE_FAILED = -1;
constexpr int32_t CHAE_BUFFER_MAX = 1024;
const std::string ERROR_MESSAGE_PARAMETER_ERROR = "Input parameter error";
const std::string ERROR_MESSAGE_NO_PERMISSION = "Permission denied";
const std::string ERROR_MESSAGE_NOT_SYSTEM_APP = "Not system App";
const std::string ERROR_MESSAGE_NO_RIGHT = "Do not have accessibility right for this operation";
const std::string ERROR_MESSAGE_SYSTEM_ABNORMALITY = "System abnormality";
const std::string ERROR_MESSAGE_PROPERTY_NOT_EXIST = "This property does not exist";
const std::string ERROR_MESSAGE_ACTION_NOT_SUPPORT = "This action is not supported";
const std::string ERROR_MESSAGE_INVALID_BUNDLE_NAME_OR_ABILITY_NAME = "Invalid bundle name or ability name";
const std::string ERROR_MESSAGE_TARGET_ABILITY_ALREADY_ENABLED = "Target ability already enabled";

enum class NAccessibilityErrorCode : int32_t {
    ACCESSIBILITY_OK = 0,
    ACCESSIBILITY_ERROR_NO_PERMISSION = 201,
    ACCESSIBILITY_ERROR_NOT_SYSTEM_APP = 202,
    ACCESSIBILITY_ERROR_INVALID_PARAM = 401,
    ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY = 9300000,
    ACCESSIBILITY_ERROR_ERROR_EXTENSION_NAME = 9300001,
    ACCESSIBILITY_ERROR_TARGET_ABILITY_ALREADY_ENABLED = 9300002,
    ACCESSIBILITY_ERROR_NO_RIGHT = 9300003,
    ACCESSIBILITY_ERROR_PROPERTY_NOT_EXIST = 9300004,
    ACCESSIBILITY_ERROR_ACTION_NOT_SUPPORT = 9300005,
};

struct NAccessibilityErrMsg {
    NAccessibilityErrorCode errCode;
    std::string message;
};

const std::map<OHOS::Accessibility::RetError, NAccessibilityErrMsg> ACCESSIBILITY_JS_TO_ERROR_CODE_MAP {
    {OHOS::Accessibility::RetError::RET_OK,
        {NAccessibilityErrorCode::ACCESSIBILITY_OK, ""}},
    {OHOS::Accessibility::RetError::RET_ERR_FAILED,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_INVALID_PARAM,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_INVALID_PARAM, ERROR_MESSAGE_PARAMETER_ERROR}},
    {OHOS::Accessibility::RetError::RET_ERR_NULLPTR,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_IPC_FAILED,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_SAMGR,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_PERMISSION,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_NO_PERMISSION, ERROR_MESSAGE_NO_PERMISSION}},
    {OHOS::Accessibility::RetError::RET_ERR_TIME_OUT,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_REGISTER_EXIST,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_REGISTER,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_CONNECTION_EXIST,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_TARGET_ABILITY_ALREADY_ENABLED,
        ERROR_MESSAGE_TARGET_ABILITY_ALREADY_ENABLED}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_CONNECTION,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_WINDOW_CONNECTION,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_CAPABILITY,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_NO_RIGHT, ERROR_MESSAGE_NO_RIGHT}},
    {OHOS::Accessibility::RetError::RET_ERR_INVALID_ELEMENT_INFO_FROM_ACE,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_PERFORM_ACTION_FAILED_BY_ACE,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NO_INJECTOR,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_SYSTEM_ABNORMALITY, ERROR_MESSAGE_SYSTEM_ABNORMALITY}},
    {OHOS::Accessibility::RetError::RET_ERR_NOT_INSTALLED,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_ERROR_EXTENSION_NAME,
        ERROR_MESSAGE_INVALID_BUNDLE_NAME_OR_ABILITY_NAME}},
    {OHOS::Accessibility::RetError::RET_ERR_NOT_ENABLED,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_ERROR_EXTENSION_NAME,
        ERROR_MESSAGE_INVALID_BUNDLE_NAME_OR_ABILITY_NAME}},
    {OHOS::Accessibility::RetError::RET_ERR_PROPERTY_NOT_EXIST,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_PROPERTY_NOT_EXIST, ERROR_MESSAGE_PROPERTY_NOT_EXIST}},
    {OHOS::Accessibility::RetError::RET_ERR_ACTION_NOT_SUPPORT,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_ACTION_NOT_SUPPORT, ERROR_MESSAGE_ACTION_NOT_SUPPORT}},
    {OHOS::Accessibility::RetError::RET_ERR_NOT_SYSTEM_APP,
        {NAccessibilityErrorCode::ACCESSIBILITY_ERROR_NOT_SYSTEM_APP, ERROR_MESSAGE_NOT_SYSTEM_APP}},
};

#endif // ACCESSIBILITY_DEFINE_H