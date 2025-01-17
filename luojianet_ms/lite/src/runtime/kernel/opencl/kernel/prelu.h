/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_PRELU_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_PRELU_H_

#include <vector>
#include <string>
#include "src/tensor.h"
#include "src/runtime/kernel/opencl/opencl_kernel.h"
#include "schema/model_generated.h"

namespace luojianet_ms::kernel {
class PReluOpenCLKernel : public OpenCLKernel {
 public:
  using OpenCLKernel::OpenCLKernel;
  ~PReluOpenCLKernel() override = default;

  int Prepare() override;
  int CheckSpecs() override;
  int SetConstArgs() override;
  void SetGlobalLocal() override;
  int Run() override;
  int InitWeights() override;

 private:
  bool enable_fp16_{false};
  uint32_t OH = {1};
  uint32_t OW = {1};
  uint32_t OC = {1};
  cl_int4 weight_shape_{};
  cl_int4 out_shape_{};
  void *weight_vector_{nullptr};
  float weight_scalar_{0.f};
  bool weight_is_scalar{false};
};

}  // namespace luojianet_ms::kernel

#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_PRELU_H_
