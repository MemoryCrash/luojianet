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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_APPLY_MOMENTUM_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_APPLY_MOMENTUM_H_

#include <vector>
#include "src/train/optimizer_kernel.h"
#include "nnacl/fp32_grad/optimizer.h"

namespace luojianet_ms::kernel {
class ApplyMomentumCPUKernel : public OptimizerKernel {
 public:
  explicit ApplyMomentumCPUKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                                  const std::vector<lite::Tensor *> &outputs, const lite::InnerContext *ctx)
      : OptimizerKernel(parameter, inputs, outputs, ctx, 2, 3),
        thread_count_(ctx->thread_num_),
        apply_momentum_param_(nullptr) {
    apply_momentum_param_ = reinterpret_cast<ApplyMomentumParameter *>(parameter);
  }
  ~ApplyMomentumCPUKernel() override {
    if (grad_sum_ != nullptr) {
      ms_context_->allocator->Free(grad_sum_);
      grad_sum_ = nullptr;
    }
  }
  int Prepare() override;
  int ReSize() override;
  int Execute(int task_id);
  int Run() override;
  int OptimizerStep() override;
  std::vector<int> GetOptimizerParamsIdxs() const override;

 private:
  int thread_count_;
  ApplyMomentumParameter *apply_momentum_param_;
};

}  // namespace luojianet_ms::kernel

#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_GRAD_APPLY_MOMENTUM_H_
