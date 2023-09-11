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

#ifndef ACCESSIBLE_ABILITY_CLIENT_H
#define ACCESSIBLE_ABILITY_CLIENT_H

#include <map>
#include <memory>
#include "accessibility_element_info.h"
#include "accessibility_event_info.h"
#include "accessibility_gesture_inject_path.h"
#include "accessibility_window_info.h"
#include "accessible_ability_listener.h"
#include "iremote_object.h"

namespace OHOS {
namespace Accessibility {
class AccessibleAbilityClient : public virtual RefBase {
public:
    /**
     * @brief Destruct
     */
    virtual ~AccessibleAbilityClient() = default;

    /**
     * @brief Gets an instance of AccessibleAbilityClient.
     * @return Return an instance of AccessibleAbilityClient.
     */
    static sptr<AccessibleAbilityClient> GetInstance();

    /**
     * @brief Gets remote object.
     * @return Remote object.
     */
    virtual sptr<IRemoteObject> GetRemoteObject() = 0;

    /**
     * @brief Obtains elementInfo of focus.
     * @param focusType The type of focus. It contains FOCUS_TYPE_INPUT and FOCUS_TYPE_ACCESSIBILITY.
     * @param elementInfo The accessibilityElementInfo of focus.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetFocus(const int32_t focusType, AccessibilityElementInfo &elementInfo) = 0;

    /**
     * @brief Obtains elementInfo of focus.
     * @param sourceInfo The source info to get focus.
     * @param focusType The type of focus. It contains FOCUS_TYPE_INPUT and FOCUS_TYPE_ACCESSIBILITY.
     * @param elementInfo The accessibilityElementInfo of focus.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetFocusByElementInfo(const AccessibilityElementInfo &sourceInfo, const int32_t focusType,
        AccessibilityElementInfo &elementInfo) = 0;

    /**
     * @brief Sends simulate gestures to the screen.
     * @param gesturePath The gesture which need to send.
     * @return Return RET_OK if the gesture sends successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError InjectGesture(const std::shared_ptr<AccessibilityGestureInjectPath> &gesturePath) = 0;

    /**
     * @brief Obtains elementInfo of the accessible root node.
     * @param elementInfo The elementInfo of the accessible root node.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetRoot(AccessibilityElementInfo &elementInfo) = 0;

    /**
     * @brief Obtains elementInfo of the accessible root node.
     * @param windowInfo The source window info to get root.
     * @param elementInfo The elementInfo of the accessible root node.
     * @return Return RET_OK if obtains elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetRootByWindow(const AccessibilityWindowInfo &windowInfo,
        AccessibilityElementInfo &elementInfo) = 0;

    /**
     * @brief Obtains elementInfos of the accessible root node in batchs.
     * @param elementInfos ElementInfos of the accessible root node and its recursive subnodes.
     * @return Return RET_OK if obtains elementInfos successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetRootBatch(std::vector<AccessibilityElementInfo>& elementInfos) = 0;

    /**
     * @brief Obtains elementInfos of the accessible root node in batchs.
     * @param windowInfo The source window info to get root.
     * @param elementInfos ElementInfos of the accessible root node and its recursive subnodes.
     * @return Return RET_OK if obtains elementInfos successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetRootByWindowBatch(const AccessibilityWindowInfo &windowInfo,
        std::vector<AccessibilityElementInfo>& elementInfos) = 0;

    /**
     * @brief Get the window information related with the event
     * @param windowId The window id.
     * @param windowInfo The window information.
     * @return Return RET_OK if obtains windowInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetWindow(const int32_t windowId, AccessibilityWindowInfo &windowInfo) = 0;

    /**
     * @brief Obtains the list of interactive windows on the device, in the layers they are visible to users.
     * @param windows The information of windows.
     * @return Return RET_OK if obtains windowInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetWindows(std::vector<AccessibilityWindowInfo> &windows) = 0;

    /**
     * @brief Obtains the list of interactive windows on the device, in the layers they are visible to users.
     * @param displayId the id of display
     * @param windows The information of windows.
     * @return Return RET_OK if obtains windowInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetWindows(const uint64_t displayId, std::vector<AccessibilityWindowInfo> &windows) = 0;

    /**
     * @brief Gets the next focused node in the specified direction of the currently focused node.
     * @param elementInfo The source info to get next info.
     * @param direction Indicates the direction to obtain the next focused node. Refer to FocusMoveDirection
     * @param nextElementInfo The info of next element.
     * @return Return RET_OK if gets next elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetNext(const AccessibilityElementInfo &elementInfo, const FocusMoveDirection direction,
        AccessibilityElementInfo &nextElementInfo) = 0;

    /**
     * @brief Get the child node information by child index.
     * @param index The index of the child.
     * @param parent The parent info to get child.
     * @param child The element info of child.
     * @return Return RET_OK if gets child elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetChildElementInfo(const int32_t index, const AccessibilityElementInfo &parent,
        AccessibilityElementInfo &child) = 0;

    /**
     * @brief Get the children node information
     * @param parent The parent info to get children.
     * @param children The element info of children.
     * @return Return RET_OK if gets children elementInfo successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetChildren(const AccessibilityElementInfo &parent,
        std::vector<AccessibilityElementInfo> &children) = 0;

    /**
     * @brief Searches for node information based on the specified content.
     * @param elementInfo The source info.
     * @param text specified content
     * @param elementInfos The element infos of specified content.
     * @return Return RET_OK if gets elementInfos successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetByContent(const AccessibilityElementInfo &elementInfo, const std::string &text,
        std::vector<AccessibilityElementInfo> &elementInfos) = 0;

    /**
     * @brief Get the node information related with the event
     * @param eventInfo The source info to get source.
     * @param elementInfo The element info of source.
     * @return Return RET_OK if gets elementInfos successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetSource(const AccessibilityEventInfo &eventInfo, AccessibilityElementInfo &elementInfo) = 0;

    /**
     * @brief Get Parent node information
     * @param child The child element info to get parent.
     * @param parent The parent element info.
     * @return Return RET_OK if gets info successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError GetParentElementInfo(const AccessibilityElementInfo &child, AccessibilityElementInfo &parent) = 0;

    /**
     * @brief Executes a specified action.
     * @param elementInfo The source info to execute action.
     * @param action: the action type
     * @param actionArguments: The parameter for action type. such as:
     *      action: ACCESSIBILITY_ACTION_NEXT_HTML_ITEM,
     *                  actionArguments(ACTION_ARGU_HTML_ELEMENT,HtmlItemType)
     *      action: ACCESSIBILITY_ACTION_PREVIOUS_HTML_ITEM,
     *                  actionArguments(ACTION_ARGU_HTML_ELEMENT,HtmlItemType)
     *      action: ACCESSIBILITY_ACTION_NEXT_TEXT,
     *                  actionArguments(ACTION_ARGU_MOVE_UNIT,MOVE_UNIT_XXX)
     *      action: ACCESSIBILITY_ACTION_PREVIOUS_TEXT,
     *                  actionArguments(ACTION_ARGU_MOVE_UNIT,MOVE_UNIT_XXX)
     *      action: ACCESSIBILITY_ACTION_SET_SELECTION,
     *                  actionArguments({ACTION_ARGU_SELECT_TEXT_START,"1"(start location)},
     *                                  {ACTION_ARGU_SELECT_TEXT_END,"10"(end location)})
     *      action: ACCESSIBILITY_ACTION_SET_TEXT,
     *                  actionArguments(ACTION_ARGU_SET_TEXT,"the text of setted")
     * @return Return RET_OK if performs action succeed, otherwise refer to the RetError for the failure.
     */
    virtual RetError ExecuteAction(const AccessibilityElementInfo &elementInfo, const ActionType action,
        const std::map<std::string, std::string> &actionArguments) = 0;

    /**
     * @brief Register ability listener.
     * @param listener The listener to add.
     * @return Return RET_OK if registers listener successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError RegisterAbilityListener(const std::shared_ptr<AccessibleAbilityListener> &listener) = 0;

    /**
     * @brief Set target bundle names.
     * @param targetBundleNames The target bundle name
     * @return Return RET_OK if sets target bundle names successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError SetTargetBundleName(const std::vector<std::string> &targetBundleNames) = 0;

    /**
     * @brief Set cache mode.
     *        The mode is used for functions: GetRoot, GetRootByWindow, GetChildElementInfo,
     *        GetChildren, GetSource, GetParentElementInfo.
     * @param cacheMode The cache mode. It includes:
     *             PREFETCH_PREDECESSORS: cache the parent node info also.
     *             PREFETCH_SIBLINGS: cache the sister/brothers node info also.
     *             PREFETCH_CHILDREN: cache the child node info also.
     *             otherwise: no cache.
     * @return Return RET_OK if sets cache mode successfully, otherwise refer to the RetError for the failure.
     */
    virtual RetError SetCacheMode(const int32_t cacheMode) = 0;
};
} // namespace Accessibility
} // namespace OHOS
#endif // ACCESSIBLE_ABILITY_CLIENT_H