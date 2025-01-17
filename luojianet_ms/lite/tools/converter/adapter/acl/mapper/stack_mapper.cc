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

#include "tools/converter/adapter/acl/mapper/stack_mapper.h"
#include "tools/converter/adapter/acl/mapper/primitive_mapper_register.h"

namespace luojianet_ms {
namespace lite {
namespace {
constexpr auto kNameNum = "num";
}

STATUS StackMapper::Mapper(const CNodePtr &cnode) {
  if (AddAttrForDynInputPrimitive(cnode, kNameNum) != RET_OK) {
    MS_LOG(ERROR) << "Stack mapper failed.";
    return RET_ERROR;
  }
  return RET_OK;
}

REGISTER_PRIMITIVE_MAPPER(kNameStack, StackMapper)
}  // namespace lite
}  // namespace luojianet_ms
