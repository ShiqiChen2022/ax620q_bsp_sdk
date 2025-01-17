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

#include <memory>
#include <vector>
#include <mutex>

#include "AXAlgo.hpp"
#include "AXFrame.hpp"
#include "AXLockQ.hpp"
#include "AXResource.hpp"
#include "AXStage.hpp"
#include "AXThread.hpp"
#include "FramerateCtrlHelper.h"
#include "IObserver.h"
#include "Md.h"
#include "Od.h"
#include "OptionHelper.h"
#include "Scd.h"

#define MAX_IVES_GROUP_NUM 2

typedef struct _IVES_ATTR_T {
    AX_U32 nGrpCount{0};
    AX_U32 nWidth{0};
    AX_U32 nHeight{0};
    AX_F32 fSrcFramerate{0};
    AX_S8 nSnsSrc{0};
} IVES_ATTR_T;

class CIVESStage final : public CAXStage {
public:
    CIVESStage(AX_VOID);
    ~CIVESStage(AX_VOID) = default;

    AX_BOOL Init(AX_U32 nGrp);
    AX_BOOL DeInit(AX_VOID) override;
    AX_BOOL Start(AX_VOID) override;
    AX_BOOL Stop(AX_VOID) override;
    AX_BOOL ProcessFrame(CAXFrame* pFrame) override;

    AX_BOOL SetMdConfig(const AX_APP_ALGO_MOTION_PARAM_T& stConfig);
    AX_BOOL SetOdConfig(const AX_APP_ALGO_OCCLUSION_PARAM_T& stConfig);
    AX_BOOL SetScdConfig(const AX_APP_ALGO_SCENE_CHANGE_PARAM_T& stConfig);

    AX_BOOL UpdateRotation(AX_U8 nRotation);
    AX_BOOL SendFrame(AX_U32 nSnsID, CAXFrame* pAxFrame);
    IVES_ATTR_T* GetIVESCfg() {
        return &m_stAttr;
    };

    AX_U32 GetGrp() {
        return m_nGroup;
    };

    CMD* GetMDInstance(AX_VOID) const {
        return m_spMDInstance.get();
    }
    COD* GetODInstance(AX_VOID) const {
        return m_spODInstance.get();
    }
    CSCD* GetSCDInstance(AX_VOID) const {
        return m_spSCDInstance.get();
    }
    AX_BOOL GetMDCapacity(AX_VOID) const {
        return m_bMDEnable;
    }
    AX_VOID SetMDCapacity(AX_BOOL enable) {
        m_bMDEnable = enable;
    }
    AX_BOOL GetODCapacity(AX_VOID) const {
        return m_bODEnable;
    }
    AX_VOID SetODCapacity(AX_BOOL enable) {
        m_bODEnable = enable;
    }
    AX_BOOL GetSCDCapacity(AX_VOID) const {
        return m_bSCDEnable;
    }
    AX_VOID SetSCDCapacity(AX_BOOL enable) {
        m_bSCDEnable = enable;
    }

    AX_VOID RegObserver(IObserver* pObserver);
    AX_VOID UnregObserver(IObserver* pObserver);

    static AX_BOOL InitModule();
    static AX_BOOL DeinitModule();

private:
    AX_VOID NotifyAll(AX_U32 nGrp, AX_U32 nChn, AX_VOID* pStream);
    AX_BOOL GetResolutionByRotate(AX_U8 nRotation, AX_U32& nWidth, AX_U32& nHeight);

private:
    std::mutex m_mutx;

    AX_BOOL m_bMDEnable{AX_FALSE};
    AX_BOOL m_bODEnable{AX_FALSE};
    AX_BOOL m_bSCDEnable{AX_FALSE};

    IVES_ATTR_T m_stAttr;

    std::unique_ptr<CMD> m_spMDInstance;
    std::unique_ptr<COD> m_spODInstance;
    std::unique_ptr<CSCD> m_spSCDInstance;
    AX_U32 m_nGroup{0};
    std::vector<IObserver*> m_vecObserver;

    std::unique_ptr<CFramerateCtrlHelper> m_spFramectrl;
    AX_BOOL m_bReseting{AX_FALSE};
};
