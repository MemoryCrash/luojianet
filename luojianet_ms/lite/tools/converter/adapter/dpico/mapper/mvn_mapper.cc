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

#include "mapper/mvn_mapper.h"
#include <memory>
#include <utility>
#include <vector>
#include "ops/op_utils.h"
#include "common/anf_util.h"
#include "common/op_attr.h"
#include "op/mvn_operator.h"

namespace luojianet_ms {
namespace dpico {
STATUS MvnMapper::Map(const CNodePtr &cnode, std::vector<BaseOperatorPtr> *base_operators, const PrimitivePtr &prim,
                      const CNodePtrList &output_cnodes) {
  if (base_operators == nullptr) {
    MS_LOG(ERROR) << "base_operators is nullptr.";
    return RET_ERROR;
  }

  auto mvn_operator = std::make_unique<mapper::MvnOperator>();
  if (mvn_operator == nullptr) {
    MS_LOG(ERROR) << "mvn_operator is nullptr.";
    return RET_ERROR;
  }

  if (SetCommonAttr(cnode, mvn_operator.get(), output_cnodes) != RET_OK) {
    MS_LOG(ERROR) << "set common attr failed. " << cnode->fullname_with_scope();
    return RET_ERROR;
  }

  mvn_operator->SetOpType(mapper::OpType::MVN);
  if (prim->GetAttr(ops::kEps) != nullptr) {
    mvn_operator->SetMVNEps(GetValue<float>(prim->GetAttr(ops::kEps)));
  }
  if (prim->GetAttr(dpico::kAcrossChannels) != nullptr) {
    mvn_operator->SetMVNAcrossChannels(GetValue<bool>(prim->GetAttr(dpico::kAcrossChannels)));
  }
  if (prim->GetAttr(dpico::kNormalizeVariance) != nullptr) {
    mvn_operator->SetMVNNormalizeVariance(GetValue<bool>(prim->GetAttr(dpico::kNormalizeVariance)));
  }
  if (prim->GetAttr(ops::kAxes) != nullptr) {
    auto axes = GetValue<std::vector<uint32_t>>(prim->GetAttr(ops::kAxes));
    for (auto axis : axes) {
      mvn_operator->PushMVNAxes(axis);
    }
  }
  if (PushOfflineArgs(cnode, mvn_operator.get(), 1) != RET_OK) {
    MS_LOG(ERROR) << "push offline args failed. " << cnode->fullname_with_scope();
    return RET_ERROR;
  }
  base_operators->push_back(std::move(mvn_operator));
  return RET_OK;
}
REG_MAPPER(Mvn, MvnMapper)
}  // namespace dpico
}  // namespace luojianet_ms
