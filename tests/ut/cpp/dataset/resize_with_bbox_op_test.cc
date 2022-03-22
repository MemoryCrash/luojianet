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

#include "common/bboxop_common.h"
#include "minddata/dataset/kernels/image/resize_with_bbox_op.h"
#include "utils/log_adapter.h"

using namespace luojianet_ms::dataset;
using luojianet_ms::LogStream;
using luojianet_ms::ExceptionType::NoExceptionType;
using luojianet_ms::MsLogLevel::INFO;

const bool kSaveExpected = false;
const char kOpName[] = "resize_with_bbox_c";

class MindDataTestResizeWithBBoxOp : public UT::CVOP::BBOXOP::BBoxOpCommon {
 protected:
  MindDataTestResizeWithBBoxOp() : BBoxOpCommon() {}
};
TEST_F(MindDataTestResizeWithBBoxOp, TestOp) {
  MS_LOG(INFO) << "Doing testResizeWithBBox.";
  // resize
  TensorTable results;
  std::unique_ptr<ResizeWithBBoxOp> op(new ResizeWithBBoxOp(500));
  for (const auto &tensor_row_ : images_and_annotations_) {
    // selected a tensorRow
    TensorRow output_row;
    Status s = op->Compute(tensor_row_, &output_row);
    EXPECT_TRUE(s.IsOk());
    results.push_back(output_row);
  }
  if (kSaveExpected) {
    SaveImagesWithAnnotations(FileType::kExpected, std::string(kOpName), results);
  }
  SaveImagesWithAnnotations(FileType::kActual, std::string(kOpName), results);
  if (!kSaveExpected) {
    CompareActualAndExpected(std::string(kOpName));
  }

  MS_LOG(INFO) << "testResizeWithBBox end.";
}
