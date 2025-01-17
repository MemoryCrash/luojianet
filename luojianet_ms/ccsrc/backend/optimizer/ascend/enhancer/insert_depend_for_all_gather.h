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

#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_INSERT_DEPEND_FOR_ALL_GATHER_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_INSERT_DEPEND_FOR_ALL_GATHER_H_
#include <vector>
#include <string>
#include <utility>
#include <memory>

#include "backend/optimizer/common/pass.h"
#include "ir/func_graph.h"
#include "ir/anf.h"
#include "backend/optimizer/common/helper.h"
#include "backend/optimizer/common/optimizer.h"
#include "backend/optimizer/ascend/ascend_helper.h"

namespace luojianet_ms {
namespace opt {
class InsertDependForAllGather : public Pass {
 public:
  InsertDependForAllGather() : Pass("insert_depend_for_all_gather"), kernel_select_(std::make_shared<KernelSelect>()) {}
  ~InsertDependForAllGather() override = default;
  bool Run(const FuncGraphPtr &graph) override;

 private:
  KernelSelectPtr kernel_select_;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_INSERT_DEPEND_FOR_ALL_GATHER_H_
