/**
 * Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
 * Copyright 2021, 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LUOJIANET_MS_CCSRC_DEBUG_RDR_STRING_RECORDER_H_
#define LUOJIANET_MS_CCSRC_DEBUG_RDR_STRING_RECORDER_H_

#include <string>
#include <memory>

#include "debug/rdr/base_recorder.h"
namespace luojianet_ms {
class StringRecorder : public BaseRecorder {
 public:
  StringRecorder() : BaseRecorder() {}
  StringRecorder(const std::string &module, const std::string &name, const std::string &data)
      : BaseRecorder(module, name), data_(data) {}
  ~StringRecorder() {}
  void Export() override;

 private:
  std::string data_;
};
using StringRecorderPtr = std::shared_ptr<StringRecorder>;
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_DEBUG_RDR_STRING_RECORDER_H_
