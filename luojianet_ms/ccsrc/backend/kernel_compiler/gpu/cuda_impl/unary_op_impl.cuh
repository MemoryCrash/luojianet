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

#ifndef LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_UNARYOPIMPL_H_
#define LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_UNARYOPIMPL_H_

#include "runtime/device/gpu/cuda_common.h"
#include "utils/complex.h"
template <typename T>
void Exponential(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Expm1(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Logarithm(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Log1p(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Erf(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Erfc(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Negative(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Reciprocal(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Square(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Sqrt(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Rsqrt(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Sin(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Cos(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Asin(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void ACos(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Atan(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Asinh(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Acosh(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Abs(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Floor(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Rint(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Round(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Sign(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Real(const Complex<T> *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Real(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Imag(const Complex<T> *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Imag(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Conj(const Complex<T> *input, Complex<T> *output, const size_t count, cudaStream_t cuda_stream);
template <typename T>
void Conj(const T *input, T *output, const size_t count, cudaStream_t cuda_stream);
#endif  // LUOJIANET_MS_CCSRC_KERNEL_GPU_CUDA_IMPL_UNARYOPIMPL_H_
