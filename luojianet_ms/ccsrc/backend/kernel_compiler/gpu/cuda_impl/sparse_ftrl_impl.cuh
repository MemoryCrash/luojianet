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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_SPARSE_FTRL_IMPL_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_SPARSE_FTRL_IMPL_H_
template <typename T, typename S>
void CalSparseApplyFtrl(const T *gradient, const S *indices, const int num_index, const size_t n_stride,
                        const float learning_rate, const float l1_regularization, const float l2_regularization,
                        const float learning_rate_power, const bool use_locking, T *variable, T *accumulation,
                        T *linear, cudaStream_t cuda_stream);

#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMP_SPARSE_FTRL_IMPL_H_