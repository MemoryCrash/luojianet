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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_ADAM_WEIGHT_DECAY_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_ADAM_WEIGHT_DECAY_H_
template <typename T>
void AdamWeightDecay(const int &element_num_, const bool &need_decay, const float *beta1, const float *one_sub_beta1,
                     const float *beta2, const float *one_sub_beta2, const float *epsilon, const float *lr,
                     const float *weight_decay, T *m, T *v, T *param, T *gradient, cudaStream_t stream);

#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_ADAM_WEIGHT_DECAY_H_
