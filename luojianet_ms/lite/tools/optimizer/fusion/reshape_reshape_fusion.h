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

#ifndef LUOJIANET_MS_LITE_SRC_PASS_FUSION_RESHAPE_RESHAPE_FUSION_H_
#define LUOJIANET_MS_LITE_SRC_PASS_FUSION_RESHAPE_RESHAPE_FUSION_H_

#include <string>
#include <memory>
#include "backend/optimizer/common/optimizer.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace opt {
class ReshapeReshapeFusion : public PatternProcessPass {
 public:
  explicit ReshapeReshapeFusion(bool multigraph = true, const std::string &name = "ReshapeReshapeFusion")
      : PatternProcessPass(name, multigraph) {}
  ~ReshapeReshapeFusion() override = default;

 private:
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;
  mutable VarPtr reshape_input_{nullptr};
  mutable VarPtr reshape_shape_{nullptr};
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_LITE_SRC_PASS_FUSION_RESHAPE_RESHAPE_FUSION_H_
