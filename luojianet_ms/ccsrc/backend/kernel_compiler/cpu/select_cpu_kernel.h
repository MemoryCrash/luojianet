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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_SELECT_CPU_KERNEL_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_SELECT_CPU_KERNEL_H_

#include <vector>
#include "backend/kernel_compiler/cpu/cpu_kernel.h"
#include "backend/kernel_compiler/cpu/cpu_kernel_factory.h"

namespace luojianet_ms {
namespace kernel {
template <typename T>
class SelectCPUKernel : public CPUKernel {
 public:
  SelectCPUKernel() = default;
  ~SelectCPUKernel() override = default;

  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &outputs) override;

  void InitKernel(const CNodePtr &kernel_node) override;

 private:
  size_t element_num_{1};
};

MS_REG_CPU_KERNEL_T(Select,
                    KernelAttr()
                      .AddInputAttr(kNumberTypeBool)
                      .AddInputAttr(kNumberTypeFloat32)
                      .AddInputAttr(kNumberTypeFloat32)
                      .AddOutputAttr(kNumberTypeFloat32),
                    SelectCPUKernel, float);

MS_REG_CPU_KERNEL_T(Select,
                    KernelAttr()
                      .AddInputAttr(kNumberTypeBool)
                      .AddInputAttr(kNumberTypeFloat64)
                      .AddInputAttr(kNumberTypeFloat64)
                      .AddOutputAttr(kNumberTypeFloat64),
                    SelectCPUKernel, double);

MS_REG_CPU_KERNEL_T(Select,
                    KernelAttr()
                      .AddInputAttr(kNumberTypeBool)
                      .AddInputAttr(kNumberTypeFloat16)
                      .AddInputAttr(kNumberTypeFloat16)
                      .AddOutputAttr(kNumberTypeFloat16),
                    SelectCPUKernel, float16);

MS_REG_CPU_KERNEL_T(Select,
                    KernelAttr()
                      .AddInputAttr(kNumberTypeBool)
                      .AddInputAttr(kNumberTypeInt32)
                      .AddInputAttr(kNumberTypeInt32)
                      .AddOutputAttr(kNumberTypeInt32),
                    SelectCPUKernel, int);
}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_SELECT_CPU_KERNEL_H_