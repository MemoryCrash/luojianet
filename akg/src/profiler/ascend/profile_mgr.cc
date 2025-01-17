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

#include <string.h>
#include "profile_mgr.h"
#include "toolchain/prof_acl_api.h"
#include <runtime/rt.h>
#include <dmlc/logging.h>
#include <tvm/runtime/registry.h>

namespace {
constexpr uint32_t kProfilingDeviceNum = 1;
constexpr auto kRtSetDeviceRegName = "profiling";
constexpr Status PROF_SUCCESS = 0;
constexpr Status PROF_FAILED = 0xFFFFFFFF;
}  // namespace

Status ProfCommandHandle(ProfCommandHandleType type) {
  return air::runtime::ProfileMgr::GetInstance().CommandHandle(type);
}

bool IsInitialize() { return true; }

namespace air {
namespace runtime {
Status ProfileMgr::PluginInit() const {
  if (reporter_cb_ == nullptr) {
    LOG(ERROR) << "reporter_cb_ is nullptr.";
    return PROF_FAILED;
  }
  return reporter_cb_(static_cast<uint32_t>(MsprofReporterModuleId::MSPROF_MODULE_FRAMEWORK),
                      static_cast<uint32_t>(MsprofReporterCallbackType::MSPROF_REPORTER_INIT), nullptr, 0);
}

void ProfileMgr::PluginUnInit() const {
  if (reporter_cb_ == nullptr) {
    LOG(ERROR) << "MsprofReporterCallback callback is nullptr.";
    return;
  }
  int32_t ret = reporter_cb_(static_cast<uint32_t>(MsprofReporterModuleId::MSPROF_MODULE_FRAMEWORK),
                             static_cast<uint32_t>(MsprofReporterCallbackType::MSPROF_REPORTER_UNINIT), nullptr, 0);
  if (ret != 0) {
    LOG(WARNING) << "profiling plugin uninit failed, ret:%d" << ret;
  }
}

uint64_t ProfileMgr::GetProfilingModule() const {
  return PROF_MODEL_EXECUTE_MASK | PROF_RUNTIME_API_MASK | PROF_RUNTIME_TRACE_MASK | PROF_SCHEDULE_TIMELINE_MASK |
         PROF_SCHEDULE_TRACE_MASK | PROF_TASK_TIME_MASK | PROF_SUBTASK_TIME_MASK | PROF_AICPU_TRACE_MASK |
         PROF_AICORE_METRICS_MASK | PROF_AIVECTORCORE_METRICS_MASK | PROF_MODEL_LOAD_MASK;
}

uint64_t ProfileMgr::GetJobId() const {
  const char *job_id = std::getenv("JOB_ID");
  return ((job_id != nullptr) ? std::strtoul(job_id, nullptr, 10) : 0);
}

Status ProfileMgr::GetProfConf(MsprofGeOptions *prof) {
  std::string job_id = std::to_string(GetJobId());

  if (memcpy(prof->jobId, job_id.c_str(), job_id.size()) == nullptr) {
    LOG(ERROR) << "Copy job_id failed.";
    return PROF_FAILED;
  }

  char *profile_dir = std::getenv("PROFILING_DIR");

  if (profile_dir == nullptr) {
    LOG(ERROR) << "Environment PROFILING_DIR not set";
    return PROF_FAILED;
  }

  const std::string prof_options_str = "{\"output\":\"" + std::string(profile_dir) +
                                       "\", \"training_trace\":\"on\", \
      \"task_trace\":\"on\", \"aic_metrics\":\"PipeUtilization\", \"aicpu\":\"on\"}";

  if (memcpy(prof->options, prof_options_str.c_str(), prof_options_str.size()) == nullptr) {
    LOG(ERROR) << "Copy profiling_options failed";
    return PROF_FAILED;
  }
  return PROF_SUCCESS;
}

bool ProfileMgr::StartupProfiling(uint32_t device_id) {
  device_id_ = device_id;

  struct MsprofGeOptions prof_conf = {0};
  if (GetProfConf(&prof_conf) != PROF_SUCCESS) {
    LOG(ERROR) << "Get prof conf failed.";
    return false;
  }

  if (!ProfStartUp(&prof_conf)) {
    LOG(ERROR) << "ProfMgrStartUp failed.";
    return false;
  }
  return true;
}

bool ProfileMgr::ProfStartUp(MsprofGeOptions *prof_conf) {
  LOG(INFO) << "Prof start up. ";

  bool ret = ProfRegisterCtrlCallback();
  if (!ret) {
    return ret;
  }

  // call profiling start up api
  int32_t cb_ret = MsprofInit(static_cast<uint32_t>(MsprofCtrlCallbackType::MSPROF_CTRL_INIT_GE_OPTIONS),
                              static_cast<void *>(prof_conf), sizeof(MsprofGeOptions));
  if (cb_ret != PROF_SUCCESS) {
    LOG(ERROR) << "Call msprofCtrlCallback failed, ret: " << cb_ret;
    return false;
  }

  LOG(INFO) << "Start up profiling success.";
  return true;
}

bool ProfileMgr::StopProfiling() {
  LOG(INFO) << "StopProfiling";

  // plugin unregister
  PluginUnInit();
  // stop runtime profiler
  int32_t cb_ret = MsprofFinalize();
  if (cb_ret != 0) {
    LOG(WARNING) << "Call MsprofFinalize failed, ret: " << cb_ret;
    return false;
  }
  return true;
}

Status ProfileMgr::CommandHandle(ProfCommandHandleType type) {
  LOG(INFO) << "ProfCommandHandle start, type:" << type;
  if (type == kProfCommandhandleInit) {
    auto cb_ret = ProfileMgr::GetInstance().PluginInit();
    if (cb_ret != PROF_SUCCESS) {
      LOG(ERROR) << "Profiling plugin int failed.";
      return PROF_FAILED;
    }
  }
  return PROF_SUCCESS;
}

ProfileMgr &ProfileMgr::GetInstance() {
  static ProfileMgr mgr;
  return mgr;
}

bool ProfileMgr::ProfRegisterCtrlCallback() const {
  rtError_t rt_ret = rtProfRegisterCtrlCallback(GE, CtrlCallbackHandle);
  if (rt_ret != RT_ERROR_NONE) {
    LOG(ERROR) << "Call rtProfRegisterCtrlCallback failed.";
    return false;
  }

  return true;
}

rtError_t CtrlCallbackHandle(uint32_t rt_type, void *data, uint32_t len) {
  if (rt_type == RT_PROF_CTRL_REPORTER) {
    ProfileMgr::GetInstance().RegReporterCallback(reinterpret_cast<MsprofReporterCallback>(data));
    LOG(INFO) << "Set MsprofReporterCallback success.";
  } else if (rt_type == RT_PROF_CTRL_SWITCH) {
    Status ret = ProfCtrlSwitchHandle(data);
    if (ret != PROF_SUCCESS) {
      LOG(ERROR) << "Start runtime profiler failed.";
    }
  }

  return RT_ERROR_NONE;
}

Status ProfCtrlSwitchHandle(void *data) {
  if (data == nullptr) {
    LOG(ERROR) << "Ctrl switch handl data is nullptr.";
    return PROF_FAILED;
  }

  rtProfCommandHandle_t *prof_config_param = reinterpret_cast<rtProfCommandHandle_t *>(data);
  auto type = static_cast<ProfCommandHandleType>(prof_config_param->type);
  return ProfCommandHandle(type);
}

TVM_REGISTER_GLOBAL("ascend_start_profiling").set_body([](TVMArgs args, TVMRetValue *ret) {
  ProfileMgr::GetInstance().StartupProfiling(static_cast<uint32_t>(args[0].operator int()));
});

TVM_REGISTER_GLOBAL("ascend_stop_profiling").set_body([](TVMArgs args, TVMRetValue *ret) {
  ProfileMgr::GetInstance().StopProfiling();
});

TVM_REGISTER_GLOBAL("ascend_get_kernel_label").set_body([](TVMArgs args, TVMRetValue *ret) {
  *ret = ProfileMgr::GetInstance().GetKernelLabel();
});

}  // namespace runtime
}  // namespace air
