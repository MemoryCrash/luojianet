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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ADAM_WEIGHT_DECAY_CPU_KERNEL_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ADAM_WEIGHT_DECAY_CPU_KERNEL_H_

#include <vector>
#include <memory>

#include "backend/kernel_compiler/cpu/cpu_kernel.h"
#include "backend/kernel_compiler/cpu/cpu_kernel_factory.h"

namespace luojianet_ms {
namespace kernel {
class AdamWeightDecayCPUKernel : public CPUKernel {
 public:
  AdamWeightDecayCPUKernel() = default;
  ~AdamWeightDecayCPUKernel() override = default;
  void InitKernel(const CNodePtr &kernel_node) override;
  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &outputs) override;

 private:
  template <typename T>
  void LaunchAdamWeightDecay(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &outputs);
  void LaunchAdamWeightDecayNnacl(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &outputs);

  TypeId dtype_{kTypeUnknown};
  enum input_list_ { VAR, M, V, LR, BETA1, BETA2, EPSILON, DECAY, GRAD };
};

MS_REG_CPU_KERNEL(AdamWeightDecay, KernelAttr(), AdamWeightDecayCPUKernel)
}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ADAM_WEIGHT_DECAY_CPU_KERNEL_H_