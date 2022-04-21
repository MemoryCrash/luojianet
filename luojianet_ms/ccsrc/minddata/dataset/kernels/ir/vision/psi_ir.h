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

#ifndef LUOJIANET_MS_CCSRC_MINDDATA_DATASET_KERNELS_IR_VISION_PSI_IR_H_
#define LUOJIANET_MS_CCSRC_MINDDATA_DATASET_KERNELS_IR_VISION_PSI_IR_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "include/api/status.h"
#include "minddata/dataset/include/dataset/constants.h"
#include "minddata/dataset/include/dataset/transforms.h"
#include "minddata/dataset/kernels/ir/tensor_operation.h"

namespace luojianet_ms {
namespace dataset {

namespace vision {

constexpr char kPSIOperation[] = "PSI";

class PSIOperation : public TensorOperation {
 public:
  PSIOperation();

  ~PSIOperation();

  std::shared_ptr<TensorOp> Build() override;

  Status ValidateParams() override;

  std::string Name() const override;

  static Status from_json(nlohmann::json op_params, std::shared_ptr<TensorOperation> *operation);
};

}  // namespace vision
}  // namespace dataset
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_MINDDATA_DATASET_KERNELS_IR_VISION_PSI_IR_H_