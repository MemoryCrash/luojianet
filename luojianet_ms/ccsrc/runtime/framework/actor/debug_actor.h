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

#ifndef LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_DEBUG_ACTOR_H_
#define LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_DEBUG_ACTOR_H_

#include <vector>
#include "runtime/framework/actor/actor_common.h"
#include "runtime/framework/device_tensor_store.h"
#include "runtime/hardware/device_context.h"

namespace luojianet_ms {
namespace runtime {
using luojianet_ms::device::DeviceContext;
using luojianet_ms::kernel::KernelLaunchInfo;

// The debug actor is used to debug and dump kernel info, it gets the kernel real time execution info in the device, so
// it is synchronous and blocked.
class DebugActor : public ActorBase {
 public:
  DebugActor() : ActorBase("DebugActor") {}
  ~DebugActor() override = default;

  // The debug of each node.
  void Debug(const AnfNodePtr &node, const KernelLaunchInfo *launch_info_, const DeviceContext *device_context,
             OpContext<DeviceTensor> *const op_context, const AID *from_aid);

  // The debug of kernel graph.
  void DebugForGraph(const KernelGraphPtr &graph, const DeviceContext *device_context,
                     OpContext<DeviceTensor> *const op_context, const AID *from_aid);

  // The debug on step begin.
  void DebugOnStepBegin(std::vector<KernelGraphPtr> graphs, std::vector<DeviceContext *> device_contexts,
                        OpContext<DeviceTensor> *const op_context, const AID *from_aid);

  // The debug on step end.
  void DebugOnStepEnd(OpContext<DeviceTensor> *const op_context, const AID *from_aid);

 private:
  // class members
  uint32_t exec_order_ = 0;
};

}  // namespace runtime
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_DEBUG_ACTOR_H_
