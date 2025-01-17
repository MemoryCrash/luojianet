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
#ifndef LUOJIANET_INCLUDE_API_CALLBACK_TRAIN_ACCURACY_H
#define LUOJIANET_INCLUDE_API_CALLBACK_TRAIN_ACCURACY_H

#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "include/api/callback/callback.h"
#include "include/api/metrics/accuracy.h"

using GraphPoint = std::pair<int, float>;

namespace luojianet_ms {

class TrainAccuracy: public TrainCallBack {
 public:
  explicit TrainAccuracy(int print_every_n = INT_MAX,
                         int accuracy_metrics = METRICS_CLASSIFICATION,
                         const std::vector<int> &input_indexes = {1},
                         const std::vector<int> &output_indexes = {0});
  virtual ~TrainAccuracy();
  const std::vector<GraphPoint> &GetAccuracyPoints();
};
}  // namespace luojianet_ms
#endif  // LUOJIANET_INCLUDE_API_CALLBACK_TRAIN_ACCURACY_H
