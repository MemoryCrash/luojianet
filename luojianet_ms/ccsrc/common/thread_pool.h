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

#ifndef LUOJIANET_MS_CCSRC_COMMON_THREAD_POOL_H_
#define LUOJIANET_MS_CCSRC_COMMON_THREAD_POOL_H_

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <atomic>
#include <memory>
#include <utility>
#include <functional>
#include <iostream>
#include "utils/log_adapter.h"

namespace luojianet_ms {
namespace common {
enum Status { FAIL = -1, SUCCESS = 0 };
using Task = std::function<int()>;

struct ThreadContext {
  std::mutex mutex;
  std::condition_variable cond_var;
  const Task *task{nullptr};
};

class ThreadPool {
 public:
  ~ThreadPool();
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  static ThreadPool &GetInstance();
  bool SyncRun(const std::vector<Task> &tasks);
  size_t GetSyncRunThreadNum() { return max_thread_num_; }
  void ClearThreadPool();

 private:
  ThreadPool();
  void SyncRunLoop(const std::shared_ptr<ThreadContext> &context);

  size_t max_thread_num_{1};
  std::mutex pool_mtx_;
  std::atomic_bool exit_run_ = {false};
  std::vector<std::thread> sync_run_threads_{};
  std::vector<std::shared_ptr<ThreadContext>> contexts_;
};
}  // namespace common
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CCSRC_COMMON_THREAD_POOL_H_