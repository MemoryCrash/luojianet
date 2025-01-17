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
#include <memory>
#include "backend/kernel_compiler/gpu/rl/tensor_array_create_kernel.h"
#include "backend/kernel_compiler/common_utils.h"
#include "runtime/device/gpu/gpu_tensor_array.h"
#include "runtime/device/tensor_array_manager.h"

namespace luojianet_ms {
namespace kernel {
using luojianet_ms::device::TensorArrayMgr;
using luojianet_ms::device::gpu::GPUTensorArray;
using luojianet_ms::device::gpu::GPUTensorArrayPtr;
TensorArrayCreateKernel::TensorArrayCreateKernel() : is_dynamic_(true), size_(0), type_(nullptr) {}

const std::vector<size_t> &TensorArrayCreateKernel::GetInputSizeList() const { return input_size_list_; }

const std::vector<size_t> &TensorArrayCreateKernel::GetOutputSizeList() const { return output_size_list_; }

const std::vector<size_t> &TensorArrayCreateKernel::GetWorkspaceSizeList() const { return workspace_size_list_; }

bool TensorArrayCreateKernel::Init(const CNodePtr &kernel_node) {
  MS_EXCEPTION_IF_NULL(kernel_node);
  auto shape = GetAttr<std::vector<int64_t>>(kernel_node, "element_shape");
  for (auto i : shape) {
    shapes_.push_back(LongToSize(i));
  }
  type_ = GetAttr<TypePtr>(kernel_node, "dtype");
  size_ = GetAttr<int64_t>(kernel_node, "size");
  is_dynamic_ = GetAttr<bool>(kernel_node, "dynamic_size");
  name_ = GetAttr<std::string>(kernel_node, "name");
  InitSizeLists();
  return true;
}

void TensorArrayCreateKernel::InitSizeLists() { output_size_list_.push_back(sizeof(int64_t)); }

bool TensorArrayCreateKernel::Launch(const std::vector<AddressPtr> &, const std::vector<AddressPtr> &,
                                     const std::vector<AddressPtr> &outputs, void *stream_ptr) {
  // Create a tensorarray, and generate an unique handle.
  int64_t tensor_array_handle = TensorArrayMgr::GetInstance().GetHandleCount();
  auto name = "GPUTensorArray_" + name_ + "_" + std::to_string(tensor_array_handle);
  GPUTensorArrayPtr tensor_array = std::make_shared<GPUTensorArray>(name, type_, shapes_);
  MS_EXCEPTION_IF_NULL(tensor_array);
  tensor_array->SetMaxSize(size_, is_dynamic_);
  auto out_addr = GetDeviceAddress<int64_t>(outputs, 0);
  // Set handle to out_addr.
  CHECK_CUDA_RET_WITH_EXCEPT(kernel_node_,
                             cudaMemcpyAsync(out_addr, &tensor_array_handle, sizeof(int64_t), cudaMemcpyHostToDevice,
                                             reinterpret_cast<cudaStream_t>(stream_ptr)),
                             "Create TensorArray failed");
  MS_LOG(DEBUG) << "Create handle id " << tensor_array_handle;
  // Put tensorarray to a saved map : map<handle, tensorarray> in tensorarray manager.
  // And increase the handle count automatically in AddTensorArray function.
  TensorArrayMgr::GetInstance().AddTensorArray(tensor_array_handle, tensor_array);
  return true;
}
}  // namespace kernel
}  // namespace luojianet_ms
