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
#include "backend/optimizer/ascend/buffer_fusion/bnupdate_eltwise_eltwise_fusion_pass.h"
#include "utils/hash_set.h"
#include "backend/kernel_compiler/kernel_fusion.h"
#include "debug/anf_ir_dump.h"
#include "backend/session/anf_runtime_algorithm.h"
#include "base/core_ops.h"
#include "utils/ms_context.h"
#include "backend/optimizer/common/fusion_id_allocator.h"
#include "backend/optimizer/common/helper.h"

namespace luojianet_ms {
namespace opt {
void BnupdateEltwiseEltwiseFusionPass::MatchBnupdateAddRelu(const CNodePtr &cnode, const AnfNodePtr &relu_input,
                                                            const session::KernelGraph &kernel_graph,
                                                            FusedNodeRecord *candidate_fusion) {
  MS_EXCEPTION_IF_NULL(cnode);
  MS_EXCEPTION_IF_NULL(candidate_fusion);
  MS_EXCEPTION_IF_NULL(relu_input);
  auto add = relu_input->cast<CNodePtr>();
  MS_EXCEPTION_IF_NULL(add);
  auto tuple_getitem = add->input(kIndex1);
  MS_EXCEPTION_IF_NULL(tuple_getitem);
  if (tuple_getitem->isa<CNode>() && AnfAlgo::GetCNodeName(tuple_getitem) == prim::kPrimTupleGetItem->name()) {
    auto getitem = tuple_getitem->cast<CNodePtr>();
    MS_EXCEPTION_IF_NULL(getitem);
    auto bnupdate = getitem->input(kRealInputNodeIndexInTupleGetItem);
    MS_EXCEPTION_IF_NULL(bnupdate);
    if (bnupdate->isa<CNode>() && AnfAlgo::GetCNodeName(bnupdate) == kBNTrainingUpdateOpName &&
        GetNodeOutputTotalUsedNum(kernel_graph, bnupdate) == kBNTrainingUpdateOutputUsedTotalNum) {
      luojianet_ms::HashSet<AnfNodePtr> record{cnode, relu_input, bnupdate};
      candidate_fusion->push_back(record);
      SetRecordFusionId(record);
    }
  }
}

void BnupdateEltwiseEltwiseFusionPass::MatchSingleFusionPattern(const session::KernelGraph &kernel_graph,
                                                                FusedNodeRecord *candidate_fusion) {
  MS_EXCEPTION_IF_NULL(candidate_fusion);
  const auto &node_list = TopoSort(kernel_graph.get_return());
  for (auto &node : node_list) {
    if (!AnfUtils::IsRealCNodeKernel(node) || fusion_id_allocator->HasFusionIdAttr(node) ||
        AnfAlgo::CheckPrimitiveType(node, prim::kPrimReturn)) {
      continue;
    }
    auto cnode = node->cast<CNodePtr>();
    MS_EXCEPTION_IF_NULL(cnode);
    if (AnfAlgo::GetKernelType(cnode) == KernelType::TBE_KERNEL &&
        AnfAlgo::GetFusionType(cnode) == kernel::FusionType::ELEMWISE &&
        AnfAlgo::GetOutputTensorNum(cnode) == ELTWISE_DOUBLE_OUTPUT_SIZE) {
      auto eltwise_input = cnode->input(kIndex1);
      MS_EXCEPTION_IF_NULL(eltwise_input);
      if (eltwise_input->isa<CNode>() && AnfAlgo::CheckPrimitiveType(eltwise_input, prim::kPrimAdd)) {
        MatchBnupdateAddRelu(cnode, eltwise_input, kernel_graph, candidate_fusion);
      }
    }
  }
}
}  // namespace opt
}  // namespace luojianet_ms