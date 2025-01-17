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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADAPTER_GRAPH_KERNEL_EXPANDER_WITH_PY_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADAPTER_GRAPH_KERNEL_EXPANDER_WITH_PY_H_
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "backend/optimizer/graph_kernel/core/graph_kernel_expander.h"
#include "ir/func_graph.h"

namespace luojianet_ms::graphkernel {
class PyExpander : public DefaultExpander {
 protected:
  virtual bool ExpandJsonInfo(const AnfNodePtr &node, nlohmann::json *kernel_json);
  FuncGraphPtr CreateExpandFuncGraph(const CNodePtr &node) override;
};

class ComplexOpExpander : public PyExpander {
 protected:
  bool ExpandJsonInfo(const AnfNodePtr &node, nlohmann::json *kernel_json);
};

class GraphKernelExpanderWithPy : public GraphKernelExpander {
 public:
  GraphKernelExpanderWithPy() : GraphKernelExpander() {}
  explicit GraphKernelExpanderWithPy(const std::string &name) : GraphKernelExpander(name) {}
  ~GraphKernelExpanderWithPy() override = default;

 protected:
  ExpanderPtr GetExpander(const AnfNodePtr &node) override;
};

class GraphKernelComplexExpander : public GraphKernelExpanderWithPy {
 public:
  GraphKernelComplexExpander() : GraphKernelExpanderWithPy("graph_kernel_complex_expander") {}
  ~GraphKernelComplexExpander() override = default;
  bool Run(const FuncGraphPtr &func_graph) override;

 protected:
  ExpanderPtr GetExpander(const AnfNodePtr &node) override;
  bool CanExpand(const CNodePtr &node) const override;
};
}  // namespace luojianet_ms::graphkernel
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_GRAPH_KERNEL_ADAPTER_GRAPH_KERNEL_EXPANDER_WITH_PY_H_
