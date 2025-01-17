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

#ifndef LUOJIANET_MS_LITE_TOOLS_CONVERTER_DPICO_PARSER_INPUTS_ADJUST_PASS_H_
#define LUOJIANET_MS_LITE_TOOLS_CONVERTER_DPICO_PARSER_INPUTS_ADJUST_PASS_H_

#include <memory>
#include <vector>
#include <string>
#include "ir/anf.h"
#include "api/ir/func_graph.h"
#include "ops/primitive_c.h"
#include "include/errorcode.h"

using luojianet_ms::lite::STATUS;
using PrimitiveCPtr = std::shared_ptr<luojianet_ms::ops::PrimitiveC>;
namespace luojianet_ms::lite {
class InputAdjust {
 public:
  InputAdjust() {}
  ~InputAdjust() = default;

  STATUS AddAttrToInput(const api::FuncGraphPtr &func_graph, const CNodePtr &cnode, int input_num,
                        const std::string &attr_name, int flag);
  bool Run(const api::FuncGraphPtr &func_graph);
};
}  // namespace luojianet_ms::lite
#endif  // LUOJIANET_MS_LITE_TOOLS_CONVERTER_DPICO_PARSER_INPUTS_ADJUST_PASS_H_
