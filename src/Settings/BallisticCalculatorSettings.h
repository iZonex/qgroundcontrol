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

    DEFINE_SETTINGFACT(AuxChannel)
    DEFINE_SETTINGFACT(AuxMinHeight)
    DEFINE_SETTINGFACT(AuxMaxHeight)
    DEFINE_SETTINGFACT(MarkerSize)
    DEFINE_SETTINGFACT(MarkerOffsetX)
    DEFINE_SETTINGFACT(MarkerOffsetY)
    DEFINE_SETTINGFACT(GimbalPitch)
    DEFINE_SETTINGFACT(WindSpeed)
    DEFINE_SETTINGFACT(WindDirection)
    DEFINE_SETTINGFACT(PayloadMass)
    DEFINE_SETTINGFACT(VerticalDragCoefficient)
    DEFINE_SETTINGFACT(HorizontalDragCoefficient)
    DEFINE_SETTINGFACT(VerticalCrossSection)
    DEFINE_SETTINGFACT(HorizontalCrossSection)
    DEFINE_SETTINGFACT(CameraOffset)
    DEFINE_SETTINGFACT(HeightMode)
    DEFINE_SETTINGFACT(FixedHeight)
    DEFINE_SETTINGFACT(CalibrationMode)
    DEFINE_SETTINGFACT(CalibrationStep)
    DEFINE_SETTINGFACT(ActiveProfile)
    DEFINE_SETTINGFACT(SavedProfiles)
    DEFINE_SETTINGFACT(WindFilterEnabled)
    DEFINE_SETTINGFACT(WindFilterPeriod)
    DEFINE_SETTINGFACT(ShowTrajectory)
    DEFINE_SETTINGFACT(ReadyToDropEnabled)
    DEFINE_SETTINGFACT(MaxDropWindSpeed)

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