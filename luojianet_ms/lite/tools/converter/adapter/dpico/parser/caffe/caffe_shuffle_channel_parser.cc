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

#include "parser/caffe/caffe_shuffle_channel_parser.h"
#include <memory>
#include <vector>
#include "common/op_attr.h"
#include "ops/custom.h"

namespace luojianet_ms {
namespace lite {
ops::PrimitiveC *CaffeShuffleChannelParser::Parse(const caffe::LayerParameter &proto,
                                                  const caffe::LayerParameter &weight) {
  auto prim = std::make_unique<ops::Custom>();
  if (prim == nullptr) {
    MS_LOG(ERROR) << "prim is nullptr.";
    return nullptr;
  }
  prim->set_type("ShuffleChannel");

  if (proto.has_shuffle_channel_param()) {
    const caffe::ShuffleChannelParameter &shuffle_channel_parameter = proto.shuffle_channel_param();
    if (shuffle_channel_parameter.has_group()) {
      prim->AddAttr(ops::kGroup, MakeValue<uint32_t>(shuffle_channel_parameter.group()));
    }
  }

  return prim.release();
}

CaffeNodeRegistrar g_caffeShuffleChannelParser("ShuffleChannel", new CaffeShuffleChannelParser());
}  // namespace lite
}  // namespace luojianet_ms
