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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_CLIP_GRAD_NORM_IMPL_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_CLIP_GRAD_NORM_IMPL_H_

#include "runtime/device/gpu/cuda_common.h"
template <typename T>
void ScalingGradOp(const size_t size, const T *x, const float *scaling_factor, float *scaling_out_addr,
                   cudaStream_t cuda_stream);

template <typename T>
void ClipGradNormOp(const size_t size, const float *x, const T *clip_norm, const float *reduce_sum_value,
                    float *output_addr, cudaStream_t cuda_stream);

#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_CLIP_GRAD_NORM_IMPL_H_
