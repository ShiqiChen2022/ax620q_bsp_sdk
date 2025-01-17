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
#include "AXSingleton.h"
#include "IPPLBuilder.h"
#include "ISensor.hpp"
#include "picojson.h"

class CSensorCfgParser : public CAXSingleton<CSensorCfgParser> {
    friend class CAXSingleton<CSensorCfgParser>;

public:
    AX_BOOL GetConfig(std::map<AX_U8, SENSOR_CONFIG_T>& mapSensorCfg, AX_U32& nSensorCount);

private:
    CSensorCfgParser(AX_VOID) = default;
    ~CSensorCfgParser(AX_VOID) = default;

    AX_BOOL InitOnce() override;

    AX_BOOL ParseFile(const std::string& strPath, std::map<AX_U8, SENSOR_CONFIG_T>& mapSensorCfg);
    AX_BOOL ParseJson(picojson::object& objJsonRoot, std::map<AX_U8, SENSOR_CONFIG_T>& mapSensorCfg);
    std::string LoadType2FileName(AX_S32 nLoadType);
};
