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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_CLIP_BY_NORM_NO_DIV_SQUARE_SUM_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_CLIP_BY_NORM_NO_DIV_SQUARE_SUM_H_

#include <vector>
#include <memory>
#include "backend/optimizer/ascend/ascend_pass_control.h"

namespace luojianet_ms {
namespace opt {
constexpr auto kInputVarName = "input";
constexpr auto kConstantSelectVarName = "constant_select";
constexpr auto kConstantGreaterVarName = "constant_greater";
constexpr auto kConstantMaximumVarName = "constant_maximum";

class ClipByNormNoDivSquareSumFusion : public PatternProcessPassWithSwitch {
 public:
  explicit ClipByNormNoDivSquareSumFusion(bool multigraph = true)
      : PatternProcessPassWithSwitch("clip_by_norm_no_div_square_sum_fusion", multigraph) {
    input_ = std::make_shared<Var>(kInputVarName);
    constant_select_ = std::make_shared<Var>(kConstantSelectVarName);
    constant_greater_ = std::make_shared<Var>(kConstantGreaterVarName);
    constant_maximum_ = std::make_shared<Var>(kConstantMaximumVarName);
    PassSwitchManager::GetInstance().RegistLicPass(name(), OptPassEnum::ClipByNormNoDivSquareSumFusion);
  }
  ~ClipByNormNoDivSquareSumFusion() override = default;
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;

 private:
  VarPtr input_;
  VarPtr constant_select_;
  VarPtr constant_greater_;
  VarPtr constant_maximum_;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_CLIP_BY_NORM_NO_DIV_SQUARE_SUM_H_
