/****************************************************************************
 *
 * (c) 2009-2023 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "SettingsGroup.h"

class CustomOSDSettings : public SettingsGroup
{
    Q_OBJECT

public:
    CustomOSDSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP_NAME(showHelloWorldName,          showHelloWorld,          CustomOSD)
    DEFINE_SETTING_NAME_GROUP_NAME(showFlightInfoName,          showFlightInfo,          CustomOSD)
    DEFINE_SETTING_NAME_GROUP_NAME(positionName,                position,                CustomOSD)
    DEFINE_SETTING_NAME_GROUP_NAME(opacityName,                 opacity,                 CustomOSD)

    Q_PROPERTY(Fact* showHelloWorld          READ showHelloWorld          CONSTANT)
    Q_PROPERTY(Fact* showFlightInfo          READ showFlightInfo          CONSTANT)
    Q_PROPERTY(Fact* position                READ position                CONSTANT)
    Q_PROPERTY(Fact* opacity                 READ opacity                 CONSTANT)

    Fact* showHelloWorld         (void);
    Fact* showFlightInfo         (void);
    Fact* position               (void);
    Fact* opacity                (void);

private:
    SettingsFact* _showHelloWorldFact;
    SettingsFact* _showFlightInfoFact;
    SettingsFact* _positionFact;
    SettingsFact* _opacityFact;
}; 