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
#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_PARTIAL_FUSION_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_PARTIAL_FUSION_H_

#include <vector>
#include "src/tensor.h"
#include "src/lite_kernel.h"

// this file is going to be removed when move create actor before schedule.
namespace luojianet_ms::kernel {
class PartialFusionKernel : public InnerKernel {
 public:
  PartialFusionKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                      const std::vector<lite::Tensor *> &outputs, const lite::InnerContext *ctx)
      : InnerKernel(parameter, inputs, outputs, ctx) {}
  ~PartialFusionKernel() override = default;
  int Prepare() override;
  int ReSize() override;
  int Run() override;
  void set_subgraph_kernels(const std::vector<LiteKernel *> &subgraph_kernels) { subgraph_kernels_ = subgraph_kernels; }
  std::vector<LiteKernel *> subgraph_kernels() const { return subgraph_kernels_; }

 private:
  // One partial corresponds to a subgraph at offline stage, after graph schedule, a subgraph may be split into many
  // graphs, so use a vector.
  std::vector<LiteKernel *> subgraph_kernels_{};
};
}  // namespace luojianet_ms::kernel

#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_BASE_PARTIAL_FUSION_H_
