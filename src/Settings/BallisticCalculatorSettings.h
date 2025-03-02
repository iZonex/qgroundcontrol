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

    // Методы доступа к Fact объектам с именами вида xxxFact()
    Fact* payloadMassFact() { return PayloadMass(); }
    Fact* verticalDragCoefficientFact() { return VerticalDragCoefficient(); }
    Fact* horizontalDragCoefficientFact() { return HorizontalDragCoefficient(); }
    Fact* verticalCrossSectionFact() { return VerticalCrossSection(); }
    Fact* horizontalCrossSectionFact() { return HorizontalCrossSection(); }
    
    Fact* windSpeedFact() { return WindSpeed(); }
    Fact* windDirectionFact() { return WindDirection(); }
    Fact* windFilterEnabledFact() { return WindFilterEnabled(); }
    Fact* windFilterPeriodFact() { return WindFilterPeriod(); }
    
    Fact* auxChannelFact() { return AuxChannel(); }
    Fact* auxMinHeightFact() { return AuxMinHeight(); }
    Fact* auxMaxHeightFact() { return AuxMaxHeight(); }
    Fact* dropHeightFact() { return DropHeight(); }
    Fact* gimbalPitchFact() { return GimbalPitch(); }
    
    Fact* markerSizeFact() { return MarkerSize(); }
    Fact* markerOffsetXFact() { return MarkerOffsetX(); }
    Fact* markerOffsetYFact() { return MarkerOffsetY(); }
    Fact* showTrajectoryFact() { return ShowTrajectory(); }
    Fact* readyToDropEnabledFact() { return ReadyToDropEnabled(); }
    Fact* maxDropWindSpeedFact() { return MaxDropWindSpeed(); }
    
    Fact* activeProfileFact() { return ActiveProfile(); }
    Fact* savedProfilesFact() { return SavedProfiles(); }

private:
    void saveProfileToJson(const QString& name, const QJsonObject& profile);
    QJsonObject loadProfileFromJson(const QString& name) const;
    QJsonObject getCurrentProfileData() const;
    void applyProfileData(const QJsonObject& profile);
}; 