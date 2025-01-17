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

#ifndef LUOJIANET_MS_CORE_OPS_EMBEDDING_LOOKUP_FUSION_H_
#define LUOJIANET_MS_CORE_OPS_EMBEDDING_LOOKUP_FUSION_H_
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameEmbeddingLookupFusion = "EmbeddingLookupFusion";
/// \brief EmbeddingLookupFusion defined EmbeddingLookup operator prototype of lite.
class MS_CORE_API EmbeddingLookupFusion : public PrimitiveC {
 public:
  /// \brief Constructor.
  EmbeddingLookupFusion() : PrimitiveC(kNameEmbeddingLookupFusion) {
    InitIOName({"params", "indices", "offset"}, {"output"});
  }

  /// \brief Destructor.
  ~EmbeddingLookupFusion() = default;

  MS_DECLARE_PARENT(EmbeddingLookupFusion, PrimitiveC);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] max_norm Define the max l2-norm value of each embedding. Each embedding will be clip if l2-norm is
  ///            larger than this value.
  void Init(const float max_norm = 0.0);

  /// \brief Method to set max_norm attribute.
  ///
  /// \param[in] max_norm Define the max l2-norm value of each embedding. Each embedding will be clip if l2-norm is
  ///            larger than this value.
  void set_max_norm(const float max_norm);

  /// \brief Method to get max_norm attribute.
  ///
  /// \return a value which indicates a max l2-norm value.
  float get_max_norm() const;
};
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_EMBEDDING_LOOKUP_FUSION_H_
