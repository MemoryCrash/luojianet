/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#include "backend/optimizer/ascend/ir_fission/batch_norm_bert_fission.h"
#include "common/backend_common_test.h"
#include "common/py_func_graph_fetcher.h"

namespace luojianet_ms {
namespace opt {
class TestHWBatchNormBertFission : public BackendCommon {
 public:
  TestHWBatchNormBertFission() : get_py_fun_("gtest_input.pre_activate.batch_norm_bert_fission_test", true) {}
  ~TestHWBatchNormBertFission() override = default;

  UT::PyFuncGraphFetcher get_py_fun_;
};

TEST_F(TestHWBatchNormBertFission, test_fused_batch_norm_fission) {
  FuncGraphPtr g = get_py_fun_.CallAndParseRet("test_batch_norm_bert_fission", "before");
  EXPECT_NE(g, nullptr);
  std::vector<int64_t> shp_x{32, 64, 112, 112};
  auto x_abstract = std::make_shared<abstract::AbstractTensor>(kFloat32, shp_x);
  std::vector<int64_t> shp_y{64};
  auto y_abstract = std::make_shared<abstract::AbstractTensor>(kFloat32, shp_y);
  AbstractBasePtrList args_spec_list{x_abstract};
  for (size_t i = 0; i < 4; ++i) {
    args_spec_list.push_back(y_abstract);
  }
  auto kg = GetKernelGraph(g, args_spec_list);
  auto ret = kg->get_return();
  EXPECT_NE(ret, nullptr);
  auto make_tuple0 = ret->input(1);
  EXPECT_NE(make_tuple0, nullptr);
  auto tuple_getitem0 = make_tuple0->cast<CNodePtr>()->input(1);
  EXPECT_NE(tuple_getitem0, nullptr);
  auto make_tuple1 = tuple_getitem0->cast<CNodePtr>()->input(1);
  EXPECT_NE(make_tuple1, nullptr);
  auto tuple_getitem1 = make_tuple1->cast<CNodePtr>()->input(1);
  EXPECT_NE(tuple_getitem1, nullptr);
  auto bn = tuple_getitem1->cast<CNodePtr>()->input(1);
  EXPECT_NE(bn, nullptr);
  auto bn_cnode = bn->cast<CNodePtr>();
  EXPECT_NE(bn_cnode, nullptr);
  auto inputs = bn_cnode->inputs();
  std::vector<AnfNodePtr> new_inputs(inputs.begin(), inputs.begin() + 4);
  bn_cnode->set_inputs(new_inputs);

  auto optimizer = std::make_shared<opt::GraphOptimizer>();
  auto pm = std::make_shared<opt::PassManager>();
  pm->AddPass(std::make_shared<opt::BatchNormBertFission>());
  optimizer->AddPassManager(pm);
  FuncGraphPtr new_graph = optimizer->Optimize(kg);

  FuncGraphPtr g_after = get_py_fun_.CallAndParseRet("test_batch_norm_bert_fission", "after");
  EXPECT_TRUE(CheckEqualGraph(g_after, new_graph));
}

TEST_F(TestHWBatchNormBertFission, test_fused_batch_norm_no_fission) {
  FuncGraphPtr g = get_py_fun_.CallAndParseRet("test_batch_norm_bert_fission", "before");
  EXPECT_NE(g, nullptr);
  std::vector<int64_t> shp_x{32, 64, 112, 112};
  auto x_abstract = std::make_shared<abstract::AbstractTensor>(kFloat32, shp_x);
  std::vector<int64_t> shp_y{64};
  auto y_abstract = std::make_shared<abstract::AbstractTensor>(kFloat32, shp_y);
  AbstractBasePtrList args_spec_list{x_abstract};
  for (size_t i = 0; i < 4; ++i) {
    args_spec_list.push_back(y_abstract);
  }
  auto kg = GetKernelGraph(g, args_spec_list);
  auto origin_graph = std::make_shared<session::KernelGraph>(*kg);

  auto optimizer = std::make_shared<opt::GraphOptimizer>();
  auto pm = std::make_shared<opt::PassManager>();
  pm->AddPass(std::make_shared<opt::BatchNormBertFission>());
  optimizer->AddPassManager(pm);
  FuncGraphPtr new_graph = optimizer->Optimize(kg);

  EXPECT_TRUE(CheckEqualGraph(origin_graph, new_graph));
}
}  // namespace opt
}  // namespace luojianet_ms
