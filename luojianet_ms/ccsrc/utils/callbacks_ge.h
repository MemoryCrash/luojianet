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
#ifndef LUOJIANET_MS_CCSRC_UTILS_CALLBACKS_GE_H_
#define LUOJIANET_MS_CCSRC_UTILS_CALLBACKS_GE_H_

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "transform/graph_ir/types.h"
#include "transform/graph_ir/util.h"
#include "ir/tensor.h"

namespace luojianet_ms {
namespace callbacks {
using luojianet_ms::tensor::TensorPtr;

uint32_t CheckpointSaveCallback(uint32_t, const std::map<std::string, ge::Tensor> &);
uint32_t SummarySaveCallback(uint32_t, const std::map<std::string, ge::Tensor> &);
}  // namespace callbacks
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_UTILS_CALLBACKS_GE_H_
