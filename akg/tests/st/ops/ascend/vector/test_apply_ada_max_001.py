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

import os
from tests.common.base import TestBase


class TestCase(TestBase):

    def setup(self):
        case_name = "test_apply_ada_max_001"
        case_path = os.getcwd()
        self.params_init(case_name, case_path)
        self.caseresult = True
        self._log.info("============= %s Setup case============", self.casename)
        self.testarg = [
            ("apply_ada_max_001", "apply_ada_max_run", ((16, 16), "float16", 1e-5)),
            ("apply_ada_max_002", "apply_ada_max_run", ((16, 16), "float32", 1e-5)),
        ]

    def test_run(self):
        self.common_run(self.testarg)

    def teardown(self):
        self._log.info("============= %s Teardown============", self.casename)
        return
