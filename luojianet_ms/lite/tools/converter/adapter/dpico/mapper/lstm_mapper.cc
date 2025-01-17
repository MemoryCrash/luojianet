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

#include "mapper/lstm_mapper.h"
#include <memory>
#include <utility>
#include <vector>
#include "common/op_attr.h"
#include "common/anf_util.h"
#include "op/lstm_operator.h"

namespace luojianet_ms {
namespace dpico {
STATUS LstmMapper::Map(const CNodePtr &cnode, std::vector<BaseOperatorPtr> *base_operators, const PrimitivePtr &prim,
                       const CNodePtrList &output_cnodes) {
  if (base_operators == nullptr) {
    MS_LOG(ERROR) << "base_operators is nullptr.";
    return RET_ERROR;
  }
  auto lstm_operator = std::make_unique<mapper::LstmOperator>();
  if (lstm_operator == nullptr) {
    MS_LOG(ERROR) << "lstm_operator is nullptr.";
    return RET_ERROR;
  }

  if (SetCommonAttr(cnode, lstm_operator.get(), output_cnodes) != RET_OK) {
    MS_LOG(ERROR) << "set common attr failed. " << cnode->fullname_with_scope();
    return RET_ERROR;
  }

  if (prim->GetAttr(kNumOutput) != nullptr) {
    lstm_operator->SetRecurrentNumOutput(GetValue<uint32_t>(prim->GetAttr(kNumOutput)));
  }
  if (prim->GetAttr(kExposeHidden) != nullptr) {
    lstm_operator->SetRecurrentExposeHidden(GetValue<bool>(prim->GetAttr(kExposeHidden)));
  }
  if (prim->GetAttr(kOutputLastFrameFlag) != nullptr) {
    lstm_operator->SetOutputLastFrameFlag(GetValue<bool>(prim->GetAttr(kOutputLastFrameFlag)));
  }
  if (prim->GetAttr(kInitialHOnlineFlag) != nullptr) {
    lstm_operator->SetInitialHOnlineFlag(GetValue<bool>(prim->GetAttr(kInitialHOnlineFlag)));
  }
  if (prim->GetAttr(kUseDefaultInitialHFlag) != nullptr) {
    lstm_operator->SetUseDefaultInitialHFlag(GetValue<bool>(prim->GetAttr(kUseDefaultInitialHFlag)));
  }
  if (prim->GetAttr(kInitialCOnlineFlag) != nullptr) {
    lstm_operator->SetInitialCOnlineFlag(GetValue<bool>(prim->GetAttr(kInitialCOnlineFlag)));
  }
  if (prim->GetAttr(kUseDefaultInitialCFlag) != nullptr) {
    lstm_operator->SetUseDefaultInitialCFlag(GetValue<bool>(prim->GetAttr(kUseDefaultInitialCFlag)));
  }
  if (prim->GetAttr(kKeepDirectionDimFlag) != nullptr) {
    lstm_operator->SetKeepDirectionDimFlag(GetValue<bool>(prim->GetAttr(kKeepDirectionDimFlag)));
  }
  if (prim->GetAttr(kPeepHoleFlag) != nullptr) {
    lstm_operator->SetPeepholeFlag(GetValue<bool>(prim->GetAttr(kPeepHoleFlag)));
  }
  if (prim->GetAttr(kLstmWeightOrderIofcFlag) != nullptr) {
    lstm_operator->SetLstmWeightOrderIofcFlag(GetValue<bool>(prim->GetAttr(kLstmWeightOrderIofcFlag)));
  }
  if (prim->GetAttr(kSequenceLensOnlineFlag) != nullptr) {
    lstm_operator->SetSequenceLensOnlineFlag(GetValue<bool>(prim->GetAttr(kSequenceLensOnlineFlag)));
  }

  if (SetRecurrentDataInfo(cnode, lstm_operator.get()) != RET_OK) {
    MS_LOG(ERROR) << "set lstm data info failed.";
    return RET_ERROR;
  }

  base_operators->push_back(std::move(lstm_operator));
  return RET_OK;
}
REG_MAPPER(Lstm, LstmMapper)
}  // namespace dpico
}  // namespace luojianet_ms
