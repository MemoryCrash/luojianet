/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: dev.h
 * Create: 2020-01-01
 */

#ifndef CCE_RUNTIME_DEVICE_H
#define CCE_RUNTIME_DEVICE_H

#include "base.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define RT_CAPABILITY_SUPPORT     (0x1U)
#define RT_CAPABILITY_NOT_SUPPORT (0x0U)
#define MEMORY_INFO_TS_4G_LIMITED (0x0U) // for compatibility

typedef struct tagRTDeviceInfo {
    uint8_t env_type;  // 0: FPGA  1: EMU 2: ESL
    uint32_t ctrl_cpu_ip;
    uint32_t ctrl_cpu_id;
    uint32_t ctrl_cpu_core_num;
    uint32_t ctrl_cpu_endian_little;
    uint32_t ts_cpu_core_num;
    uint32_t ai_cpu_core_num;
    uint32_t ai_core_num;
    uint32_t ai_core_freq;
    uint32_t ai_cpu_core_id;
    uint32_t ai_core_id;
    uint32_t aicpu_occupy_bitmap;
    uint32_t hardware_version;
    uint32_t ts_num;
} rtDeviceInfo_t;

typedef enum tagRtRunMode {
    RT_RUN_MODE_OFFLINE = 0,
    RT_RUN_MODE_ONLINE,
    RT_RUN_MODE_AICPU_SCHED,
    RT_RUN_MODE_RESERVED
} rtRunMode;

typedef enum tagRtAicpuDeployType {
    AICPU_DEPLOY_CROSS_OS = 0x0,
    AICPU_DEPLOY_CROSS_PROCESS,
    AICPU_DEPLOY_CROSS_THREAD,
    AICPU_DEPLOY_RESERVED
} rtAicpuDeployType_t;

typedef enum tagRtFeatureType {
    FEATURE_TYPE_MEMCPY = 0,
    FEATURE_TYPE_MEMORY,
    FEATURE_TYPE_RSV
} rtFeatureType_t;

typedef enum tagRtDeviceFeatureType {
  FEATURE_TYPE_SCHE,
  FEATURE_TYPE_BLOCKING_OPERATOR,
  FEATURE_TYPE_FFTS_MODE,
  FEATURE_TYPE_END,
} rtDeviceFeatureType_t;

typedef enum tagMemcpyInfo {
    MEMCPY_INFO_SUPPORT_ZEROCOPY = 0,
    MEMCPY_INFO_RSV
} rtMemcpyInfo_t;

typedef enum tagMemoryInfo {
    MEMORY_INFO_TS_LIMITED = 0,
    MEMORY_INFO_RSV
} rtMemoryInfo_t;

typedef enum tagRtDeviceModuleType {
    RT_MODULE_TYPE_SYSTEM = 0,  /**< system info*/
    RT_MODULE_TYPE_AICPU,       /** < aicpu info*/
    RT_MODULE_TYPE_CCPU,        /**< ccpu_info*/
    RT_MODULE_TYPE_DCPU,        /**< dcpu info*/
    RT_MODULE_TYPE_AICORE,      /**< AI CORE info*/
    RT_MODULE_TYPE_TSCPU,       /**< tscpu info*/
    RT_MODULE_TYPE_PCIE,        /**< PCIE info*/
    RT_MODULE_TYPE_VECTOR_CORE, /**< VECTOR CORE info*/
} rtDeviceModuleType_t;

// used for rtGetDevMsg callback function
typedef void (*rtGetMsgCallback)(const char_t *msg, uint32_t len);

typedef enum tagGetDevMsgType {
    RT_GET_DEV_ERROR_MSG = 0,
    RT_GET_DEV_RUNNING_STREAM_SNAPSHOT_MSG,
    RT_GET_DEV_MSG_RESERVE
} rtGetDevMsgType_t;

/**
 * @ingroup dvrt_dev
 * @brief get total device number.
 * @param [in|out] cnt the device number
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDeviceCount(int32_t *cnt);
/**
 * @ingroup dvrt_dev
 * @brief get device ids
 * @param [in|out] get details of device ids
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_DRV_ERR for error
 */
RTS_API rtError_t rtGetDeviceIDs(uint32_t *devices, uint32_t len);

/**
 * @ingroup dvrt_dev
 * @brief get device infomation.
 * @param [in] device   the device id
 * @param [in] moduleType   module type
               typedef enum {
                    MODULE_TYPE_SYSTEM = 0,   system info
                    MODULE_TYPE_AICPU,        aicpu info
                    MODULE_TYPE_CCPU,         ccpu_info
                    MODULE_TYPE_DCPU,         dcpu info
                    MODULE_TYPE_AICORE,       AI CORE info
                    MODULE_TYPE_TSCPU,        tscpu info
                    MODULE_TYPE_PCIE,         PCIE info
               } DEV_MODULE_TYPE;
 * @param [in] infoType   info type
               typedef enum {
                    INFO_TYPE_ENV = 0,
                    INFO_TYPE_VERSION,
                    INFO_TYPE_MASTERID,
                    INFO_TYPE_CORE_NUM,
                    INFO_TYPE_OS_SCHED,
                    INFO_TYPE_IN_USED,
                    INFO_TYPE_ERROR_MAP,
                    INFO_TYPE_OCCUPY,
                    INFO_TYPE_ID,
                    INFO_TYPE_IP,
                    INFO_TYPE_ENDIAN,
               } DEV_INFO_TYPE;
 * @param [out] val   the device info
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_DRV_ERR for error
 */
RTS_API rtError_t rtGetDeviceInfo(uint32_t deviceId, int32_t moduleType, int32_t infoType, int64_t *val);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] devId   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetDevice(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] devId   the device id
 * @param [int] deviceMode   the device mode
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetDeviceV2(int32_t devId, rtDeviceMode deviceMode);

/**
 * @ingroup dvrt_dev
 * @brief get deviceMode
 * @param [out] deviceMode   the device mode
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDeviceMode(rtDeviceMode *deviceMode);

/**
 * @ingroup dvrt_dev
 * @brief set target die for current thread
 * @param [int] die   the die id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetDie(int32_t die);

/**
 * @ingroup dvrt_dev
 * @brief get target die of current thread
 * @param [in|out] die   the die id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDie(int32_t *die);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] devId   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetDeviceEx(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief get Index by phyId.
 * @param [in] phyId   the physical device id
 * @param [out] devIndex   the logic device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDeviceIndexByPhyId(uint32_t phyId, uint32_t *devIndex);

/**
 * @ingroup dvrt_dev
 * @brief get phyId by Index.
 * @param [in] devIndex   the logic device id
 * @param [out] phyId   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDevicePhyIdByIndex(uint32_t devIndex, uint32_t *phyId);

/**
 * @ingroup dvrt_dev
 * @brief enable direction:devIdDes---->phyIdSrc.
 * @param [in] devIdDes   the logical device id
 * @param [in] phyIdSrc   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtEnableP2P(uint32_t devIdDes, uint32_t phyIdSrc, uint32_t flag);

/**
 * @ingroup dvrt_dev
 * @brief disable direction:devIdDes---->phyIdSrc.
 * @param [in] devIdDes   the logical device id
 * @param [in] phyIdSrc   the physical device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDisableP2P(uint32_t devIdDes, uint32_t phyIdSrc);

/**
 * @ingroup dvrt_dev
 * @brief get cability of P2P omemry copy betwen device and peeredevic.
 * @param [in] devId   the logical device id
 * @param [in] peerDevice   the physical device id
 * @param [outv] *canAccessPeer   1:enable 0:disable
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceCanAccessPeer(int32_t *canAccessPeer, uint32_t devId, uint32_t peerDevice);

/**
 * @ingroup dvrt_dev
 * @brief get status
 * @param [in] devIdDes   the logical device id
 * @param [in] phyIdSrc   the physical device id
 * @param [in|out] status   status value
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetP2PStatus(uint32_t devIdDes, uint32_t phyIdSrc, uint32_t *status);

/**
 * @ingroup dvrt_dev
 * @brief get value of current thread
 * @param [in|out] pid   value of pid
 * @return RT_ERROR_NONE for ok
 */
RTS_API rtError_t rtDeviceGetBareTgid(uint32_t *pid);

/**
 * @ingroup dvrt_dev
 * @brief get target device of current thread
 * @param [in|out] devId   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDevice(int32_t *devId);

/**
 * @ingroup dvrt_dev
 * @brief reset all opened device
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceReset(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief reset opened device
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceResetEx(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief get total device infomation.
 * @param [in] devId   the device id
 * @param [in] type     limit type RT_LIMIT_TYPE_LOW_POWER_TIMEOUT=0
 * @param [in] val    limit value
 * @param [out] info   the device info
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceSetLimit(int32_t devId, rtLimitType_t type, uint32_t val);

/**
 * @ingroup dvrt_dev
 * @brief Wait for compute device to finish
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceSynchronize(void);

/**
 * @ingroup dvrt_dev
 * @brief get priority range of current device
 * @param [in|out] leastPriority   least priority
 * @param [in|out] greatestPriority   greatest priority
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceGetStreamPriorityRange(int32_t *leastPriority, int32_t *greatestPriority);

/**
 * @ingroup dvrt_dev
 * @brief Setting Scheduling Type of Graph
 * @param [in] tsId   the ts id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetTSDevice(uint32_t tsId);

/**
 * @ingroup dvrt_dev
 * @brief init aicpu executor
 * @param [out] runtime run mode
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_DRV_ERR for can not get run mode
 */
RTS_API rtError_t rtGetRunMode(rtRunMode *runMode);

/**
 * @ingroup dvrt_dev
 * @brief get aicpu deploy
 * @param [out] aicpu deploy
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_DRV_ERR for can not get aicpu deploy
 */
RTS_API rtError_t rtGetAicpuDeploy(rtAicpuDeployType_t *deployType);

/**
 * @ingroup dvrt_dev
 * @brief set chipType
 * @return RT_ERROR_NONE for ok
 */
RTS_API rtError_t rtSetSocVersion(const char_t *ver);

/**
 * @ingroup dvrt_dev
 * @brief get chipType
 * @return RT_ERROR_NONE for ok
 */
RTS_API rtError_t rtGetSocVersion(char_t *ver, const uint32_t maxLen);

/**
 * @ingroup dvrt_dev
 * @brief get status
 * @param [in] devId   the logical device id
 * @param [in] otherDevId   the other logical device id
 * @param [in] infoType   info type
 * @param [in|out] val   pair info
 * @return RT_ERROR_NONE for ok
 */
RTS_API rtError_t rtGetPairDevicesInfo(uint32_t devId, uint32_t otherDevId, int32_t infoType, int64_t *val);

/**
 * @ingroup dvrt_dev
 * @brief get capability infomation.
 * @param [in] featureType  feature type
               typedef enum tagRtFeatureType {
                    FEATURE_TYPE_MEMCPY = 0,
                    FEATURE_TYPE_RSV,
               } rtFeatureType_t;
 * @param [in] featureInfo  info type
               typedef enum tagMemcpyInfo {
                    MEMCPY_INFO_SUPPORT_ZEROCOPY = 0,
                    MEMCPY_INFO _RSV,
               } rtMemcpyInfo_t;
 * @param [out] val  the capability info RT_CAPABILITY_SUPPORT or RT_CAPABILITY_NOT_SUPPORT
 * @return RT_ERROR_NONE for ok
 */
RTS_API rtError_t rtGetRtCapability(rtFeatureType_t featureType, int32_t featureInfo, int64_t *val);

/**
 * @ingroup dvrt_dev
 * @brief set target device for current thread
 * @param [int] devId   the device id
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtSetDeviceWithoutTsd(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief reset all opened device
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtDeviceResetWithoutTsd(int32_t devId);

/**
 * @ingroup dvrt_dev
 * @brief get device message
 * @param [in] rtGetDevMsgType_t getMsgType:msg type
 * @param [in] GetMsgCallback callback:acl callback function
 * @return RT_ERROR_NONE for ok
 * @return RT_ERROR_INVALID_VALUE for error input
 */
RTS_API rtError_t rtGetDevMsg(rtGetDevMsgType_t getMsgType, rtGetMsgCallback callback);
#if defined(__cplusplus)
}
#endif

#endif  // CCE_RUNTIME_DEVICE_H
