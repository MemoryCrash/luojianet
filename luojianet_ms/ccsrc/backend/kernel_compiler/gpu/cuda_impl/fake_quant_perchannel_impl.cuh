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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_FAKE_QUANT_PERCHANNEL_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_FAKE_QUANT_PERCHANNEL_H_

#include "runtime/device/gpu/cuda_common.h"

void CalNudgePerChannel(float *input_min, float *input_max, const float quant_min, const float quant_max,
                        float *nudge_min, float *nudge_max, float *scale, const int channel_num, const bool symmetric,
                        cudaStream_t cuda_stream);

void CalFakeQuantPerChannel(const float *input, float *output, const int total_num, const int channel_num,
                            const float *nudge_min, const float *nudge_max, const float *scale,
                            cudaStream_t cuda_stream);

void CalFakeQuantPerChannelGrad(const float *input, const float *gradient, float *output, const int total_num,
                                const int channel_num, const float *nudge_min, const float *nudge_max,
                                cudaStream_t cuda_stream);

#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_FAKE_QUANT_PERCHANNEL_H_
