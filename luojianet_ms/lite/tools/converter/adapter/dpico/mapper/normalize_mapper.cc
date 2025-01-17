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

#include "mapper/normalize_mapper.h"
#include <memory>
#include <utility>
#include <vector>
#include "common/op_attr.h"
#include "ops/op_utils.h"
#include "op/normalize_operator.h"

namespace luojianet_ms {
namespace dpico {
namespace {
STATUS SetNormalizeDataInfo(const CNodePtr &cnode, mapper::NormalizeOperator *normalize_operator) {
  if (normalize_operator == nullptr) {
    MS_LOG(ERROR) << "normalize_operator is nullptr.";
    return RET_ERROR;
  }
  for (size_t i = 1; i < cnode->inputs().size(); i++) {
    AnfNodePtr input_node = cnode->input(i);
    if (utils::isa<CNode>(input_node)) {
      MS_LOG(INFO) << "cnode don't have blobs";
      continue;
    }
    if (utils::isa<ParameterPtr>(input_node)) {
      auto input_param_node = input_node->cast<ParameterPtr>();
      if (!input_param_node->has_default()) {
        MS_LOG(INFO) << "graph input don't have blobs";
        continue;
      }
      auto tensor_info = std::dynamic_pointer_cast<tensor::Tensor>(input_param_node->default_param());
      if (tensor_info != nullptr && tensor_info->DataSize() != 0) {
        auto raw_datas = static_cast<float *>(tensor_info->data_c());
        auto elem_count = tensor_info->DataSize();
        normalize_operator->SetNormScaleVec(std::vector<float>(raw_datas, raw_datas + elem_count));
      } else {
        MS_LOG(ERROR) << "tensor_info is nullptr, or DataSize equals zero. " << input_param_node->fullname_with_scope();
        return RET_ERROR;
      }
    }
  }
  return RET_OK;
}
}  // namespace
STATUS NormalizeMapper::Map(const CNodePtr &cnode, std::vector<BaseOperatorPtr> *base_operators,
                            const PrimitivePtr &prim, const CNodePtrList &output_cnodes) {
  if (base_operators == nullptr) {
    MS_LOG(ERROR) << "base_operators is nullptr.";
    return RET_ERROR;
  }

  auto normalize_operator = std::make_unique<mapper::NormalizeOperator>();
  if (normalize_operator == nullptr) {
    MS_LOG(ERROR) << "normalize_operator is nullptr.";
    return RET_ERROR;
  }

  if (SetCommonAttr(cnode, normalize_operator.get(), output_cnodes) != RET_OK) {
    MS_LOG(ERROR) << "set common attr failed. " << cnode->fullname_with_scope();
    return RET_ERROR;
  }

  normalize_operator->SetOpType(mapper::OpType::NORMALIZE);
  if (prim->GetAttr(dpico::kAcrossSpatial) != nullptr) {
    normalize_operator->SetNormAcrossSpatial(GetValue<bool>(prim->GetAttr(dpico::kAcrossSpatial)));
  }
  if (prim->GetAttr(dpico::kChannelShared) != nullptr) {
    normalize_operator->SetNormChannelShared(GetValue<bool>(prim->GetAttr(dpico::kChannelShared)));
  }
  if (prim->GetAttr(dpico::kSqrtA) != nullptr) {
    normalize_operator->SetNormAlpha(GetValue<float>(prim->GetAttr(dpico::kSqrtA)));
  }
  if (prim->GetAttr(ops::kEps) != nullptr) {
    normalize_operator->SetNormEps(GetValue<float>(prim->GetAttr(ops::kEps)));
  }

  if (SetNormalizeDataInfo(cnode, normalize_operator.get()) != RET_OK) {
    MS_LOG(ERROR) << "set normalize data info failed.";
    return RET_ERROR;
  }

  base_operators->push_back(std::move(normalize_operator));
  return RET_OK;
}
REG_MAPPER(Normalize, NormalizeMapper)
}  // namespace dpico
}  // namespace luojianet_ms
