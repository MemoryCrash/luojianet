/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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
#include "backend/kernel_compiler/kernel.h"
#include "backend/kernel_compiler/aicpu/aicpu_kernel_load.h"

namespace luojianet_ms {
namespace kernel {

/*
 * @brief build op and return a callable mod
 */
KernelModPtr AicpuOpBuild(const AnfNodePtr &anf_node) { return nullptr; }

bool AicpuOpKernelLoad::LaunchAicpuKernelSo() { return true; }
void AicpuOpKernelLoad::FreeDeviceMemory() {}
}  // namespace kernel
}  // namespace luojianet_ms
