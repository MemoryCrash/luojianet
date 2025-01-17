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
"""test function jvp in pynative mode """

import numpy as np
import pytest
import luojianet_ms.nn as nn
import luojianet_ms.context as context
from luojianet_ms import Tensor
from luojianet_ms import ms_function
from luojianet_ms.ops.functional import jvp

context.set_context(mode=context.PYNATIVE_MODE)

class SingleInputSingleOutputNet(nn.Module):
    def call(self, x):
        return x**3


class SingleInputMultipleOutputNet(nn.Module):
    def call(self, x):
        return x**3, 2*x


class MultipleInputSingleOutputNet(nn.Module):
    def call(self, x, y):
        return 2*x + 3*y


class MultipleInputMultipleOutputNet(nn.Module):
    def call(self, x, y):
        return 2*x, y**3


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_single_input_single_output_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with single input, single output and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    net = SingleInputSingleOutputNet()
    expect_primal = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    primal, grad = jvp(net, x, v)
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_single_input_single_output_custom_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with single input, single output and custom v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    net = SingleInputSingleOutputNet()
    expect_primal = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad = Tensor(np.array([[3, 24], [81, 192]]).astype(np.float32))
    primal, grad = jvp(net, x, v)
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_single_input_multiple_outputs_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with single input, multiple outputs and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    net = SingleInputMultipleOutputNet()
    expect_primal_0 = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_primal_1 = Tensor(np.array([[2, 4], [6, 8]]).astype(np.float32))
    expect_grad_0 = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    expect_grad_1 = Tensor(np.array([[2, 2], [2, 2]]).astype(np.float32))
    primal, grad = jvp(net, x, v)
    assert isinstance(primal, tuple)
    assert len(primal) == 2
    assert np.allclose(primal[0].asnumpy(), expect_primal_0.asnumpy())
    assert np.allclose(primal[1].asnumpy(), expect_primal_1.asnumpy())
    assert isinstance(grad, tuple)
    assert len(grad) == 2
    assert np.allclose(grad[0].asnumpy(), expect_grad_0.asnumpy())
    assert np.allclose(grad[1].asnumpy(), expect_grad_1.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_single_input_multiple_outputs_custom_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with single input, multiple outputs and custom v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    net = SingleInputMultipleOutputNet()
    expect_primal_0 = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_primal_1 = Tensor(np.array([[2, 4], [6, 8]]).astype(np.float32))
    expect_grad_0 = Tensor(np.array([[3, 24], [81, 192]]).astype(np.float32))
    expect_grad_1 = Tensor(np.array([[2, 4], [6, 8]]).astype(np.float32))
    primal, grad = jvp(net, x, v)
    assert isinstance(primal, tuple)
    assert len(primal) == 2
    assert np.allclose(primal[0].asnumpy(), expect_primal_0.asnumpy())
    assert np.allclose(primal[1].asnumpy(), expect_primal_1.asnumpy())
    assert isinstance(grad, tuple)
    assert len(grad) == 2
    assert np.allclose(grad[0].asnumpy(), expect_grad_0.asnumpy())
    assert np.allclose(grad[1].asnumpy(), expect_grad_1.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_multiple_inputs_multiple_outputs_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with multiple inputs, multiple outputs and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    y = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    net = MultipleInputMultipleOutputNet()
    expect_primal_0 = Tensor(np.array([[2, 4], [6, 8]]).astype(np.float32))
    expect_primal_1 = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad_0 = Tensor(np.array([[2, 2], [2, 2]]).astype(np.float32))
    expect_grad_1 = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    primal, grad = jvp(net, (x, y), (v, v))
    assert isinstance(primal, tuple)
    assert len(primal) == 2
    assert np.allclose(primal[0].asnumpy(), expect_primal_0.asnumpy())
    assert np.allclose(primal[1].asnumpy(), expect_primal_1.asnumpy())
    assert isinstance(grad, tuple)
    assert len(grad) == 2
    assert np.allclose(grad[0].asnumpy(), expect_grad_0.asnumpy())
    assert np.allclose(grad[1].asnumpy(), expect_grad_1.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_multiple_inputs_multiple_outputs_custom_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with multiple inputs, multiple outputs and custom v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    y = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v1 = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    v2 = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    net = MultipleInputMultipleOutputNet()
    expect_primal_0 = Tensor(np.array([[2, 4], [6, 8]]).astype(np.float32))
    expect_primal_1 = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad_0 = Tensor(np.array([[2, 2], [2, 2]]).astype(np.float32))
    expect_grad_1 = Tensor(np.array([[3, 24], [81, 192]]).astype(np.float32))
    primal, grad = jvp(net, (x, y), (v1, v2))
    assert isinstance(primal, tuple)
    assert len(primal) == 2
    assert np.allclose(primal[0].asnumpy(), expect_primal_0.asnumpy())
    assert np.allclose(primal[1].asnumpy(), expect_primal_1.asnumpy())
    assert isinstance(grad, tuple)
    assert len(grad) == 2
    assert np.allclose(grad[0].asnumpy(), expect_grad_0.asnumpy())
    assert np.allclose(grad[1].asnumpy(), expect_grad_1.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_multiple_inputs_single_output_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with multiple inputs, single output and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    y = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    net = MultipleInputSingleOutputNet()
    expect_primal = Tensor(np.array([[5, 10], [15, 20]]).astype(np.float32))
    expect_grad = Tensor(np.array([[5, 5], [5, 5]]).astype(np.float32))
    primal, grad = jvp(net, (x, y), (v, v))
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_multiple_inputs_single_output_custom_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with multiple inputs, single output and custom v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    y = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v1 = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    v2 = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    net = MultipleInputSingleOutputNet()
    expect_primal = Tensor(np.array([[5, 10], [15, 20]]).astype(np.float32))
    expect_grad = Tensor(np.array([[5, 8], [11, 14]]).astype(np.float32))
    primal, grad = jvp(net, (x, y), (v1, v2))
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_ms_function_single_input_single_output_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with ms_function, single input, single output and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))
    net = SingleInputSingleOutputNet()

    @ms_function
    def jvp_with_ms_function(inputs, vectors):
        output, jvp_grad = jvp(net, inputs, vectors)
        return output, jvp_grad

    primal, grad = jvp_with_ms_function(x, v)
    expect_primal = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_input_function_single_input_single_output_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with function, single input, single output and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))

    def test_function(inputs):
        return inputs**3

    primal, grad = jvp(test_function, x, v)
    expect_primal = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())


@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_jvp_construct_single_input_single_output_default_v_pynative():
    """
    Features: Function jvp
    Description: Test jvp with Module call, single input, single output and default v in pynative mode.
    Expectation: No exception.
    """
    x = Tensor(np.array([[1, 2], [3, 4]]).astype(np.float32))
    v = Tensor(np.array([[1, 1], [1, 1]]).astype(np.float32))

    class Net(nn.Module):
        def __init__(self, network):
            super(Net, self).__init__()
            self.net = network

        def call(self, inputs, vectors):
            net_out, jvp_out = jvp(self.net, inputs, vectors)
            return net_out, jvp_out

    test_net = Net(SingleInputSingleOutputNet())
    primal, grad = test_net(x, v)
    expect_primal = Tensor(np.array([[1, 8], [27, 64]]).astype(np.float32))
    expect_grad = Tensor(np.array([[3, 12], [27, 48]]).astype(np.float32))
    assert np.allclose(primal.asnumpy(), expect_primal.asnumpy())
    assert np.allclose(grad.asnumpy(), expect_grad.asnumpy())
