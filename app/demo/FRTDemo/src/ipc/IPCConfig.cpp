#include "IPCConfig.h"
#include "CommonUtils.hpp"
#include "OptionHelper.h"
#include "WebOptionHelper.h"
AX_BOOL CIPCConfig::Init(AX_VOID) {
    if (!InitOsdConfig()) {
        return AX_FALSE;
    }

    m_tPPLConfig.strAppName = "FRTDemo";
    m_tPPLConfig.strPPLName = CCommonUtils::GetPPLSpecification();

    return AX_TRUE;
}

AX_BOOL CIPCConfig::InitPost() {
    WEB_CAMERA_ATTR_T tAttr = CWebOptionHelper::GetInstance()->GetCamera(0);
    tAttr.bCaptureEnable = AX_FALSE;
    CWebOptionHelper::GetInstance()->SetCamera(0, tAttr);
    AX_S32 nScenario = 0;
    CCmdLineParser::GetInstance()->GetScenario(nScenario);
    if (E_APP_SCENARIO_DEFAULT == nScenario) {
        WEB_CAMERA_ATTR_T tAttr = CWebOptionHelper::GetInstance()->GetCamera(1);
        tAttr.bCaptureEnable = AX_FALSE;
        CWebOptionHelper::GetInstance()->SetCamera(1, tAttr);
    }

    return AX_TRUE;
}

AX_BOOL CIPCConfig::InitOsdConfig(AX_VOID) {
    std::vector<OSD_CFG_T> vecOsdCfg;
    int nCharLen = 0;
    wchar_t wszOsdStr[MAX_OSD_WSTR_CHAR_LEN] = {0};
    AX_S32 nScenario = 0;
    CCmdLineParser::GetInstance()->GetScenario(nScenario);
    OSD_CFG_T tOsdConfig;

    std::string resDir = GetResPath();
    do {
        /* sensor0 primary*/
        vecOsdCfg.clear();
        // TIME
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_TIME;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nZIndex = 0;
        tOsdConfig.tTimeAttr.eFormat = OSD_DATE_FORMAT_YYMMDDHHmmSS;
        tOsdConfig.tTimeAttr.nColor = 0xFFFFFF;
        tOsdConfig.tTimeAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tTimeAttr.nColorInv = 0x0;
        tOsdConfig.tTimeAttr.nFontSize = 128;
        tOsdConfig.nBoundaryX = 48;
        tOsdConfig.nBoundaryY = 20;
        nCharLen = 19;
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // LOGO
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_PICTURE;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_BOTTOM;
        tOsdConfig.nZIndex = 1;
        std::string strPicPath = resDir + "axera_logo_256x64.argb1555";
        strcpy(tOsdConfig.tPicAttr.szFileName, strPicPath.c_str());
        tOsdConfig.tPicAttr.nWidth = 256;
        tOsdConfig.tPicAttr.nHeight = 64;
        tOsdConfig.nBoundaryX = 32;
        tOsdConfig.nBoundaryY = 48;
        tOsdConfig.nBoundaryW = 256;
        tOsdConfig.nBoundaryH = 64;

        vecOsdCfg.push_back(tOsdConfig);

        // Channel
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_CHANNEL;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_TOP;
        tOsdConfig.nZIndex = 2;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "CHANNEL-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 48;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Location
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_LOCATION;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_BOTTOM;
        tOsdConfig.nZIndex = 3;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "LOCATION-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 48;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Privacy
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_PRIVACY;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nBoundaryX = 0;
        tOsdConfig.nBoundaryY = 0;
        tOsdConfig.nBoundaryW = -1;
        tOsdConfig.nBoundaryH = -1;
        tOsdConfig.nZIndex = 4;
        tOsdConfig.tPrivacyAttr.eType = OSD_PRIVACY_TYPE_RECT;
        tOsdConfig.tPrivacyAttr.nLineWidth = 2;
        tOsdConfig.tPrivacyAttr.nColor = 0xFFFFFF;
        tOsdConfig.tPrivacyAttr.bSolid = AX_FALSE;
        int nSlide = 200;
        tOsdConfig.tPrivacyAttr.tPt[0].x = 20;
        tOsdConfig.tPrivacyAttr.tPt[0].y = 200;  // nHeight / 2 - nSlide;
        tOsdConfig.tPrivacyAttr.tPt[1].x = 20 + nSlide * 2;
        tOsdConfig.tPrivacyAttr.tPt[1].y = 200;  // nHeight / 2 - nSlide;
        tOsdConfig.tPrivacyAttr.tPt[2].x = 20 + nSlide * 2;
        tOsdConfig.tPrivacyAttr.tPt[2].y = 400;  // nHeight / 2 + nSlide;
        tOsdConfig.tPrivacyAttr.tPt[3].x = 20;
        tOsdConfig.tPrivacyAttr.tPt[3].y = 400;  // nHeight / 2 + nSlide;

        vecOsdCfg.push_back(tOsdConfig);

        // Rect
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.nZIndex = 5;
        tOsdConfig.eType = OSD_TYPE_RECT;
        vecOsdCfg.push_back(tOsdConfig);
        CWebOptionHelper::GetInstance()->SetOsdConfig(0, 0, 0, vecOsdCfg);

        /* sensor0 Secondary*/
        vecOsdCfg.clear();
        // TIME
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_TIME;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nZIndex = 0;
        tOsdConfig.tTimeAttr.eFormat = OSD_DATE_FORMAT_YYMMDDHHmmSS;
        tOsdConfig.tTimeAttr.nColor = 0xFFFFFF;
        tOsdConfig.tTimeAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tTimeAttr.nColorInv = 0x0;
        tOsdConfig.tTimeAttr.nFontSize = 16;
        tOsdConfig.nBoundaryX = 14;
        tOsdConfig.nBoundaryY = 8;
        nCharLen = 19;
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // LOGO
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_PICTURE;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_BOTTOM;
        tOsdConfig.nZIndex = 1;
        strPicPath = resDir + "axera_logo_96x28.argb1555";
        strcpy(tOsdConfig.tPicAttr.szFileName, strPicPath.c_str());
        tOsdConfig.tPicAttr.nWidth = 96;
        tOsdConfig.tPicAttr.nHeight = 28;
        tOsdConfig.nBoundaryX = 8;
        tOsdConfig.nBoundaryY = 8;
        tOsdConfig.nBoundaryW = 96;
        tOsdConfig.nBoundaryH = 28;

        vecOsdCfg.push_back(tOsdConfig);

        // Channel
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_CHANNEL;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_TOP;
        tOsdConfig.nZIndex = 2;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "CHANNEL-*");
        tOsdConfig.nBoundaryX = 8;
        tOsdConfig.nBoundaryY = 8;
        tOsdConfig.tStrAttr.nFontSize = 16;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Location
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_LOCATION;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_BOTTOM;
        tOsdConfig.nZIndex = 3;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "LOCATION-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 16;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Rect
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.nZIndex = 5;
        tOsdConfig.eType = OSD_TYPE_RECT;
        vecOsdCfg.push_back(tOsdConfig);
        CWebOptionHelper::GetInstance()->SetOsdConfig(0, 0, 2, vecOsdCfg);
    } while (0);

    if (E_APP_SCENARIO_DEFAULT == nScenario) {
        /* sensor1 primary*/
        vecOsdCfg.clear();
        // TIME
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_TIME;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nZIndex = 0;
        tOsdConfig.tTimeAttr.eFormat = OSD_DATE_FORMAT_YYMMDDHHmmSS;
        tOsdConfig.tTimeAttr.nColor = 0xFFFFFF;
        tOsdConfig.tTimeAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tTimeAttr.nColorInv = 0x0;
        tOsdConfig.tTimeAttr.nFontSize = 128;
        tOsdConfig.nBoundaryX = 48;
        tOsdConfig.nBoundaryY = 20;
        nCharLen = 19;
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // LOGO
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_PICTURE;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_BOTTOM;
        tOsdConfig.nZIndex = 1;
        std::string strPicPath = resDir + "axera_logo_256x64.argb1555";
        strcpy(tOsdConfig.tPicAttr.szFileName, strPicPath.c_str());
        tOsdConfig.tPicAttr.nWidth = 256;
        tOsdConfig.tPicAttr.nHeight = 64;
        tOsdConfig.nBoundaryX = 32;
        tOsdConfig.nBoundaryY = 48;
        tOsdConfig.nBoundaryW = 256;
        tOsdConfig.nBoundaryH = 64;

        vecOsdCfg.push_back(tOsdConfig);

        // Channel
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_CHANNEL;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_TOP;
        tOsdConfig.nZIndex = 2;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "CHANNEL-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 48;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Location
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_LOCATION;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_BOTTOM;
        tOsdConfig.nZIndex = 3;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "LOCATION-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 48;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Privacy
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_PRIVACY;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nBoundaryX = 0;
        tOsdConfig.nBoundaryY = 0;
        tOsdConfig.nBoundaryW = -1;
        tOsdConfig.nBoundaryH = -1;
        tOsdConfig.nZIndex = 4;
        tOsdConfig.tPrivacyAttr.eType = OSD_PRIVACY_TYPE_RECT;
        tOsdConfig.tPrivacyAttr.nLineWidth = 2;
        tOsdConfig.tPrivacyAttr.nColor = 0xFFFFFF;
        tOsdConfig.tPrivacyAttr.bSolid = AX_FALSE;
        int nSlide = 200;
        tOsdConfig.tPrivacyAttr.tPt[0].x = 20;
        tOsdConfig.tPrivacyAttr.tPt[0].y = 200;  // nHeight / 2 - nSlide;
        tOsdConfig.tPrivacyAttr.tPt[1].x = 20 + nSlide * 2;
        tOsdConfig.tPrivacyAttr.tPt[1].y = 200;  // nHeight / 2 - nSlide;
        tOsdConfig.tPrivacyAttr.tPt[2].x = 20 + nSlide * 2;
        tOsdConfig.tPrivacyAttr.tPt[2].y = 400;  // nHeight / 2 + nSlide;
        tOsdConfig.tPrivacyAttr.tPt[3].x = 20;
        tOsdConfig.tPrivacyAttr.tPt[3].y = 400;  // nHeight / 2 + nSlide;

        vecOsdCfg.push_back(tOsdConfig);

        // Rect
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.nZIndex = 5;
        tOsdConfig.eType = OSD_TYPE_RECT;
        vecOsdCfg.push_back(tOsdConfig);
        CWebOptionHelper::GetInstance()->SetOsdConfig(1, 1, 0, vecOsdCfg);

        /*sensor1 Secondary*/
        vecOsdCfg.clear();
        // TIME
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_TIME;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_TOP;
        tOsdConfig.nZIndex = 0;
        tOsdConfig.tTimeAttr.eFormat = OSD_DATE_FORMAT_YYMMDDHHmmSS;
        tOsdConfig.tTimeAttr.nColor = 0xFFFFFF;
        tOsdConfig.tTimeAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tTimeAttr.nColorInv = 0x0;
        tOsdConfig.tTimeAttr.nFontSize = 16;
        tOsdConfig.nBoundaryX = 14;
        tOsdConfig.nBoundaryY = 8;
        nCharLen = 19;
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tTimeAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // LOGO
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.eType = OSD_TYPE_PICTURE;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_BOTTOM;
        tOsdConfig.nZIndex = 1;
        strPicPath = resDir + "axera_logo_96x28.argb1555";
        strcpy(tOsdConfig.tPicAttr.szFileName, strPicPath.c_str());
        tOsdConfig.tPicAttr.nWidth = 96;
        tOsdConfig.tPicAttr.nHeight = 28;
        tOsdConfig.nBoundaryX = 8;
        tOsdConfig.nBoundaryY = 8;
        tOsdConfig.nBoundaryW = 96;
        tOsdConfig.nBoundaryH = 28;

        vecOsdCfg.push_back(tOsdConfig);

        // Channel
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_CHANNEL;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_RIGHT_TOP;
        tOsdConfig.nZIndex = 2;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "CHANNEL-*");
        tOsdConfig.nBoundaryX = 8;
        tOsdConfig.nBoundaryY = 8;
        tOsdConfig.tStrAttr.nFontSize = 16;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Location
        tOsdConfig.bEnable = AX_FALSE;
        tOsdConfig.eType = OSD_TYPE_STRING_LOCATION;
        tOsdConfig.eAlign = OSD_ALIGN_TYPE_LEFT_BOTTOM;
        tOsdConfig.nZIndex = 3;
        tOsdConfig.tStrAttr.nColor = 0xFFFFFF;
        tOsdConfig.tStrAttr.bInvEnable = AX_FALSE;
        tOsdConfig.tStrAttr.nColorInv = 0x0;
        sprintf(tOsdConfig.tStrAttr.szStr, "LOCATION-*");
        tOsdConfig.nBoundaryX = 20;
        tOsdConfig.nBoundaryY = 20;
        tOsdConfig.tStrAttr.nFontSize = 16;
        nCharLen = swprintf(&wszOsdStr[0], MAX_OSD_WSTR_CHAR_LEN, L"%s", tOsdConfig.tStrAttr.szStr);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize / 2, BASE_FONT_SIZE) * nCharLen;
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.tStrAttr.nFontSize, BASE_FONT_SIZE);
        tOsdConfig.nBoundaryW = ALIGN_UP(tOsdConfig.nBoundaryW, OSD_ALIGN_WIDTH);
        tOsdConfig.nBoundaryH = ALIGN_UP(tOsdConfig.nBoundaryH, OSD_ALIGN_HEIGHT);

        vecOsdCfg.push_back(tOsdConfig);

        // Rect
        tOsdConfig.bEnable = AX_TRUE;
        tOsdConfig.nZIndex = 5;
        tOsdConfig.eType = OSD_TYPE_RECT;
        vecOsdCfg.push_back(tOsdConfig);
        CWebOptionHelper::GetInstance()->SetOsdConfig(1, 1, 2, vecOsdCfg);
    }

    OsdVideoChnIndex2IvpsGrp();
    return AX_TRUE;
}

AX_VOID CIPCConfig::OsdVideoChnIndex2IvpsGrp(AX_VOID) {
    /*pair[sensorID, videoIndex] = pair[ivpsGrp, ivpsChn] */
    std::map<pair<AX_U8, AX_U8>, pair<AX_U8, AX_U8>> pairVideo2Ivps;
    // AX620E TODO:
    pair<AX_U8, AX_U8> tVenc = make_pair(0, 0);
    pair<AX_U8, AX_U8> tIvps = make_pair(0, 0);
    pairVideo2Ivps[tVenc] = tIvps;

    tVenc = make_pair(0, 1);
    tIvps = make_pair(0, 2);
    pairVideo2Ivps[tVenc] = tIvps;

    AX_S32 nScenario = 0;
    CCmdLineParser::GetInstance()->GetScenario(nScenario);
    if (E_APP_SCENARIO_DEFAULT == nScenario) {
        /* sensor1*/
        pair<AX_U8, AX_U8> tVenc = make_pair(1, 0);
        pair<AX_U8, AX_U8> tIvps = make_pair(1, 0);
        pairVideo2Ivps[tVenc] = tIvps;

        tVenc = make_pair(1, 1);
        tIvps = make_pair(1, 2);
        pairVideo2Ivps[tVenc] = tIvps;
    }
    CWebOptionHelper::GetInstance()->SetIvpsGrp2VideoIndex(pairVideo2Ivps);
}

std::string CIPCConfig::GetResPath() {
#ifdef SLT
    return "/opt/bin/res/ipc/res/";
#else
    return "./res/";
#endif
}
