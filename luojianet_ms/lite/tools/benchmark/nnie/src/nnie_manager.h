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

#ifndef LUOJIANET_MS_LITE_SRC_RUNTIME_AGENT_NNIE_NNIE_MANAGER_H_
#define LUOJIANET_MS_LITE_SRC_RUNTIME_AGENT_NNIE_NNIE_MANAGER_H_
#include <vector>
#include "include/errorcode.h"
#include "include/api/types.h"
#include "src/nnie_common.h"

namespace luojianet_ms {
namespace nnie {
class NNIEManager {
 public:
  static NNIEManager *GetInstance() {
    static NNIEManager manager;
    return &manager;
  }

  NNIEManager() {}

  ~NNIEManager() {}

  int Init(char *model_buf, int size, const std::vector<luojianet_ms::MSTensor> &inputs);

  int CfgInit(int max_roi_num, int step, const std::vector<int> &core_id);

  void SetInputNum(int max_input_num);

  int FillData(std::vector<luojianet_ms::MSTensor> *inputs, unsigned int seg_id);

  int Run(std::vector<luojianet_ms::MSTensor> *outputs, unsigned int seg_id,
          const std::vector<std::vector<int64_t>> &outputs_shape);

  void Release();

 private:
  int GetOutputData(std::vector<luojianet_ms::MSTensor> *outputs, const std::vector<std::vector<int64_t>> &outputs_shape,
                    bool run_box = false);
  int FillRoiPooling(luojianet_ms::MSTensor *input);
  char *wk_model_ = nullptr;

  int model_size_ = 0;

  NnieRunCfg nnie_cfg_;
};
}  // namespace nnie
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_LITE_SRC_RUNTIME_AGENT_NNIE_NNIE_MANAGER_H_
