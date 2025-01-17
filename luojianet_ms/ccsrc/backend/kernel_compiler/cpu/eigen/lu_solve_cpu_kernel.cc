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

#include "backend/kernel_compiler/cpu/eigen/lu_solve_cpu_kernel.h"
#include <vector>
#include <string>
#include "utils/ms_utils.h"
#include "backend/kernel_compiler/cpu/eigen/eigen_common_utils.h"
#include "Eigen/Dense"
#include "Eigen/LU"
namespace luojianet_ms {
namespace kernel {
namespace {
constexpr size_t kLUInputsNum = 2;
constexpr size_t kLUaIndex = 0;
constexpr size_t kLUbIndex = 1;
constexpr size_t kLUOutputsNum = 1;
constexpr size_t kLuIndex = 0;
constexpr size_t kLUDefaultShape = 1;
constexpr size_t kRowIndex = 2;
constexpr size_t kColIndex = 1;
}  // namespace

template <typename T>
void LUSolverCPUKernel<T>::InitKernel(const CNodePtr &kernel_node) {
  MS_EXCEPTION_IF_NULL(kernel_node);
  kernel_name_ = AnfAlgo::GetCNodeName(kernel_node);
  dtype_ = AnfAlgo::GetInputDeviceDataType(kernel_node, 0);
  size_t input_num = AnfAlgo::GetInputTensorNum(kernel_node);
  CHECK_KERNEL_INPUTS_NUM(input_num, kLUInputsNum, kernel_name_);
  size_t output_num = AnfAlgo::GetOutputTensorNum(kernel_node);
  CHECK_KERNEL_OUTPUTS_NUM(output_num, kLUOutputsNum, kernel_name_);
  auto a_shape = AnfAlgo::GetPrevNodeOutputInferShape(kernel_node, kLUaIndex);
  auto b_shape = AnfAlgo::GetPrevNodeOutputInferShape(kernel_node, kLUbIndex);
  if (a_shape.empty() || b_shape.empty()) {
    MS_LOG_EXCEPTION << kernel_name_ << " input a or b matrix shape invalid.";
  }
  if (a_shape.size() == kLUDefaultShape) {
    a_row_ = a_shape.front();
  } else {
    a_row_ = a_shape.at(a_shape.size() - kRowIndex);
    a_col_ = a_shape.at(a_shape.size() - kColIndex);
  }
  if (b_shape.size() == kLUDefaultShape) {
    b_row_ = b_shape.front();
  } else {
    b_row_ = b_shape.at(b_shape.size() - kRowIndex);
    b_col_ = b_shape.at(b_shape.size() - kColIndex);
  }
  auto output_lu_shape = AnfAlgo::GetOutputInferShape(kernel_node, kLuIndex);
  if (output_lu_shape.empty()) {
    MS_LOG_EXCEPTION << kernel_name_ << " output lu shape invalid.";
  }
  if (output_lu_shape.size() == kLUDefaultShape) {
    out_row_ = output_lu_shape.front();
  } else {
    out_row_ = output_lu_shape.at(output_lu_shape.size() - kRowIndex);
    out_col_ = output_lu_shape.at(output_lu_shape.size() - kColIndex);
  }
  trans_ = AnfAlgo ::GetNodeAttr<std::string>(kernel_node, TRANS);
}

template <typename T>
bool LUSolverCPUKernel<T>::Launch(const std::vector<kernel::AddressPtr> &inputs,
                                  const std::vector<kernel::AddressPtr> &,
                                  const std::vector<kernel::AddressPtr> &outputs) {
  T *a_value = reinterpret_cast<T *>(inputs[kLUaIndex]->addr);
  Map<Matrix<T, RowMajor>> input_a(a_value, a_row_, a_col_);

  T *b_value = reinterpret_cast<T *>(inputs[kLUbIndex]->addr);
  Map<Matrix<T, RowMajor>> input_b(b_value, b_row_, b_col_);
  T *output_lu_value = reinterpret_cast<T *>(outputs[kLuIndex]->addr);
  Map<Matrix<T, RowMajor>> output_lu(output_lu_value, out_row_, out_col_);
  if (trans_ == "N") {
    output_lu.noalias() = input_a.template triangularView<UnitLower>().solve(input_b);
    output_lu.noalias() = input_a.template triangularView<Upper>().solve(output_lu);
  } else if (trans_ == "T") {
    output_lu.noalias() = input_a.template triangularView<Upper>().solve(input_b);
    output_lu.noalias() = input_a.template triangularView<UnitLower>().solve(output_lu);
  } else if (trans_ == "C") {
    MS_LOG_EXCEPTION << kernel_name_ << " trans_ flag is not supported C:  " << trans_;
  } else {
    MS_LOG_EXCEPTION << kernel_name_ << " trans_ flag is invalid:  " << trans_;
  }

  if (output_lu.RowsAtCompileTime == 0 || output_lu.ColsAtCompileTime == 0) {
    MS_LOG_EXCEPTION << kernel_name_ << " output lu shape invalid.";
  }

  return true;
}
}  // namespace kernel
}  // namespace luojianet_ms
