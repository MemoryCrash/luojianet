/**
 * This is the C++ adaptation and derivative work of Myia (https://github.com/mila-iqia/myia/).
 *
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

#include "pipeline/jit/validator.h"

#include <memory>
#include <mutex>

#include "ir/manager.h"
#include "ir/dtype.h"
#include "pipeline/jit/static_analysis/prim.h"
#include "pipeline/jit/parse/resolve.h"

namespace luojianet_ms {
namespace validator {
using luojianet_ms::abstract::AbstractBase;
using luojianet_ms::abstract::AbstractClass;
using luojianet_ms::abstract::AbstractCSRTensor;
using luojianet_ms::abstract::AbstractError;
using luojianet_ms::abstract::AbstractFunction;
using luojianet_ms::abstract::AbstractJTagged;
using luojianet_ms::abstract::AbstractList;
using luojianet_ms::abstract::AbstractRef;
using luojianet_ms::abstract::AbstractRowTensor;
using luojianet_ms::abstract::AbstractScalar;
using luojianet_ms::abstract::AbstractSparseTensor;
using luojianet_ms::abstract::AbstractTensor;
using luojianet_ms::abstract::AbstractTuple;
using luojianet_ms::abstract::AbstractType;

void ValidateOperation(const AnfNodePtr &node) {
  if (!IsValueNode<Primitive>(node)) {
    return;
  }

  // Primitive must in whitelist
  auto prim = GetValueNode<PrimitivePtr>(node);
  MS_EXCEPTION_IF_NULL(prim);
  if (abstract::IsInWhiteList(prim)) {
    return;
  }
  if (prim->HasAttr("is_load")) {
    return;
  }
  if (prim->HasPyEvaluator()) {
    MS_LOG(DEBUG) << "Primitive " << prim->name() << " has python evaluator.";
    return;
  }
  if (prim->prim_type() == PrimType::kPrimTypePyCheck) {
    MS_LOG(DEBUG) << "Primitive " << prim->name() << " has python inference checking method.";
    return;
  }
  if (prim->name() == "fake_bprop") {
    MS_LOG(EXCEPTION) << "Illegal primitive: " << GetValue<std::string>(prim->GetAttr("info"));
  }

  MS_LOG(EXCEPTION) << "Illegal primitive: " << prim->name();
}

bool CheckAbstractScalar(const AnfNodePtr &node) {
  MS_EXCEPTION_IF_NULL(node);
  AbstractBasePtr abstract = node->abstract();
  if (abstract->isa<AbstractScalar>()) {
    TypePtr type = abstract->GetTypeTrack();
    MS_EXCEPTION_IF_NULL(type);
    if (type->isa<EnvType>()) {
      MS_LOG(EXCEPTION) << "Illegal type in the graph: " << abstract->ToString() << ", node: " << node->DebugString();
    }
    if (type->isa<Problem>() || type->isa<External>()) {
      // Only allow string type from external.
      if (!IsValueNode<StringImm>(node)) {
        // Validate a type.
        MS_LOG(EXCEPTION) << "Illegal type in the graph: " << abstract->ToString() << ", node: " << node->DebugString();
      }
    }
    return true;
  }
  return false;
}

void ValidateAbstract(const AnfNodePtr &node) {
  if (node == nullptr) {
    MS_LOG(DEBUG) << "Node to validate is invalid";
    return;
  }
  AbstractBasePtr abstract = node->abstract();
  if (abstract == nullptr) {
    MS_LOG(DEBUG) << "Abstract is null in node: " << node->DebugString();
    return;
  }
  if (abstract->isa<AbstractClass>() || abstract->isa<AbstractJTagged>()) {
    // Validate a type.
    MS_LOG(EXCEPTION) << "Illegal type in the graph: " << abstract->ToString() << ", node: " << node->DebugString();
  }
  if (CheckAbstractScalar(node)) {
    return;
  }
  if (abstract->isa<AbstractError>()) {
    // NOTICE: validate dead code?
    MS_LOG(DEBUG) << "AbstractError in the graph: " << abstract->ToString();
    return;
  }
  bool is_legal_abstract = abstract->isa<AbstractType>() || abstract->isa<AbstractFunction>() ||
                           abstract->isa<AbstractTuple>() || abstract->isa<AbstractList>() ||
                           abstract->isa<AbstractTensor>() || abstract->isa<AbstractRowTensor>() ||
                           abstract->isa<AbstractSparseTensor>() || abstract->isa<AbstractCSRTensor>() ||
                           abstract->isa<abstract::AbstractRefKey>() || abstract->isa<AbstractRef>() ||
                           abstract->isa<abstract::AbstractNone>() || abstract->isa<abstract::AbstractMonad>();
  if (is_legal_abstract) {
    return;
  }

  // Other types show exception
  MS_LOG(EXCEPTION) << "Illegal type in the graph: " << abstract->ToString();
}

void ValidateValueNode(const AnfNodePtr &node) {
  if (node == nullptr) {
    MS_LOG(DEBUG) << "Node to validate is invalid";
    return;
  }
  // InterpretedNode should be consumed during compile, not left to Runtime.
  if (IsValueNode<parse::InterpretedObject>(node)) {
    MS_LOG(EXCEPTION)
      << "Should not use Python object in runtime, node: " << node->DebugString()
      << "\n\nWe suppose all nodes generated by JIT Fallback would not return to outside of graph. "
      << "For more information about JIT Fallback, please refer to the FAQ at https://www.luojianet_ms.cn.";
  }
}

void Validate(const FuncGraphPtr &fg) {
  FuncGraphManagerPtr mgr = Manage(fg, false);
  MS_EXCEPTION_IF_NULL(mgr);
  AnfNodeSet &all_nodes = mgr->all_nodes();
  for (const auto &node : all_nodes) {
    ValidateOperation(node);
    ValidateValueNode(node);
  }
  for (const auto &node : all_nodes) {
    ValidateAbstract(node);
  }
}
}  // namespace validator
}  // namespace luojianet_ms