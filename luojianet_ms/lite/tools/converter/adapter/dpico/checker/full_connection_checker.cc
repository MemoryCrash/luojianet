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

#include "checker/full_connection_checker.h"
#include <vector>
#include <string>
#include "common/anf_util.h"
#include "common/op_attr.h"
#include "common/op_enum.h"

namespace luojianet_ms {
namespace dpico {
bool FullConnectionChecker::Check(CNodePtr op, int32_t output_num, luojianet_ms::Format format) {
  if (!CheckInputW(op, 1, format, kMaxInputWOf4Dims)) {
    MS_LOG(WARNING) << "input_w is not supported. " << op->fullname_with_scope();
    return false;
  }

  if (op->inputs().size() < kInputIndex3) {
    MS_LOG(ERROR) << "FullConnection should have 2 inputs at least, but is " << (op->inputs().size() - 1);
    return false;
  }
  std::vector<int64_t> weight_shape;
  if (GetInputShapeFromCNode(op, kInputIndex2, &weight_shape) != RET_OK) {
    MS_LOG(ERROR) << "get weight shape failed";
    return false;
  }
  if (weight_shape.empty()) {
    MS_LOG(ERROR) << "weight shape is empty.";
    return false;
  }

  auto primitive = GetValueNode<PrimitivePtr>(op->input(0));
  if (primitive == nullptr) {
    MS_LOG(ERROR) << "primitive is nullptr";
    return false;
  }
  if (primitive->GetAttr(kNumOutput) != nullptr) {
    auto num_output = GetValue<uint32_t>(primitive->GetAttr(kNumOutput));
    if (num_output > kMaxNumOutput) {
      MS_LOG(WARNING) << "num_output val:" << num_output << " should be less than " << kMaxNumOutput << " "
                      << op->fullname_with_scope();
      return false;
    }
  }

  return true;
}

OpCheckerRegistrar g_FullConnectionChecker("FullConnection", new FullConnectionChecker());
}  // namespace dpico
}  // namespace luojianet_ms
