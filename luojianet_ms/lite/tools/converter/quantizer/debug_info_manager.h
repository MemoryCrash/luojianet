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

#ifndef LUOJIANET_MS_LITE_TOOLS_CONVERTER_DEBUGINFO_H
#define LUOJIANET_MS_LITE_TOOLS_CONVERTER_DEBUGINFO_H

#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include "tools/converter/quantizer/quantize_util.h"
#include "nnacl/op_base.h"
#include "tools/common/statistic_utils.h"
#include "src/tensor.h"

namespace luojianet_ms::lite {
enum InOutFlag { INPUT, OUTPUT };
enum DataTypeFlag { ORIGIN, DEQUANT };
enum TensorTypeFlag { ACTIVATION, WEIGHT };

struct PrimaryKey {
  std::string node_name;
  InOutFlag in_out_flag;
  size_t index;
  friend bool operator<(const struct PrimaryKey &p1, const struct PrimaryKey &p2) {
    return p1.node_name < p2.node_name || (p1.node_name == p2.node_name && p1.in_out_flag < p2.in_out_flag) ||
           (p1.node_name == p2.node_name && p1.in_out_flag == p2.in_out_flag && p1.index < p2.index);
  }
  friend std::ostream &operator<<(std::ostream &os, const PrimaryKey &p) {  // for struct output
    os << "[" << p.node_name << "," << p.in_out_flag << "," << p.index << "]";
    return os;
  }
};

struct TensorData {
  void *data = nullptr;
  TypeId data_type;
  size_t size;
  size_t elements_num;
};

struct QuantDebugInfo {
  PrimaryKey primary_key;
  std::string node_type;
  std::string tensor_name;
  DataTypeFlag data_type_flag;
  TensorTypeFlag tensor_type_flag;
  float min{0};
  float quartile1{0};
  float median{0};
  float quartile3{0};
  float max{0};
  float mean{0};
  float var{0};
  float sparsity{0};
  float clip{0};
  float cos_similarity{1};
  TensorData tensor_data;
};

struct QuantParamExtend {
  std::string node_name;
  std::string node_type;
  std::string tensor_name;
  int64_t element_num;
  std::vector<int> dims;
  Vector<lite::LiteQuantParam> quant_params;
};

class DebugInfoManager {
 public:
  int CompareOriginWithQuant(const quant::SessionModel &origin, const quant::SessionModel &quant,
                             const std::map<std::string, OpParameter *> &op_parameters,
                             const std::string &debug_info_save_path,
                             const preprocess::DataPreProcessParam &data_preprocess);

 private:
  int AddOriginInfo(const luojianet_ms::CallBackParam &call_back_param, OpParameter *op_parameter, bool is_input,
                    int tensor_index, luojianet_ms::lite::Tensor *origin_tensor);

  int AddComparedInfo(const luojianet_ms::CallBackParam &call_back_param, OpParameter *op_parameter, bool is_input,
                      int tensor_index, luojianet_ms::lite::Tensor *compared_tensor);

  void PrintAllDebugInfo();

  int SaveInfo(const std::string &file_path);

  int SetOriginStaticInfo(QuantDebugInfo *quant_debug_info, const luojianet_ms::lite::Tensor &tensor);

  int SetQuantStaticInfo(OpParameter *op_parameter, int tensor_index, QuantDebugInfo *quant_debug_info,
                         const luojianet_ms::lite::Tensor &tensor);

  std::string ParseDataTypeFlagToString(DataTypeFlag data_type_flag);

  std::string ParseTensorTypeFlagToString(TensorTypeFlag tensor_type_flag);

  std::string ParseInOutTensorToString(InOutFlag in_out_flag);

  void PrintInfo(const QuantDebugInfo &info);

  void SaveInfo(std::ofstream &out_file, const QuantDebugInfo &info);

  std::map<std::string, luojianet_ms::schema::Tensor *> ParseInputTensorFromModel(const Model &model);

  std::map<std::string, luojianet_ms::schema::Tensor *> ParseOutputTensorFromModel(const Model &model);

  int GetDataFromTensorMap(const luojianet_ms::schema::Tensor &schema_tensor, luojianet_ms::lite::Tensor *dst_tensor);

  KernelCallBack GetBeforeCallBack(const std::map<std::string, luojianet_ms::schema::Tensor *> &input_tensor_map,
                                   const std::map<std::string, OpParameter *> &op_parameters, bool is_origin);

  KernelCallBack GetOriginBeforeCallBack(const std::map<std::string, luojianet_ms::schema::Tensor *> &input_tensor_map,
                                         const std::map<std::string, OpParameter *> &op_parameters);

  KernelCallBack GetQuantBeforeCallBack(const std::map<std::string, luojianet_ms::schema::Tensor *> &input_tensor_map,
                                        const std::map<std::string, OpParameter *> &op_parameters);

  KernelCallBack GetAfterCallBack(const std::map<std::string, OpParameter *> &op_parameters, bool is_origin);

  int GetConstTensor(const std::map<std::string, luojianet_ms::schema::Tensor *> &input_tensor_map,
                     luojianet_ms::tensor::MSTensor *tensor, luojianet_ms::lite::Tensor *new_tensor);

  void FreeBuffer();

  void PrintQuantParam();

  int SaveQuantParam(const std::string &file_path);

  int GetClipAndCos();

  template <typename T>
  void GetStatByTensor(const T *tensor_data, size_t element_num, QuantDebugInfo *infos) {
    MS_ASSERT(infos != nullptr);
    std::vector<T> bak_data(tensor_data, tensor_data + element_num);
    auto size = bak_data.size();
    auto const Q1 = static_cast<int>(0.25 * size);
    auto const Q2 = static_cast<int>(0.50 * size);
    auto const Q3 = static_cast<int>(0.75 * size);
    infos->min = luojianet_ms::lite::GetMinValue(bak_data);
    std::nth_element(bak_data.begin() + 1, bak_data.begin() + Q1, bak_data.end());
    infos->quartile1 = bak_data.at(Q1);
    std::nth_element(bak_data.begin() + Q1 + 1, bak_data.begin() + Q2, bak_data.end());
    infos->median = bak_data.at(Q2);
    std::nth_element(bak_data.begin() + Q2 + 1, bak_data.begin() + Q3, bak_data.end());
    infos->quartile3 = bak_data.at(Q3);
    infos->max = luojianet_ms::lite::GetMaxValue(bak_data);
    auto mean_var = luojianet_ms::lite::GetMeanVar(bak_data);
    infos->mean = mean_var.first;
    infos->var = mean_var.second;
    infos->sparsity = luojianet_ms::lite::GetSparsity(bak_data);
  }

 private:
  // the key is {node_name, tensor_index}
  std::map<PrimaryKey, QuantDebugInfo> origin_info_;
  // Use vector to preserve ordert, There may be more nodes, such as QuantCast, bias etc.
  std::vector<QuantDebugInfo> compared_info_;
  std::vector<QuantParamExtend> quant_params_;
  // Mark whether to save quantization parameters. only save 1 round.
  bool save_flag_ = true;
};
}  // namespace luojianet_ms::lite

#endif  // LUOJIANET_MS_LITE_TOOLS_CONVERTER_DEBUGINFO_H
