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

#include "checker/spp_checker.h"
#include <string>
#include "common/op_attr.h"

namespace luojianet_ms {
namespace dpico {
bool SppChecker::Check(CNodePtr op, int32_t output_num, luojianet_ms::Format format) {
  auto primitive = GetValueNode<PrimitivePtr>(op->input(0));
  if (primitive == nullptr) {
    MS_LOG(ERROR) << "primitive is nullptr";
    return false;
  }
  if (primitive->GetAttr(dpico::kPoolMethod) != nullptr) {
    auto pool_method = GetValue<int32_t>(primitive->GetAttr(dpico::kPoolMethod));
    if (pool_method != 0 && pool_method != 1) {
      MS_LOG(WARNING) << "only supports max && ave pool by dpico. " << op->fullname_with_scope();
      return false;
    }
  }
  return true;
}

OpCheckerRegistrar g_SppChecker("Spp", new SppChecker());
}  // namespace dpico
}  // namespace luojianet_ms
