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
""" test dtype and shape as attr"""
import numpy as np
import pytest

import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms import context
from luojianet_ms import dtype as mstype
from luojianet_ms.ops import operations as P

context.set_context(mode=context.GRAPH_MODE)


def test_dtype_and_shape_as_attr():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()

        def call(self, x):
            shape = x.shape
            dtype = x.dtype
            return shape, dtype

    net = Net()
    x = Tensor(np.ones([1, 2, 3], np.int32))
    ret = net(x)
    assert ret == ((1, 2, 3), mstype.int32)


def test_dtype_and_shape_as_attr_to_new_tensor():
    class Net(nn.Module):
        def __init__(self, value):
            super(Net, self).__init__()
            self.fill = P.Fill()
            self.value = value

        def call(self, x):
            dtype = x.dtype
            shape = x.shape
            y = self.fill(dtype, shape, self.value)
            return y

    net = Net(2.2)
    x = Tensor(np.ones([1, 2, 3], np.float32))
    ret = net(x)
    assert (ret.asnumpy() == (np.zeros([1, 2, 3], np.float32) + 2.2)).all()


def test_type_not_have_the_attr():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()

        def call(self, x):
            shape = x.shapes
            return shape

    net = Net()
    x = Tensor(np.ones([1, 2, 3], np.int32))
    with pytest.raises(RuntimeError):
        net(x)


def test_type_not_have_the_method():
    class Net(nn.Module):
        def __init__(self):
            super(Net, self).__init__()

        def call(self, x):
            shape = x.dtypes()
            return shape

    net = Net()
    x = Tensor(np.ones([1, 2, 3], np.int32))
    with pytest.raises(RuntimeError):
        net(x)
