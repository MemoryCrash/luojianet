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
import pytest
from luojianet_ms.common import dtype as mstype
from luojianet_ms import nn
from luojianet_ms import Tensor
from luojianet_ms.ops import composite as C
from luojianet_ms import context

context.set_context(mode=context.GRAPH_MODE)


class ForwardNet(nn.Module):
    def call(self, x, y):
        y = y + 10
        while x < y:
            x = (x + 2) * (y - 9)
            y = y + 2
        x = x + 5
        return x


class BackwardNet(nn.Module):
    def __init__(self, forward_net):
        super(BackwardNet, self).__init__()
        self.forward_net = forward_net
        self.grad = C.GradOperation()

    def call(self, *inputs):
        grads = self.grad(self.forward_net)(*inputs)
        return grads


@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.platform_arm_ascend_training
@pytest.mark.platform_x86_ascend_training
@pytest.mark.env_onecard
def test_forward():
    c1 = Tensor([0], mstype.int32)
    c2 = Tensor([0], mstype.int32)
    expect = Tensor([75], mstype.int32)
    forward_net = ForwardNet()
    output = forward_net(c1, c2)
    assert expect == output


@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.platform_arm_ascend_training
@pytest.mark.platform_x86_ascend_training
@pytest.mark.env_onecard
def test_backward():
    c1 = Tensor([0], mstype.int32)
    c2 = Tensor([0], mstype.int32)
    expect = Tensor([75], mstype.int32)
    forward_net = ForwardNet()
    output = forward_net(c1, c2)
    assert expect == output
