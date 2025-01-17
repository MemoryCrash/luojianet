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
from luojianet_ms import Tensor
from luojianet_ms.nn import Module
import luojianet_ms.ops.operations as P

class Net(Module):
    def __init__(self):
        super(Net, self).__init__()
        self.squaresumall = P.SquareSumAll()

    def call(self, x0, x1):
        return self.squaresumall(x0, x1)

def get_output(inp0, inp1, enable_graph_kernel=False):
    context.set_context(enable_graph_kernel=enable_graph_kernel)
    net = Net()
    output = net(inp0, inp1)
    return output

def test_basic(datatype):
    inp0 = Tensor(np.random.normal(1, 0.1, [800, 96]).astype(datatype))
    inp1 = Tensor(np.random.normal(1, 0.1, [800, 96]).astype(datatype))
    expect = get_output(inp0, inp1, False)
    output = get_output(inp0, inp1, True)
    expect_np0 = expect[0].asnumpy().copy()
    output_np0 = output[0].asnumpy().copy()
    expect_np1 = expect[1].asnumpy().copy()
    output_np1 = output[1].asnumpy().copy()
    assert np.allclose(expect_np0, output_np0, 1.e-4, 1.e-7)
    assert np.allclose(expect_np1, output_np1, 1.e-4, 1.e-7)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_gpu_1():
    context.set_context(mode=context.GRAPH_MODE, device_target="GPU")
    test_basic(np.float16)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_gpu_2():
    context.set_context(mode=context.GRAPH_MODE, device_target="GPU")
    test_basic(np.float32)
