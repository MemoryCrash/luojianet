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
#ifndef LUOJIANET_MS_CCSRC_MINDDATA_DATASET_ENGINE_IR_DATASETOPS_SOURCE_TEDLIUM_NODE_H_
#define LUOJIANET_MS_CCSRC_MINDDATA_DATASET_ENGINE_IR_DATASETOPS_SOURCE_TEDLIUM_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "minddata/dataset/engine/ir/datasetops/dataset_node.h"

namespace luojianet_ms {
namespace dataset {
class TedliumNode : public MappableSourceNode {
 public:
  /// \brief Constructor.
  TedliumNode(const std::string &dataset_dir, const std::string &release, const std::string &usage,
              const std::string &extensions, const std::shared_ptr<SamplerObj> &sampler,
              const std::shared_ptr<DatasetCache> &cache);

  /// \brief Destructor.
  ~TedliumNode() = default;

  /// \brief Node name getter.
  /// \return Name of the current node.
  std::string Name() const override { return kTedliumNode; }

  /// \brief Print the description.
  /// \param out - The output stream to write output to.
  void Print(std::ostream &out) const override;

  /// \brief Copy the node to a new object.
  /// \return A shared pointer to the new copy.
  std::shared_ptr<DatasetNode> Copy() override;

  /// \brief a base class override function to create the required runtime dataset op objects for this class.
  /// \param node_ops - A vector containing shared pointer to the Dataset Ops that this object will create.
  /// \return Status Status::OK() if build successfully.
  Status Build(std::vector<std::shared_ptr<DatasetOp>> *const node_ops) override;

  /// \brief Parameters validation.
  /// \return Status Status::OK() if all the parameters are valid.
  Status ValidateParams() override;

  /// \brief Get the shard id of node.
  /// \param[in] shard_id Shard id.
  /// \return Status Status::OK() if get shard id successfully.
  Status GetShardId(int32_t *shard_id) override;

  /// \brief Base-class override for GetDatasetSize.
  /// \param[in] size_getter Shared pointer to DatasetSizeGetter.
  /// \param[in] estimate This is only supported by some of the ops and it's used to speed up the process of getting
  ///     dataset size at the expense of accuracy.
  /// \param[out] dataset_size the size of the dataset.
  /// \return Status of the function.
  Status GetDatasetSize(const std::shared_ptr<DatasetSizeGetter> &size_getter, bool estimate,
                        int64_t *dataset_size) override;

  /// \brief Get the arguments of node.
  /// \param[out] out_json JSON string of all attributes.
  /// \return Status of the function.
  Status to_json(nlohmann::json *out_json) override;

  /// \brief Sampler getter.
  /// \return SamplerObj of the current node.
  std::shared_ptr<SamplerObj> Sampler() override { return sampler_; }

  /// \brief Sampler setter.
  /// \param[in] sampler Sampler object used to choose samples from the dataset.
  void SetSampler(std::shared_ptr<SamplerObj> sampler) override { sampler_ = sampler; }

  /// \brief Release getter.
  /// \return Release of the current node.
  const std::string &Release() const { return release_; }

  /// \brief DatasetDir getter.
  /// \return DatasetDir of the current node.
  const std::string &DatasetDir() const { return dataset_dir_; }

  /// \brief Usage getter.
  /// \return Usage of the current node.
  const std::string &Usage() const { return usage_; }

  /// \brief Extensions getter.
  /// \return Extensions of the current node.
  const std::string &Extensions() const { return extensions_; }

 private:
  std::string dataset_dir_;
  std::string release_;
  std::string usage_;
  std::string extensions_;
  std::shared_ptr<SamplerObj> sampler_;
};  // class TedliumNode
}  // namespace dataset
}  // namespace luojianet_ms
#endif  // LUOJIANET_MS_CCSRC_MINDDATA_DATASET_ENGINE_IR_DATASETOPS_SOURCE_TEDLIUM_NODE_H_