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

#ifndef LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_FLAGS_H
#define LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_FLAGS_H

#include <string>
#include "tools/common/flag_parser.h"
#include "ir/dtype/type_id.h"

namespace luojianet_ms {
namespace lite {
namespace cropper {
class CropperFlags : public virtual luojianet_ms::lite::FlagParser {
 public:
  CropperFlags();
  ~CropperFlags() override = default;
  int Init(int argc, const char **argv);

  STATUS CheckPackageFile();
  STATUS CheckModelFile();
  STATUS CheckConfigFile();
  STATUS CheckOutputFile();

 public:
  std::string package_file_;
  std::string model_file_;
  std::string model_folder_path_;
  std::string config_file_;
  std::string output_file_;
};
}  // namespace cropper
}  // namespace lite
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_LITE_TOOLS_CROPPER_CROPPER_FLAGS_H
