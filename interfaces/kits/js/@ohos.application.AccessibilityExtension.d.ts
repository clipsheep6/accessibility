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

import accessibility from "./@ohos.accessibility";
import { KeyEvent } from 'keyevent'

/**
 * class of accessibility extension.
 *
 * @since
 * @sysCap AccessibilityExtension
 */
export default class AccessibilityExtension {
    /**
     * Indicates accessibility extension context.
     *
     * @since
     * @sysCap AccessibilityExtension
     * @systemapi hide for inner use.
     * @StageModelOnly
     */
    context: accessibility.AccessibilityExtensionContext;

    /**
     * Called when your accessibility service is successfully connected to the OS. You can implement
      this method to perform subsequent initialization operations.
     * @since
     * @note hide
     * @sysCap AccessibilityExtension
     * @param -
     * @return -
     */
    onAbilityConnected(): void;

    /**
     * Called when an accessibility event occurs.
     * @since
     * @sysCap AccessibilityExtension
     * @param -
     * @return -
     */
    onAccessibilityEvent(accessibilityEvent: accessibility.EventInfo): void;

    /**
     * Called when a key event occurs, for example, a user presses the key to increase or decrease the volume.
     * @since
     * @sysCap AccessibilityExtension
     * @param -
     * @return -
     */
    onKeyPressEvent(keyEvent: KeyEvent): boolean;
}