/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Ningbo) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Ningbo) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Ningbo) Co., Ltd.
 *
 **************************************************************************************************/

#ifdef IMX678_SUPPORT
#include "IMX678.h"
#include "GlobalDef.h"

#define SENSOR_ATTR_WIDTH (3840)
#define SENSOR_ATTR_HEIGHT (2160)

#define SENSOR_OUT_WIDTH (3840)
#define SENSOR_OUT_HEIGHT (2160)

CIMX678::CIMX678(SENSOR_CONFIG_T tSensorConfig) : CBaseSensor(tSensorConfig) {
    m_tSnsResolution.nSnsAttrWidth = SENSOR_ATTR_WIDTH;
    m_tSnsResolution.nSnsAttrHeight = SENSOR_ATTR_HEIGHT;

    m_tSnsResolution.nSnsOutWidth = SENSOR_OUT_WIDTH;
    m_tSnsResolution.nSnsOutHeight = SENSOR_OUT_HEIGHT;

    m_eImgFormatSDR = AX_FORMAT_BAYER_RAW_12BPP;
    m_eImgFormatHDR = AX_FORMAT_BAYER_RAW_10BPP;
}

CIMX678::~CIMX678(AX_VOID) {
}

AX_SNS_HDR_MODE_E CIMX678::GetMaxHdrMode() {
    if (!GetAbilities().bSupportHDR) {
        return AX_SNS_LINEAR_MODE;
    }

    return AX_SNS_HDR_2X_MODE;
}

AX_VOID CIMX678::InitSnsLibraryInfo(AX_VOID) {
#ifndef USE_STATIC_LIBS
    m_tSnsLibInfo.strLibName = "libsns_imx678.so";
    m_tSnsLibInfo.strObjName = "gSnsimx678Obj";
#else
    extern AX_SENSOR_REGISTER_FUNC_T gSnsimx678Obj;
    m_pSnsObj = &gSnsimx678Obj;
#endif  // USE_STATIC_LIBS
}

AX_VOID CIMX678::InitSnsAttr() {
    /* Referenced by AX_ISP_SetSnsAttr */
    m_tSnsAttr.nWidth = m_tSnsResolution.nSnsAttrWidth;
    m_tSnsAttr.nHeight = m_tSnsResolution.nSnsAttrHeight;
    m_tSnsAttr.fFrameRate = m_tSnsCfg.fFrameRate;

    m_tSnsAttr.eSnsMode = m_tSnsCfg.eSensorMode;
    m_tSnsAttr.eRawType = (m_tSnsCfg.eSensorMode > AX_SNS_LINEAR_MODE) ? AX_RT_RAW10 : AX_RT_RAW12;
    m_tSnsAttr.eBayerPattern = AX_BP_RGGB;
    m_tSnsAttr.bTestPatternEnable = AX_FALSE;
}

AX_VOID CIMX678::InitSnsClkAttr() {
    /* Referenced by AX_ISP_OpenSnsClk */
    m_tSnsClkAttr.nSnsClkIdx = m_tSnsCfg.nSnsID;
    m_tSnsClkAttr.eSnsClkRate = AX_SNS_CLK_24M;
}

AX_VOID CIMX678::InitMipiRxAttr() {
    /* Referenced by AX_MIPI_RX_SetAttr */
    m_tMipiRxDev.eInputMode = AX_INPUT_MODE_MIPI;
    m_tMipiRxDev.tMipiAttr.ePhyMode = AX_MIPI_PHY_TYPE_DPHY;
    m_tMipiRxDev.tMipiAttr.eLaneNum = (AX_MIPI_LANE_NUM_E)m_tSnsCfg.nLaneNum;
    m_tMipiRxDev.tMipiAttr.nDataRate = 1440;
    m_tMipiRxDev.tMipiAttr.nDataLaneMap[0] = 0x00;
    m_tMipiRxDev.tMipiAttr.nDataLaneMap[1] = 0x01;
    m_tMipiRxDev.tMipiAttr.nDataLaneMap[2] = 0x02;
    m_tMipiRxDev.tMipiAttr.nDataLaneMap[3] = 0x03;

    m_tMipiRxDev.tMipiAttr.nClkLane[0] = 0x01;
    m_tMipiRxDev.tMipiAttr.nClkLane[1] = 0x00;
}

AX_VOID CIMX678::InitDevAttr() {
    /* Referenced by AX_VIN_SetDevAttr */
    m_tDevAttr.bImgDataEnable = AX_TRUE;
    m_tDevAttr.bNonImgDataEnable = AX_FALSE;
    m_tDevAttr.eDevMode = m_tSnsCfg.eDevMode;
    m_tDevAttr.eSnsIntfType = AX_SNS_INTF_TYPE_MIPI_RAW;
    for (AX_U8 i = 0; i < AX_HDR_CHN_NUM; i++) {
        m_tDevAttr.tDevImgRgn[i] = {0, 0, m_tSnsResolution.nSnsOutWidth, m_tSnsResolution.nSnsOutHeight};
    }
    m_tDevAttr.ePixelFmt = (m_tSnsAttr.eRawType == AX_RT_RAW12) ? AX_FORMAT_BAYER_RAW_12BPP_PACKED : AX_FORMAT_BAYER_RAW_10BPP_PACKED;
    m_tDevAttr.eBayerPattern = AX_BP_RGGB;
    m_tDevAttr.eSnsMode = m_tSnsCfg.eSensorMode;
    m_tDevAttr.eSnsOutputMode = m_tSnsCfg.eSensorMode > AX_SNS_LINEAR_MODE ? AX_SNS_DOL_HDR : AX_SNS_NORMAL;

    if (AX_SNS_LINEAR_MODE == m_tSnsCfg.eSensorMode) {
        m_tDevAttr.tMipiIntfAttr.szImgVc[0] = 0;
        m_tDevAttr.tMipiIntfAttr.szImgVc[1] = 1;
        m_tDevAttr.tMipiIntfAttr.szImgDt[0] = 44;
        m_tDevAttr.tMipiIntfAttr.szImgDt[1] = 44;
        m_tDevAttr.tMipiIntfAttr.szInfoVc[0] = 31;
        m_tDevAttr.tMipiIntfAttr.szInfoVc[1] = 31;
        m_tDevAttr.tMipiIntfAttr.szInfoDt[0] = 63;
        m_tDevAttr.tMipiIntfAttr.szInfoDt[1] = 63;
    } else {
        m_tDevAttr.tMipiIntfAttr.szImgVc[0] = 0;
        m_tDevAttr.tMipiIntfAttr.szImgVc[1] = 1;
        m_tDevAttr.tMipiIntfAttr.szImgDt[0] = 43;
        m_tDevAttr.tMipiIntfAttr.szImgDt[1] = 43;
        m_tDevAttr.tMipiIntfAttr.szInfoVc[0] = 31;
        m_tDevAttr.tMipiIntfAttr.szInfoVc[1] = 31;
        m_tDevAttr.tMipiIntfAttr.szInfoDt[0] = 63;
        m_tDevAttr.tMipiIntfAttr.szInfoDt[1] = 63;
    }
}

AX_VOID CIMX678::InitPipeAttr() {
    /* Referenced by AX_VIN_SetPipeAttr */
    for (AX_U8 i = 0; i < m_tSnsCfg.nPipeCount; i++) {
        AX_U8 nPipe = m_tSnsCfg.arrPipeAttr[i].nPipeID;
        AX_VIN_PIPE_ATTR_T tPipeAttr;
        memset(&tPipeAttr, 0, sizeof(AX_VIN_PIPE_ATTR_T));
        tPipeAttr.ePipeWorkMode = AX_VIN_PIPE_NORMAL_MODE1;
        tPipeAttr.tPipeImgRgn = {0, 0, m_tSnsResolution.nSnsOutWidth, m_tSnsResolution.nSnsOutHeight};
        tPipeAttr.eBayerPattern = AX_BP_RGGB;
        tPipeAttr.nWidthStride = ALIGN_UP(m_tSnsResolution.nSnsOutWidth, 2);
        tPipeAttr.ePixelFmt = m_tDevAttr.ePixelFmt;
        tPipeAttr.eSnsMode = m_tSnsCfg.eSensorMode;
        if (E_SNS_TISP_MODE_E == m_tSnsCfg.arrPipeAttr[i].eAiIspMode) {
            tPipeAttr.bAiIspEnable = AX_FALSE;
        } else {
            tPipeAttr.bAiIspEnable = AX_TRUE;
        }
        if (tPipeAttr.eSnsMode < AX_SNS_HDR_MODE_MAX) {
            tPipeAttr.tCompressInfo = m_tSnsCfg.arrPipeAttr[i].tIfeCompress[tPipeAttr.eSnsMode];
            tPipeAttr.tNrAttr.t3DnrAttr.tCompressInfo = m_tSnsCfg.arrPipeAttr[i].t3DNrCompress[tPipeAttr.eSnsMode];
            tPipeAttr.tNrAttr.tAinrAttr.tCompressInfo = m_tSnsCfg.arrPipeAttr[i].tAiNrCompress[tPipeAttr.eSnsMode];
        }
        tPipeAttr.tFrameRateCtrl.fSrcFrameRate = m_tSnsCfg.fFrameRate;
        tPipeAttr.tFrameRateCtrl.fDstFrameRate = m_tSnsCfg.arrPipeAttr[i].fPipeFramerate;
        tPipeAttr.tMotionAttr.bMotionEst = m_tSnsCfg.arrPipeAttr[i].tDisAttr.bMotionEst;
        tPipeAttr.tMotionAttr.bMotionShare = m_tSnsCfg.arrPipeAttr[i].tDisAttr.bMotionShare;
        m_mapPipe2Attr[nPipe] = tPipeAttr;
    }
}

AX_VOID CIMX678::InitChnAttr() {
    /* Referenced by AX_VIN_SetChnAttr */
    for (AX_U8 i = 0; i < m_tSnsCfg.nPipeCount; i++) {
        PIPE_CONFIG_T& tPipeAttr = m_tSnsCfg.arrPipeAttr[i];

        AX_VIN_CHN_ATTR_T arrChnAttr[AX_VIN_CHN_ID_MAX];
        memset(&arrChnAttr[0], 0, sizeof(AX_VIN_CHN_ATTR_T) * AX_VIN_CHN_ID_MAX);

        for (AX_U8 nChn = 0; nChn < AX_VIN_CHN_ID_MAX; nChn++) {
            AX_F32 fChnDstFrameRate =
                tPipeAttr.arrChannelAttr[nChn].fFrameRate == 0 ? tPipeAttr.fPipeFramerate : tPipeAttr.arrChannelAttr[nChn].fFrameRate;
            tPipeAttr.arrChannelAttr[nChn].fFrameRate = fChnDstFrameRate;
            arrChnAttr[nChn].nWidth = (tPipeAttr.arrChannelAttr[nChn].nWidth < 0) ? m_tSnsResolution.nSnsOutWidth : tPipeAttr.arrChannelAttr[nChn].nWidth;
            arrChnAttr[nChn].nHeight = (tPipeAttr.arrChannelAttr[nChn].nHeight < 0) ? m_tSnsResolution.nSnsOutHeight : tPipeAttr.arrChannelAttr[nChn].nHeight;
            arrChnAttr[nChn].eImgFormat = AX_FORMAT_YUV420_SEMIPLANAR;
            arrChnAttr[nChn].nWidthStride = ALIGN_UP(
                arrChnAttr[nChn].nWidth,
                tPipeAttr.arrChannelAttr[nChn].tChnCompressInfo.enCompressMode == AX_COMPRESS_MODE_NONE ? AX_VIN_NONE_FBC_STRIDE_ALIGN_VAL : AX_VIN_FBC_STRIDE_ALIGN_VAL);
            arrChnAttr[nChn].nDepth = tPipeAttr.arrChannelAttr[nChn].nYuvDepth;
            arrChnAttr[nChn].tCompressInfo = tPipeAttr.arrChannelAttr[nChn].tChnCompressInfo;
            arrChnAttr[nChn].tFrameRateCtrl.fSrcFrameRate = tPipeAttr.fPipeFramerate;
            arrChnAttr[nChn].tFrameRateCtrl.fDstFrameRate = fChnDstFrameRate;

            m_mapPipe2ChnAttr[tPipeAttr.nPipeID][nChn] = arrChnAttr[nChn];
        }
    }
}

AX_VOID CIMX678::InitAbilities() {
    m_tAbilities.bSupportHDR = AX_TRUE;
    m_tAbilities.bSupportHDRSplit = AX_FALSE;
    m_tAbilities.fShutterSlowFpsThr = 20;
}
#endif
