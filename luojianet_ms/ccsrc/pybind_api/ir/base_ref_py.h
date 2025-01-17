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
#ifndef LUOJIANET_MS_CCSRC_PYBIND_API_IR_BASE_REF_PY_H_
#define LUOJIANET_MS_CCSRC_PYBIND_API_IR_BASE_REF_PY_H_

#include <memory>
#include <string>
#include <utility>

#include "pybind11/pybind11.h"
#include "base/base_ref.h"

namespace py = pybind11;

namespace luojianet_ms {

class PyObjectRef : public BaseRef {
 public:
  explicit PyObjectRef(const py::object &py_object) : BaseRef(), object_(py_object) {}
  explicit PyObjectRef(const py::tuple &tuple_obj) : BaseRef(), object_(tuple_obj) {}

  ~PyObjectRef() override = default;

  std::shared_ptr<Base> copy() const override { return std::make_shared<PyObjectRef>(object_); }
  MS_DECLARE_PARENT(PyObjectRef, BaseRef)

  uint32_t type() const override { return tid(); }
  std::string ToString() const override { return py::str(object_); }
  bool operator==(const BaseRef &other) const override;
  bool operator==(const PyObjectRef &other) const;

  py::object object_;
};
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_PYBIND_API_IR_BASE_REF_PY_H_
