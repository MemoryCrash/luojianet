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

#ifndef LUOJIANET_MS_LITE_SRC_CONTROLFLOW_EXIT_ACTOR_H_
#define LUOJIANET_MS_LITE_SRC_CONTROLFLOW_EXIT_ACTOR_H_
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "src/lite_mindrt.h"

namespace luojianet_ms::lite {
class LiteExitOpActor : public LiteOpActor {
 public:
  explicit LiteExitOpActor(kernel::LiteKernel *kernel, lite::InnerContext *ctx) : LiteOpActor(kernel, ctx) {}
  ~LiteExitOpActor() override = default;
  void RunOpData(OpData<Tensor> *inputs, OpContext<Tensor> *context = nullptr) override;
  int PreInit(std::vector<std::shared_ptr<LiteOpActor>> *actors,
              std::unordered_map<Tensor *, Tensor *> *input_map) override;
  int PostInit() override;

 protected:
  void AsyncOutput(OpContext<Tensor> *context) override;
  int PrepareOutputData() override;
  void InitInputData() override;
  void SetInputShape() override;

 private:
  struct MappingInfo {
    MappingInfo(kernel::LiteKernel *partial, kernel::LiteKernel *call) : partial_node(partial), call_node(call) {}
    kernel::LiteKernel *partial_node = nullptr;
    kernel::LiteKernel *call_node = nullptr;
    AID partial_input_aid;
    AID call_output_aid;
  };
  int CreateMappingInfo();
  int RecordCallNodeOutputActor(std::vector<std::shared_ptr<LiteOpActor>> *actors);
  void RecordPartialNodeInputActor();
  void SetEntranceInputAID(OpData<Tensor> *inputs);

  std::vector<std::shared_ptr<LiteOpActor>> *actors_{};
  std::vector<MappingInfo> all_mapping_info_{};
  AID entrance_input_aid_;
};
}  // namespace luojianet_ms::lite
#endif  // LUOJIANET_MS_LITE_SRC_CONTROLFLOW_EXIT_ACTOR_H_
