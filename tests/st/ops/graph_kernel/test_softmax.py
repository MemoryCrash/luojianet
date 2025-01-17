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
from luojianet_ms.ops import operations as P

class Net(nn.Module):
    def __init__(self, axis=-1):
        super(Net, self).__init__()
        self.Softmax = P.Softmax(axis)

    def call(self, x):
        return self.Softmax(x)

def get_output(x, enable_graph_kernel=False):
    context.set_context(enable_graph_kernel=enable_graph_kernel)
    opt = Net()
    output = opt(Tensor(x))
    return output

def test_softmax(shape, dtype):
    np.random.seed(0)
    x = np.random.normal(0, 1, shape).astype(dtype)

    expect = get_output(x, False)
    output = get_output(x, True)

    rtol = 1.e-4
    atol = 1.e-4
    if dtype == "float16":
        rtol = 1.e-3
        atol = 1.e-3

    assert np.allclose(expect.asnumpy(), output.asnumpy(), rtol, atol, equal_nan=True)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_softmax_gpu():
    context.set_context(mode=context.GRAPH_MODE, device_target="GPU")
    test_softmax([4, 32, 48], np.float32)

@pytest.mark.level1
@pytest.mark.platform_arm_ascend_training
@pytest.mark.platform_x86_ascend_training
@pytest.mark.env_onecard
def test_softmax_ascend():
    context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")
    test_softmax([2, 32, 48, 64], np.float32)
