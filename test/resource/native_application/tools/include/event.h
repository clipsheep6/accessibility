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

#ifndef AA_TEST_EVENT_H
#define AA_TEST_EVENT_H

#include <vector>

namespace STtools {
class Event {
public:
    Event();
    ~Event();
    bool Compare();
    int WaitingMessage(const std::string &message, int timeout_ms, bool locked);
    void CompleteMessage(const std::string &message);
    void Clean();

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::string waiting_message_;
    std::vector<std::string> complete_message_;
};

int WaitCompleted(Event &event, const std::string &eventName, const int code, const int timeout = 60);
void Completed(Event &event, const std::string &eventName, const int code);
void CleanMsg(Event &event);
} // namespace STtools
#endif // AA_TEST_EVENT_H