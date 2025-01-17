# Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
# Copyright 2021, 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
import numpy as np

import luojianet_ms
import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.ops import operations as P

context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.unsorted_segment_sum = P.UnsortedSegmentSum()
        self.num_segments = 3

    def call(self, x, segment_ids):
        x = self.unsorted_segment_sum(x, segment_ids, self.num_segments)
        return x


def test_net():
    input_x = np.random.randn(3, 39, 1).astype(np.float32)
    segment_ids = Tensor([0, 1, 2], luojianet_ms.int32)
    net = Net()
    output = net(Tensor(input_x), segment_ids)
    print("result", output.asnumpy())


if __name__ == "__main__":
    test_net()
