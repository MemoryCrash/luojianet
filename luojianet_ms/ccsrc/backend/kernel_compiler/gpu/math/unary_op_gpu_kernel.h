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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_MATH_UNARYOP_GPU_KERNEL_H_
#define LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_MATH_UNARYOP_GPU_KERNEL_H_

#include <cuda_runtime_api.h>
#include <vector>
#include <string>
#include <map>
#include "backend/kernel_compiler/gpu/gpu_kernel.h"
#include "backend/kernel_compiler/gpu/gpu_kernel_factory.h"
#include "backend/kernel_compiler/gpu/cuda_impl/unary_op_impl.cuh"

namespace luojianet_ms {
namespace kernel {
enum UnaryOptype {
  UNARY_OP_EXP = 0,
  UNARY_OP_EXPM1,
  UNARY_OP_LOG,
  UNARY_OP_LOG1P,
  UNARY_OP_ERF,
  UNARY_OP_ERFC,
  UNARY_OP_NEG,
  UNARY_OP_RECIPROCAL,
  UNARY_OP_SQUARE,
  UNARY_OP_SQRT,
  UNARY_OP_RSQRT,
  UNARY_OP_SIN,
  UNARY_OP_COS,
  UNARY_OP_ASIN,
  UNARY_OP_ACOS,
  UNARY_OP_ATAN,
  UNARY_OP_ASINH,
  UNARY_OP_ACOSH,
  UNARY_OP_ABS,
  UNARY_OP_FLOOR,
  UNARY_OP_RINT,
  UNARY_OP_ROUND,
  UNARY_OP_SIGN,
  UNARY_OP_REAL,
  UNARY_OP_IMAG,
  UNARY_OP_CONJ,
  UNARY_OP_INVALID_TYPE = 255
};

static const std::map<std::string, UnaryOptype> kUnaryOpTypeMap = {
  {"Exp", UNARY_OP_EXP},       {"Expm1", UNARY_OP_EXPM1},
  {"Log", UNARY_OP_LOG},       {"Log1p", UNARY_OP_LOG1P},
  {"Erf", UNARY_OP_ERF},       {"Erfc", UNARY_OP_ERFC},
  {"Neg", UNARY_OP_NEG},       {"Reciprocal", UNARY_OP_RECIPROCAL},
  {"Square", UNARY_OP_SQUARE}, {"Sqrt", UNARY_OP_SQRT},
  {"Rsqrt", UNARY_OP_RSQRT},   {"Sin", UNARY_OP_SIN},
  {"Cos", UNARY_OP_COS},       {"Asin", UNARY_OP_ASIN},
  {"ACos", UNARY_OP_ACOS},     {"Atan", UNARY_OP_ATAN},
  {"Asinh", UNARY_OP_ASINH},   {"Acosh", UNARY_OP_ACOSH},
  {"Abs", UNARY_OP_ABS},       {"Floor", UNARY_OP_FLOOR},
  {"Rint", UNARY_OP_RINT},     {"Round", UNARY_OP_ROUND},
  {"Real", UNARY_OP_REAL},     {"Imag", UNARY_OP_IMAG},
  {"Sign", UNARY_OP_SIGN},     {"Conj", UNARY_OP_CONJ}};

template <typename T>
class UnaryOpGpuKernel : public GpuKernel {
 public:
  UnaryOpGpuKernel() { ResetResource(); }
  ~UnaryOpGpuKernel() override = default;

  const std::vector<size_t> &GetInputSizeList() const override { return input_size_list_; }
  const std::vector<size_t> &GetOutputSizeList() const override { return output_size_list_; }
  const std::vector<size_t> &GetWorkspaceSizeList() const override { return workspace_size_list_; }

  bool Launch(const std::vector<AddressPtr> &inputs, const std::vector<AddressPtr> &,
              const std::vector<AddressPtr> &outputs, void *stream_ptr) override {
    if (is_null_input_) {
      return true;
    }
    T *input_addr = GetDeviceAddress<T>(inputs, 0);
    T *output_addr = GetDeviceAddress<T>(outputs, 0);

    switch (unary_op_type_) {
      case UNARY_OP_EXP: {
        Exponential(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_EXPM1: {
        Expm1(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_LOG: {
        Logarithm(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_LOG1P: {
        Log1p(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ERF: {
        Erf(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ERFC: {
        Erfc(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_NEG: {
        Negative(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_RECIPROCAL: {
        Reciprocal(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_SQUARE: {
        Square(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_SQRT: {
        Sqrt(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_RSQRT: {
        Rsqrt(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_SIN: {
        Sin(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_COS: {
        Cos(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ASIN: {
        Asin(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ACOS: {
        ACos(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ATAN: {
        Atan(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ASINH: {
        Asinh(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ACOSH: {
        Acosh(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ABS: {
        Abs(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_FLOOR: {
        Floor(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_RINT: {
        Rint(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_ROUND: {
        Round(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      case UNARY_OP_SIGN: {
        Sign(input_addr, output_addr, inputs[0]->size / sizeof(T), reinterpret_cast<cudaStream_t>(stream_ptr));
        break;
      }
      default: {
        MS_LOG(EXCEPTION) << "Unary operation " << unary_op_type_ << " is not supported.";
      }
    }
    return true;
  }
  bool Init(const CNodePtr &kernel_node) override {
    std::string kernel_name = AnfAlgo::GetCNodeName(kernel_node);
    auto iter = kUnaryOpTypeMap.find(kernel_name);
    if (iter == kUnaryOpTypeMap.end()) {
      MS_LOG(EXCEPTION) << "Unary operation " << kernel_name << " is not supported.";
    }
    unary_op_type_ = iter->second;
    size_t input_num = AnfAlgo::GetInputTensorNum(kernel_node);
    if (input_num != 1) {
      MS_LOG(ERROR) << "Input number is " << input_num << ", but unary op needs 1 inputs.";
      return false;
    }
    size_t output_num = AnfAlgo::GetOutputTensorNum(kernel_node);
    if (output_num != 1) {
      MS_LOG(ERROR) << "Output number is " << output_num << ", but unary op needs 1 output.";
      return false;
    }
    auto input_shape = AnfAlgo::GetInputRealDeviceShapeIfExist(kernel_node, 0);
    is_null_input_ = CHECK_NULL_INPUT(input_shape);
    if (is_null_input_) {
      MS_LOG(WARNING) << "For 'UnaryOpGpuKernel', input is null";
      InitSizeLists();
      return true;
    }
    for (size_t i = 0; i < input_shape.size(); i++) {
      input_size_ *= input_shape[i];
    }
    output_size_ = input_size_;
    InitSizeLists();
    return true;
  }
  void ResetResource() noexcept override {
    unary_op_type_ = UNARY_OP_INVALID_TYPE;
    input_size_ = sizeof(T);
    output_size_ = sizeof(T);
    workspace_size_ = 0;
    is_null_input_ = false;
    input_size_list_.clear();
    output_size_list_.clear();
    workspace_size_list_.clear();
  }

 protected:
  void InitSizeLists() override {
    input_size_list_.push_back(input_size_);
    output_size_list_.push_back(output_size_);
  }

 private:
  UnaryOptype unary_op_type_;
  size_t input_size_;
  size_t output_size_;
  size_t workspace_size_;
  bool is_null_input_;
  std::vector<size_t> input_size_list_;
  std::vector<size_t> output_size_list_;
  std::vector<size_t> workspace_size_list_;
};
}  // namespace kernel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_KERNEL_COMPILER_GPU_MATH_UNARYOP_GPU_KERNEL_H_
