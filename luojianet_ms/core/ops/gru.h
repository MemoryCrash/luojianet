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

#ifndef LUOJIANET_MS_CORE_OPS_GRU_H_
#define LUOJIANET_MS_CORE_OPS_GRU_H_

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "ops/op_utils.h"
#include "ops/primitive_c.h"
#include "abstract/primitive_infer_map.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameGRU = "GRU";
/// \brief GRU defined the GRU operator prototype.
class MS_CORE_API GRU : public PrimitiveC {
 public:
  /// \brief Constructor.
  GRU() : PrimitiveC(kNameGRU) {
    InitIOName({"x", "weight_input", "weight_hidden", "bias_input", "bias_hidden", "seq_length", "init_h"},
               {"output", "output_h", "update", "reset", "new", "hidden_new"});
  }

  /// \brief Destructor.
  ~GRU() = default;

  MS_DECLARE_PARENT(GRU, PrimitiveC);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] bidirectional Define a boolean value to indicate whether the gru is single or double direction.
  void Init(bool bidirectional = false);

  /// \brief Method to set bidirectional attribute.
  ///
  /// \param bidirectional Define a boolean value to indicate whether the gru is single or double direction.
  void set_bidirectional(bool bidirectional);

  /// \brief Method to get bidirectional attribute.
  ///
  /// \return a boolean value.
  bool get_bidirectional() const;
};

}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_GRU_H_
