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

#ifndef LUOJIANET_MS_CCSRC_PIPELINE_PYNATIVE_PYNATIVE_EXECUTE_GE_H_
#define LUOJIANET_MS_CCSRC_PIPELINE_PYNATIVE_PYNATIVE_EXECUTE_GE_H_

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "utils/hash_map.h"
#include "pipeline/pynative/base.h"
#include "transform/graph_ir/convert.h"
#include "transform/graph_ir/graph_runner.h"
#include "transform/graph_ir/types.h"
#include "utils/ms_context.h"

using GeTensor = ge::Tensor;
using GeTensorPtr = std::shared_ptr<GeTensor>;
using GeGraph = ge::Graph;
using GeGraphPtr = std::shared_ptr<GeGraph>;

namespace luojianet_ms {
namespace pynative {
bool BuildSingleOpGraph(const OpExecInfoPtr &op_exec_info, const std::vector<GeTensorPtr> &inputs,
                        const luojianet_ms::HashMap<std::string, ValuePtr> &attrs, const GeGraphPtr &graph);

py::object RunOpInGE(const OpExecInfoPtr &op_exec_info, PynativeStatusCode *status);
}  // namespace pynative
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_PIPELINE_PYNATIVE_PYNATIVE_EXECUTE_GE_H_
