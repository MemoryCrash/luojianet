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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_CAST_MATMUL_FUSION_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_CAST_MATMUL_FUSION_H_

#include <map>
#include <memory>

#include "backend/optimizer/common/pass.h"
#include "ir/func_graph.h"

namespace luojianet_ms::graphkernel {
class CastMatmulFusion : public opt::Pass {
 public:
  CastMatmulFusion() : Pass("cast_matmul_fusion") {}
  ~CastMatmulFusion() override = default;
  bool Run(const FuncGraphPtr &graph) override;
};
using OptimizeMatmulPtr = std::shared_ptr<CastMatmulFusion>;
}  // namespace luojianet_ms::graphkernel
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_CAST_MATMUL_FUSION_H_
