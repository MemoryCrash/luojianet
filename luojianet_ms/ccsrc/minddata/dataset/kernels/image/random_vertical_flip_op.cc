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

#include "minddata/dataset/kernels/image/random_vertical_flip_op.h"

#include "minddata/dataset/kernels/image/image_utils.h"
#include "minddata/dataset/util/status.h"

namespace luojianet_ms {
namespace dataset {
const float RandomVerticalFlipOp::kDefProbability = 0.5;

Status RandomVerticalFlipOp::Compute(const TensorRow &input, TensorRow *output) {
  IO_CHECK_VECTOR(input, output);
  const int output_count = input.size();
  output->resize(output_count);
  if (distribution_(rnd_)) {
    for (size_t i = 0; i < input.size(); i++) {
      RETURN_IF_NOT_OK(VerticalFlip(input[i], &(*output)[i]));
    }
    return Status::OK();
  }
  *output = input;
  return Status::OK();
}
}  // namespace dataset
}  // namespace luojianet_ms
