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
import pytest

import luojianet_ms
import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.ops.operations import _inner_ops as ops

context.set_context(mode=context.GRAPH_MODE, device_target="GPU")


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.stitch = ops.DynamicStitch()

    def call(self, indices, data):
        return self.stitch(indices, data)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_net_int32():
    """
    Feature: ALL TO ALL
    Description: test cases for dynamicstitch.
    Expectation: the result match expected array.
    """
    x1 = Tensor([6], luojianet_ms.int32)
    x2 = Tensor(np.array([4, 1]), luojianet_ms.int32)
    x3 = Tensor(np.array([[5, 2], [0, 3]]), luojianet_ms.int32)
    y1 = Tensor(np.array([[61, 62]]), luojianet_ms.int32)
    y2 = Tensor(np.array([[41, 42], [11, 12]]), luojianet_ms.int32)
    y3 = Tensor(np.array([[[51, 52], [21, 22]], [[1, 2], [31, 32]]]), luojianet_ms.int32)
    expected = np.array([[1, 2], [11, 12], [21, 22],
                         [31, 32], [41, 42], [51, 52], [61, 62]]).astype(np.int32)

    indices = [x1, x2, x3]
    data = [y1, y2, y3]
    net = Net()
    output = net(indices, data)
    assert np.array_equal(output.asnumpy(), expected)
