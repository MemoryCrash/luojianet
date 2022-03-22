/**
 * Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
 * Copyright 2021, 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "luojianet_ms/lite/src/common/dynamic_library_loader.h"
#include "common/common_test.h"

namespace luojianet_ms {
class LoaderUtilTest : public luojianet_ms::CommonTest {
 public:
  LoaderUtilTest() {}
};

/*
 in file add.c, the code is:
 int add(int a, int b) {return a + b;}
 use this command to generate so file:
 gcc add.cc -fPIC -shared -o libadd.so
 use this command to see the symbol table:
 nm -D libadd.so
*/
TEST_F(LoaderUtilTest, TestAdd) {
  lite::DynamicLibraryLoader loader;
  loader.Open("./libadd.so");
  int (*add)(int a, int b);
  add = (int (*)(int, int))loader.GetFunc("add");
  int res = add(7, 8);
  loader.Close();
  ASSERT_EQ(15, res);
}
}  // namespace luojianet_ms