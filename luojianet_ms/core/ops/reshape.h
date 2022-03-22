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
#ifndef LUOJIANET_MS_CORE_OPS_RESHAPE_H_
#define LUOJIANET_MS_CORE_OPS_RESHAPE_H_

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameReshape = "Reshape";
/// \brief Reshapes the input tensor with the same values based on a given shape tuple.
/// Refer to Python API @ref luojianet_ms.ops.Reshape for more details.
class MS_CORE_API Reshape : public PrimitiveC {
 public:
  /// \brief Constructor.
  Reshape() : PrimitiveC(kNameReshape) { InitIOName({"tensor", "shape"}, {"output"}); }
  /// \brief Destructor.
  ~Reshape() = default;
  MS_DECLARE_PARENT(Reshape, PrimitiveC);
  /// \brief Init.
  void Init() {}
};

AbstractBasePtr ReshapeInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                             const std::vector<AbstractBasePtr> &input_args);
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_RESHAPE_H_