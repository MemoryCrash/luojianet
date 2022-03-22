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

#ifndef LUOJIANET_MS_CCSRC_MINDDATA_DATASET_AUDIO_IR_KERNELS_MAGPHASE_IR_H_
#define LUOJIANET_MS_CCSRC_MINDDATA_DATASET_AUDIO_IR_KERNELS_MAGPHASE_IR_H_

#include <memory>
#include <string>

#include "include/api/status.h"
#include "minddata/dataset/include/dataset/constants.h"
#include "minddata/dataset/include/dataset/transforms.h"
#include "minddata/dataset/kernels/ir/tensor_operation.h"

namespace luojianet_ms {
namespace dataset {

namespace audio {

constexpr char kMagphaseOperation[] = "Magphase";

class MagphaseOperation : public TensorOperation {
 public:
  explicit MagphaseOperation(float power);

  ~MagphaseOperation() = default;

  std::shared_ptr<TensorOp> Build() override;

  std::string Name() const override { return kMagphaseOperation; }

  Status ValidateParams() override;

  Status to_json(nlohmann::json *out_json) override;

  static Status from_json(nlohmann::json op_params, std::shared_ptr<TensorOperation> *operation);

 private:
  float power_;
};

}  // namespace audio
}  // namespace dataset
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_MINDDATA_DATASET_AUDIO_IR_KERNELS_MAGPHASE_IR_H_