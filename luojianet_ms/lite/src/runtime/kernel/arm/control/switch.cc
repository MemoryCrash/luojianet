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
#include "src/runtime/kernel/arm/control/switch.h"
#include "src/kernel_registry.h"
#include "include/errorcode.h"

using luojianet_ms::lite::KernelRegistrar;
using luojianet_ms::lite::RET_ERROR;
using luojianet_ms::lite::RET_OK;
using luojianet_ms::schema::PrimitiveType_Switch;

namespace luojianet_ms::kernel {
int SwitchCPUKernel::Prepare() { return RET_OK; }
int SwitchCPUKernel::ReSize() { return RET_OK; }
int SwitchCPUKernel::Run() { return RET_OK; }

REG_KERNEL(kCPU, kNumberTypeFloat32, PrimitiveType_Switch, LiteKernelCreator<SwitchCPUKernel>)
REG_KERNEL(kCPU, kNumberTypeFloat16, PrimitiveType_Switch, LiteKernelCreator<SwitchCPUKernel>)
REG_KERNEL(kCPU, kNumberTypeBool, PrimitiveType_Switch, LiteKernelCreator<SwitchCPUKernel>)
REG_KERNEL(kCPU, kNumberTypeInt32, PrimitiveType_Switch, LiteKernelCreator<SwitchCPUKernel>)
}  // namespace luojianet_ms::kernel