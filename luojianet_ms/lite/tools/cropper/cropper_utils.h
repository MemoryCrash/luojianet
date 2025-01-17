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

#ifndef LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_UTILS_H
#define LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_UTILS_H
#include <string>
namespace luojianet_ms {
namespace lite {
namespace cropper {
int ValidFileSuffix(const std::string &filename, const std::string &suffix);
int ValidFile(std::ifstream &in_file, const std::string &file_path);
}  // namespace cropper
}  // namespace lite
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_UTILS_H
