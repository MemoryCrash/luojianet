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
#include "tools/converter/parser/tf/tf_loop_cond_parser.h"
#include <string>
#include <map>
#include <vector>
#include "tools/converter/parser/tf/tf_node_parser_registry.h"
#include "tools/converter/ops/ops_def.h"

namespace luojianet_ms {
namespace lite {
ops::PrimitiveC *TFLoopCondParser::Parse(const tensorflow::NodeDef &tf_op,
                                         const std::map<string, const tensorflow::NodeDef *> &tf_node_map,
                                         std::vector<std::string> *inputs, int *output_size) {
  auto prim = std::make_unique<LoopCond>();
  MS_CHECK_TRUE_RET(prim != nullptr, nullptr);
  *output_size = tf_op.input_size();
  for (int i = 0; i < tf_op.input_size(); i++) {
    inputs->emplace_back(tf_op.input(i));
  }

  return prim.release();
}
TFNodeRegistrar g_tfLoopCondParser("LoopCond", new TFLoopCondParser());
}  // namespace lite
}  // namespace luojianet_ms