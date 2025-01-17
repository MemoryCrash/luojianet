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

from luojianet_ms import context
from luojianet_ms import log as logger
from luojianet_ms.common.tensor import Tensor
from luojianet_ms.nn import Module, FastGelu
from luojianet_ms.ops import operations as P
from luojianet_ms.ops.composite import GradOperation

context.set_context(mode=context.GRAPH_MODE, device_target="Ascend")


class Grad(Module):
    def __init__(self, network):
        super(Grad, self).__init__()
        self.grad = GradOperation(get_all=True, sens_param=True)
        self.network = network

    def call(self, input_, output_grad):
        return self.grad(self.network)(input_, output_grad)


def fast_gelu_backward_me_impl(input_, output_grad):
    n = FastGelu()
    grad_with_sense = Grad(n)
    grad_with_sense.set_train()
    input_grad = grad_with_sense(input_, output_grad)
    return input_grad


def fast_gelu_backward_cmp(input_shape):
    input_np = np.random.randn(*input_shape).astype(np.float32)
    input_me = Tensor(input_np)

    output_grad_shape = input_shape
    output_grad_np = np.random.randn(*output_grad_shape).astype(np.float32)
    output_grad_me = Tensor(output_grad_np)

    output_grad_me = fast_gelu_backward_me_impl(input_me, output_grad_me)
    logger.info("---------me--------")
    logger.info(output_grad_me)


# ----------    LARGE INPUT  ---------------

class MEGeluLargeIn(Module):
    def __init__(self):
        super(MEGeluLargeIn, self).__init__()
        self.matmul = P.MatMul()
        self.fast_gelu = P.GeLU()

    def call(self, x1, x2):
        x = self.matmul(x1, x2)
        return self.fast_gelu(x)


class GradLargeIn(Module):
    def __init__(self, network):
        super(GradLargeIn, self).__init__()
        self.grad = GradOperation(get_all=True, sens_param=True)
        self.network = network

    def call(self, x1, x2, output_grad):
        return self.grad(self.network)(x1, x2, output_grad)


def fast_gelu_backward_me_large_in_impl(x1, x2, output_grad):
    n = FastGelu()
    grad_with_sense = GradLargeIn(n)
    grad_with_sense.set_train()
    input_grad = grad_with_sense(x1, x2, output_grad)
    return input_grad[0].asnumpy(), input_grad[1].asnumpy()


def test_grad_fast_gelu_input_10240_1024():
    input_shape = [10240, 1024]
    fast_gelu_backward_cmp(input_shape)
