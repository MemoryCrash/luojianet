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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_BATCH_NORM_GRAD_SPLIT_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_BATCH_NORM_GRAD_SPLIT_H_

#include <vector>
#include "backend/optimizer/common/optimizer.h"
#include "backend/optimizer/common/helper.h"

namespace luojianet_ms {
namespace opt {
class BatchNormGradSplit : public PatternProcessPass {
 public:
  explicit BatchNormGradSplit(bool multigraph = true) : PatternProcessPass("batch_norm_grad_split", multigraph) {}
  ~BatchNormGradSplit() override = default;
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;

 private:
  void CreateOutputsOfUpdateGrad(const FuncGraphPtr &graph, const CNodePtr &bn_grad_node,
                                 std::vector<AnfNodePtr> *bn_update_grad_outputs) const;
  void CreateOutputsOfReduceGrad(const FuncGraphPtr &graph, const CNodePtr &bn_grad_node,
                                 const std::vector<AnfNodePtr> &bn_update_grad_outputs,
                                 std::vector<AnfNodePtr> *bn_reduce_grad_outputs) const;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_BATCH_NORM_GRAD_SPLIT_H_
