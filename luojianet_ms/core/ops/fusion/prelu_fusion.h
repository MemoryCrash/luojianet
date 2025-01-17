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

#ifndef LUOJIANET_MS_CORE_OPS_PRELU_FUSION_H_
#define LUOJIANET_MS_CORE_OPS_PRELU_FUSION_H_
#include <vector>

#include "ops/prelu.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNamePReLUFusion = "PReLUFusion";
/// \brief PReLUFusion defined PReLU operator prototype of lite.
class MS_CORE_API PReLUFusion : public PReLU {
 public:
  /// \brief Constructor.
  PReLUFusion() : PReLU(kNamePReLUFusion) {}

  /// \brief Destructor.
  ~PReLUFusion() = default;

  MS_DECLARE_PARENT(PReLUFusion, PReLU);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] channel_shared Define a boolean value to indicate whether channel is shared or not.
  /// \param[in] slope Define a size factor applied to the elements less than 0.
  void Init(const bool channel_shared, const std::vector<float> &slope);

  /// \brief Method to set channel_shared attribute.
  ///
  /// \param[in] channel_shared Define a boolean value to indicate whether channel is shared or not.
  void set_channel_shared(const bool channel_shared);

  /// \brief Method to set slope attribute.
  ///
  /// \param[in] slope Define size factors applied to the elements less than 0.
  void set_slope(const std::vector<float> &slope);

  /// \brief Method to get channel_shared attribute.
  ///
  /// \return a boolean value.
  bool get_channel_shared() const;

  /// \brief Method to get slope attribute.
  ///
  /// \return size factors.
  std::vector<float> get_slope() const;
};
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_PRELU_FUSION_H_
