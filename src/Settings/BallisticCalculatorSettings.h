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

    // Параметры сброса
    DEFINE_SETTINGFACT(AuxChannel)
    DEFINE_SETTINGFACT(AuxMinHeight)
    DEFINE_SETTINGFACT(AuxMaxHeight)
    DEFINE_SETTINGFACT(DropHeight)
    DEFINE_SETTINGFACT(GimbalPitch)

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

    // Режимы определения высоты
    static const int HeightModeFixed = 0;
    static const int HeightModeBarometric = 1;
    static const int HeightModeRemote = 2;

    // Методы для работы с профилями
    Q_INVOKABLE void saveCurrentProfile(const QString& name);
    Q_INVOKABLE void loadProfile(const QString& name);
    Q_INVOKABLE void deleteProfile(const QString& name);
    Q_INVOKABLE QStringList getProfileList() const;
    Q_INVOKABLE bool isReadyToDrop() const;

private:
    void saveProfileToJson(const QString& name, const QJsonObject& profile);
    QJsonObject loadProfileFromJson(const QString& name) const;
    QJsonObject getCurrentProfileData() const;
    void applyProfileData(const QJsonObject& profile);
}; 