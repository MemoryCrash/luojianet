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
#include "common/common_test.h"
#include "nnacl/infer/unsorted_segment_sum_infer.h"

namespace luojianet_ms {

class UnsortedSegmentSumInferTest : public luojianet_ms::CommonTest {
 public:
  UnsortedSegmentSumInferTest() {}
};

TEST_F(UnsortedSegmentSumInferTest, UnsortedSegmentSumInferTest0) {
  size_t inputs_size = 3;
  std::vector<TensorC *> inputs(inputs_size, NULL);
  inputs[0] = new TensorC;
  inputs[0]->shape_size_ = 5;
  inputs[0]->shape_[0] = 4;
  inputs[0]->shape_[1] = 5;
  inputs[0]->shape_[2] = 6;
  inputs[0]->shape_[3] = 7;
  inputs[0]->shape_[4] = 8;
  inputs[1] = new TensorC;
  inputs[1]->shape_size_ = 2;
  inputs[2] = new TensorC;
  std::vector<TensorC *> outputs(1, NULL);
  outputs[0] = new TensorC;
  UnsortedSegmentSumParameter *parameter = new UnsortedSegmentSumParameter;
  parameter->segments_num_ = 10;
  int ret = UnsortedSegmentSumInferShape((const TensorC **)inputs.data(), inputs.size(), outputs.data(), outputs.size(),
                                         reinterpret_cast<OpParameter *>(parameter));
  ASSERT_EQ(ret, NNACL_OK);
  ASSERT_EQ(outputs[0]->shape_size_, 4);
  ASSERT_EQ(outputs[0]->shape_[0], 10);
  ASSERT_EQ(outputs[0]->shape_[1], 6);
  ASSERT_EQ(outputs[0]->shape_[2], 7);
  ASSERT_EQ(outputs[0]->shape_[3], 8);
  delete parameter;
  for (size_t i = 0; i < inputs_size; i++) {
    delete inputs[i];
  }
  for (size_t i = 0; i < outputs.size(); i++) {
    delete outputs[i];
  }
}

}  // namespace luojianet_ms
