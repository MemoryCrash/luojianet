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

#ifndef LUOJIANET_MS_CORE_OPS_ZEROS_LIKE_H_
#define LUOJIANET_MS_CORE_OPS_ZEROS_LIKE_H_
#include <memory>
#include <vector>

#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
/// \brief Creates a new tensor. Refer to Python API @ref luojianet_ms.ops.ZerosLike for more details.
class MS_CORE_API ZerosLike : public PrimitiveC {
 public:
  /// \brief Constructor.
  ZerosLike() : PrimitiveC(prim::kPrimZerosLike->name()) { InitIOName({"x"}, {"y"}); }
  /// \brief Destructor.
  ~ZerosLike() = default;
  MS_DECLARE_PARENT(ZerosLike, PrimitiveC);
  /// \brief Init.
  void Init() {}
};
AbstractBasePtr ZerosLikeInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                               const std::vector<AbstractBasePtr> &input_args);
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_ZEROS_LIKE_H_
