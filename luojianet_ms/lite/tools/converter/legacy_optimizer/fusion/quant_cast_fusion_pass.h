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

#ifndef LUOJIANET_MS_PREDICT_QUANT_CAST_FUSION_PASS_H
#define LUOJIANET_MS_PREDICT_QUANT_CAST_FUSION_PASS_H

#include <memory>
#include <string>
#include <unordered_map>
#include "tools/converter/legacy_optimizer/fusion/fusion_pass.h"

namespace luojianet_ms {
namespace lite {
constexpr const char *kQuantCastSrcOp = "QuantCastSrcOp";
constexpr const char *kFormatTransOp = "FormatTransOp";
constexpr const char *kQuantCastDstOp = "QuantCastDstOp";

constexpr const char *kQuantCastFusionPattern = "QuantCastFusionPattern";
constexpr const char *kQuantCastPassFusionPattern = "QuantCastPassFusionPattern";

class QuantCastFusionPass : public FusionPass {
 public:
  QuantCastFusionPass() = default;

  ~QuantCastFusionPass() override = default;

  STATUS Run(schema::MetaGraphT *graph) override;

 private:
  STATUS DefinePattern() override;

  STATUS DoFusion(schema::MetaGraphT *graph, const std::string &patternName,
                  std::unordered_map<std::string, std::shared_ptr<Path>> &matchedPath) override;
};
}  // namespace lite
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_PREDICT_QUANT_CAST_FUSION_PASS_H