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
from luojianet_ms import Tensor, ops


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.rint = ops.Rint()

    def call(self, x):
        return self.rint(x)


def generate_testcases(nptype):
    context.set_context(mode=context.GRAPH_MODE, device_target="GPU")
    x = np.array([-1.7, -1.5, -0.2, 0.2, 1.5, 1.7, 2.0]).astype(nptype)
    net = Net()
    output = net(Tensor(x))
    expect = np.rint(x).astype(nptype)
    np.testing.assert_almost_equal(output.asnumpy(), expect)

    context.set_context(mode=context.PYNATIVE_MODE, device_target="GPU")
    x = np.array([-1.7, -1.5, -0.2, 0.2, 1.5, 1.7, 2.0]).astype(nptype)
    net = Net()
    output = net(Tensor(x))
    expect = np.rint(x).astype(nptype)
    np.testing.assert_almost_equal(output.asnumpy(), expect)


@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_sign_float32():
    generate_testcases(np.float32)


@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_sign_float16():
    generate_testcases(np.float16)
