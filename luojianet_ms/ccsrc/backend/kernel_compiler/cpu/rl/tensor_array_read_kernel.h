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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_RL_TENSOR_ARRAY_READ_KERNEL_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_RL_TENSOR_ARRAY_READ_KERNEL_H_

#include <vector>
#include <string>
#include "backend/kernel_compiler/cpu/cpu_kernel.h"
#include "backend/kernel_compiler/cpu/cpu_kernel_factory.h"

namespace luojianet_ms {
namespace kernel {
class TensorArrayCPUReadKernel : public CPUKernel {
 public:
  TensorArrayCPUReadKernel();
  ~TensorArrayCPUReadKernel() = default;

  const std::vector<size_t> &GetInputSizeList() const override;
  const std::vector<size_t> &GetOutputSizeList() const override;
  const std::vector<size_t> &GetWorkspaceSizeList() const override;
  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &workspace,
              const std::vector<AddressPtr> &outputs) override;
  void InitKernel(const CNodePtr &kernel_node) override;

 private:
  size_t value_size_;
  std::vector<int64_t> shapes_;
  TypePtr type_;

  std::vector<size_t> input_size_list_;
  std::vector<size_t> output_size_list_;
  std::vector<size_t> workspace_size_list_;
};
// index int64
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeInt64),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeInt32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeInt16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeUInt32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeUInt16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeUInt8),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeUInt64),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeFloat32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeFloat16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt64).AddOutputAttr(kNumberTypeBool),
  TensorArrayCPUReadKernel);
// index int32
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeInt64),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeInt32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeInt16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeUInt32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeUInt16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeUInt8),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeUInt64),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeFloat32),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeFloat16),
  TensorArrayCPUReadKernel);
MS_REG_CPU_KERNEL(
  TensorArrayRead,
  KernelAttr().AddInputAttr(kNumberTypeInt64).AddInputAttr(kNumberTypeInt32).AddOutputAttr(kNumberTypeBool),
  TensorArrayCPUReadKernel);
}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_CPU_RL_TENSOR_ARRAY_READ_KERNEL_H_
