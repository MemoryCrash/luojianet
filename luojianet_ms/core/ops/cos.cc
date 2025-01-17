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
#include "ops/cos.h"

#include <set>
#include <map>
#include <string>

namespace luojianet_ms {
namespace ops {
namespace {
abstract::ShapePtr CosInferShape(const PrimitivePtr &primitive, const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  auto prim_name = primitive->name();
  (void)CheckAndConvertUtils::CheckArgs<abstract::AbstractTensor>(prim_name, input_args, 0);
  auto x = input_args[0]->BuildShape();
  MS_EXCEPTION_IF_NULL(x);
  auto shape_element = x->cast<abstract::ShapePtr>();
  MS_EXCEPTION_IF_NULL(shape_element);
  return shape_element;
}

TypePtr CosInferType(const PrimitivePtr &prim, const std::vector<AbstractBasePtr> &input_args) {
  auto x_dtype = input_args[0]->BuildType();
  (void)CheckAndConvertUtils::CheckTensorTypeValid("x", x_dtype, common_valid_types, prim->name());
  return x_dtype;
}

AbstractBasePtr CosInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                         const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  const int64_t input_num = 1;
  CheckAndConvertUtils::CheckInputArgs(input_args, kGreaterEqual, input_num, primitive->name());

  return abstract::MakeAbstract(CosInferShape(primitive, input_args), CosInferType(primitive, input_args));
}
}  // namespace
REGISTER_PRIMITIVE_EVAL_IMPL(Cos, prim::kPrimCos, CosInfer, nullptr, true);
}  // namespace ops
}  // namespace luojianet_ms
