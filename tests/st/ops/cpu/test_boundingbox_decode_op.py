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

import luojianet_ms
import luojianet_ms.context as context
import luojianet_ms.nn as nn
from luojianet_ms import Tensor
from luojianet_ms.ops import operations as P


class NetBoundingBoxDecode(nn.Module):
    def __init__(self, means=(0.0, 0.0, 0.0, 0.0), stds=(1.0, 1.0, 1.0, 1.0)):
        super(NetBoundingBoxDecode, self).__init__()
        self.decode = P.BoundingBoxDecode(max_shape=(768, 1280), means=means, stds=stds,
                                          wh_ratio_clip=0.016)

    def call(self, anchor, groundtruth):
        return self.decode(anchor, groundtruth)

@pytest.mark.level0
@pytest.mark.platform_x86_cpu
@pytest.mark.env_onecard
def test_boundingbox_decode():
    anchor = np.array([[4, 1, 2, 1], [2, 2, 2, 3]], np.float32)
    deltas = np.array([[3, 1, 2, 2], [1, 2, 1, 4]], np.float32)
    means = (0.1, 0.1, 0.2, 0.2)
    stds = (2.0, 2.0, 3.0, 3.0)
    anchor_box = Tensor(anchor, luojianet_ms.float32)
    deltas_box = Tensor(deltas, luojianet_ms.float32)
    expect_deltas = np.array([[28.6500, 0.0000, 0.0000, 33.8500],
                              [0.0000, 0.0000, 15.8663, 72.7000]], np.float32)

    error = np.ones(shape=[2, 4]) * 1.0e-4

    context.set_context(mode=context.GRAPH_MODE, device_target='CPU')
    boundingbox_decode = NetBoundingBoxDecode(means, stds)
    output = boundingbox_decode(anchor_box, deltas_box)
    diff = output.asnumpy() - expect_deltas
    assert np.all(abs(diff) < error)

    context.set_context(mode=context.PYNATIVE_MODE, device_target='CPU')
    boundingbox_decode = NetBoundingBoxDecode(means, stds)
    output = boundingbox_decode(anchor_box, deltas_box)
    diff = output.asnumpy() - expect_deltas
    assert np.all(abs(diff) < error)
