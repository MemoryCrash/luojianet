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

#ifndef LUOJIANET_MS_CORE_IR_VISITOR_H_
#define LUOJIANET_MS_CORE_IR_VISITOR_H_

#include <vector>
#include "ir/anf.h"

namespace luojianet_ms {
using VisitFuncType = std::function<void(const AnfNodePtr &)>;
using PredicateFuncType = std::function<bool(const AnfNodePtr &)>;
class AnfIrVisitor {
 public:
  virtual void Visit(const AnfNodePtr &);
  virtual void Visit(const CNodePtr &);
  virtual void Visit(const ValueNodePtr &);
  virtual void Visit(const ParameterPtr &);
  VisitFuncType Match(const PrimitivePtr &, const std::vector<PredicateFuncType> & = {});
  virtual ~AnfIrVisitor() = default;
};
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CORE_IR_VISITOR_H_
