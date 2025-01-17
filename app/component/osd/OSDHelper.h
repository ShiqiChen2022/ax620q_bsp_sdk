/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Ningbo) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Ningbo) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Ningbo) Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include <atomic>
#include <condition_variable.hpp>
#include <map>
#include <vector>
#include "AXThread.hpp"
#include "IOSDHelper.h"
#include "OSDHandlerWrapper.h"
#include "OsdConfig.h"
#include "ax_base_type.h"
#include "ax_ivps_api.h"

#include "WebOptionHelper.h"

#define MAX_REGION_COUNT (32)
#define APP_OSD_CHANNEL_FILTER_0 (0)
#define APP_OSD_CHANNEL_FILTER_1 (1)
#define APP_OSD_GROUP_FILTER_0 (0)
#define APP_OSD_GROUP_FILTER_1 (1)

class CIVPSGrpStage;

typedef struct _OSD_REGION_PARAM {
    IVPS_GRP nGroup{-1};
    AX_S32 nChn{-1};
    IVPS_RGN_HANDLE hRgn{AX_IVPS_INVALID_REGION_HANDLE};
    AX_IVPS_FILTER nFilter{0};
    OSD_CFG_T tOsdCfg;
    AX_BOOL bExit{AX_FALSE};
    CAXThread m_EventThread;
} OSD_REGION_PARAM_T;

class COSDHelper : public IOSDHelper {
public:
    COSDHelper();
    virtual ~COSDHelper() = default;

public:
    AX_BOOL StartOSD(CIVPSGrpStage* pIvpsInstance) override;
    AX_BOOL StopOSD() override;
    AX_BOOL EnableAiRegion(AX_BOOL bEnable = AX_TRUE) override;
    AX_U8 GetAttachedFilter() override;
    AX_BOOL Refresh() override;
    AX_BOOL UpdateOSDRect(const std::vector<AX_IVPS_RGN_POLYGON_T>& vecRgn) override;
    AX_BOOL UpdateOSDRect(const std::vector<AX_APP_OSD_BOX_T>& vecBox) override;

private:
    AX_BOOL UpdateOSD(OSD_REGION_PARAM_T* pOsd);
    AX_VOID TimeThreadFunc(OSD_REGION_PARAM_T* pThreadParam);
    AX_VOID RectThreadFunc(OSD_REGION_PARAM_T* pThreadParam);
    AX_VOID UpdateOSDPic(OSD_REGION_PARAM_T* pThreadParam);
    AX_VOID UpdateOSDStr(OSD_REGION_PARAM_T* pThreadParam);
    AX_VOID UpdateOSDPri(OSD_REGION_PARAM_T* pThreadParam);
    AX_VOID PerformUpdateOSD(AX_VOID);
    std::string GetResPath();

private:
    CIVPSGrpStage* m_pIvpsGrpInstance{nullptr};
    OSD_REGION_PARAM_T m_arrRgnThreadParam[MAX_REGION_COUNT];
    COSDHandlerWrapper m_osdWrapper;
    AX_U32 m_nRgnCount{0};

    AX_BOOL m_bExited{AX_FALSE};
    AX_S32 m_nUpdateOSDTasks{0};
    CAXThread m_updateThread;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};