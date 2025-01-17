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

#ifndef LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_CONTROLFLOW_SWITCH_ACTOR_H_
#define LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_CONTROLFLOW_SWITCH_ACTOR_H_

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include "runtime/framework/actor/actor_common.h"
#include "runtime/framework/actor/control_flow/control_actor.h"

namespace luojianet_ms {
namespace runtime {
using luojianet_ms::device::DeviceContext;
using luojianet_ms::session::KernelWithIndex;

// Switch actor is used to execute the branch according to the input condition.
// Switch and SwitchLayer node will be converted to switch actor.
class SwitchActor : public ControlActor {
 public:
  SwitchActor(const std::string &name, const AID &memory_manager_aid, const std::vector<KernelWithIndex> &parameters,
              const AnfNodePtr &node);
  ~SwitchActor() override = default;

  void Init() override;

 protected:
  void FetchInput(OpContext<DeviceTensor> *const context) override;

 private:
  friend class ControlNodeScheduler;
  // Get the output branch index of the switch actor.
  size_t GetIndex(const OpContext<DeviceTensor> *const context) const;
  size_t index_{0};
};

using SwitchActorPtr = std::shared_ptr<SwitchActor>;
}  // namespace runtime
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_RUNTIME_FRAMEWORK_ACTOR_CONTROLFLOW_SWITCH_ACTOR_H_
