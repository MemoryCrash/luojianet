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
import luojianet_ms as ms
from luojianet_ms import Tensor
from luojianet_ms.ops import operations as P
from luojianet_ms.ops import composite as C


class L2LossNet(nn.Module):
    def __init__(self):
        super(L2LossNet, self).__init__()
        self.l2_loss = P.L2Loss()

    def call(self, x):
        return self.l2_loss(x)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_pynative_fp32_2x2():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([[1., 2.], [3., 4.]]), ms.float32)
    expect = np.array(15, np.float32)
    output = P.L2Loss()(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_pynative_fp16_2x2():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([[1., 2.], [3., 4.]]), ms.float16)
    expect = np.array(15, np.float16)
    output = P.L2Loss()(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_pynative_fp32_1x4():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([1., 2., 3., 4.]), ms.float32)
    expect = np.array(15, np.float32)
    output = P.L2Loss()(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_pynative_fp16_1x4():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([1., 2., 3., 4.]), ms.float16)
    expect = np.array(15, np.float16)
    output = P.L2Loss()(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_graph_fp32_1x4():
    context.set_context(mode=context.GRAPH_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([1., 2., 3., 4.]), ms.float32)
    expect = np.array(15, np.float32)
    l2_loss = L2LossNet()
    output = l2_loss(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_graph_fp16_1x4():
    context.set_context(mode=context.GRAPH_MODE, device_target="CPU")
    error = 1e-4
    x = Tensor(np.array([1., 2., 3., 4.]), ms.float16)
    expect = np.array(15, np.float16)
    l2_loss = L2LossNet()
    output = l2_loss(x)
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

class GradNet(nn.Module):
    def __init__(self, net):
        super(GradNet, self).__init__()
        self.net = net
        self.grad_op = C.GradOperation(get_all=True)

    def call(self, x):
        gradient_function = self.grad_op(self.net)
        return gradient_function(x)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_grad_fp32():
    context.set_context(mode=context.GRAPH_MODE, device_target="CPU")
    x = Tensor(np.array([2.4, 3.2, 1.2, 5.9, 9.]).astype(np.float32))
    error = 1e-4
    net = L2LossNet()
    output = GradNet(net)(x)[0]
    expect = x
    diff = output.asnumpy() - expect
    assert np.all(diff < error)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_l2loss_grad_fp16():
    context.set_context(mode=context.GRAPH_MODE, device_target="CPU")
    x = Tensor(np.array([[2.4, 3.2, 4.8], [1.2, 5.9, 9.]]).astype(np.float16))
    error = 1e-4
    net = L2LossNet()
    output = GradNet(net)(x)[0]
    expect = x
    diff = output.asnumpy() - expect
    assert np.all(diff < error)
