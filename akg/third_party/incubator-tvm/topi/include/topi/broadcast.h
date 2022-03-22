/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \brief Broadcast op constructions
 * \file topi/broadcast.h
 */
#ifndef TOPI_BROADCAST_H_
#define TOPI_BROADCAST_H_

#include "topi/detail/broadcast.h"
#include "topi/detail/constant_utils.h"
#include "topi/tags.h"
#include <algorithm>
#include <string>

namespace topi {

/*!
 * \brief Creates an operation that broadcasts a tensor into a compatible
 * shape according to numpy's rules
 *
 * \param t The input tensor
 * \param output_shape The target output shape, must be compatible
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return A Tensor whose op member is a broadcast operation
 */
inline air::Tensor broadcast_to(const air::Tensor &t,
                                 const air::Array<air::Expr> &output_shape,
                                 std::string name = "T_broadcast_to",
                                 std::string tag = kBroadcast) {
  CHECK_GE(output_shape.size(), t->shape.size())
      << "Not a broadcast, output dimensionality smaller than input.\noutput: "
      << output_shape << "\nvs\ninput: " << t;
  auto bh = detail::BroadcastShape(output_shape, t->shape);
  CHECK_EQ(output_shape.size(), bh.common_shape.size());
  for (size_t i = 0; i < output_shape.size(); ++i) {
    CHECK(topi::detail::EqualCheck(output_shape[i], bh.common_shape[i]));
  }
  auto l = [&](air::Array<air::Var> ovars) {
    return t(detail::InputIndexFromBroadcast(ovars, t, bh.vars2, bh.all_vars));
  };
  return air::compute(
      air::Array<air::Expr>(bh.common_shape.begin(), bh.common_shape.end()),
      l, name, tag);
}

#define TOPI_DEFINE_BCAST_OP(Name, ComputeRule)                                \
  inline air::Expr Name(const air::Expr &a, const air::Expr &b) {           \
    ComputeRule;                                                               \
  }                                                                            \
  inline air::Tensor Name(const air::Tensor &A, const air::Tensor &B,       \
                           std::string name = "T_" #Name,                      \
                           std::string tag = kBroadcast) {                     \
    auto l = [](air::Expr a, air::Expr b) { ComputeRule; };                  \
    (void)name.append("_").append(A->op->name);                                \
    (void)name.append("_").append(B->op->name);                                \
    return detail::WithBroadcast(l, A, B, name, tag);                          \
  }                                                                            \
  inline air::Tensor Name(const air::Tensor &A, const air::Expr &B,         \
                           std::string name = "T_" #Name,                      \
                           std::string tag = kElementWise) {                   \
    auto l = [](air::Expr a, air::Expr b) { ComputeRule; };                  \
    (void)name.append("_").append(A->op->name);                                \
    return compute(                                                            \
        A->shape,                                                              \
        [&](const ::air::Array<::air::Var> &i) { return l(A(i), B); }, name, \
        tag);                                                                  \
  }                                                                            \
  inline air::Tensor Name(const air::Expr &A, const air::Tensor &B,         \
                           std::string name = "T_" #Name,                      \
                           std::string tag = kElementWise) {                   \
    auto l = [&](air::Expr a, air::Expr b) { ComputeRule; };                 \
    (void)name.append("_").append(B->op->name);                                \
    return compute(                                                            \
        B->shape,                                                              \
        [&](const ::air::Array<::air::Var> &i) { return l(A, B(i)); }, name, \
        tag);                                                                  \
  }

#define TOPI_DEFINE_OP_OVERLOAD(Name, OpName)                                  \
  inline air::Tensor Name(const air::Tensor &A, const air::Tensor &B) {     \
    return topi::OpName(A, B);                                                 \
  }                                                                            \
  inline air::Tensor Name(const air::Expr &A, const air::Tensor &B) {       \
    return topi::OpName(A, B);                                                 \
  }                                                                            \
  inline air::Tensor Name(const air::Tensor &A, const air::Expr &B) {       \
    return topi::OpName(A, B);                                                 \
  }

/*!
 * \fn logical_and
 * \brief Compute A && B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(logical_and, { return a && b; });
TOPI_DEFINE_OP_OVERLOAD(operator&&, logical_and);

/*!
 * \fn logical_or
 * \brief Compute A || B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(logical_or, { return a || b; });
TOPI_DEFINE_OP_OVERLOAD(operator||, logical_or);

/*!
 * \fn add
 * \brief Compute A + B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(add, { return a + b; });
TOPI_DEFINE_OP_OVERLOAD(operator+, add);

/*!
 * \fn subtract
 * \brief Compute A - B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(subtract, { return a - b; });
TOPI_DEFINE_OP_OVERLOAD(operator-, subtract);

/*!
 * \fn multiply
 * \brief Compute A * B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(multiply, { return a * b; });
TOPI_DEFINE_OP_OVERLOAD(operator*, multiply);

/*!
 * \fn divide
 * \brief Compute A / B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(divide, { return div(a, b); });

/*!
 * \fn floor divide
 * \brief Compute floor(A / B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(floor_divide, {
  if (a.type().is_int() || a.type().is_uint()) {
    return floordiv(a, b);
  } else {
    return floor(div(a, b));
  }
});

/*!
 * \fn mod
 * \brief Compute A % B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(mod, { return truncmod(a, b); });

/*!
 * \fn floor mod
 * \brief Compute A - floor_div(A, B) * B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(floor_mod, {
  if (a.type().is_int() || a.type().is_uint()) {
    return floormod(a, b);
  } else {
    return a - floor_divide(a, b) * b;
  }
});

/*!
 * \fn maximum
 * \brief Compute maximum(A, B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(maximum, { return air::max(a, b); });

/*!
 * \fn minimum
 * \brief Compute minimum(A, B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(minimum, { return air::min(a, b); });

/*!
 * \fn power
 * \brief Compute power(A, B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(power, { return air::pow(a, b); });

/*!
 * \fn left_shift
 * \brief Compute A << B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(left_shift, { return a << b; });
TOPI_DEFINE_OP_OVERLOAD(operator<<, left_shift);

/*!
 * \fn right_shift
 * \brief Compute A >> B with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(right_shift, { return a >> b; });
TOPI_DEFINE_OP_OVERLOAD(operator>>, right_shift);

/*!
 * \fn greater
 * \brief Compute (A > B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(greater, { return (a > b); });

/*!
 * \fn less
 * \brief Compute (A < B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(less, { return (a < b); });

/*!
 * \fn equal
 * \brief Compute (A == B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(equal, { return (a == b); });

/*!
 * \fn not_equal
 * \brief Compute (A != B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(not_equal, { return (a != b); });

/*!
 * \fn greater_equal
 * \brief Compute (A >= B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(greater_equal, { return (a >= b); });

/*!
 * \fn less_equal
 * \brief Compute (A <= B) with auto-broadcasting.
 *
 * \param A The first tensor, or Expr
 * \param B The second tensor, or Expr
 * \param name The name of the operation
 * \param tag The tag to mark the operation
 *
 * \return The result.
 */
TOPI_DEFINE_BCAST_OP(less_equal, { return (a <= b); });

} // namespace topi

#endif // TOPI_BROADCAST_H_