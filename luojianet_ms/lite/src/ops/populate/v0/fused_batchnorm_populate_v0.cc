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

#include "schema/model_v0_generated.h"
#include "src/ops/populate/populate_register.h"
#include "nnacl/batchnorm_parameter.h"

namespace luojianet_ms {
namespace lite {
namespace {
OpParameter *PopulateFusedBatchNormParameter(const void *prim) {
  auto *primitive = static_cast<const schema::v0::Primitive *>(prim);
  MS_ASSERT(primitive != nullptr);
  auto fused_batchnorm_prim = primitive->value_as_FusedBatchNorm();
  if (fused_batchnorm_prim == nullptr) {
    MS_LOG(ERROR) << "fused_batchnorm_prim is nullptr";
    return nullptr;
  }
  auto *batch_norm_param = reinterpret_cast<BatchNormParameter *>(malloc(sizeof(BatchNormParameter)));
  if (batch_norm_param == nullptr) {
    MS_LOG(ERROR) << "malloc BatchNormParameter failed.";
    return nullptr;
  }
  memset(batch_norm_param, 0, sizeof(BatchNormParameter));
  batch_norm_param->op_parameter_.type_ = schema::PrimitiveType_FusedBatchNorm;

  batch_norm_param->epsilon_ = fused_batchnorm_prim->epsilon();
  batch_norm_param->momentum_ = fused_batchnorm_prim->momentum();
  batch_norm_param->fused_ = true;
  return reinterpret_cast<OpParameter *>(batch_norm_param);
}
}  // namespace

Registry g_fusedBatchNormV0ParameterRegistry(schema::v0::PrimitiveType_FusedBatchNorm, PopulateFusedBatchNormParameter,
                                             SCHEMA_V0);
}  // namespace lite
}  // namespace luojianet_ms
