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

"""softplus_grad test case"""

import os
import pytest
from tests.common.base import TestBase
from tests.common.test_run.ascend.softplus_grad_run import softplus_grad_run


class TestSoftplusGrad(TestBase):
    def setup(self):
        case_name = "test_akg_softplus_grad_001"
        case_path = os.getcwd()

        # params init
        self.params_init(case_name, case_path)

        self.caseresult = True
        self._log.info("========================{0}  Setup case=================".format(self.casename))
        self.testarg = [
            # testflag,opfuncname,testRunArgs, dimArgs
            ("softplus_grad_f16_01", softplus_grad_run, ((32, 16), "float16")),
            ("softplus_grad_f32_02", softplus_grad_run, ((32, 16), "float32")),
        ]
        self.testarg_cloud = [
            ("softplus_grad_f16_01", softplus_grad_run, ((32, 16), "float16")),
            ("softplus_grad_f32_02", softplus_grad_run, ((32, 16), "float32")),
            ("softplus_grad_i32_03", softplus_grad_run, ((32, 16), "int32")),
            ("softplus_grad_si8_04", softplus_grad_run, ((32, 16), "int8")),
            ("softplus_grad_ui8_05", softplus_grad_run, ((32, 16), "uint8")),
        ]
        return

    @pytest.mark.level0
    @pytest.mark.platform_arm_ascend_training
    @pytest.mark.platform_x86_ascend_training
    @pytest.mark.env_onecard
    def test_run(self):
        self.common_run(self.testarg)

    def test_run_cloud(self):
        self.common_run(self.testarg_cloud)

    def teardown(self):
        """
        clean environment
        :return:
        """
        self._log.info("============= {0} Teardown============".format(self.casename))
        return
