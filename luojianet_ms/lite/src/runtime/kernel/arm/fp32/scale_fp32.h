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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_SCALE_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_SCALE_H_

#include <vector>
#include "src/inner_kernel.h"
#include "nnacl/fp32/scale_fp32.h"

namespace luojianet_ms::kernel {

class ScaleCPUKernel : public InnerKernel {
 public:
  ScaleCPUKernel(OpParameter *parameter, const std::vector<lite::Tensor *> &inputs,
                 const std::vector<lite::Tensor *> &outputs, const lite::InnerContext *ctx)
      : InnerKernel(parameter, inputs, outputs, ctx) {
    scale_param_ = reinterpret_cast<ScaleParameter *>(op_parameter_);
  }
  ~ScaleCPUKernel() override;

  int Prepare() override;
  int ReSize() override;
  int Run() override;
  int CalculateParameter();
  virtual int InitScaleOffset();
  int Scale(int task_id);

 protected:
  ScaleParameter *scale_param_;

 private:
  float *input_ptr_ = nullptr;
  float *scale_ = nullptr;
  float *offset_ = nullptr;
  float *output_ptr_ = nullptr;
};
}  // namespace luojianet_ms::kernel

#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_KERNEL_ARM_FP32_SCALE_H_
