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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ARGMIN_WITH_VALUE_CPU_KERNEL_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ARGMIN_WITH_VALUE_CPU_KERNEL_H_

#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "backend/kernel_compiler/cpu/cpu_kernel.h"
#include "backend/kernel_compiler/cpu/cpu_kernel_factory.h"

namespace luojianet_ms {
namespace kernel {
template <typename T>
class ArgMinWithValueCPUKernel : public CPUKernel {
 public:
  ArgMinWithValueCPUKernel() = default;
  ~ArgMinWithValueCPUKernel() override = default;

  void InitKernel(const CNodePtr &kernel_node) override;

  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &workspace,
              const std::vector<AddressPtr> &outputs) override;

 private:
  std::vector<size_t> shape_;
  size_t num_before_axis_{0};
  size_t num_after_axis_{0};
  size_t dim_axis_{0};
};

MS_REG_CPU_KERNEL_T(ArgMinWithValue, KernelAttr(), ArgMinWithValueCPUKernel, float);
MS_REG_CPU_KERNEL_T(ArgMinWithValue, KernelAttr(), ArgMinWithValueCPUKernel, float16);
}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_ARGMIN_WITH_VALUE_CPU_KERNEL_H_
