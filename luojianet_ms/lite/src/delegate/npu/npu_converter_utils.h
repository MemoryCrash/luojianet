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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_DELEGATE_NPU_NPU_CONVERTER_UITLS_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_DELEGATE_NPU_NPU_CONVERTER_UITLS_H_
#include <string>
#include <memory>
#include <vector>
#ifdef ENABLE_ARM64
#include <arm_neon.h>
#endif
#include "schema/ops_generated.h"
#include "include/graph/tensor.h"
#include "include/graph/op/array_defs.h"
#include "include/api/types.h"
#include "include/api/data_type.h"

namespace luojianet_ms {
enum NCHW_SHAPE { NCHW_INVALID = -1, NCHW_N = 0, NCHW_C = 1, NCHW_H = 2, NCHW_W = 3 };
enum NHWC_SHAPE { NHWC_N = 0, NHWC_H = 1, NHWC_W = 2, NHWC_C = 3 };
inline const std::vector<int> NHWC2NCHW_PERM = {0, 3, 1, 2};
inline const std::vector<int> NCHW2NHWC_PERM = {0, 2, 3, 1};

enum NPU_ACTIVATION_MODE {
  ACTIVATION_INVALID = -1,
  SIGMOID = 0,
  RELU = 1,
  TANH = 2,
  CLIPPED_RELU = 3,
  ELU = 4,
  P_RELU = 5,
  ABS = 6,
  RELU1 = 7,
  SOFTSIGN = 8,
  SOFTPLUS = 9,
  HARD_SIGMOID = 10,
  THRESHOLD_RELU = 11,
  SELU = 12,
  LINEAR = 13,
  RELU6 = 14,
  GELU = 15,
};

enum PAD {
  PAD_UP = 0,
  PAD_DOWN = 1,
  PAD_LEFT = 2,
  PAD_RIGHT = 3,
};

enum NPU_PAD_MODE {
  PAD_VALID = 5,
  PAD_SAME = 6,
};

#ifdef ENABLE_ARM64
void Float32ToFloat16(const float *__restrict input, float16_t *__restrict output, int number);

void Float16ToFloat32(const float16_t *__restrict input, float *__restrict output, int number);
#endif

std::shared_ptr<ge::Tensor> ConverterToNPUTensor(luojianet_ms::MSTensor src, bool is_expand_4d = false);

hiai::op::Data *ConverterToNPUData(const luojianet_ms::MSTensor &src, const std::string &name);

ge::Format ConverterToNPUFormat(schema::Format format);

ge::DataType ConverterToNPUDataType(DataType type_id);

ge::Shape ConverterToNPUShape(const std::vector<int64_t> &src_shape, bool is_expand_4d = false);

int ConverterToNPUEltwiseMode(schema::EltwiseMode mode);

int ConverterToNPUActivationMode(schema::ActivationType type);

int TransFormAxis(int axis);

void AssistDataNHWC2NCHW(int *data, size_t unit_size);

int MaskDataNHWC2NCHW(int mask);
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_DELEGATE_NPU_NPU_CONVERTER_UITLS_H_
