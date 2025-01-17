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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_COMMON_CONST_INPUT_TO_ATTR_REGISTRY_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_COMMON_CONST_INPUT_TO_ATTR_REGISTRY_H_
#include <string>

#include "utils/hash_map.h"
#include "utils/hash_set.h"
#include "utils/ms_utils.h"

namespace luojianet_ms {
namespace opt {
class ConstInputToAttrInfoRegister {
 public:
  explicit ConstInputToAttrInfoRegister(const std::string &op_name = "") : op_name_(op_name) {}
  virtual ~ConstInputToAttrInfoRegister() = default;

  ConstInputToAttrInfoRegister &SetConstInputToAttr(size_t input_index) {
    (void)input_attr_set_.insert(input_index);
    return *this;
  }

  ConstInputToAttrInfoRegister &SetConstInputToAttr(const luojianet_ms::HashSet<size_t> &input_attr_set) {
    (void)input_attr_set_.insert(input_attr_set.begin(), input_attr_set.end());
    return *this;
  }

  const luojianet_ms::HashSet<size_t> &GetConstInputAttrInfo() const { return input_attr_set_; }
  const std::string &GetOpName() const { return op_name_; }

 private:
  std::string op_name_;
  luojianet_ms::HashSet<size_t> input_attr_set_;
};

class ConstInputToAttrInfoRegistry {
 public:
  static ConstInputToAttrInfoRegistry &Instance();
  void Register(const ConstInputToAttrInfoRegister &reg);
  void Register(const std::string &op_name, const luojianet_ms::HashSet<size_t> &input_attr_set);
  bool GetRegisterByOpName(const std::string &op_name, ConstInputToAttrInfoRegister *reg) const;

 private:
  ConstInputToAttrInfoRegistry();
  ~ConstInputToAttrInfoRegistry() = default;
  DISABLE_COPY_AND_ASSIGN(ConstInputToAttrInfoRegistry)
  luojianet_ms::HashMap<std::string, ConstInputToAttrInfoRegister> op_input_to_attr_map_;
};

struct ConstInputToAttrInfoReceiver {
  // Note: This is implicitly converting constructor
  ConstInputToAttrInfoReceiver(const ConstInputToAttrInfoRegister &reg) {  // NOLINT(runtime/explicit)
    ConstInputToAttrInfoRegistry::Instance().Register(reg);
  }
};
}  // namespace opt

#define REG_CONST_INPUT_TO_ATTR(op_name) REG_CONST_INPUT_TO_ATTR_UNIQ_HELPER(__COUNTER__, op_name)
#define REG_CONST_INPUT_TO_ATTR_UNIQ_HELPER(ctr, op_name) REG_CONST_INPUT_TO_ATTR_UNIQ(ctr, op_name)
#define REG_CONST_INPUT_TO_ATTR_UNIQ(ctr, op_name)                                                                     \
  static ::luojianet_ms::opt::ConstInputToAttrInfoReceiver g_const_input_to_attr_register_##ctr __attribute__((unused)) = \
    ::luojianet_ms::opt::ConstInputToAttrInfoRegister(op_name)
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_COMMON_CONST_INPUT_TO_ATTR_REGISTRY_H_
