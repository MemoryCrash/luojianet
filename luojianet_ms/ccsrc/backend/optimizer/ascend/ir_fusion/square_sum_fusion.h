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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SQUARE_SUM_FUSION_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SQUARE_SUM_FUSION_H_

#include "backend/optimizer/ascend/ascend_pass_control.h"

namespace luojianet_ms {
namespace opt {
class SquareSumFusion : public PatternProcessPassWithSwitch {
 public:
  explicit SquareSumFusion(bool multigraph = true) : PatternProcessPassWithSwitch("square_sum_fusion", multigraph) {
    PassSwitchManager::GetInstance().RegistLicPass(name(), OptPassEnum::SquareSumFusion);
  }
  ~SquareSumFusion() override = default;
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;

 private:
  CNodePtr GenerateSquareSumV1(const FuncGraphPtr &graph, const CNodePtr &square, const CNodePtr &sum) const;
  CNodePtr GenerateSquareSumV2(const FuncGraphPtr &graph, const CNodePtr &square, const CNodePtr &sum) const;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SQUARE_SUM_FUSION_H_
