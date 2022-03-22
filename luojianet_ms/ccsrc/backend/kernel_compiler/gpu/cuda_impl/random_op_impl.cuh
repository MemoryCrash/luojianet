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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_RANDOMOPIMPL_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_RANDOMOPIMPL_H_

#include <curand_kernel.h>
#include <random>
#include "runtime/device/gpu/cuda_common.h"

template <typename T>
void StandardNormal(int seed, int seed2, curandState *globalState,
                    T *output, size_t count, cudaStream_t cuda_stream);
template <typename T>
bool UniformInt(int seed, int seed2, curandState *globalState,
                T *input1, size_t input_size_1, T *input2, size_t input_size_2,
                T *output, size_t count, cudaStream_t cuda_stream);
template <typename T>
void UniformReal(int seed, int seed2, curandState *globalState,
                 T *output, size_t count, cudaStream_t cuda_stream);
#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_RANDOMOPIMPL_H_