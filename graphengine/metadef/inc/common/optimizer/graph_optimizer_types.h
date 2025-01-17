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

#ifndef INC_COMMON_OPTIMIZER_GRAPH_OPTIMIZER_TYPES_H_
#define INC_COMMON_OPTIMIZER_GRAPH_OPTIMIZER_TYPES_H_

#include <cstdint>
#include <string>

namespace ge {
enum OPTIMIZER_SCOPE {
  UNIT = 0,
  ENGINE,
};

struct GraphOptimizerAttribute {
  std::string engineName;
  OPTIMIZER_SCOPE scope;
};
}  // namespace ge

#endif  // INC_COMMON_OPTIMIZER_GRAPH_OPTIMIZER_TYPES_H_
