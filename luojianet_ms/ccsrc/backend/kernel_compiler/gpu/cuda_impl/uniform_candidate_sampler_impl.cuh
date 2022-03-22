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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CUDA_IMPL_UNIFORM_CANDIDATE_SAMPLER_IMPL_CUH_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CUDA_IMPL_UNIFORM_CANDIDATE_SAMPLER_IMPL_CUH_
#include <cuda_runtime.h>
#include "runtime/device/gpu/cuda_common.h"

template <typename S>
void CalUniformCandidateSampler(const int64_t true_size, const int64_t num_sampled, const S prob_val,
                                S *true_expected_count, S *sampled_expected_count, cudaStream_t cuda_stream);

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_CUDA_IMPL_UNIFORM_CANDIDATE_SAMPLER_IMPL_CUH_