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
#ifndef LUOJIANET_MS_LITE_INCLUDE_TRAIN_METRICS_H_
#define LUOJIANET_MS_LITE_INCLUDE_TRAIN_METRICS_H_
#include <vector>
#include <string>
#include <tuple>
#include <unordered_map>
#include "include/ms_tensor.h"

namespace luojianet_ms {
namespace session {

class Metrics {
 public:
  virtual ~Metrics() = default;
  virtual void Clear() {}
  virtual float Eval() { return 0.0; }
  virtual void Update(std::vector<tensor::MSTensor *> inputs, std::vector<tensor::MSTensor *> outputs) = 0;
};

}  // namespace session
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_LITE_INCLUDE_TRAIN_METRICS_H_
