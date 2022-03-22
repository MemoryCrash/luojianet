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

#ifndef LUOJIANET_MS_CCSRC_VM_GRAPH_PARTITION_H_
#define LUOJIANET_MS_CCSRC_VM_GRAPH_PARTITION_H_
#include <vector>
#include <string>
#include <memory>
#include "ir/anf.h"
#include "ir/func_graph.h"
#include "ir/graph_utils.h"
#include "base/base_ref.h"

namespace luojianet_ms {
extern const char kMsVm[];
extern const char kGeVm[];
extern const char kMsConvert[];

namespace compile {
class GraphPartition {
 public:
  explicit GraphPartition(const std::vector<PrimitivePtr> &cut_list, const std::string &backend_name);
  ~GraphPartition() = default;
  std::vector<GraphSegmentPtr> Partition(const FuncGraphPtr &func_graph, bool *multi_target = nullptr);

 private:
  bool IsCut(const AnfNodePtr &node);
  std::vector<PrimitivePtr> cut_list_;
  std::string backend_name_;
};

using GraphPartitionPtr = std::shared_ptr<GraphPartition>;
}  // namespace compile
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_VM_GRAPH_PARTITION_H_