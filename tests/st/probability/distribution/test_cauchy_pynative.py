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
"""test cases for cauchy distribution"""

import pytest
import numpy as np
import luojianet_ms.context as context
import luojianet_ms.nn as nn
import luojianet_ms.nn.probability.distribution as msd
from luojianet_ms import Tensor
from luojianet_ms import dtype as ms

context.set_context(mode=context.PYNATIVE_MODE, device_target="Ascend")


class CauchyMean(nn.Module):
    def __init__(self, loc, scale, seed=10, dtype=ms.float32, name='Cauchy'):
        super().__init__()
        self.b = msd.Cauchy(loc, scale, seed, dtype, name)

    def call(self):
        out4 = self.b.entropy()
        return out4



@pytest.mark.level1
@pytest.mark.platform_arm_ascend_training
@pytest.mark.env_onecard
def test_probability_cauchy_mean_loc_scale_rand_2_ndarray():
    loc = np.random.randn(1024, 512, 7, 7).astype(np.float32)
    scale = np.random.uniform(0.0001, 100, size=(1024, 512, 7, 7)).astype(np.float32)
    net = CauchyMean(loc, scale)
    net()


class CauchyProb(nn.Module):
    def __init__(self, loc, scale, seed=10, dtype=ms.float32, name='Cauchy'):
        super().__init__()
        self.b = msd.Cauchy(loc, scale, seed, dtype, name)

    def call(self, value):
        out1 = self.b.prob(value)
        out2 = self.b.log_prob(value)
        out3 = self.b.cdf(value)
        out4 = self.b.log_cdf(value)
        out5 = self.b.survival_function(value)
        out6 = self.b.log_survival(value)
        return out1, out2, out3, out4, out5, out6


@pytest.mark.level1
@pytest.mark.platform_arm_ascend_training
@pytest.mark.env_onecard
def test_probability_cauchy_prob_cdf_loc_scale_rand_4_ndarray():
    loc = np.random.randn(1024, 512, 7, 7).astype(np.float32)
    scale = np.random.uniform(0.0001, 100, size=(1024, 512, 7, 7)).astype(np.float32)
    value = np.random.randn(1024, 512, 7, 7).astype(np.float32)
    net = CauchyProb(loc, scale)
    net(Tensor(value))
