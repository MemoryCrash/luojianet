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

#ifndef DPICO_CHECKER_OP_CHECKER_H_
#define DPICO_CHECKER_OP_CHECKER_H_

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include "include/errorcode.h"
#include "ir/anf.h"
#include "ops/op_utils.h"
using luojianet_ms::lite::RET_ERROR;
using luojianet_ms::lite::RET_OK;
using luojianet_ms::lite::STATUS;
namespace luojianet_ms {
namespace dpico {
class OpChecker {
 public:
  explicit OpChecker(std::string node_name) : name(std::move(node_name)) {}
  virtual ~OpChecker() = default;
  virtual bool Check(CNodePtr op, int32_t output_num, luojianet_ms::Format format) = 0;

 protected:
  const std::string name;
};

STATUS GetWidth(const std::vector<int64_t> &shape, luojianet_ms::Format format, int64_t *width);
STATUS GetTensorChannel(const std::vector<int64_t> &shape, luojianet_ms::Format format, int64_t *channel);
STATUS GetVectorChannel(const std::vector<int64_t> &shape, int64_t *channel);
bool HasOfflineData(const AnfNodePtr &node);
bool CheckInputW(const CNodePtr &op, size_t index, luojianet_ms::Format format, int limit_w);

class OpCheckerRegistry {
 public:
  OpCheckerRegistry() = default;
  ~OpCheckerRegistry();

  static OpCheckerRegistry *GetInstance();

  OpChecker *GetOpChecker(const std::string &type);

  std::unordered_map<std::string, OpChecker *> checkers;
};

class OpCheckerRegistrar {
 public:
  OpCheckerRegistrar(const std::string &type, OpChecker *parser) {
    OpCheckerRegistry::GetInstance()->checkers[type] = parser;
  }
  ~OpCheckerRegistrar() = default;
};
}  // namespace dpico
}  // namespace luojianet_ms

#endif  // DPICO_CHECKER_OP_CHECKER_H_
