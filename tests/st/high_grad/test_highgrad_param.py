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
"""Test high order grad with respect to parameter first, then input."""

import pytest
import numpy as np
import luojianet_ms.nn as nn
import luojianet_ms.ops as ops
from luojianet_ms import Tensor, context
from luojianet_ms import ParameterTuple, Parameter


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.mul = ops.Mul()
        weight_np = np.array([2, 2]).astype(np.float32)
        self.weight = Parameter(Tensor(weight_np), name="weight", requires_grad=True)

    def call(self, x):
        x_square = self.mul(x, x)
        x_square_z = self.mul(x_square, self.weight)
        output = self.mul(x_square_z, self.weight)
        return output


class Grad(nn.Module):
    def __init__(self, network):
        super(Grad, self).__init__()
        self.grad = ops.GradOperation(get_by_list=True, sens_param=False)
        self.network = network
        self.params = ParameterTuple(network.trainable_params())

    def call(self, x):
        output = self.grad(self.network, self.params)(x)
        return output


class GradSec(nn.Module):
    def __init__(self, network):
        super(GradSec, self).__init__()
        self.grad = ops.GradOperation(get_all=True, sens_param=False)
        self.network = network

    def call(self, x):
        output = self.grad(self.network)(x)
        return output


@pytest.mark.level1
@pytest.mark.platform_arm_ascend_training
@pytest.mark.platform_x86_ascend_training
@pytest.mark.platform_x86_gpu_training
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_sit_high_order_grad_params():
    context.set_context(mode=context.GRAPH_MODE)
    x = Tensor(np.array([1, 1]).astype(np.float32))
    net = Net()
    first_grad = Grad(net)
    second_grad = GradSec(first_grad)
    grad = second_grad(x)
    assert (grad[0].asnumpy() == np.array([8, 8])).all()
