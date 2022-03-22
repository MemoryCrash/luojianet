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

#ifndef LUOJIANET_MS_CCSRC_FRONTEND_PARALLEL_CONTEXT_H_
#define LUOJIANET_MS_CCSRC_FRONTEND_PARALLEL_CONTEXT_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "abstract/abstract_value.h"
#include "frontend/parallel/ops_info/ops_utils.h"
#include "frontend/parallel/status.h"
#include "ir/anf.h"
#include "ir/func_graph.h"
#include "utils/convert_utils.h"
#include "utils/info.h"
#include "pipeline/jit/pipeline.h"

namespace luojianet_ms {
namespace parallel {
constexpr char STAND_ALONE[] = "stand_alone";
constexpr char DATA_PARALLEL[] = "data_parallel";
constexpr char HYBRID_PARALLEL[] = "hybrid_parallel";
constexpr char AUTO_PARALLEL[] = "auto_parallel";
constexpr char SEMI_AUTO_PARALLEL[] = "semi_auto_parallel";

constexpr char DYNAMIC_PROGRAMMING[] = "dynamic_programming";
constexpr char RECURSIVE_PROGRAMMING[] = "recursive_programming";
constexpr char SHARDING_PROPAGATION[] = "sharding_propagation";

constexpr char TRAINING[] = "training";
constexpr char ACCUMULATION[] = "accumulation";

constexpr char ALL_GROUP_PARALLEL[] = "all_group_parallel";
constexpr char SAME_SERVER_GROUP_PARALLEL[] = "same_server_group_parallel";
constexpr char NO_GROUP_PARALLEL[] = "no_group_parallel";

constexpr char IS_FIRST_ITERATION[] = "is_first_iteration";

constexpr char FUSION_AUTO[] = "auto";
constexpr char FUSION_SIZE[] = "size";
constexpr char FUSION_INDEX[] = "index";
constexpr int64_t FUSUION_THRESHOLD = 64;
class ParallelContext {
 public:
  ~ParallelContext() = default;
  ParallelContext(const ParallelContext &) = delete;
  ParallelContext &operator=(const ParallelContext &) = delete;

  static std::shared_ptr<ParallelContext> GetInstance();

  void set_gradients_mean(bool gradients_mean);
  bool gradients_mean() const { return gradients_mean_; }

  void set_full_batch(bool full_batch);
  bool full_batch() const { return full_batch_; }

  void set_dataset_strategy(const std::vector<std::vector<int64_t>> &dataset_strategy);
  std::vector<std::vector<int64_t>> dataset_strategy() const { return dataset_strategy_; }

  void set_gradient_fp32_sync(bool gradient_fp32_sync);
  bool gradient_fp32_sync() const { return gradient_fp32_sync_; }

  void set_loss_repeated_mean(bool loss_repeated_mean);
  bool loss_repeated_mean() const { return loss_repeated_mean_; }

  void set_device_num(int64_t device_num);
  int64_t device_num() const { return device_num_; }

  void set_fusion_threshold_mb(int64_t fusion_threshold);
  int64_t fusion_threshold_mb() const { return fusion_threshold_mb_; }
  bool set_fusion_mode(const std::string &fusion_mode);
  std::string get_fusion_mode() const { return fusion_mode_; }

  void set_pipeline_stage_split_num(const int64_t stages);
  int64_t pipeline_stage_split_num() const { return pipeline_stage_split_num_; }

  void set_global_rank(int64_t global_rank);
  int64_t global_rank() const { return global_rank_; }

  void set_grad_accumulation_step(int64_t grad_accumulation_step);
  int64_t grad_accumulation_step() const { return grad_accumulation_step_; }

  bool set_parallel_mode(const std::string &parallel_mode);
  std::string parallel_mode() const { return parallel_mode_; }

  bool set_strategy_search_mode(const std::string &strategy_search_mode);
  std::string strategy_search_mode() const { return strategy_search_mode_; }

  void set_parameter_broadcast(bool parameter_broadcast);
  bool parameter_broadcast() const { return parameter_broadcast_; }

  bool device_num_is_set() const { return device_num_is_set_; }
  bool global_rank_is_set() const { return global_rank_is_set_; }
  bool parameter_broadcast_is_set() const { return parameter_broadcast_is_set_; }

  void set_optimizer_weight_shard_size(int64_t optimizer_weight_shard_size);
  int64_t optimizer_weight_shard_size() const { return optimizer_weight_shard_size_; }
  void set_optimizer_weight_shard_aggregated_save(bool optimizer_weight_shard_aggregated_save);
  bool optimizer_weight_shard_aggregated_save() const { return optimizer_weight_shard_aggregated_save_; }

  void SetAllReduceFusionSplitIndices(const std::vector<uint32_t> &indices, const std::string &group);
  std::vector<uint32_t> GetAllReduceFusionSplitIndices(const std::string &group) const;
  void SetAllReduceFusionSplitSizes(const std::vector<uint32_t> &sizes, const std::string &group);
  std::vector<uint32_t> GetAllReduceFusionSplitSizes(const std::string &group) const;
  void set_enable_all_reduce_fusion(bool enable_all_reduce_fusion) {
    enable_all_reduce_fusion_ = enable_all_reduce_fusion;
  }
  bool enable_all_reduce_fusion() const { return enable_all_reduce_fusion_; }

  void set_strategy_ckpt_load_file(const std::string &strategy_ckpt_load_file);
  std::string strategy_ckpt_load_file() const { return strategy_ckpt_load_file_; }
  void set_strategy_ckpt_save_file(const std::string &strategy_ckpt_save_file);
  std::string strategy_ckpt_save_file() const { return strategy_ckpt_save_file_; }
  void set_group_ckpt_save_file(const std::string &group_ckpt_save_file);
  std::string group_ckpt_save_file() const { return group_ckpt_save_file_; }

  void set_enable_parallel_optimizer(bool enable_parallel_optimizer) {
    enable_parallel_optimizer_ = enable_parallel_optimizer;
  }
  bool enable_parallel_optimizer() const { return enable_parallel_optimizer_; }

  void set_hccl_test_available(bool hccl_test_available) { hccl_test_available_ = hccl_test_available; }
  bool hccl_test_available() const { return hccl_test_available_; }
  void set_grad_accumulation_shard(const bool grad_accumulation_shard) {
    grad_accumulation_shard_ = grad_accumulation_shard;
  }
  bool grad_accumulation_shard() const { return grad_accumulation_shard_; }

  bool set_communi_parallel_mode(const std::string &communi_parallel_mode);
  std::string communi_parallel_mode() const { return communi_parallel_mode_; }
  void set_enable_all2all(const bool);
  bool enable_all2all() const { return enable_all2all_; }
  void set_dataset_repeat_dim_right(const bool dataset_repeat_dim_right) {
    dataset_repeat_dim_right_ = dataset_repeat_dim_right;
  }
  bool dataset_repeat_dim_right() const { return dataset_repeat_dim_right_; }

  void Reset();
  void ParallelParameterContextInitShape(const FuncGraphPtr &func_graph);
  void ParallelParameterContextRestoreShape(const FuncGraphPtr &func_graph, const ParameterPtr &param_node,
                                            const AbstractBasePtr &ptr);
  void ParallelParameterContextCkptShape(const FuncGraphPtr &func_graph, const ParameterPtr &param_node,
                                         const AbstractBasePtr &ptr);
  void set_sharding_propagation(const bool);
  bool sharding_propagation() const { return sharding_propagation_; }

 private:
  ParallelContext();
  static std::shared_ptr<ParallelContext> inst_context_;
  bool gradients_mean_;
  bool full_batch_;
  bool gradient_fp32_sync_;
  bool loss_repeated_mean_;
  int64_t device_num_;
  int64_t fusion_threshold_mb_;
  int64_t global_rank_;
  int64_t grad_accumulation_step_;
  std::string parallel_mode_;
  std::string strategy_search_mode_;
  int64_t pipeline_stage_split_num_;
  bool parameter_broadcast_;
  bool device_num_is_set_;
  bool fusion_threshold_is_set_;
  bool global_rank_is_set_;
  bool parameter_broadcast_is_set_;
  bool enable_all_reduce_fusion_;
  std::map<std::string, std::vector<uint32_t>> all_reduce_fusion_split_indices_;
  std::map<std::string, std::vector<uint32_t>> all_reduce_fusion_split_sizes_;
  std::string strategy_ckpt_load_file_;
  std::string strategy_ckpt_save_file_;
  std::string group_ckpt_save_file_;
  bool enable_parallel_optimizer_;
  bool init_param_shape_;
  std::string communi_parallel_mode_;
  int64_t optimizer_weight_shard_size_;
  bool optimizer_weight_shard_aggregated_save_;
  bool grad_accumulation_shard_;
  // Enable AllToAll or not. If false, use AllGather and Split.
  bool enable_all2all_;
  std::vector<std::vector<int64_t>> dataset_strategy_;
  bool dataset_repeat_dim_right_ = false;
  bool hccl_test_available_ = false;
  bool sharding_propagation_;
  std::string fusion_mode_;
};

}  // namespace parallel
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_FRONTEND_PARALLEL_CONTEXT_H_