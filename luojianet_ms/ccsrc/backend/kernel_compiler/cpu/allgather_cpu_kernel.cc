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

#include "backend/kernel_compiler/cpu/allgather_cpu_kernel.h"
#include "runtime/device/cpu/cpu_device_address.h"
#include "runtime/device/cpu/mpi/mpi_interface.h"
#include "utils/log_adapter.h"

namespace luojianet_ms {
namespace kernel {
namespace {
constexpr size_t kAllGatherInputsNum = 1;
constexpr size_t kAllGatherOutputsNum = 1;
constexpr auto kRanksGroup = "group";
}  // namespace

void AllGatherCPUKernel::InitKernel(const CNodePtr &kernel_node) {
  MS_EXCEPTION_IF_NULL(kernel_node);
  kernel_name_ = AnfAlgo::GetCNodeName(kernel_node);
  size_t input_num = AnfAlgo::GetInputTensorNum(kernel_node);
  CHECK_KERNEL_INPUTS_NUM(input_num, kAllGatherInputsNum, kernel_name_);
  ranks_group_ = AnfAlgo::GetNodeAttr<std::vector<int>>(kernel_node, kRanksGroup);
}

bool AllGatherCPUKernel::Launch(const std::vector<kernel::AddressPtr> &inputs, const std::vector<kernel::AddressPtr> &,
                                const std::vector<kernel::AddressPtr> &outputs) {
  CHECK_KERNEL_INPUTS_NUM(inputs.size(), kAllGatherInputsNum, kernel_name_);
  CHECK_KERNEL_OUTPUTS_NUM(outputs.size(), kAllGatherOutputsNum, kernel_name_);
  auto *input_addr = reinterpret_cast<float *>(inputs[0]->addr);
  auto *output_addr = reinterpret_cast<float *>(outputs[0]->addr);
  auto input_data_num = inputs[0]->size / sizeof(float);
  return MPIAllGather(input_addr, output_addr, ranks_group_, input_data_num);
}
}  // namespace kernel
}  // namespace luojianet_ms