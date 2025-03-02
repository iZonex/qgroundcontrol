/****************************************************************************
 *
 * (c) 2009-2023 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "CustomOSDSettings.h"

#include <QQmlEngine>
#include <QtQml>

const char* CustomOSDSettings::name = "CustomOSD";
const char* CustomOSDSettings::settingsGroup = "CustomOSD";

CustomOSDSettings::CustomOSDSettings(QObject* parent)
    : SettingsGroup(name, settingsGroup, parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterUncreatableType<CustomOSDSettings>("QGroundControl.SettingsManager", 1, 0, "CustomOSDSettings", "Reference only");
}

DECLARE_SETTINGSFACT(CustomOSDSettings, showHelloWorld)
DECLARE_SETTINGSFACT(CustomOSDSettings, showFlightInfo)
DECLARE_SETTINGSFACT(CustomOSDSettings, position)
DECLARE_SETTINGSFACT(CustomOSDSettings, opacity) 