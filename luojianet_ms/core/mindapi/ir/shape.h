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

#ifndef LUOJIANET_MS_CORE_MINDAPI_IR_SHAPE_H_
#define LUOJIANET_MS_CORE_MINDAPI_IR_SHAPE_H_

#include "mindapi/base/base.h"
#include "mindapi/base/shape_vector.h"
#include "mindapi/ir/common.h"

namespace luojianet_ms::api {
/// \brief Shape defines dimensions of a tensor.
class MIND_API Shape : public Base {
 public:
  MIND_API_BASE_MEMBER(Shape);

  /// \brief Get the shape dimensions.
  ///
  /// \return The shape dimensions.
  const ShapeVector &shape() const;
};
}  // namespace luojianet_ms::api
#endif  // LUOJIANET_MS_CORE_MINDAPI_IR_SHAPE_H_
