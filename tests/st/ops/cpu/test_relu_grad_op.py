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

import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.common.initializer import initializer
from luojianet_ms.common.parameter import Parameter
from luojianet_ms.ops.operations import _grad_ops as G

context.set_context(mode=context.GRAPH_MODE, device_target='CPU')


class NetReluGrad(nn.Module):
    def __init__(self):
        super(NetReluGrad, self).__init__()
        self.relu_grad = G.ReluGrad()
        self.x = Parameter(initializer(Tensor(np.array([[[[-1, 1, 1],
                                                          [1, -1, 1],
                                                          [1, 1, -1]]]]).astype(np.float32)), [1, 1, 3, 3]), name='x')
        self.dy = Parameter(initializer(Tensor(np.array([[[[1, 0, 1],
                                                           [0, 1, 0],
                                                           [1, 1, 1]]]]).astype(np.float32)), [1, 1, 3, 3]), name='dy')

    def call(self):
        return self.relu_grad(self.dy, self.x)


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_relu_grad():
    relu_grad = NetReluGrad()
    output = relu_grad()
    expect = np.array([[[[0, 0, 1], [0, 0, 0], [1, 1, 0]]]]).astype(np.float32)
    error = np.ones(shape=[3, 3]) * 1.0e-6
    diff = np.abs(output.asnumpy() - expect)
    assert np.all(diff < error)
