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
#include "backend/optimizer/ascend/ir_fission/splitv_fission.h"
#include "backend/session/anf_runtime_algorithm.h"

namespace luojianet_ms::opt {
const BaseRef SplitVFission::DefinePattern() const {
  VarPtr Xs = std::make_shared<SeqVar>();
  auto split_prim = std::make_shared<Primitive>(kSplitVOpName);
  return VectorRef({split_prim, Xs});
}

const AnfNodePtr SplitVFission::Process(const FuncGraphPtr &func_graph, const AnfNodePtr &node,
                                        const EquivPtr &) const {
  MS_EXCEPTION_IF_NULL(node);
  if (AnfAlgo::IsDynamicShape(node)) {
    return nullptr;
  }
  auto cnode = node->cast<CNodePtr>();
  MS_EXCEPTION_IF_NULL(cnode);
  // Check output num
  if (!AnfAlgo::HasNodeAttr(kAttrNumSplit, cnode)) {
    return nullptr;
  }
  auto num_split = AnfAlgo::GetNodeAttr<int64_t>(cnode, kAttrNumSplit);
  if (num_split <= outputs_divisor_) {
    return nullptr;
  }
  return DoFission(func_graph, cnode, num_split, outputs_divisor_, AnfAlgo::GetNodeAttr<int64_t>(cnode, kAttrSplitDim));
}
}  // namespace luojianet_ms::opt
