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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_STITCH_ATOMIC_CLEAN_GPU_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_STITCH_ATOMIC_CLEAN_GPU_H_

#include <memory>
#include <tuple>
#include <utility>
#include <vector>
#include "backend/optimizer/common/optimizer.h"
#include "backend/optimizer/graph_kernel/add_atomic_clean.h"
#include "backend/session/kernel_graph.h"

namespace luojianet_ms::graphkernel {
class StitchAtomicCleanInsertter : public AtomicCleanInsertter {
 public:
  StitchAtomicCleanInsertter() : AtomicCleanInsertter("stitch_atomic_clean") {}
  ~StitchAtomicCleanInsertter() override = default;
  bool Run(const FuncGraphPtr &func_graph) override;

 private:
  void CorrectKernelBuildInfo(const AnfNodePtr &composite_node,
                              const std::vector<std::pair<AtomicAddInfo, AnfNodePtr>> &clean_infos) override;
  CNodePtr CreateInplaceAssignNode(const FuncGraphPtr &sub_graph, const AnfNodePtr &new_parameter,
                                   const AtomicAddInfo &info) const;
  std::vector<std::pair<AnfNodePtr, int>> FindInnerCNodeUsers(const AnfNodePtr &inner_node,
                                                              const CNodePtr &target) const;
  void ProcessOriginCNode(
    const AnfNodePtr &composite_node,
    const std::vector<std::pair<AtomicAddInfo, AnfNodePtr>> &info_and_broadcast_to_nodes) override;
  std::pair<bool, AtomicAddInfo> IsStitchWithAtomic(const AnfNodePtr &anf_node);

  void AddDepend(const FuncGraphPtr &main_graph, const AnfNodePtr &clean_node, const AnfNodePtr &composite_node,
                 const AnfNodePtr &user_node, int index) const;

  AnfNodePtr stitch_node_{nullptr};
};
using StitchAtomicCleanInsertterPtr = std::shared_ptr<StitchAtomicCleanInsertter>;
}  // namespace luojianet_ms::graphkernel
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADD_STITCH_ATOMIC_CLEAN_GPU_H_