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

#include "runtime/device/gpu/gpu_device_address.h"
#include <vector>
#include <memory>
#include "utils/log_adapter.h"
#include "utils/ms_context.h"
#include "ir/tensor.h"
#include "runtime/device/gpu/gpu_device_manager.h"
#include "runtime/device/gpu/gpu_memory_allocator.h"
#include "runtime/hardware/gpu/gpu_device_context.h"
#ifdef ENABLE_DEBUGGER
#include "debug/debug_services.h"
#include "debug/tensor_load.h"
#include "debug/debugger/debugger.h"
#endif
#ifdef ENABLE_DUMP_IR
#include "debug/rdr/running_data_recorder.h"
#endif

namespace luojianet_ms {
namespace device {
namespace gpu {
bool GPUDeviceAddress::SyncDeviceToHost(size_t size, void *host_ptr) const {
  // The input or output may be empty.
  bool need_sync = (size != 0) && (size_ != 0) && (size <= size_);
  if (!need_sync) {
    MS_LOG(INFO) << "No need sync, host size: " << size << ", device size: " << size_;
    return true;
  }
  MS_EXCEPTION_IF_NULL(host_ptr);
  if (ptr_ == nullptr) {
    MS_LOG(ERROR) << "The device address is null!";
    return false;
  }

  auto &stream = GPUDeviceManager::GetInstance().default_stream();
  MS_EXCEPTION_IF_NULL(stream);
  auto ret = GPUDeviceManager::GetInstance().SyncStream(stream);
  if (!ret) {
#ifdef ENABLE_DUMP_IR
    luojianet_ms::RDR::TriggerAll();
#endif
    MS_LOG(ERROR) << "SyncStream failed";
    return ret;
  }
  if (size != size_) {
    // nccl kernel input and output device address is aligned, may lead to host size is not equal to device size
    MS_LOG(INFO) << "Sync memory size is inconsistent, host size: " << size << ", device size " << size_;
  }
  return GPUDeviceManager::GetInstance().CopyDeviceMemToHost(host_ptr, ptr_, size);
}

bool GPUDeviceAddress::SyncHostToDevice(size_t size, const void *host_ptr) const {
  // The input or output may be empty.
  bool need_sync = (size != 0) && (size_ != 0) && (size <= size_);
  if (!need_sync) {
    MS_LOG(INFO) << "No need sync, host size: " << size << ", device size: " << size_;
    return true;
  }
  MS_EXCEPTION_IF_NULL(host_ptr);
  if (ptr_ == nullptr) {
    MS_LOG(ERROR) << "The device address is null!";
    return false;
  }

  if (size != size_) {
    // nccl kernel input and output device address is aligned, may lead to host size is not equal to device size
    MS_LOG(INFO) << "Sync memory size is inconsistent, host size: " << size << ", device size " << size_;
  }

  // Bind device by device name and device id on the current thread.
  if (device_name_ != "") {
    auto device_context =
      device::DeviceContextManager::GetInstance().GetOrCreateDeviceContext({device_name_, device_id_});
    auto gpu_device_context = dynamic_cast<GPUDeviceContext *>(device_context);
    MS_EXCEPTION_IF_NULL(gpu_device_context);
    if (!gpu_device_context->BindDeviceToCurrentThread()) {
      MS_LOG(EXCEPTION) << "BindDeviceToCurrentThread failed.";
    }
  }

  auto &stream = GPUDeviceManager::GetInstance().default_stream();
  MS_EXCEPTION_IF_NULL(stream);
  if (!GPUDeviceManager::GetInstance().CopyHostMemToDeviceAsync(ptr_, host_ptr, size, stream)) {
    MS_LOG(ERROR) << "CopyHostMemToDeviceAsync failed";
    return false;
  }
  return GPUDeviceManager::GetInstance().SyncStream(stream);
}

bool GPUDeviceAddress::SyncDeviceToHost(const ShapeVector &, size_t size, TypeId, void *host_ptr) const {
  return SyncDeviceToHost(size, host_ptr);
}

bool GPUDeviceAddress::SyncHostToDevice(const ShapeVector &, size_t size, TypeId, const void *host_ptr,
                                        const std::string &format) const {
  auto ms_context = MsContext::GetInstance();
  MS_EXCEPTION_IF_NULL(ms_context);
  bool execution_mode = ms_context->get_param<int>(MS_CTX_EXECUTION_MODE);
  if (execution_mode != kPynativeMode) {
    return SyncHostToDevice(size, host_ptr);
  }

  // PyNative mode need copy async to improve performance.
  MS_EXCEPTION_IF_NULL(host_ptr);
  bool need_sync = (size != 0) && (size_ != 0) && (size <= size_);
  if (!need_sync) {
    return true;
  }
  auto &stream = GPUDeviceManager::GetInstance().default_stream();
  MS_EXCEPTION_IF_NULL(stream);
  return GPUDeviceManager::GetInstance().CopyHostMemToDeviceAsync(ptr_, host_ptr, size, stream);
}

void GPUDeviceAddress::ClearDeviceMemory() {
  if (ptr_ == nullptr) {
    return;
  }
  if (from_mem_pool_) {
    GPUMemoryAllocator::GetInstance().FreeTensorMem(ptr_);
    ptr_ = nullptr;
  }
}

GPUDeviceAddress::~GPUDeviceAddress() { ClearDeviceMemory(); }

#ifdef ENABLE_DEBUGGER
bool GPUDeviceAddress::LoadMemToHost(const std::string &tensor_name, int execution_order, const std::string &host_fmt,
                                     const ShapeVector &host_shape, TypeId host_type, size_t slot, bool keep_prev,
                                     uint32_t root_graph_id) const {
  bool ret = false;
  if (size_ == 0) {
    return true;
  }

  MS_EXCEPTION_IF_NULL(Debugger::GetInstance());
  if (Debugger::GetInstance()->TensorExistsInCurrent(tensor_name)) {
    MS_LOG(INFO) << tensor_name << " already loaded for this step so not loading it again.";
    return true;
  }

  if (host_type > TypeId::kNumberTypeEnd || host_type < TypeId::kNumberTypeBegin || host_type == kNumberTypeComplex64) {
    MS_LOG(INFO) << "Cannot create tensor with type: " << TypeIdLabel(host_type);
    return false;
  }
  luojianet_ms::tensor::TensorPtr out_tensor = std::make_shared<tensor::Tensor>(host_type, host_shape);
  size_t host_size = out_tensor->data().nbytes();
  auto ret_rt_memcpy = SyncDeviceToHost(host_shape, host_size, host_type, out_tensor->data_c());
  if (!ret_rt_memcpy) {
    MS_LOG(ERROR) << "Copy device mem to host failed";
    return ret;
  }
  auto tensor_data = std::make_shared<luojianet_ms::TensorData>();
  MS_EXCEPTION_IF_NULL(tensor_data);
  tensor_data->SetName(tensor_name);
  tensor_data->SetExecutionOrder(execution_order);
  tensor_data->SetSlot(slot);
  tensor_data->SetTensor(out_tensor);
  tensor_data->SetDataPtr(static_cast<char *>(out_tensor->data_c()));
  tensor_data->SetByteSize(out_tensor->data().nbytes());
  tensor_data->SetType((unsigned int)host_type);
  tensor_data->SetShape(out_tensor->shape());
  tensor_data->SetRootGraphId(root_graph_id);
  ret = Debugger::GetInstance()->LoadNewTensor(tensor_data, keep_prev);
  MS_LOG(INFO) << "E2E tensor name is " << tensor_name;
  return ret;
}
#endif
}  // namespace gpu
}  // namespace device
}  // namespace luojianet_ms
