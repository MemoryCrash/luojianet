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

#include "backend/kernel_compiler/rts/label_set.h"
#include "runtime/stream.h"
#include "runtime/device/ascend/ge_runtime/task_info.h"
#include "backend/session/anf_runtime_algorithm.h"

using luojianet_ms::ge::model_runner::LabelSetTaskInfo;
using LabelSetTaskInfoPtr = std::shared_ptr<LabelSetTaskInfo>;

namespace luojianet_ms {
namespace kernel {
LabelSetKernel::LabelSetKernel() { label_ = 0; }

LabelSetKernel::~LabelSetKernel() {}

bool LabelSetKernel::Init(const AnfNodePtr &anf_node) {
  MS_EXCEPTION_IF_NULL(anf_node);
  MS_LOG(INFO) << "LabelSetKernel init";
  auto cnode = anf_node->cast<CNodePtr>();
  if (!AnfAlgo::HasNodeAttr(kAttrLabelIndex, cnode)) {
    MS_LOG(EXCEPTION) << "LabelSetKernel has no attr label_index";
  }
  auto primitive = AnfAlgo::GetCNodePrimitive(anf_node);
  MS_EXCEPTION_IF_NULL(primitive);
  label_ = GetValue<uint32_t>(primitive->GetAttr(kAttrLabelIndex));
  MS_LOG(INFO) << "LabelSetKernel get attr label:" << label_;
  return true;
}

bool LabelSetKernel::Launch(const std::vector<AddressPtr> & /*inputs*/, const std::vector<AddressPtr> & /*workspace*/,
                            const std::vector<AddressPtr> & /*outputs*/, void * /*stream_ptr*/) {
  MS_LOG(INFO) << "LabelSetKernel launch";
  return true;
}

std::vector<TaskInfoPtr> LabelSetKernel::GenTask(const std::vector<AddressPtr> &, const std::vector<AddressPtr> &,
                                                 const std::vector<AddressPtr> &, uint32_t stream_id) {
  MS_LOG(INFO) << "LabelSetKernel GenTask label:" << label_ << ", stream id:" << stream_id;
  std::vector<TaskInfoPtr> task_info_list;
  std::shared_ptr<LabelSetTaskInfo> task_info_ptr = std::make_shared<LabelSetTaskInfo>(unique_name_, stream_id, label_);
  MS_EXCEPTION_IF_NULL(task_info_ptr);
  task_info_list.emplace_back(task_info_ptr);
  return task_info_list;
}
}  // namespace kernel
}  // namespace luojianet_ms
