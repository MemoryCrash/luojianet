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

import numpy as np
from scipy import special as sp

from akg.utils import kernel_exec as utils
from tests.common.test_op.ascend.bessel_i0e import bessel_i0e
from tests.common.gen_random import random_gaussian
from tests.common.base import get_rtol_atol
from tests.common.tensorio import compare_tensor


def bessel_i0e_run(x_shape, x_dtype, attrs):
    shapes = [x_shape]
    dtypes = [x_dtype]
    mod = utils.op_build_test(bessel_i0e, shapes, dtypes,
                              kernel_name="bessel_i0e", attrs=attrs)
    bench_mark, inputs, output = gen_data(dtypes, shapes)
    output = utils.mod_launch(mod, inputs + [output], expect=bench_mark)
    rtol, atol = get_rtol_atol("bessel_i0e", dtypes[0])
    compare_res = compare_tensor(output, bench_mark, rtol=rtol, atol=atol)
    return inputs, output, bench_mark, compare_res


def gen_data(dtypes, shapes):
    dtype = dtypes[0]
    shape = shapes[0]
    input = random_gaussian(shape, miu=3.75).astype(dtype)
    expect = sp.i0e(input)
    output = np.full(expect.shape, np.nan, dtype)
    return expect, [input], output
