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
""" test reshape"""
import pytest

import luojianet_ms.nn as nn
import luojianet_ms.common.dtype as mstype
from luojianet_ms import Tensor
from luojianet_ms import context

context.set_context(mode=context.GRAPH_MODE)


def test_reshape():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.value = Tensor([[1, 2, 3], [4, 5, 6]], dtype=mstype.float32)

        def call(self):
            return self.value.reshape(-1)

    net = Net()
    net()


def test_reshape_1():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.value = Tensor([[1, 2, 3], [4, 5, 6]], dtype=mstype.float32)

        def call(self):
            return self.value.reshape([3, 2, 1])

    net = Net()
    net()


def test_reshape_2():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.value = Tensor([[1, 2, 3], [4, 5, 6]], dtype=mstype.float32)

        def call(self):
            return self.value.reshape((-1, 2))

    net = Net()
    net()


def test_reshape_error():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.value = Tensor([[1, 2, 3], [4, 5, 6]], dtype=mstype.float32)

        def call(self):
            return self.value.reshape(1, 2, 4)

    net = Net()
    with pytest.raises(ValueError):
        net()


def test_reshape_error_1():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()
            self.value = Tensor([[1, 2, 3], [4, 5, 6]], dtype=mstype.float32)

        def call(self):
            return self.value.reshape((1, 2, 3.5))

    net = Net()
    with pytest.raises(TypeError):
        net()
