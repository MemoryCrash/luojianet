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

#ifndef LUOJIANET_MS_CCSRC_CXXAPI_SESSION_ACL_OPTION_PARSER_H
#define LUOJIANET_MS_CCSRC_CXXAPI_SESSION_ACL_OPTION_PARSER_H
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <memory>
#include <optional>
#include "include/api/types.h"
#include "include/api/status.h"
#include "include/api/context.h"

namespace luojianet_ms {
class AclModelOptions {
 public:
  explicit AclModelOptions(const std::shared_ptr<Context> &context);
  ~AclModelOptions() = default;
  std::string GenAclOptionsKey() const;
  uint32_t GetDeviceID() const { return device_id_; }
  void RenameInput(const std::vector<std::string> &);

  // return tuple<init_options, build_options>
  std::tuple<std::map<std::string, std::string>, std::map<std::string, std::string>> GenAclOptions() const;
  void SetFirstGraph(bool is_first_graph) noexcept { first_graph_flag_ = is_first_graph; }

 private:
  std::string output_node_;  // todo: at convert.cc::BuildGraph(), no atc options
  // build options
  std::string insert_op_cfg_path_;
  std::string input_format_;
  std::string input_shape_;
  std::string output_type_;
  std::string precision_mode_;
  std::string op_select_impl_mode_;
  std::string fusion_switch_cfg_path_;
  std::string soc_version_ = "Ascend310";
  std::string dynamic_batch_size_;
  std::string dynamic_image_size_;
  std::string buffer_optimize_mode_;
  std::map<int, std::vector<int>> input_shape_map_;
  // other options
  uint32_t device_id_;
  std::optional<bool> first_graph_flag_;
};
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_CXXAPI_SESSION_ACL_OPTION_PARSER_H
