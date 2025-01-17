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

#ifndef LUOJIANET_MS_CORE_OPS_MAKE_TUPLE_H_
#define LUOJIANET_MS_CORE_OPS_MAKE_TUPLE_H_
#include "ops/primitive_c.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameMakeTuple = "MakeTuple";
/// \brief MakeTuple op is used to pack multiple nodes into a whole, which is only used in FuncGraph.
class MS_CORE_API MakeTuple : public PrimitiveC {
 public:
  /// \brief Constructor.
  MakeTuple() : PrimitiveC(kNameMakeTuple) {}

  /// \brief Destructor.
  ~MakeTuple() = default;

  MS_DECLARE_PARENT(MakeTuple, PrimitiveC);
};
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_MAKE_TUPLE_H_
