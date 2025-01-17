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

import akg.tvm
import numpy as np
from akg.utils import kernel_exec as utils
from akg.ops.math.ascend import MatMul
from tests.common.test_run.ascend.matmul_run import *
from akg.ops.math import Mul

def matmul_mul_transdata(x, y, c, b, out_dtype, left_format="zZ", right_format="nZ", out_format="zN", transpose_x=False,
                        transpose_y=False, attrs={}, target="cce"):
    matmul_res, attrs = MatMul(x, y, b, out_dtype, left_format, right_format, out_format, transpose_x, transpose_y, attrs=None)
    res = Mul(matmul_res, c, target='cce')
    if out_format == 'zN':
        n1, m1, m0, n0 = matmul_res.shape[-4:]
        new_shape = matmul_res.shape[:-4] + [m1 * m0, n1 * n0]
    elif out_format == 'zZ':
        m1, n1, m0, n0 = matmul_res.shape[-4:]
        new_shape = matmul_res.shape[:-4] + [m1 * m0, n1 * n0]

    func = akg.tvm.get_global_func("TransData")
    res = func([res], {"src_format" : "FRACTAL_NZ", "dst_format" : "DefaultFormat", "output_shape": new_shape})
    return res, attrs

def matmul_mul_transdata_compile(shape_x, shape_y, bias, muls_c, left_format, right_format, output_format, adj_x, adj_y, dtype, bias_dtype, out_dtype, kernel_name, attrs, tuning=False):
    batch_tuple, m, k, n = extract_dim(shape_x, shape_y, adj_x, adj_y)
    m = (m + 15) // 16 * 16
    n = (n + 15) // 16 * 16
    k = (k + 15) // 16 * 16
    shape_xx, shape_yy, bias_shape, _, k = get_converted_shapes(m, n, k, batch_tuple, adj_x, adj_y, bias,
                                                                        left_format, right_format, output_format)
    input_shapes = [shape_xx, shape_yy, muls_c, bias_shape]
    input_types = [dtype, dtype, out_dtype, bias_dtype]
    has_bias = False
    if bias == 1:
        has_bias = True
    op_attrs = [out_dtype, left_format, right_format, output_format, adj_x, adj_y, attrs]
    if has_bias == False:
        input_shapes = [shape_xx, shape_yy, muls_c]
        input_types = [dtype, dtype, out_dtype]
        op_attrs = [None, out_dtype, left_format, right_format, output_format, adj_x, adj_y, attrs]
    return utils.op_build_test(matmul_mul_transdata, input_shapes, input_types, op_attrs, kernel_name, attrs=attrs, tuning=tuning)

def matmul_mul_transdata_execute(shape_x, shape_y, bias, muls_c, left_format, right_format, out_format, adj_x, adj_y, dtype, bias_dtype, out_dtype, kernel_name, attrs={}):
    batch_tuple, m, k, n = extract_dim(shape_x, shape_y, adj_x, adj_y)
    m = (m + 15) // 16 * 16
    n = (n + 15) // 16 * 16
    k = (k + 15) // 16 * 16
    _, _, _, out_shape, k = get_converted_shapes(m, n, k, batch_tuple, adj_x, adj_y, bias, left_format, right_format, out_format)
    if muls_c == "scalar":
        cmul_shape = (1, )
    else:
        cmul_shape = out_shape
    mod = matmul_mul_transdata_compile(shape_x, shape_y, bias, cmul_shape, left_format, right_format, out_format, adj_x, adj_y, dtype, bias_dtype, out_dtype, kernel_name, attrs={})
    # Generate data
    m_x, m_y, bench_mark, bias_data = matmul_data(batch_tuple, m, k, n, dtype, bias_dtype, out_dtype, bias, adj_x, adj_y, left_format, right_format, out_format)
    cadd_data = random_gaussian(cmul_shape, miu=0.5, sigma=0.01).astype(out_dtype)

    bench_mark = bench_mark * cadd_data

    transpose_axis = []
    new_shape = []
    out_shape = list(out_shape)
    if out_format == 'zN':
        n1, m1, m0, n0 = out_shape[-4:]
        new_shape = out_shape[:-4] + [m1 * m0, n1 * n0]
        transpose_axis = [0, 1+1, 2+1, 0+1, 3+1]
    elif out_format == 'zZ':
        m1, n1, m0, n0 = out_shape[-4:]
        new_shape = out_shape[:-4] + [m1 * m0, n1 * n0]
        transpose_axis = [0, 0+1, 2+1, 1+1, 3+1]
    bench_mark = bench_mark.transpose(transpose_axis)
    bench_mark = np.reshape(bench_mark,new_shape)

    # mod launch
    output = np.full(bench_mark.shape, np.nan, out_dtype)
    if bias == 0:
        output = utils.mod_launch(mod, (m_x, m_y, cadd_data, output), expect=bench_mark)
    elif bias == 1:
        output = utils.mod_launch(mod, (m_x, m_y, cadd_data, bias_data, output), expect=bench_mark)
    # compare result
    rtol, atol = get_rtol_atol("matmul", dtype)
    compare_result = compare_tensor(output, bench_mark, rtol=rtol, atol=atol, equal_nan=True)
    return (m_x, m_y), output, bench_mark, compare_result
