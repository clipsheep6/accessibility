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

#include "element_name.h"

namespace OHOS {
namespace AppExecFwk {
void ElementName::SetElementDeviceID(ElementName *element, const char *deviceId)
{
    if (!element) {
        return;
    }
    element->SetDeviceID(deviceId);
}

void ElementName::SetElementBundleName(ElementName *element, const char *bundleName)
{
    if (!element) {
        return;
    }
    element->SetBundleName(bundleName);
}

void ElementName::SetElementAbilityName(ElementName *element, const char *abilityName)
{
    if (!element) {
        return;
    }
    element->SetAbilityName(abilityName);
}

void ElementName::SetElementModuleName(ElementName *element, const char *moduleName)
{
    if (!element) {
        return;
    }
    element->SetModuleName(moduleName);
}

void ElementName::ClearElement(ElementName *element)
{
    if (!element) {
        return;
    }
    element->SetDeviceID("");
    element->SetBundleName("");
    element->SetAbilityName("");
    element->SetModuleName("");
}

ElementName::ElementName(const std::string &deviceId, const std::string &bundleName,
    const std::string &abilityName, const std::string &moduleName)
    : deviceId_(deviceId), bundleName_(bundleName), abilityName_(abilityName), moduleName_(moduleName)
{
}

ElementName::ElementName()
{
}

ElementName::~ElementName()
{
}

std::string ElementName::GetURI() const
{
    return deviceId_ + "/" + bundleName_ + "/" + abilityName_;
}

std::string ElementName::GetElementNameURI() const
{
    return deviceId_ + "/" + bundleName_ + "/" + moduleName_ + "/" + abilityName_;
}

bool ElementName::operator==(const ElementName &element) const
{
    return (deviceId_ == element.GetDeviceID() && bundleName_ == element.GetBundleName() &&
        abilityName_ == element.GetAbilityName() && moduleName_ == element.GetModuleName());
}

bool ElementName::ReadFromParcel(Parcel &parcel)
{
    return true;
}

ElementName *ElementName::Unmarshalling(Parcel &parcel)
{
    ElementName *elementName = new (std::nothrow) ElementName();
    if (elementName && !elementName->ReadFromParcel(parcel)) {
        delete elementName;
        elementName = nullptr;
    }
    return elementName;
}

bool ElementName::Marshalling(Parcel &parcel) const
{
    return true;
}
} // namespace AppExecFwk
} // namespace OHOS
