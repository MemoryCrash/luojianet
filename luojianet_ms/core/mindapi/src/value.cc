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

#include "mindapi/ir/value.h"
#include "mindapi/ir/type.h"
#include "mindapi/ir/abstract.h"
#include "mindapi/src/helper.h"
#include "abstract/abstract_value.h"
#include "ir/anf.h"
#include "ir/dtype/type.h"
#include "ir/value.h"
#include "ir/scalar.h"

namespace luojianet_ms::api {
using ValueImpl = luojianet_ms::Value;
using ValueSequenceImpl = luojianet_ms::ValueSequence;
using ValueTupleImpl = luojianet_ms::ValueTuple;
using StringImmImpl = luojianet_ms::StringImm;
using ScalarImpl = luojianet_ms::Scalar;
using BoolImmImpl = luojianet_ms::BoolImm;
using IntegerImmImpl = luojianet_ms::IntegerImm;
using Int64ImmImpl = luojianet_ms::Int64Imm;
using FloatImmImpl = luojianet_ms::FloatImm;
using FP32ImmImpl = luojianet_ms::FP32Imm;

MIND_API_BASE_IMPL(Value, ValueImpl, Base);

TypePtr Value::type() const {
  auto t = ToRef<ValueImpl>(impl_).type();
  return ToWrapper<Type>(t);
}

AbstractBasePtr Value::ToAbstract() const {
  auto abs = ToRef<ValueImpl>(impl_).ToAbstract();
  return ToWrapper<AbstractBase>(abs);
}

MIND_API_BASE_IMPL(ValueSequence, ValueSequenceImpl, Value);

std::size_t ValueSequence::size() const { return ToRef<ValueSequenceImpl>(impl_).size(); }

std::vector<ValuePtr> ValueSequence::value() const {
  auto &elements = ToRef<ValueSequenceImpl>(impl_).value();
  return ToWrapperVector<Value>(elements);
}

MIND_API_BASE_IMPL(ValueTuple, ValueTupleImpl, ValueSequence);

ValueTuple::ValueTuple(const std::vector<ValuePtr> &elements)
    : ValueSequence(std::make_shared<ValueTupleImpl>(ToImplVector<ValueImpl>(elements))) {}

MIND_API_BASE_IMPL(StringImm, StringImmImpl, Value);

StringImm::StringImm(const std::string &str) : Value(std::make_shared<StringImmImpl>(str)) {}

const std::string &StringImm::value() const { return ToRef<StringImmImpl>(impl_).value(); }

MIND_API_BASE_IMPL(Scalar, ScalarImpl, Value);

MIND_API_BASE_IMPL(BoolImm, BoolImmImpl, Scalar);

BoolImm::BoolImm(bool b) : Scalar(std::make_shared<BoolImmImpl>(b)) {}

bool BoolImm::value() const { return ToRef<BoolImmImpl>(impl_).value(); }

MIND_API_BASE_IMPL(IntegerImm, IntegerImmImpl, Scalar);

MIND_API_BASE_IMPL(Int64Imm, Int64ImmImpl, IntegerImm);

Int64Imm::Int64Imm(int64_t value) : IntegerImm(std::make_shared<Int64ImmImpl>(value)) {}

int64_t Int64Imm::value() const { return ToRef<Int64ImmImpl>(impl_).value(); }

MIND_API_BASE_IMPL(FloatImm, FloatImmImpl, Scalar);

MIND_API_BASE_IMPL(FP32Imm, FP32ImmImpl, FloatImm);

FP32Imm::FP32Imm(float value) : FloatImm(std::make_shared<FP32ImmImpl>(value)) {}

float FP32Imm::value() const { return ToRef<FP32ImmImpl>(impl_).value(); }
}  // namespace luojianet_ms::api
