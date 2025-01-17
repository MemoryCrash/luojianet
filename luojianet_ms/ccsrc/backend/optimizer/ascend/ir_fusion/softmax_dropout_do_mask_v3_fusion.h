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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SOFTMAX_DROPOUT_DO_MASK_V3_FUSION_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SOFTMAX_DROPOUT_DO_MASK_V3_FUSION_H_

#include <memory>
#include "backend/optimizer/common/optimizer.h"

namespace luojianet_ms {
namespace opt {
class SoftmaxDropoutDoMaskV3Fusion : public PatternProcessPass {
 public:
  explicit SoftmaxDropoutDoMaskV3Fusion(bool multigraph = true)
      : PatternProcessPass("softmax_dropout_do_mask_v3_fusion", multigraph) {}
  ~SoftmaxDropoutDoMaskV3Fusion() override = default;
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FUSION_SOFTMAX_DROPOUT_DO_MASK_V3_FUSION_H_
