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
#ifndef LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_UNSORTED_SEGMENT_SUM_FISSION_H_
#define LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_UNSORTED_SEGMENT_SUM_FISSION_H_

#include <vector>
#include <memory>
#include "backend/optimizer/common/optimizer.h"
#include "backend/optimizer/common/helper.h"
#include "backend/optimizer/ascend/ascend_helper.h"

namespace luojianet_ms {
namespace opt {
class UnsortSegmentSumFission : public PatternProcessPass {
 public:
  explicit UnsortSegmentSumFission(bool multigraph = true)
      : PatternProcessPass("unsorted_segment_sum_fission", multigraph) {}
  ~UnsortSegmentSumFission() override = default;
  const BaseRef DefinePattern() const override;
  const AnfNodePtr Process(const FuncGraphPtr &, const AnfNodePtr &, const EquivPtr &) const override;

 private:
  CNodePtr CreatePadding(const FuncGraphPtr &graph, const CNodePtr &origin_node, const size_t &pad_dim_size) const;
  CNodePtr CreateUnsortedSegmentSum(const FuncGraphPtr &graph, const CNodePtr &origin_node, const CNodePtr &padding,
                                    const size_t &pad_dim_size) const;
  CNodePtr CreateSlice(const FuncGraphPtr &graph, const CNodePtr &unsort_segment_sum,
                       const CNodePtr &unsorted_segment_sum8) const;
};
}  // namespace opt
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_BACKEND_OPTIMIZER_ASCEND_IR_FISSION_UNSORTED_SEGMENT_SUM_FISSION_H_
