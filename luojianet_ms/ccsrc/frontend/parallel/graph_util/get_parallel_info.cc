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

#include "frontend/parallel/graph_util/get_parallel_info.h"

#include <memory>
#include <string>
#include <vector>

#include "ir/func_graph.h"
#include "frontend/parallel/ops_info/operator_info.h"
#include "frontend/parallel/graph_util/graph_info.h"
#include "frontend/parallel/strategy.h"
#include "frontend/parallel/tensor_layout/tensor_layout.h"

namespace luojianet_ms {
namespace parallel {
py::dict GetParameterLayoutFromGraph(const FuncGraphPtr &graph) {
  MS_EXCEPTION_IF_NULL(graph);
  py::dict dict;
  std::vector<AnfNodePtr> graph_params = graph->parameters();

  for (auto para : graph_params) {
    std::string name = std::static_pointer_cast<Parameter>(para)->name();
    auto tensor_layout = para->user_data<parallel::TensorLayout>();
    if (tensor_layout == nullptr) {
      MS_LOG(INFO) << "GetParameterLayout nullptr name = " << name;
    } else {
      const auto &device_arrangement = tensor_layout->device_arrangement().array();
      const auto &tensor_map = tensor_layout->tensor_map().array();
      const auto &slice_shape = tensor_layout->slice_shape().array();
      int64_t field_size = tensor_layout->get_field_size();
      bool uniform_split = tensor_layout->uniform_split();
      const std::string &opt_shard_group = tensor_layout->opt_shard_group();
      py::tuple layout =
        py::make_tuple(device_arrangement, tensor_map, slice_shape, field_size, uniform_split, opt_shard_group);
      dict[py::str(name)] = layout;
      MS_LOG(INFO) << "GetParameterLayout name = " << name << ", layout " << tensor_layout->ToString();
    }
  }
  return dict;
}

py::dict GetParameterLayoutFromResource(const pipeline::ResourcePtr &resource) {
  py::dict dict;
  const auto &layout_map = resource->get_layout_map();
  for (auto iter = layout_map.begin(); iter != layout_map.end(); ++iter) {
    auto name = iter->first;
    auto layout = iter->second;
    const auto &device_arrangement = layout->get_device_arrangement();
    const auto &tensor_map = layout->get_tensor_map();
    const auto &slice_shape = layout->get_slice_shape();
    int64_t field_size = layout->get_field_size();
    bool uniform_split = layout->get_uniform_split();
    const std::string &opt_shard_group = layout->get_opt_shard_group();
    py::tuple layout_tuple =
      py::make_tuple(device_arrangement, tensor_map, slice_shape, field_size, uniform_split, opt_shard_group);
    dict[py::str(name)] = layout_tuple;
  }
  return dict;
}

py::dict GetAllreduceFusion(const FuncGraphPtr &graph) {
  MS_EXCEPTION_IF_NULL(graph);
  py::dict dict;
  auto allreduce_prim_list = FindPrimtive(graph, ALL_REDUCE);

  for (auto prim : allreduce_prim_list) {
    auto name_ptr = prim->GetAttr("parameter");
    auto fusion_ptr = prim->GetAttr("fusion");
    if (fusion_ptr == nullptr) {
      MS_LOG(EXCEPTION) << "fusion_ptr is nullptr";
    } else if (name_ptr == nullptr) {
      continue;
    }
    if (!name_ptr->isa<StringImm>()) {
      MS_LOG(EXCEPTION) << "name is not StringImm";
    }
    auto name = name_ptr->cast<StringImmPtr>()->value();
    if (!fusion_ptr->isa<Int64Imm>()) {
      MS_LOG(EXCEPTION) << "fusion is not Int64Imm";
    }
    int64_t fusion = fusion_ptr->cast<Int64ImmPtr>()->value();
    dict[py::str(name)] = fusion;
  }
  return dict;
}

// In pipeline parallel mode, many parameters are not used and need to be deleted
py::list GetParallelParameterNameListFromGraph(const FuncGraphPtr &graph) {
  MS_EXCEPTION_IF_NULL(graph);

  py::list parallel_parameter_name_list;
  std::vector<AnfNodePtr> graph_params = graph->parameters();

  for (auto param : graph_params) {
    auto param_ptr = std::static_pointer_cast<Parameter>(param);
    MS_EXCEPTION_IF_NULL(param_ptr);
    std::string name = param_ptr->name();
    parallel_parameter_name_list.append(name);
  }
  return parallel_parameter_name_list;
}

py::list GetParallelParameterNameListFromResource(const pipeline::ResourcePtr &resource) {
  auto &layout_map = resource->get_layout_map();
  py::list parallel_parameter_name_list;
  for (auto iter = layout_map.begin(); iter != layout_map.end(); ++iter) {
    auto name = iter->first;
    parallel_parameter_name_list.append(name);
  }
  return parallel_parameter_name_list;
}
}  // namespace parallel
}  // namespace luojianet_ms
