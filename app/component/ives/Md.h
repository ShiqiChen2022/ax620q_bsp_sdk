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
#include <mutex>
#include <vector>
#include "AXAlgo.hpp"
#include "AXFrame.hpp"
#include "AXSingleton.h"
#include "WebServer.h"
#include "ax_ives_api.h"

typedef struct {
    AX_MD_CHN_ATTR_T stAttr;
    AX_U32 nConfidence;
} MD_AREA_ATTR_T;

class CMD {
public:
    CMD(AX_VOID) {
        InitOnce();
    }
    ~CMD(AX_VOID) = default;

    AX_BOOL Startup(AX_S32 nSnsID, AX_U32 nWidth, AX_U32 nHeight);
    AX_VOID Cleanup(AX_VOID);

    AX_VOID GetDefaultThresholdY(AX_S32 nSnsID, AX_U8 &nThrd, AX_U8 &nConfidence);

    AX_VOID AddArea(const AX_APP_ALGO_MOTION_PARAM_T &stConfig);
    /* add an area and return area id (equal to MD chn id), if returns -1 means fail */
    AX_S32 AddArea(AX_U32 x, AX_U32 y, AX_U32 w, AX_U32 h, AX_U32 mbW, AX_U32 mbH, AX_S32 nThrdY, AX_S32 nConfidenceY, AX_U32 nWidth,
                   AX_U32 nHeight, AX_S32 nSnsID = -1);
    AX_BOOL RemoveArea(AX_S32 nAreaId);

    AX_BOOL SetThresholdY(AX_S32 nSnsId, AX_S32 nAreaId, AX_U8 nThrd, AX_U8 nConfidence);

    /* process and return md results */
    const std::vector<AX_U8> &ProcessFrame(const AX_U32 nSnsID, const CAXFrame *pFrame);

    AX_S32 GetAreaCount(AX_VOID) const {
        return m_vecAreas.size();
    }

    AX_VOID SetConfig(const AX_APP_ALGO_MOTION_PARAM_T &stConfig, AX_BOOL bUpdateParam = AX_FALSE);
    AX_VOID UpdateResolution(AX_U8 nRotation, AX_U32 nFrmW, AX_U32 nFrmH);

private:
    AX_BOOL IsEqualArea(const AX_IVES_RECT_T &a, const AX_IVES_RECT_T &b) const {
        return ((a.u32X == b.u32X) && (a.u32Y == b.u32Y) && (a.u32W == b.u32W) && (a.u32H == b.u32H)) ? AX_TRUE : AX_FALSE;
    }

    AX_BOOL LoadConfig(AX_S32 nSnsID);
    AX_VOID DestoryAreas(AX_VOID);

    AX_BOOL InitOnce(AX_VOID) {
        memset(&m_stConfig, 0x00, sizeof(m_stConfig));
        return AX_TRUE;
    };

private:
    std::mutex m_mutx;

    std::vector<MD_AREA_ATTR_T *> m_vecAreas;
    std::vector<AX_U8> m_vecRslts;
    AX_APP_ALGO_MOTION_PARAM_T m_stConfig;

    AX_BOOL m_bInited{AX_FALSE};

    AX_U32 m_mdImgW{0};
    AX_U32 m_mdImgH{0};
    AX_S32 m_nSnsID{0};
    AX_U8 m_nRotation{0};
};
