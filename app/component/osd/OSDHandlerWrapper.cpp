/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Ningbo) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Ningbo) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Ningbo) Co., Ltd.
 *
 **************************************************************************************************/

#include "OSDHandlerWrapper.h"

COSDHandlerWrapper::COSDHandlerWrapper(AX_VOID) {
}

COSDHandlerWrapper::~COSDHandlerWrapper(AX_VOID) {
}

COSDHandler *COSDHandlerWrapper::NewInstance(AX_VOID) {
    return new COSDHandler();
}

AX_VOID COSDHandlerWrapper::ReleaseInstance(COSDHandler **ppInstance) {
    delete *ppInstance;
    *ppInstance = 0;
}

AX_BOOL COSDHandlerWrapper::InitHandler(COSDHandler *pOSDHandler, const AX_CHAR *fontFilePath) {
    return pOSDHandler->InitOSDHandler(fontFilePath);
}

AX_VOID *COSDHandlerWrapper::GenARGB(COSDHandler *pOSDHandler, wchar_t *pTextStr, AX_U16 *pArgbBuffer, AX_U32 u32OSDWidth,
                                     AX_U32 u32OSDHeight, AX_S16 sX, AX_S16 sY, AX_U16 uFontSize, AX_BOOL bIsBrushSide, AX_U32 uFontColor,
                                     AX_U32 uBgColor, AX_U32 uSideColor, OSD_ALIGN_TYPE_E enAlign) {
    return pOSDHandler->GenARGB(pTextStr, pArgbBuffer, u32OSDWidth, u32OSDHeight, sX, sY, uFontSize, bIsBrushSide, uFontColor, uBgColor,
                                uSideColor, enAlign);
}

AX_VOID *COSDHandlerWrapper::GenBitmap(COSDHandler *pOSDHandler, wchar_t *pTextStr, AX_U8 *pArgbBuffer, AX_U32 u32OSDWidth,
                                       AX_U32 u32OSDHeight, AX_S16 sX, AX_S16 sY, AX_U16 uFontSize, OSD_ALIGN_TYPE_E enAlign) {
    return pOSDHandler->GenBitmap(pTextStr, pArgbBuffer, u32OSDWidth, u32OSDHeight, sX, sY, uFontSize, enAlign);
}

AX_S32 COSDHandlerWrapper::CalcStrSize(COSDHandler *pOSDHandler, wchar_t *pTextStr, AX_U16 uFontSize, AX_U32 &u32OSDWidth,
                                       AX_U32 &u32OSDHeight) {
    return pOSDHandler->CalcStrSize(pTextStr, uFontSize, u32OSDWidth, u32OSDHeight);
}
