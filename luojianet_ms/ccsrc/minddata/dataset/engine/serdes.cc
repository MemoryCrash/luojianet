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
#include <fstream>
#include <stack>
#include "minddata/dataset/engine/serdes.h"

#include "minddata/dataset/core/pybind_support.h"
#include "utils/file_utils.h"
#include "utils/utils.h"

namespace luojianet_ms {
namespace dataset {

std::map<std::string, Status (*)(nlohmann::json json_obj, std::shared_ptr<TensorOperation> *operation)>
  Serdes::func_ptr_ = Serdes::InitializeFuncPtr();

Status Serdes::SaveToJSON(std::shared_ptr<DatasetNode> node, const std::string &filename, nlohmann::json *out_json) {
  RETURN_UNEXPECTED_IF_NULL(node);
  RETURN_UNEXPECTED_IF_NULL(out_json);
  // Dump attributes of current node to json string
  nlohmann::json args;
  RETURN_IF_NOT_OK(node->to_json(&args));
  args["op_type"] = node->Name();

  // If the current node isn't leaf node, visit all its children and get all attributes
  std::vector<nlohmann::json> children_pipeline;
  if (!node->IsLeaf()) {
    for (auto child : node->Children()) {
      nlohmann::json child_args;
      RETURN_IF_NOT_OK(SaveToJSON(child, "", &child_args));
      children_pipeline.push_back(child_args);
    }
  }
  args["children"] = children_pipeline;

  // Save json string into file if filename is given.
  if (!filename.empty()) {
    RETURN_IF_NOT_OK(SaveJSONToFile(args, filename));
  }

  *out_json = args;
  return Status::OK();
}

Status Serdes::SaveJSONToFile(nlohmann::json json_string, const std::string &file_name) {
  try {
    std::optional<std::string> dir = "";
    std::optional<std::string> local_file_name = "";
    FileUtils::SplitDirAndFileName(file_name, &dir, &local_file_name);
    if (!dir.has_value()) {
      dir = ".";
    }
    auto realpath = FileUtils::GetRealPath(dir.value().data());
    if (!realpath.has_value()) {
      MS_LOG(ERROR) << "Invalid file, get real path failed, path=" << file_name;
      RETURN_STATUS_UNEXPECTED("Invalid file, get real path failed, path=" + file_name);
    }

    std::optional<std::string> whole_path = "";
    FileUtils::ConcatDirAndFileName(&realpath, &local_file_name, &whole_path);

    std::ofstream file(whole_path.value());
    file << json_string;
    file.close();

    ChangeFileMode(whole_path.value(), S_IRUSR | S_IWUSR);
  } catch (const std::exception &err) {
    RETURN_STATUS_UNEXPECTED("Invalid data, failed to save json string into file: " + file_name +
                             ", error message: " + err.what());
  }
  return Status::OK();
}

Status Serdes::Deserialize(const std::string &json_filepath, std::shared_ptr<DatasetNode> *ds) {
  nlohmann::json json_obj;
  CHECK_FAIL_RETURN_UNEXPECTED(json_filepath.size() != 0, "Json path is null");
  std::ifstream json_in(json_filepath);
  CHECK_FAIL_RETURN_UNEXPECTED(json_in, "Invalid file, failed to open json file: " + json_filepath);
  try {
    json_in >> json_obj;
  } catch (const std::exception &e) {
    return Status(StatusCode::kMDSyntaxError,
                  "Invalid file, failed to parse json file: " + json_filepath + ", error message: " + e.what());
  }
  RETURN_IF_NOT_OK(ConstructPipeline(json_obj, ds));
  return Status::OK();
}

Status Serdes::ConstructPipeline(nlohmann::json json_obj, std::shared_ptr<DatasetNode> *ds) {
  CHECK_FAIL_RETURN_UNEXPECTED(json_obj.find("children") != json_obj.end(), "Failed to find children");
  std::shared_ptr<DatasetNode> child_ds;

  if (json_obj["children"].size() == 0) {
    // If the JSON object has no child, then this node is a leaf node. Call create node to construct the corresponding
    // leaf node
    RETURN_IF_NOT_OK(CreateNode(nullptr, json_obj, ds));
  } else if (json_obj["children"].size() == 1) {
    // This node only has one child, construct the sub-tree under it first, and then call create node to construct the
    // corresponding node
    RETURN_IF_NOT_OK(ConstructPipeline(json_obj["children"][0], &child_ds));
    RETURN_IF_NOT_OK(CreateNode(child_ds, json_obj, ds));
  } else {
    std::vector<std::shared_ptr<DatasetNode>> datasets;
    for (auto child_json_obj : json_obj["children"]) {
      RETURN_IF_NOT_OK(ConstructPipeline(child_json_obj, &child_ds));
      datasets.push_back(child_ds);
    }
    if (json_obj["op_type"] == "Zip") {
      CHECK_FAIL_RETURN_UNEXPECTED(datasets.size() > 1, "Should zip more than 1 dataset");
      RETURN_IF_NOT_OK(ZipNode::from_json(datasets, ds));
    } else if (json_obj["op_type"] == "Concat") {
      CHECK_FAIL_RETURN_UNEXPECTED(datasets.size() > 1, "Should concat more than 1 dataset");
      RETURN_IF_NOT_OK(ConcatNode::from_json(json_obj, datasets, ds));
    } else {
      return Status(StatusCode::kMDUnexpectedError,
                    "Invalid data, unsupported operation type: " + std::string(json_obj["op_type"]));
    }
  }
  return Status::OK();
}

Status Serdes::CreateNode(const std::shared_ptr<DatasetNode> &child_ds, nlohmann::json json_obj,
                          std::shared_ptr<DatasetNode> *ds) {
  CHECK_FAIL_RETURN_UNEXPECTED(json_obj.find("op_type") != json_obj.end(), "Failed to find op_type in json.");
  std::string op_type = json_obj["op_type"];
  if (child_ds == nullptr) {
    // if dataset doesn't have any child, then create a source dataset IR. e.g., ImageFolderNode, CocoNode
    RETURN_IF_NOT_OK(CreateDatasetNode(json_obj, op_type, ds));
  } else {
    // if the dataset has at least one child, then create an operation dataset IR, e.g., BatchNode, MapNode
    RETURN_IF_NOT_OK(CreateDatasetOperationNode(child_ds, json_obj, op_type, ds));
  }
  return Status::OK();
}

Status Serdes::CreateDatasetNode(const nlohmann::json &json_obj, const std::string &op_type,
                                 std::shared_ptr<DatasetNode> *ds) {
  if (op_type == kAlbumNode) {
    RETURN_IF_NOT_OK(AlbumNode::from_json(json_obj, ds));
  } else if (op_type == kCelebANode) {
    RETURN_IF_NOT_OK(CelebANode::from_json(json_obj, ds));
  } else if (op_type == kCifar10Node) {
    RETURN_IF_NOT_OK(Cifar10Node::from_json(json_obj, ds));
  } else if (op_type == kCifar100Node) {
    RETURN_IF_NOT_OK(Cifar100Node::from_json(json_obj, ds));
  } else if (op_type == kCLUENode) {
    RETURN_IF_NOT_OK(CLUENode::from_json(json_obj, ds));
  } else if (op_type == kCocoNode) {
    RETURN_IF_NOT_OK(CocoNode::from_json(json_obj, ds));
  } else if (op_type == kCSVNode) {
    RETURN_IF_NOT_OK(CSVNode::from_json(json_obj, ds));
  } else if (op_type == kFlickrNode) {
    RETURN_IF_NOT_OK(FlickrNode::from_json(json_obj, ds));
  } else if (op_type == kImageFolderNode) {
    RETURN_IF_NOT_OK(ImageFolderNode::from_json(json_obj, ds));
  } else if (op_type == kManifestNode) {
    RETURN_IF_NOT_OK(ManifestNode::from_json(json_obj, ds));
  } else if (op_type == kMnistNode) {
    RETURN_IF_NOT_OK(MnistNode::from_json(json_obj, ds));
  } else if (op_type == kTextFileNode) {
    RETURN_IF_NOT_OK(TextFileNode::from_json(json_obj, ds));
  } else if (op_type == kTFRecordNode) {
    RETURN_IF_NOT_OK(TFRecordNode::from_json(json_obj, ds));
  } else if (op_type == kVOCNode) {
    RETURN_IF_NOT_OK(VOCNode::from_json(json_obj, ds));
  } else {
    return Status(StatusCode::kMDUnexpectedError, "Invalid data, unsupported operation type: " + op_type);
  }
  return Status::OK();
}

Status Serdes::CreateDatasetOperationNode(const std::shared_ptr<DatasetNode> &ds, const nlohmann::json &json_obj,
                                          const std::string &op_type, std::shared_ptr<DatasetNode> *result) {
  if (op_type == kBatchNode) {
    RETURN_IF_NOT_OK(BatchNode::from_json(json_obj, ds, result));
  } else if (op_type == kMapNode) {
    RETURN_IF_NOT_OK(MapNode::from_json(json_obj, ds, result));
  } else if (op_type == kProjectNode) {
    RETURN_IF_NOT_OK(ProjectNode::from_json(json_obj, ds, result));
  } else if (op_type == kRenameNode) {
    RETURN_IF_NOT_OK(RenameNode::from_json(json_obj, ds, result));
  } else if (op_type == kRepeatNode) {
    RETURN_IF_NOT_OK(RepeatNode::from_json(json_obj, ds, result));
  } else if (op_type == kShuffleNode) {
    RETURN_IF_NOT_OK(ShuffleNode::from_json(json_obj, ds, result));
  } else if (op_type == kSkipNode) {
    RETURN_IF_NOT_OK(SkipNode::from_json(json_obj, ds, result));
  } else if (op_type == kTransferNode) {
    RETURN_IF_NOT_OK(TransferNode::from_json(json_obj, ds, result));
  } else if (op_type == kTakeNode) {
    RETURN_IF_NOT_OK(TakeNode::from_json(json_obj, ds, result));
  } else {
    return Status(StatusCode::kMDUnexpectedError, "Invalid data, unsupported operation type: " + op_type);
  }
  return Status::OK();
}

Status Serdes::ConstructSampler(nlohmann::json json_obj, std::shared_ptr<SamplerObj> *sampler) {
  CHECK_FAIL_RETURN_UNEXPECTED(json_obj.find("num_samples") != json_obj.end(), "Failed to find num_samples");
  CHECK_FAIL_RETURN_UNEXPECTED(json_obj.find("sampler_name") != json_obj.end(), "Failed to find sampler_name");
  int64_t num_samples = json_obj["num_samples"];
  std::string sampler_name = json_obj["sampler_name"];
  if (sampler_name == "DistributedSampler") {
    RETURN_IF_NOT_OK(DistributedSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "PKSampler") {
    RETURN_IF_NOT_OK(PKSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "RandomSampler") {
    RETURN_IF_NOT_OK(RandomSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "SequentialSampler") {
    RETURN_IF_NOT_OK(SequentialSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "SubsetSampler") {
    RETURN_IF_NOT_OK(SubsetSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "SubsetRandomSampler") {
    RETURN_IF_NOT_OK(SubsetRandomSamplerObj::from_json(json_obj, num_samples, sampler));
  } else if (sampler_name == "WeightedRandomSampler") {
    RETURN_IF_NOT_OK(WeightedRandomSamplerObj::from_json(json_obj, num_samples, sampler));
  } else {
    return Status(StatusCode::kMDUnexpectedError, "Invalid data, unsupported sampler type: " + sampler_name);
  }
  return Status::OK();
}

Status Serdes::ConstructTensorOps(nlohmann::json json_obj, std::vector<std::shared_ptr<TensorOperation>> *result) {
  std::vector<std::shared_ptr<TensorOperation>> output;
  for (nlohmann::json item : json_obj) {
    if (item.find("python_module") != item.end()) {
      if (Py_IsInitialized() != 0) {
        RETURN_IF_NOT_OK(PyFuncOp::from_json(item, result));
      } else {
        LOG_AND_RETURN_STATUS_SYNTAX_ERROR(
          "Python module is not initialized or Pyfunction is not supported on this platform.");
      }
    } else {
      CHECK_FAIL_RETURN_UNEXPECTED(item.find("tensor_op_name") != item.end(), "Failed to find tensor_op_name");
      CHECK_FAIL_RETURN_UNEXPECTED(item.find("tensor_op_params") != item.end(), "Failed to find tensor_op_params");
      std::string op_name = item["tensor_op_name"];
      nlohmann::json op_params = item["tensor_op_params"];
      std::shared_ptr<TensorOperation> operation = nullptr;
      CHECK_FAIL_RETURN_UNEXPECTED(func_ptr_.find(op_name) != func_ptr_.end(),
                                   "Invalid data, unsupported operation: " + op_name);
      RETURN_IF_NOT_OK(func_ptr_[op_name](op_params, &operation));
      output.push_back(operation);
      *result = output;
    }
  }
  return Status::OK();
}

std::map<std::string, Status (*)(nlohmann::json json_obj, std::shared_ptr<TensorOperation> *operation)>
Serdes::InitializeFuncPtr() {
  std::map<std::string, Status (*)(nlohmann::json json_obj, std::shared_ptr<TensorOperation> * operation)> ops_ptr;
  ops_ptr[vision::kAdjustGammaOperation] = &(vision::AdjustGammaOperation::from_json);
  ops_ptr[vision::kAffineOperation] = &(vision::AffineOperation::from_json);
  ops_ptr[vision::kAutoContrastOperation] = &(vision::AutoContrastOperation::from_json);
  ops_ptr[vision::kBoundingBoxAugmentOperation] = &(vision::BoundingBoxAugmentOperation::from_json);
  ops_ptr[vision::kCenterCropOperation] = &(vision::CenterCropOperation::from_json);
  ops_ptr[vision::kCropOperation] = &(vision::CropOperation::from_json);
  ops_ptr[vision::kCutMixBatchOperation] = &(vision::CutMixBatchOperation::from_json);
  ops_ptr[vision::kCutOutOperation] = &(vision::CutOutOperation::from_json);
  ops_ptr[vision::kDecodeOperation] = &(vision::DecodeOperation::from_json);
#ifdef ENABLE_ACL
  ops_ptr[vision::kDvppCropJpegOperation] = &(vision::DvppCropJpegOperation::from_json);
  ops_ptr[vision::kDvppDecodeResizeOperation] = &(vision::DvppDecodeResizeOperation::from_json);
  ops_ptr[vision::kDvppDecodeResizeCropOperation] = &(vision::DvppDecodeResizeCropOperation::from_json);
  ops_ptr[vision::kDvppNormalizeOperation] = &(vision::DvppNormalizeOperation::from_json);
  ops_ptr[vision::kDvppResizeJpegOperation] = &(vision::DvppResizeJpegOperation::from_json);
#endif
  ops_ptr[vision::kEqualizeOperation] = &(vision::EqualizeOperation::from_json);
  ops_ptr[vision::kGaussianBlurOperation] = &(vision::GaussianBlurOperation::from_json);
  ops_ptr[vision::kHorizontalFlipOperation] = &(vision::HorizontalFlipOperation::from_json);
  ops_ptr[vision::kHwcToChwOperation] = &(vision::HwcToChwOperation::from_json);
  ops_ptr[vision::kInvertOperation] = &(vision::InvertOperation::from_json);
  ops_ptr[vision::kMixUpBatchOperation] = &(vision::MixUpBatchOperation::from_json);
  ops_ptr[vision::kNormalizeOperation] = &(vision::NormalizeOperation::from_json);
  ops_ptr[vision::kNormalizePadOperation] = &(vision::NormalizePadOperation::from_json);
  ops_ptr[vision::kPadOperation] = &(vision::PadOperation::from_json);
  ops_ptr[vision::kRandomAffineOperation] = &(vision::RandomAffineOperation::from_json);
  ops_ptr[vision::kRandomColorOperation] = &(vision::RandomColorOperation::from_json);
  ops_ptr[vision::kRandomColorAdjustOperation] = &(vision::RandomColorAdjustOperation::from_json);
  ops_ptr[vision::kRandomCropDecodeResizeOperation] = &(vision::RandomCropDecodeResizeOperation::from_json);
  ops_ptr[vision::kRandomCropOperation] = &(vision::RandomCropOperation::from_json);
  ops_ptr[vision::kRandomCropWithBBoxOperation] = &(vision::RandomCropWithBBoxOperation::from_json);
  ops_ptr[vision::kRandomHorizontalFlipOperation] = &(vision::RandomHorizontalFlipOperation::from_json);
  ops_ptr[vision::kRandomHorizontalFlipWithBBoxOperation] = &(vision::RandomHorizontalFlipWithBBoxOperation::from_json);
  ops_ptr[vision::kRandomPosterizeOperation] = &(vision::RandomPosterizeOperation::from_json);
  ops_ptr[vision::kRandomResizeOperation] = &(vision::RandomResizeOperation::from_json);
  ops_ptr[vision::kRandomResizeWithBBoxOperation] = &(vision::RandomResizeWithBBoxOperation::from_json);
  ops_ptr[vision::kRandomResizedCropOperation] = &(vision::RandomResizedCropOperation::from_json);
  ops_ptr[vision::kRandomResizedCropWithBBoxOperation] = &(vision::RandomResizedCropWithBBoxOperation::from_json);
  ops_ptr[vision::kRandomRotationOperation] = &(vision::RandomRotationOperation::from_json);
  ops_ptr[vision::kRandomSelectSubpolicyOperation] = &(vision::RandomSelectSubpolicyOperation::from_json);
  ops_ptr[vision::kRandomSharpnessOperation] = &(vision::RandomSharpnessOperation::from_json);
  ops_ptr[vision::kRandomSolarizeOperation] = &(vision::RandomSolarizeOperation::from_json);
  ops_ptr[vision::kRandomVerticalFlipOperation] = &(vision::RandomVerticalFlipOperation::from_json);
  ops_ptr[vision::kRandomVerticalFlipWithBBoxOperation] = &(vision::RandomVerticalFlipWithBBoxOperation::from_json);
  ops_ptr[vision::kRandomSharpnessOperation] = &(vision::RandomSharpnessOperation::from_json);
  ops_ptr[vision::kRandomSolarizeOperation] = &(vision::RandomSolarizeOperation::from_json);
  ops_ptr[vision::kRescaleOperation] = &(vision::RescaleOperation::from_json);
  ops_ptr[vision::kResizeOperation] = &(vision::ResizeOperation::from_json);
  ops_ptr[vision::kResizePreserveAROperation] = &(vision::ResizePreserveAROperation::from_json);
  ops_ptr[vision::kResizeWithBBoxOperation] = &(vision::ResizeWithBBoxOperation::from_json);
  ops_ptr[vision::kRgbaToBgrOperation] = &(vision::RgbaToBgrOperation::from_json);
  ops_ptr[vision::kRgbaToRgbOperation] = &(vision::RgbaToRgbOperation::from_json);
  ops_ptr[vision::kRgbToBgrOperation] = &(vision::RgbToBgrOperation::from_json);
  ops_ptr[vision::kRgbToGrayOperation] = &(vision::RgbToGrayOperation::from_json);
  ops_ptr[vision::kRotateOperation] = &(vision::RotateOperation::from_json);
  ops_ptr[vision::kSlicePatchesOperation] = &(vision::SlicePatchesOperation::from_json);
  ops_ptr[vision::kSoftDvppDecodeRandomCropResizeJpegOperation] =
    &(vision::SoftDvppDecodeRandomCropResizeJpegOperation::from_json);
  ops_ptr[vision::kSoftDvppDecodeResizeJpegOperation] = &(vision::SoftDvppDecodeResizeJpegOperation::from_json);
  ops_ptr[vision::kSwapRedBlueOperation] = &(vision::SwapRedBlueOperation::from_json);
  ops_ptr[vision::kUniformAugOperation] = &(vision::UniformAugOperation::from_json);
  ops_ptr[vision::kVerticalFlipOperation] = &(vision::VerticalFlipOperation::from_json);
  ops_ptr[transforms::kFillOperation] = &(transforms::FillOperation::from_json);
  ops_ptr[transforms::kOneHotOperation] = &(transforms::OneHotOperation::from_json);
  ops_ptr[transforms::kTypeCastOperation] = &(transforms::TypeCastOperation::from_json);
  ops_ptr[text::kToNumberOperation] = &(text::ToNumberOperation::from_json);
  return ops_ptr;
}

Status Serdes::ParseMindIRPreprocess(const std::vector<std::string> &map_json_string,
                                     std::vector<std::shared_ptr<luojianet_ms::dataset::Execute>> *data_graph) {
  CHECK_FAIL_RETURN_UNEXPECTED(!map_json_string.empty(), "Invalid data, no json data in map_json_string.");

  const std::string process_column = "[\"image\"]";
  MS_LOG(WARNING) << "Only supports parse \"image\" column from dataset object.";

  nlohmann::json map_json;
  try {
    for (auto &json : map_json_string) {
      map_json = nlohmann::json::parse(json);
      if (map_json["input_columns"].dump() == process_column) {
        break;
      }
    }
  } catch (const std::exception &err) {
    MS_LOG(ERROR) << "Invalid json content, failed to parse JSON data, error message: " << err.what();
    RETURN_STATUS_UNEXPECTED("Invalid json content, failed to parse JSON data.");
  }

  if (map_json.empty()) {
    MS_LOG(ERROR) << "Invalid json content, no JSON data found for given input column: " + process_column;
    RETURN_STATUS_UNEXPECTED("Invalid json content, no JSON data found for given input column: " + process_column);
  }

  while (map_json != nullptr) {
    CHECK_FAIL_RETURN_UNEXPECTED(map_json["op_type"] == "Map", "Invalid json content, this is not a MapOp.");

    std::vector<std::shared_ptr<TensorOperation>> tensor_ops;
    RETURN_IF_NOT_OK(ConstructTensorOps(map_json["operations"], &tensor_ops));
    if (map_json["input_columns"].dump() == process_column) {
      std::vector<std::string> op_names;
      std::transform(tensor_ops.begin(), tensor_ops.end(), std::back_inserter(op_names),
                     [](const auto &op) { return op->Name(); });
      MS_LOG(INFO) << "Find valid preprocess operations: " << op_names;
      data_graph->push_back(std::make_shared<Execute>(tensor_ops));
    }
    map_json = map_json["children"];
  }

  if (!data_graph->size()) {
    MS_LOG(WARNING) << "Can not find any valid preprocess operation.";
  }

  return Status::OK();
}

// In the current stage, there is a cyclic dependency between libluojianet.so and c_dataengine.so,
// we make a C function here and dlopen by libminspore.so to avoid linking explicitly,
// will be fix after decouling libminspore.so into multi submodules
extern "C" {
// ParseMindIRPreprocess_C has C-linkage specified, but returns user-defined type 'luojianet_ms::Status'
// which is incompatible with C
void ParseMindIRPreprocess_C(const std::vector<std::string> &dataset_json,
                             std::vector<std::shared_ptr<luojianet_ms::dataset::Execute>> *data_graph, Status *s) {
  Status ret = Serdes::ParseMindIRPreprocess(dataset_json, data_graph);
  *s = Status(ret);
}
}

}  // namespace dataset
}  // namespace luojianet_ms