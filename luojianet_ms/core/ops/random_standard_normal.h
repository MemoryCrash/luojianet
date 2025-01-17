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

#ifndef LUOJIANET_MS_CORE_OPS_RANDOM_STANDARD_NORMAL_H_
#define LUOJIANET_MS_CORE_OPS_RANDOM_STANDARD_NORMAL_H_
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameRandomStandardNormal = "RandomStandardNormal";
/// \brief RandomStandardNormal defined RandomStandardNormal operator prototype of lite.
class MS_CORE_API RandomStandardNormal : public PrimitiveC {
 public:
  /// \brief Constructor.
  RandomStandardNormal() : PrimitiveC(kNameRandomStandardNormal) {}

  /// \brief Destructor.
  ~RandomStandardNormal() = default;

  MS_DECLARE_PARENT(RandomStandardNormal, PrimitiveC);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] seed Define random seed.
  /// \param[in] seed2 Define random seed2.
  void Init(int64_t seed, int64_t seed2);

  /// \brief Method to set seed attributes.
  ///
  /// \param[in] seed Define random seed.
  void set_seed(int64_t seed);

  /// \brief Method to set seed2 attributes.
  ///
  /// \param[in] seed2 Define random seed2.
  void set_seed2(int64_t seed2);

  /// \brief Method to get seed attributes.
  ///
  /// \return seed attributes.
  int64_t get_seed() const;

  /// \brief Method to get seed2 attributes.
  ///
  /// \return seed2 attributes.
  int64_t get_seed2() const;
};
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_RANDOM_STANDARD_NORMAL_H_
