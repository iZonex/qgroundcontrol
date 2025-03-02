#pragma once

#include "SettingsGroup.h"
#include <QJsonDocument>
#include <QJsonObject>

class BallisticCalculatorSettings : public SettingsGroup
{
    Q_OBJECT

public:
    BallisticCalculatorSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP()

    // Основные настройки
    DEFINE_SETTINGFACT(Enabled)
    
    // Параметры груза
    DEFINE_SETTINGFACT(PayloadMass)
    DEFINE_SETTINGFACT(VerticalDragCoefficient)
    DEFINE_SETTINGFACT(HorizontalDragCoefficient)
    DEFINE_SETTINGFACT(VerticalCrossSection)
    DEFINE_SETTINGFACT(HorizontalCrossSection)

    // Параметры ветра
    DEFINE_SETTINGFACT(WindSpeed)
    DEFINE_SETTINGFACT(WindDirection)
    DEFINE_SETTINGFACT(WindFilterEnabled)
    DEFINE_SETTINGFACT(WindFilterPeriod)
    DEFINE_SETTINGFACT(WindAltitude)

    // Параметры сброса
    DEFINE_SETTINGFACT(AuxChannel)
    DEFINE_SETTINGFACT(AuxMinHeight)
    DEFINE_SETTINGFACT(AuxMaxHeight)
    DEFINE_SETTINGFACT(DropHeight)
    DEFINE_SETTINGFACT(GimbalPitch)
    DEFINE_SETTINGFACT(DropAltitude)
    DEFINE_SETTINGFACT(DropSpeed)
    DEFINE_SETTINGFACT(DropDirection)
    DEFINE_SETTINGFACT(CameraOffset)

    // Параметры OSD
    DEFINE_SETTINGFACT(MarkerSize)
    DEFINE_SETTINGFACT(MarkerOffsetX)
    DEFINE_SETTINGFACT(MarkerOffsetY)
    DEFINE_SETTINGFACT(ShowTrajectory)
    DEFINE_SETTINGFACT(ReadyToDropEnabled)
    DEFINE_SETTINGFACT(MaxDropWindSpeed)

    // Профили
    DEFINE_SETTINGFACT(ActiveProfile)
    DEFINE_SETTINGFACT(SavedProfiles)
    
    // Калибровка
    DEFINE_SETTINGFACT(CalibrationMode)
    DEFINE_SETTINGFACT(CalibrationStep)

    // Режимы определения высоты
    static const int HeightModeFixed = 0;
    static const int HeightModeBarometric = 1;
    static const int HeightModeRemote = 2;

    // Методы для работы с профилями
    Q_INVOKABLE void saveCurrentProfile(const QString& name);
    Q_INVOKABLE void loadProfile(const QString& name);
    Q_INVOKABLE void deleteProfile(const QString& name);
    Q_INVOKABLE QStringList getProfileList();
    Q_INVOKABLE bool isReadyToDrop();

private:
    QJsonObject loadProfileFromJson(const QString& name);
    QJsonObject getCurrentProfileData();
    void applyProfileData(const QJsonObject& profile);
}; 