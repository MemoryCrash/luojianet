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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_TBE_KERNEL_SELECT_COMMON_UTILS_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_TBE_KERNEL_SELECT_COMMON_UTILS_H_
#include <string>
#include <vector>
#include "backend/session/anf_runtime_algorithm.h"

namespace luojianet_ms {
namespace kernel {
struct SupportFormat {
  std::vector<std::vector<std::string>> input_format;
  std::vector<std::vector<std::string>> output_format;
};
using SupportFormatItem = std::vector<std::string>;

class HostCheck {
 public:
  HostCheck() = default;
  ~HostCheck() = default;
  static bool CheckValidDeviceShape(const AnfNodePtr &node);

 private:
  static bool CheckValidOutputDeviceShape(const AnfNodePtr &node, const size_t output_idx, const std::string &format);
  static std::vector<int64_t> GetFinalInferShape(const AnfNodePtr &node, const size_t output_idx,
                                                 const std::string &format);
};

}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_TBE_COMMON_UTILS_H_
