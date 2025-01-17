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

#ifndef LUOJIANET_MS_CORE_OPS_ADDCDIV_H_
#define LUOJIANET_MS_CORE_OPS_ADDCDIV_H_
#include <memory>
#include <vector>

#include "ops/primitive_c.h"
#include "ops/op_utils.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameAddcdiv = "Addcdiv";
class Addcdiv : public PrimitiveC {
 public:
  Addcdiv() : PrimitiveC(kNameAddcdiv) { InitIOName({"input_data", "x1", "x2", "value"}, {"output"}); }
  ~Addcdiv() = default;
  MS_DECLARE_PARENT(Addcdiv, PrimitiveC);
};

AbstractBasePtr AddcdivInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                             const std::vector<AbstractBasePtr> &input_args);
using PrimAddcdivPtr = std::shared_ptr<Addcdiv>;
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_ADDCDIV_H_
