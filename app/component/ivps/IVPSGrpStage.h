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

#include <map>
#include "AXStage.hpp"
// #include "BmpOSD.hpp"
#include "GlobalDef.h"
#include "IOSDHelper.h"
#include "IObserver.h"
// #include "OSDHandlerWrapper.h"
#include "ax_ivps_api.h"

class CIVPSGrpStage;

// AX620E TODO: 16->10?
#define MAX_IVPS_GROUP_NUM (10)
#define IVPS_MAX_CHANNEL_PER_GROUP (5)
#define IVPS_IN_FIFO_DEPTH (2)
#define IVPS_OUT_FIFO_DEPTH (2)

typedef AX_S32 AX_IVPS_FILTER;
typedef struct _IVPS_GROUP_CFG_T {
    IVPS_GRP nGrp;
    AX_U8 nSnsSrc;
    AX_U8 nGrpChnNum;
    AX_IVPS_ENGINE_E eGrpEngineType0;                               /* Engine of group filter 0 */
    AX_IVPS_ENGINE_E eGrpEngineType1;                               /* Engine of group filter 1 */
    AX_U8 nGrpLinkFlag;                                             /* Link flag for previous module to ivps group */
    AX_IVPS_ENGINE_E arrChnEngineType0[IVPS_MAX_CHANNEL_PER_GROUP]; /* Engines of channel filter 0 */
    AX_IVPS_ENGINE_E arrChnEngineType1[IVPS_MAX_CHANNEL_PER_GROUP]; /* Engines of channel filter 1 */
    AX_F32 arrGrpFramerate[2];                                      /* Framerate contrl on group filter 0 */
    AX_F32 arrChnFramerate[IVPS_MAX_CHANNEL_PER_GROUP][2];          /* Framerate contrl on channel filter 0 */
    AX_S16 arrGrpResolution[2];                                     /* Resize on group filter 0 */
    AX_S16 arrChnResolution[IVPS_MAX_CHANNEL_PER_GROUP][2];         /* Resize on channel filter 0 */
    AX_U8 arrChnLinkFlag[IVPS_MAX_CHANNEL_PER_GROUP]; /* Link flag for ivps out channels to next module. NOTICE: Duplicated with value in
                                                         ppl.json now */
    AX_U8 arrGrpFBC[2];                               /* 2: [mode, level] */
    AX_U8 arrChnFBC[IVPS_MAX_CHANNEL_PER_GROUP][2];   /* 2: [mode, level] */
    AX_BOOL arrGrpInplace[2];                         /* 2: [filter0 inplace flag, filter1 inplace flag] */
    AX_BOOL arrChnInplace[IVPS_MAX_CHANNEL_PER_GROUP][2]; /* 2: [filter0 inplace flag, filter1 inplace flag] */
    AX_U8 arrChnSCL[IVPS_MAX_CHANNEL_PER_GROUP];          /* scl type for ivps out channels to next module. */
    AX_BOOL bArrChnVoOsdFlag[IVPS_MAX_CHANNEL_PER_GROUP]; /* VoOsd flag for ivps out channels to next module. */
    AX_U8 arrChnOutFifoDepth[IVPS_MAX_CHANNEL_PER_GROUP]; /* output chn fifo depth. */

    AX_U8 nGrpInFifoDepth;

    AX_U8 nMaskEnable{0};
    // LDC
    AX_U8 nLdcEnable;
    AX_BOOL bLdcAspect;
    AX_S16 nLdcXRatio;
    AX_S16 nLdcYRatio;
    AX_S16 nLdcXYRatio;
    AX_S16 nLdcDistortionRatio;

    _IVPS_GROUP_CFG_T() {
        memset(this, 0, sizeof(_IVPS_GROUP_CFG_T));

        eGrpEngineType0 = AX_IVPS_ENGINE_BUTT;
        eGrpEngineType1 = AX_IVPS_ENGINE_BUTT;
        for (AX_U8 i = 0; i < IVPS_MAX_CHANNEL_PER_GROUP; i++) {
            arrChnEngineType0[i] = AX_IVPS_ENGINE_BUTT;
            arrChnEngineType1[i] = AX_IVPS_ENGINE_BUTT;
            arrChnOutFifoDepth[i] = IVPS_OUT_FIFO_DEPTH;
        }
        nGrpInFifoDepth = IVPS_IN_FIFO_DEPTH;
    }
} IVPS_GROUP_CFG_T, *IVPS_GROUP_CFG_PTR;

typedef struct _IVPS_GRP_T {
    AX_U16 nGroup;
    AX_IVPS_GRP_ATTR_T tGroupAttr;
    AX_IVPS_PIPELINE_ATTR_T tPipelineAttr;

    _IVPS_GRP_T() {
        memset(this, 0, sizeof(*this));
    }
} IVPS_GRP_T, *IVPS_GRP_PTR;

typedef struct _IVPS_GET_THREAD_PARAM {
    AX_BOOL nChnEnable;
    AX_U8 nIvpsGrp;
    AX_U8 nIvpsChn;
    CIVPSGrpStage* pReleaseStage;
    AX_BOOL bExit;

    _IVPS_GET_THREAD_PARAM() {
        nChnEnable = AX_TRUE;
        nIvpsGrp = 0;
        nIvpsChn = 0;
        pReleaseStage = nullptr;
        bExit = AX_TRUE;
    }
} IVPS_GET_THREAD_PARAM_T, *IVPS_GET_THREAD_PARAM_PTR;

class CIVPSGrpStage : public CAXStage, public IFrameRelease {
public:
    CIVPSGrpStage(IVPS_GROUP_CFG_T& tGrpConfig);
    virtual ~CIVPSGrpStage(AX_VOID) = default;

    virtual AX_BOOL Init() override;
    virtual AX_BOOL DeInit() override;
    virtual AX_BOOL Start(STAGE_START_PARAM_PTR pStartParams) override;
    virtual AX_BOOL Stop() override;

    AX_BOOL RecvFrame(CAXFrame* pFrame);
    AX_BOOL InitParams();

    AX_VOID RegObserver(AX_S32 nChannel, IObserver* pObserver);
    AX_VOID UnregObserver(AX_S32 nChannel, IObserver* pObserver);

    virtual AX_BOOL ProcessFrame(CAXFrame* pFrame) override;
    virtual AX_VOID VideoFrameRelease(CAXFrame* pFrame) override;

    AX_VOID AttachOsdHelper(IOSDHelper* pOsdHelper) {
        m_pOsdHelper = pOsdHelper;
    }
    AX_VOID DetachOsdHelper() {
        SAFE_DELETE_PTR(m_pOsdHelper);
    }

    /* Dynamic switch interfaces */
    AX_BOOL EnableChannel(AX_U8 nChn, AX_BOOL bEnable = AX_TRUE);

    AX_BOOL UpdateChnResolution(AX_U8 nChn, AX_S32 nWidth, AX_S32 nHeight);
    AX_BOOL UpdateRotation(AX_IVPS_ROTATION_E eRotation);
    AX_BOOL UpdateChnFRC(AX_F32 fSnsCurFrameRate, AX_F32 fSnsFrameRate);
    AX_BOOL UpdateGrpLDC(AX_BOOL bLdcEnable, AX_BOOL bAspect, AX_S16 nXRatio, AX_S16 nYRatio, AX_S16 nXYRatio, AX_S16 nDistorRatio);
    AX_VOID RefreshOSDByResChange();
    AX_BOOL StartOSD();
    AX_BOOL StopOSD();

    IVPS_GROUP_CFG_T* GetGrpCfg() {
        return &m_tIvpsGrpCfg;
    }

    IVPS_GRP_T* GetGrpPPLAttr() {
        return &m_tIvpsGrp;
    }

    IOSDHelper* GetOsdHelper() {
        return m_pOsdHelper;
    }

    AX_U8 GetSensorSrc() {
        return m_tIvpsGrpCfg.nSnsSrc;
    }

    AX_BOOL GetChannelState(AX_U8 nChn) {
        return m_mapChnState[nChn];
    }

    AX_VOID SetChannelState(AX_U8 nChn, AX_BOOL bEnable) {
        m_mapChnState[nChn] = bEnable;
    }

    AX_VOID SetChnInplace(AX_S32 nChannel, AX_BOOL bEnable);
    AX_BOOL IsMaskGroup() {
        return m_tIvpsGrpCfg.nMaskEnable ? AX_TRUE : AX_FALSE;
    }

    AX_BOOL GetCurResolution(AX_U8 nChn, AX_U32& nWidth, AX_U32& nHeight);

    AX_VOID DisableRotation(AX_BOOL bEnable = AX_FALSE) {
        m_bEnableRotation = bEnable;
    }

private:
    AX_VOID NotifyAll(AX_U32 nGrp, AX_U32 nChn, CAXFrame* pFrame);
    AX_VOID StartWorkThread();
    AX_VOID StopWorkThread();
    AX_VOID FrameGetThreadFunc(IVPS_GET_THREAD_PARAM_PTR pThreadParam);

    AX_BOOL GetResolutionByRotate(AX_U8 nChn, AX_IVPS_ROTATION_E eRotation, AX_U32& nWidth, AX_U32& nHeight);
    AX_BOOL UpdateRotationResolution(AX_IVPS_ROTATION_E eRotation, AX_U8 nChn, AX_U32 nWidth, AX_U32 nHeight);

    AX_VOID UpdateCompressInfo(AX_FRAME_COMPRESS_INFO_T& tCompressInfo, AX_U16 nChnWidth);

private:
    AX_BOOL m_bStarted{AX_FALSE};
    IVPS_GROUP_CFG_T m_tIvpsGrpCfg;
    AX_U16 m_nIvpsGrp;
    IVPS_GRP_T m_tIvpsGrp;
    IVPS_GET_THREAD_PARAM_T m_tGetThreadParam[IVPS_MAX_CHANNEL_PER_GROUP];
    std::thread m_hGetThread[IVPS_MAX_CHANNEL_PER_GROUP];
    std::map<AX_U32, AX_BOOL> m_mapChnState;
    std::map<AX_U32, std::vector<IObserver*>> m_mapChnObserver;
    std::mutex m_mtxObserver;
    IOSDHelper* m_pOsdHelper{nullptr};
    AX_IVPS_ROTATION_E m_eRotation{AX_IVPS_ROTATION_0};
    /* Record current resolution*/
    AX_S16 m_arrChnResolution[IVPS_MAX_CHANNEL_PER_GROUP][2];
    /* Switching rotation state pause AX_IVPS_SendFrame
       Switching resolution pause AX_IVPS_GetChnFrame */
    std::mutex m_mutxSend;
    std::mutex m_mutxRecv[IVPS_MAX_CHANNEL_PER_GROUP];
    AX_BOOL m_bEnableRotation{AX_TRUE};
};
