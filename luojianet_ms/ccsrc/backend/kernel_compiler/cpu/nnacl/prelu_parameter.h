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

#ifndef LUOJIANET_MS_NNACL_PRELU_PARAMETER_H_
#define LUOJIANET_MS_NNACL_PRELU_PARAMETER_H_

#include "nnacl/op_base.h"
typedef struct PReluParameter {
  // Primitive parameter
  OpParameter op_parameter_;
  // other parameter
  bool channelShared;
  int channel_num_;
  int input_num_;
} PReluParameter;

#endif  // LUOJIANET_MS_NNACL_PRELU_PARAMETER_H_