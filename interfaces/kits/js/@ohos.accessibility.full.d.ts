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

import { AsyncCallback, Callback } from 'basic';
import { AbilityInfo } from 'abilityinfo'
import { KeyEvent } from 'keyevent'
import { Rect } from './@componet/rect'
import { GridInfo, GridItemInfo } from "./@componet/gridInfo"
import { RangeInfo } from "./@componet/progress"
import { GesturePath, GesturePos } from "./@componet/gesturePath"
import { DisplayResizeInfo } from "./@componet/displayResizeInfo"
/**
 * Accessibility
 * @name Accessibility
 * @since 3
 * @sysCap Accessibility
 * @devices phone, tablet
 * @child NotSupport
 * @permission 权限列表
 * @note 标记说明
 */
declare namespace accessibility {
/************************************************************************************************
 * Type define
***********************************************************************************************/
  /**
   * The type of the Ability app.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type AbilityType = 'audible' | 'generic' | 'haptic' | 'spoken' | 'visual';

  /**
   * The action that the ability can execute.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type Action = 'accessibilityFocus' | 'clearAccessibilityFocus' | 'focus' | 'clearFocus' | 'clearSelection' |
    'click' | 'longClick' | 'cut' | 'copy' | 'paste' | 'select' | 'setText' | 'delete' |
    'scrollForward' | 'scrollBackward' |
    'setSelection' |
    'unfold' | 'fold' | 'nextText' | 'previousText' | 'nextHtmlItem' | 'previousHtmlItem';

  /**
   * The type of the accessibility event.
   * @note windowsChange:页面变化(迁移)的通知，旁白需要确定新画面的焦点【must】
   * @note windowContentChange:窗口中内容变更通知，旁白朗读屏幕变更的内容（在焦点控件/LiveRegion的情况下）【must】
   * @note windowStateChange:页面变化通知(比如迁移到新页面，弹出Alert画面)，用于朗读新页面的title信息，提示用户页面迁移。【must】
   * @note announcement:应用程序发布公告的事件，（A侧）会通知来电的电话号码信息，以及通知栏中的具体控件(如蓝牙快捷键)操作信息。【Must】
   * @note notificationChange：应用程序的通知，比如邮件、短信等通知消息，旁白需要朗读通知中的内容【must】
   * @note textTraversedAtMove:编辑情况下光标移动，会朗读光标移动期间的文本信息【must】
   * @since 3
   * @sysCap Accessibility
   */
  type EventType = 'accessibilityFocus' | 'accessibilityFocusClear' |
    'click' | 'longClick' | 'focus' | 'select' | 'hoverEnter' | 'hoverExit' |
    'textUpdate' | 'textSelectionUpdate' | 'scroll' | 'textMoveUnit' |
    'touchBegin' | 'touchEnd' | 'touchGuideBegin' | 'touchGuideEnd' |
    'touchGuideGestureBegin' | 'touchGuideGestureEnd' |
    'windowUpdate' | 'pageContentUpdate' | 'pageStateUpdate' |
    'publicNotice' | 'notificationUpdate';

  /**
   * The change type of the windowsChange event.
   * @note It's used when received the {@code windowsChange} event.
   * @since 3
   * @sysCap Accessibility
   */
  type WindowUpdateType = 'add' | 'remove' | 'title' | 'bounds' | 'layer' | 'active' |
    'focus' | 'accessibilityFocus' | 'parent' | 'children' | 'pip';

  /**
   * The type of the StateEvent.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type StateEventType = 'accessibility' | 'touchGuide';

  /**
   * The type of the window.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type WindowType = 'application' | 'inputMethod' | 'system' | 'accessibilityOverlay' | 'screenDivider';

  /**
   * The type of the focus.
   * @note accessibility: accessibility focus; input: input focus;
   * @since 3
   * @sysCap Accessibility
   */
  type FocusType = 'accessibility' | 'normal';

  /**
   * The type of the ability state.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type AbilityState = 'enable' | 'disable' | 'install';

  /**
   * The direction of the focus move.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type Direction = 'up' | 'down' | 'left' | 'right' | 'forward' | 'backward';

  /**
   * The ability that accessibility subsystem support.
   * @note touchExplorer: Describes the capability to talkback.
    magnification: Describes the capability to request to control the display magnification.
    gesturesSimulation: Describes the capability to request to simulate the gesture.
    windowContent: Describes the capability to search for the content of the active window.
    filterKeyEvents: Describes the capability to request to filter key events.
    fingerprintGesture: Describes the capability to request to fingerprint gesture.
   * @since 3
   * @sysCap Accessibility
   */
  type Capability = 'retrieve' | 'touchGuide' | 'keyEventObserver' | 'zoom' | 'gesture';

  /**
   * The global action that the ability can execute.
   * @note back: Indicates a global action of returning to the previous page.
    home: Indicates a global action of returning to the home page.
    recents: Indicates a global action of displaying a list of recent tasks.
    notifications: Indicates a global action of opening the notification panel.
    quickSettings: Indicates a global action of opening quick settings.
    powerDialog: Indicates a global action of pressing and holding the power button.
    toggleSplitScreen: Indicates a global action of switching between split screens.
    lockScreen: Indicates a global action of locking the screen.
    takeScreenShot: Indicates a global action of taking a screenshot.
   * @since 3
   * @sysCap Accessibility
   */
  type GlobalActionType = 'back' | 'home' | 'recent' | 'notification' | 'popUpPowerDialog' | 'divideScreen'
  | 'lockScreen' | 'captureScreen';

  /**
   * The types of swipe gestures which are performed on the touch screen.
   * And they are also the gesture's interception type.
   * @note -
   * @since 3
   * @sysCap Accessibility
   */
  type GestureType = 'left' | 'leftThenRight' | 'leftThenUp' | 'leftThenDown' |
    'right' | 'rightThenLeft' | 'rightThenUp' | 'rightThenDown' |
    'up' | 'upThenLeft' | 'upThenRight' | 'upThenDown' |
    'down' | 'downThenLeft' | 'downThenRight' | 'downThenUp';

  /**
    * The granularity of text move.
    * @note -
    * @since 3
    * @sysCap Accessibility
  */
  type TextMoveUnit = 'char' | 'word' | 'line' | 'page' | 'paragraph';

  /**
    * The argument type for operation.
    * @note -
    * @since 3
    * @sysCap Accessibility
  */
  type OperatorParamType = 'selectTextBegin' | 'selectTextEnd' | 'htmlItem' | 'setText' | 'textMoveUnit';

  /**
    * The category of the notification
    * @note 应用Notification的具体分类，用于朗读通知信息时提示是哪类通知.
    * @since 3
    * @sysCap Accessibility
  */
  type NotificationCategory = 'call' | 'msg' | 'email' | 'event' |
    'promo' | 'alarm' | 'progress' | 'social' | 'err' | 'transport' | 'sys' | 'service';

  /**
    * The html element type. The arguments of  nextHtmlElement/previousHtmlElement
    * @note html元素的具体分类，用于实现网页的按类别导航，如“跳到下一个链接或按钮”.
    * @since 3
    * @sysCap Accessibility
  */
  type HtmlItemType = 'link' | 'control' | 'graphic' | 'listItem' | 'list' | 'table' |
    'combox' | 'heading' | 'button' | 'checkBox' | 'landmark' | 'textField' | 'focusable' |
    'h1' | 'h2' | 'h3' | 'h4' | 'h5' | 'h6';

  /**
    * The key that the ability can intercept.
    * @note hide
    * @since 3
    * @sysCap Accessibility
    */
  type KeyInterceptCode = 'power' | 'volumeUp' | 'volumeDown';

  /**
    * The softkey boad show mode .
    * @note hide
    * @since 3
    * @sysCap Accessibility
    */
  type ShowModeKeyboard = 'auto' | 'hide' | 'ignoreHardKeyboard' | 'hardKeyboardOverridden';

  /**
    * The swipe type of fingerprint gesture.
    * @note hide
    * @since 3
    * @sysCap Accessibility
    */
  type FingerprintGestureType = 'up' | 'donw' | 'left' | 'right';



  /**
   * Checks whether accessibility ability is enabled.
   * @since 3
   * @sysCap Accessibility
   * @param -
   * @return Returns {@code true} if the accessibility is enabled; returns {@code false} otherwise.
  */
  function isOpenAccessibility(callback: AsyncCallback<boolean>): void;
  function isOpenAccessibility(): Promise<boolean>;

  /**
   * Checks touch browser ability (which is used by talkback) is enabled.
   * @since 3
   * @sysCap Accessibility
   * @param -
   * @return Returns {@code true} if the touch browser is enabled; returns {@code false} otherwise.
  */
  function isOpenTouchGuide(callback: AsyncCallback<boolean>): void;
  function isOpenTouchGuide(): Promise<boolean>;

  /**
   * Queries the list of accessibility abilities.
   * @since 3
   * @sysCap Accessibility
   * @param abilityType The type of the accessibility ability. {@code AbilityType} eg.spoken
   * @param stateType The state of the accessibility ability.  {@code AbilityState} eg.installed
   * @return Returns the list of abilityInfos.
  */
  function getAbilityLists(abilityType: AbilityType, stateType: AbilityState,
    callback: AsyncCallback<Array<AccessibilityAbilityInfo>>): void;
  function getAbilityLists(abilityType: AbilityType,
    stateType: AbilityState): Promise<Array<AccessibilityAbilityInfo>>;

  /**
   * Send accessibility Event.
   * @since 3
   * @sysCap Accessibility
   * @param event The object of the accessibility {@code EventInfo} .
   * @return Returns {@code true} if successed; returns {@code false} otherwise.
  */
  function sendEvent(event: EventInfo, callback: AsyncCallback<boolean>): void;
  function sendEvent(event: EventInfo): Promise<boolean>;

  /**
   * Register the observe of the state changed.
   * @since 3
   * @sysCap Accessibility
   * @param type state event type
   * @return Returns {@code true} if the register is successed; returns {@code false} otherwise.
  */
  function on(type: StateEventType, callback: Callback<StateEvent>): void;

  /**
   * Deregister the observe of the state changed.
   * @since 3
   * @sysCap Accessibility
   * @param type state event type
   * @return Returns {@code true} if the deregister is successed; returns {@code false} otherwise.
  */
  function off(type: StateEventType, callback?: Callback<boolean>): void;

  /**
   * Get the ablity client.
   * @since 3
   * @sysCap Accessibility
   * @return Returns the object of ability.
  */
  function getAccessibleAbility(callback: AsyncCallback<AccessibleAbility>): void;
  function getAccessibleAbility(): Promise<AccessibleAbility>;

  /**
   * Obtains the suggested interval for switching the UI.e.
   * @since 3
   * @note hide
   * @sysCap Accessibility
   * @param name ability name
   * @return Returns the object of ability.
  */
  function getSuggestedInterval(timeout: number, contentType: number, callback: AsyncCallback<number>): void;
  function getSuggestedInterval(timeout: number, contentType: number): Promise<number>;


  interface AccessibleAbility {
    /**
     * Connect to the accessibility subsystem.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return -.
    */
    connect(callback: AsyncCallback<boolean>): void;
    connect(): Promise<boolean>;

    /**
     * The controller for display resizing operations.
     * @default -
     * @note hide
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly displayResizeController: DisplayResizeController;

    /**
     * The controller for softkeyboard.
     * @default -
     * @note hide
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly softKeyBoardController: SoftKeyBoardController;
    /**
     * The controller for fingerPrint.
     * @default -
     * @note hide
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly fingerprintController: FingerprintController;

    /**
     * Obtains the accessibility node that gains the focus.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param focusType The type of focus. {@code FocusType}
     * @return -.
    */
    getFocusElementInfo(focusType: FocusType, callback: AsyncCallback<AccessibilityElementInfo>): void;
    getFocusElementInfo(focusType: FocusType): Promise<AccessibilityElementInfo>;

    /**
     * Obtains information about the accessibility root node.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return The root node info.
    */
    getRootElementInfo(callback: AsyncCallback<AccessibilityElementInfo>): void;
    getRootElementInfo(): Promise<AccessibilityElementInfo>;

    /**
     * Obtains the list of interactive windows on the device, in the layers they are visible to users.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return the list of interactive windows.
    */
    getWindows(callback: AsyncCallback<Array<WindowInfo>>): void;
    getWindows(): Promise<Array<WindowInfo>>;

    /**
     * Executes a specified action.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param action The action wanted to be excused.
     * @return The window info.
    */
    performCommonAction(action: GlobalActionType, callback: AsyncCallback<boolean>): void;
    performCommonAction(action: GlobalActionType): Promise<boolean>;

    /**
     * Disables the accessibility ability
     * @since 3
     * @note hide
     * @sysCap Accessibility
     * @param name ability name
     * @return Returns the object of ability.
    */
    disableAbility(callback: AsyncCallback<void>): void;
    disableAbility(): Promise<void>;

    /**
     * Called when an accessibility event occurs.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return -
    */
    on(type: 'accessibilityEvent', callback: Callback<EventInfo>): void;

    /**
     * Called when a user performs a specified gesture on the device that your accessibility application
     has requested to be in touch browser mode.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return -
    */
    on(type: 'gesture', callback: Callback<GestureType>): void;

    /**
     * Sends customized gestures to the screen.
     * @since 3
     * @note hide
     * @sysCap AccessibilityAbility
     * @param gesturePathDefineList The path of simulation gesture.
     * @param listener The result of simulation gesture.
     * @return -.
    */
    gestureInject(gesturePathDefineList: Array<GesturePath>, callback: AsyncCallback<boolean>): void;
    gestureInject(gesturePathDefineList: Array<GesturePath>): Promise<boolean>;

    /**
     * Disables the accessibility ability
     * @since 3
     * @note hide
     * @sysCap Accessibility
     * @param name ability name
     * @return Returns the object of ability.
    */
    disableAbility(callback: AsyncCallback<void>): void;
    disableAbility(): Promise<void>;

    /**
     * Called when a key event occurs, for example, a user presses the key to increase or decrease the volume.
     * @since 3
     * @note hide
     * @sysCap AccessibilityAbility
     * @param -
     * @return -
    */
    on(type: 'keyPress', callback: Callback<KeyEvent>): void;

    /**
     * Called when your accessibility service is successfully connected to the OS. You can implement
      this method to perform subsequent initialization operations.
     * @since 3
     * @note hide
     * @sysCap AccessibilityAbility
     * @param -
     * @return -
    */
    on(type: 'AbilityConnected' | 'interrupt', callback: Callback<void>): void;
  }

  interface AccessibilityAbilityInfo {
    /**
     * The ability id.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly id: string;

    /* The ability name.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly name: string;

    /* The bundle name of the ability.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly bundleName: string;

    /**
     * The ability info.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly abilityInfo: AbilityInfo;

    /**
     * The type of the ability.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly abilityTypes: Array<AbilityType>;

    /**
     * The capabilities of the ability.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly capabilities: Array<Capability>;

    /**
     * The desciption of the ability.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly description: string;

    /**
     * The events which the accesisibility ability wants to observe.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly eventTypes: Array<EventType>;

    /**
     * The applition names which the accessiblity ability wants to observe.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly filterBundleNames: Array<string>;
  }

  // AccessibilityEventInfo
  class EventInfo {
    constructor();
    /**
     * The type of an accessibility event.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    type: EventType;

    /**
     * The type of the window change event.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    windowUpdateType?: WindowUpdateType;

    /**
     * The bundle name of the target application.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    bundleName: string;

    /**
     * The type of the event source component,such as button, chart.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    componentType?: string;

    /**
    * The time stamp when send the event.
    * @default -
    * @since 3
    * @sysCap Accessibility
    */
    timeStamp?: number;

    /**
     * The window id of the event source.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    windowId?: number;

    /** The page id of the event source.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    pageId?: number;

    /**
     * The component Id associated with the accessibility event.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    componentId?: number;

    /**
     * The accessibility event description.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    description?: string;

    /**
     * The action that triggers the accessibility event, for example, clicking or focusing a view.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    triggerAction: Action;

    /**
     * The movement step used for reading texts.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    textMoveUnit?: TextMoveUnit;

    /**
     * The content list.
     * @note 旁白朗读Event信息时，会优先朗读description(通常由开发者定义)，如果无description信息，则朗读contents.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    contents?: Array<string>;

    /**
     * The content changed before.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    lastContent?: string;

    /**
     * The start index of listed items on the screen.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    beginIndex?: number;

    /**
     * The index of the current item on the screen.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    currentIndex?: number;

    /**
     * The end index of listed items on the screen.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    endIndex?: number;

    /**
     * The total of the items.
     * @note talkback used it when scroll.
     * @default 0
     * @since 3
     * @sysCap Accessibility
     */
    itemCount?: number;

    /**
     * The category type for notificationChanged event.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    categoryNotification?: NotificationCategory;

    /**
     * Get the nodeinfo who send the event.
     * @since 3
     * @sysCap Accessibility
     * @return AccessibilityElementInfo.
    */
    getSource(callback: AsyncCallback<AccessibilityElementInfo>): void;
    getSource(): Promise<AccessibilityElementInfo>;
  }
  // AccessibilityElementInfo
  interface AccessibilityElementInfo {
    /**
     * The id of the window which the node in.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly windowId: number;

    /**
     * The accessibility id of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly accessibilityId: number;

    /**
     * The id of the view which the node in.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly componentId: number;

    /**
     * The bundle name.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly bundleName: string;

    /**
     * The type of the event source component,such as button, chart.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly componentType: string;

    /**
     * The type of the input text.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly inputType: number;

    /**
     * The text of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly text: string;

    /**
     * The hint text of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly hintText: string;

    /**
     * The description of the node.
     * @node 朗读控件时用到的信息，通常是控件名称，比如“蓝牙”开关
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly description: string;

    /**
     * The resource name of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly resourceName: string;

    /**
     * The list of the children node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly childNodeIds: Array<number>;

    /**
     * The operation list of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly operations: Array<Operation>;

    /**
     * The max text length of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly textLengthLimit: number;

    /**
     * The rect of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly rect: Rect;

    /**
     * Whether the node can be check.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly checkable: boolean;

    /**
     * Whether the node is checked.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly checked: boolean;

    /**
     * Whether the node can be focused.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly focusable: boolean;

    /**
     * Whether the node is focused.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly focused: boolean;

    /**
     * Whether the node is visible.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isVisible: boolean;

    /**
     * Whether the node is accessibility focused.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly accessibilityFocused: boolean;

    /**
     * Whether the node is selected.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly selected: boolean;

    /**
     * Whether the node can be click.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly clickable: boolean;

    /**
     * Whether the node can be long click.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly longClickable: boolean;

    /**
     * Whether the node can be enable.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isEnable: boolean;

    /**
     * Whether the node is password.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isPassword: boolean;

    /**
     * Whether the node can be scrollable.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly scrollable: boolean;

    /**
     * Whether the node can be editable.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly editable: boolean;

    /**
     * Whether the node can popup.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly popupSupported: boolean;

    /**
     * Whether the node is multiline.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly pluralLineSupported: boolean;

    /**
     * Whether the node can be delete.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly deleteable: boolean;

    /**
     * Whether the node is displaying hint.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isHint: boolean;

    /**
     * Whether the node is important.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isEssential: boolean;

    /**
     * current index of children.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly currentIndex: number;

    /**
     * start index of children.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly startIndex: number;

    /**
     * end index of children.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly endIndex: number;

    /**
     * Range info of the progess node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly rangeInfo: RangeInfo;

    /**
     * It is used when the node is collection.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly grid: GridInfo;

    /**
     * collection item info.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly gridItem: GridItemInfo;

    /**
     * The live range of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly activeRegion: number;

    /**
     * Whether the content is invalid.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isContentInvalid: boolean;

    /**
     * error information.
     * @note 控件发生错误，如输入类型不合法，旁白会读出错误信息。
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly error: string;

    /**
     * The label of the node.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly label: number;

    /**
     * The start position of text selected.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly beginSelected: number;

    /**
     * The end position of text selected.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly endSelected: number;

    /**
     * Perform the specified action on the node.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param action: accessibility action.
     * @param args: Key: specify the argument type, such as: OperatorParamType
     *              Value: The value of arguments for Node
     * @return true: success, false: failed.
     */
    executeAction(action: Action, callback: AsyncCallback<boolean>, args?: object): void;
    executeAction(action: Action, args?: object): Promise<boolean>;

    /**
     * Find node list information through text.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param text: Text information to find.
     * @return the list of accessibleInfo.
     */
    getByContent(text: string, callback: AsyncCallback<Array<AccessibilityElementInfo>>): void;
    getByContent(text: string): Promise<Array<AccessibilityElementInfo>>;

    /**
     * Obtains information about the node that gains accessibility focus.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param focus: focus view.
     * @return Node information of focus view conforming to focus type.
     */
    get(focus: FocusType, callback: AsyncCallback<AccessibilityElementInfo>): void;
    get(focus: FocusType): Promise<AccessibilityElementInfo>;

    /**
     * The information of nodes near focus is retrieved according to the input direction.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param direction: Direction relative to the current node position.
     * @return Node information of focus view conforming to focus type.
     */
    next(direction: Direction, callback: AsyncCallback<AccessibilityElementInfo>): void;
    next(direction: Direction): Promise<AccessibilityElementInfo>;

    /**
     * Obtains information about the child node at a specified index.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param index: The child node information of the specified index of the current node.
     * @return Node information of focus view conforming to focus type.
     */
    getChild(index: number, callback: AsyncCallback<AccessibilityElementInfo>): void;
    getChild(index: number): Promise<AccessibilityElementInfo>;

    /**
     * Gets the parent node information of this node.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param none
     * @return Parent node information.
     */
    getParent(callback: AsyncCallback<AccessibilityElementInfo>): void;
    getParent(): Promise<AccessibilityElementInfo>;
  }

  // Defines an accessibility operation.
  interface Operation {
    /**
     * The type of the operation.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly type: Action;

    /**
     * The description of the operation.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly description: string;
  }

  interface StateEvent {
    /**
     * The type of the state event.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly eventType: StateEventType;

    /**
     * The state of the ability.
     * @default false
     * @since 3
     * @sysCap Accessibility
     */
    readonly state: boolean;

    /**
     * The description of the ability.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly description: string;
  }

  interface WindowInfo {
    /**
     * The rect of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly screenRect: Rect;

    /**
     * The id of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly id: number;

    /**
     * The layer of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly layer: number;

    /**
     * The title of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly title: string;

    /**
     * The type of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly type: WindowType;

    /**
     * The list of the children windows.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly childIds: Array<number>;

    /**
     * The parent of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly parentId: number;

    /**
     * The accessibility focus of the window.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isAccessibilityFocused: boolean;

    /**
     * The status of the window is active or not.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isActive: boolean;

    /**
     * The window gained focus or not.
     * @default -
     * @since 3
     * @sysCap Accessibility
     */
    readonly isFocused: boolean;

    /**
     * Obtains an anchor accessibility node that anchors this window to another window.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return An anchor accessibility node.
     */
    getAnchorElementInfo(callback: AsyncCallback<AccessibilityElementInfo>): void;
    getAnchorElementInfo(): Promise<AccessibilityElementInfo>;

    /**
     * Obtains an root accessibility in the active window.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return An anchor accessibility node.
     */
    getRootElementInfo(callback: AsyncCallback<AccessibilityElementInfo>): void;
    getRootElementInfo(): Promise<AccessibilityElementInfo>;

    /**
     * Obtains an parent AccessibilityWindowInfo instance.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param options The object of window
     * @return The instance of parent Accessibility WindowInfo.
     */
    getParent(callback: AsyncCallback<WindowInfo>): void;
    getParent(): Promise<WindowInfo>;

    /**
     * Obtains an child AccessibilityWindowInfo instance.
     * @since 3
     * @sysCap AccessibilityAbility
     * @param index The index of child window
     * @return The instance of Accessibility WindowInfo.
     */
    getChild(index: number, callback: AsyncCallback<WindowInfo>): void;
    getChild(index: number): Promise<WindowInfo>;
  }

  //@note hide
  interface DisplayResizeController {
    /**
     * The id of the display.
     * @default -
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly displayId: number;

    /**
     * Obtains the X coordinate of the center in this display resizing rectangle.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @return Returns the x coordinate of the resize center.
     */
    getCenterX(callback: AsyncCallback<number>): void;
    getCenterX(): Promise<number>;

    /**
     * Obtains the Y coordinate of the center in this display resizing rectangle.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @return Returns the y coordinate of the resize center.
     */
    getCenterY(callback: AsyncCallback<number>): void;
    getCenterY(): Promise<number>;

    /**
     * Obtains the display resizing rectangle.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @return Returns the rectangle of the display.
     */
    getDisplayResizeRect(callback: AsyncCallback<Rect>): void;
    getDisplayResizeRect(): Promise<Rect>;

    /**
     * Obtains the resizing scale of this display resizing rectangle.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @return Returns the scale of the resizing display.
     */
    getScale(callback: AsyncCallback<number>): void;
    getScale(): Promise<number>;

    /**
     * Sets the center coordinates for the display resizing rectangle.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @param centerX the x coordinate of the resize center
     * @param centerY the y coordinate of the resize center
     * @param isShouldAnimate set true if need animate
     * @return Returns true if the coordinate is successfully set; returns false otherwise.
     */
    setCenter(centerX: number, centerY: number, isShouldAnimate: boolean, callback: AsyncCallback<boolean>): void;
    setCenter(centerX: number, centerY: number, isShouldAnimate: boolean): Promise<boolean>;

    /**
     * Sets the display resizing scale.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @param scale the scale wanted to be set
     * @param isShouldAnimate set true if need animate
     * @return Returns true if the scale is successfully set; returns false otherwise.
     */
    setScale(scale: number, isShouldAnimate: boolean, callback: AsyncCallback<boolean>): void;
    setScale(scale: number, isShouldAnimate: boolean): Promise<boolean>;

    /**
     * Resets the display to its initial size.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @param isShouldAnimate set true if need animate
     * @return Returns true if the display is successfully reset; returns false otherwise.
     */
    reset(isShouldAnimate: boolean, callback: AsyncCallback<boolean>): void;
    reset(isShouldAnimate: boolean): Promise<boolean>;

    /**
     * Called when the rectangle, scale, or center coordinate for performing the resizing operations is changed.
     * @devices phone
     * @since 3
     * @sysCap AccessibilityAbility
     * @param -
     * @return -
     */
    on(type: 'displayResized', callback: Callback<DisplayResizeInfo>): void;
    off(type: 'displayResized', callback: Callback<boolean>): void;
  }

  //@note hide
  interface SoftKeyBoardController {
    /**
     * The show mode of the softkeyboard.
     * @default auto
     * @since 3
     * @sysCap AccessibilityAbility
     */
    readonly showMode: ShowModeKeyboard;

    /**
     * Called when the show/hide mode of the soft keyboard changes.
     * @devices phone
     * @since 3
     * @sysCap -
     * @param -
     * @return -
     */
    on(type: 'softKeyBoardShowModeChanged', callback: Callback<number>): void;
    off(type: 'softKeyBoardShowModeChanged', callback?: Callback<number>): void;
  }

  //@note hide
  interface FingerprintController {
    /**
     * Determine whether the gesture detection function of the fingerprint sensor is available.
     * @devices phone
     * @since 3
     * @sysCap -
     * @param -
     * @return Returns true if the fingerprint sensor's gesture detection function is available;
     * Otherwise it returns false(for example, if the fingerprint sensor is registering a gesture).
     */
    isFingerprintGestureDetectionValid(callback: AsyncCallback<boolean>): void;

    /**
     * Callback when the validity status of the fingerprint sensor's gesture detection changes.
     * @devices phone
     * @since 3
     * @sysCap -
     * @param -
     * @return -
     */
    on(type: 'fingerprintGestureStatusChanged', callback: Callback<boolean>): void;
    off(type: 'fingerprintGestureStatusChanged', callback?: Callback<boolean>): void;

    /**
     * Callback when the fingerprint sensor detects a gesture.
     * @devices phone
     * @since 3
     * @sysCap -
     * @param -
     * @return -
     */
    on(type: 'fingerprintGesture', callback: Callback<FingerprintGestureType>): void;
    off(type: 'fingerprintGesture', callback?: Callback<FingerprintGestureType>): void;
  }

}
export default accessibility;