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

#ifndef LUOJIANET_MS_LITE_SRC_COMMON_PRIM_INNER_H_
#define LUOJIANET_MS_LITE_SRC_COMMON_PRIM_INNER_H_
#include <string>

namespace luojianet_ms {
namespace lite {
enum PRIM_INNER_TYPE : int {
  PRIM_TO_FORMAT = 10000,
  PRIM_RETURN = 10001,
  PRIM_MAKE_TUPLE = 10002,
  PRIM_TUPLE_GET_ITEM = 10003,
  PRIM_LOOP = 10004,
  PRIM_CONSTANT = 10005,
  PRIM_OPENCL_FUSION_ELTWISE = 10006,
#ifdef ENABLE_OPENGL_TEXTURE
  PRIM_GLTEXTURE_TO_OPENCL = 10007,
#endif
};

}  // namespace lite
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_LITE_SRC_COMMON_PRIM_INNER_H_
