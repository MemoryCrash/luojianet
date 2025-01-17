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

#ifndef GE_COMMON_FORMATS_FORMATS_H_
#define GE_COMMON_FORMATS_FORMATS_H_

#include <memory>
#include <vector>

#include "common/formats/format_transfers/datatype_transfer.h"
#include "register/register_format_transfer.h"
#include "external/graph/types.h"
#include "framework/common/ge_inner_error_codes.h"
#include "graph/ge_tensor.h"

namespace ge {
namespace formats {
/**
 * Convert the data format, and put the converted format and length in the result
 * @param args
 * @param result
 * @return
 */
Status TransFormat(const TransArgs &args, TransResult &result);

Status TransShape(Format src_format, const std::vector<int64_t> &src_shape, DataType data_type,
                  Format dst_format, std::vector<int64_t> &dst_shape);

Status TransDataType(const CastArgs &args, TransResult &result);

bool IsTransFormatSupport(const TransArgs &args);

bool IsTransDataTypeSupport(const CastArgs &args);
}  // namespace formats
}  // namespace ge
#endif  // GE_COMMON_FORMATS_FORMATS_H_
