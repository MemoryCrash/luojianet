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

#ifndef LUOJIANET_MS_CCSRC_FRONTEND_OPTIMIZER_IRPASS_LOAD_ELIMINATE_H_
#define LUOJIANET_MS_CCSRC_FRONTEND_OPTIMIZER_IRPASS_LOAD_ELIMINATE_H_

#include "frontend/optimizer/optimizer.h"
#include "frontend/optimizer/anf_visitor.h"

namespace luojianet_ms::opt::irpass {
//
// LoadEliminater eliminates redundant Load related nodes.
//
class LoadEliminater : public AnfVisitor {
 public:
  AnfNodePtr operator()(const OptimizerPtr &, const AnfNodePtr &node) override;
};
}  // namespace luojianet_ms::opt::irpass
#endif  // LUOJIANET_MS_CCSRC_FRONTEND_OPTIMIZER_IRPASS_LOAD_ELIMINATE_H_