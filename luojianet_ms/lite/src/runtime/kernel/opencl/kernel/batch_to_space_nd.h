/**
 * Copyright 2019 Huawei Technologies Co., Ltd
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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_BATCH_TO_SPACE_ND_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_OPENCL_KERNEL_BATCH_TO_SPACE_ND_H_

#include <vector>
#include "src/runtime/kernel/opencl/opencl_kernel.h"
#include "nnacl/batch_to_space.h"

namespace luojianet_ms::kernel {
class BatchToSpaceNDOpenCLKernel : public OpenCLKernel {
 public:
  using OpenCLKernel::OpenCLKernel;

  ~BatchToSpaceNDOpenCLKernel() override = default;

  int Run() override;
  int Prepare() override;

  int CheckSpecs() override;
  int SetConstArgs() override;
  void SetGlobalLocal() override;
  int Tune() override { return lite::RET_OK; }

 private:
};
}  // namespace luojianet_ms::kernel
#endif
