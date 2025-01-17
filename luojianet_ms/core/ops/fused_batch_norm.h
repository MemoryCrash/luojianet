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

#ifndef LUOJIANET_MS_CORE_OPS_FUSED_BATCH_NORM_H_
#define LUOJIANET_MS_CORE_OPS_FUSED_BATCH_NORM_H_
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameFusedBatchNorm = "FusedBatchNorm";
/// \brief FusedBatchNorm defined Enhanced BatchNorm operator prototype.
class MS_CORE_API FusedBatchNorm : public PrimitiveC {
 public:
  /// \brief Constructor.
  FusedBatchNorm() : PrimitiveC(kNameFusedBatchNorm) {
    InitIOName({"x", "scale", "b", "mean", "variance"},
               {"y", "running_mean", "running_variance", "save_mean", "save_inv_variance"});
  }

  /// \brief Destructor.
  ~FusedBatchNorm() = default;

  MS_DECLARE_PARENT(FusedBatchNorm, PrimitiveC);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] mode Define the mode of batchnorm, which is useless.
  /// \param[in] epsilon Define a small value added for numerical stability, which is used to avoid that divisor is
  ///            zero.
  /// \param[in] momentum Define the hyper parameter to compute moving average for running_mean and running_var.
  void Init(const int64_t mode = 0, const float epsilon = 1e-5, const float momentum = 0.1);

  /// \brief Method to set mode attribute, which has been abandoned.
  ///
  /// \param[in] mode Define the mode of batchnorm, which is useless.
  void set_mode(const int64_t mode);

  /// \brief Method to set epsilon attribute.
  ///
  /// \param[in] epsilon Define a small value added for numerical stability, which is used to avoid that divisor is
  ///            zero.
  void set_epsilon(const float epsilon);

  /// \brief Method to set momentum attribute.
  ///
  /// \param[in] momentum Define the hyper parameter to compute moving average for running_mean and running_var.
  void set_momentum(const float momentum);

  /// \brief Method to get mode attribute, which has been abandoned.
  int64_t get_mode() const;

  /// \brief Method to get epsilon attribute.
  ///
  /// \return a small float value.
  float get_epsilon() const;

  /// \brief Method to get momentum attribute.
  ///
  /// \return the hyper parameter .
  float get_momentum() const;
};
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_FUSED_BATCH_NORM_H_
