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

#ifndef LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_DENSE_GRAD_ACCUM_KERNEL_H_
#define LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_DENSE_GRAD_ACCUM_KERNEL_H_

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "backend/kernel_compiler/cpu/cpu_kernel.h"
#include "fl/server/kernel/aggregation_kernel.h"
#include "fl/server/kernel/aggregation_kernel_factory.h"

namespace luojianet_ms {
namespace fl {
namespace server {
namespace kernel {
constexpr size_t kDenseGradAccumKernelInputsNum = 2;
template <typename T>
class DenseGradAccumKernel : public AggregationKernel {
 public:
  DenseGradAccumKernel() = default;
  ~DenseGradAccumKernel() override = default;

  void InitKernel(const CNodePtr &) override { return; }

  bool Launch(const std::vector<AddressPtr> &, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &) override {
    return true;
  }

  void Reset() { accum_count_ = 0; }

  bool IsAggregationDone() { return accum_count_ >= done_count_; }

  void GenerateReuseKernelNodeInfo() override { return; }
};
}  // namespace kernel
}  // namespace server
}  // namespace fl
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_FL_SERVER_KERNEL_DENSE_GRAD_ACCUM_KERNEL_H_
