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

#ifndef LUOJIANET_MS_PREDICT_TOPOLOGICAL_SORT_PASS_H
#define LUOJIANET_MS_PREDICT_TOPOLOGICAL_SORT_PASS_H

#include <memory>
#include <vector>
#include "luojianet_ms/lite/tools/converter/optimizer.h"
#include "tools/common/graph_util.h"

namespace luojianet_ms {
namespace lite {
class TopologicalSortPass : public GraphPass {
 public:
  TopologicalSortPass() = default;

  ~TopologicalSortPass() override = default;

  STATUS Run(schema::MetaGraphT *graph) override;

 private:
  bool IsNodeNonDepend(const std::unique_ptr<schema::CNodeT> &node, const std::vector<size_t> &sinkedTensorIdxes);
};
}  // namespace lite
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_PREDICT_TOPOLOGICAL_SORT_PASS_H