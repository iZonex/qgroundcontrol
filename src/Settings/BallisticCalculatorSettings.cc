#include "BallisticCalculatorSettings.h"
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>
#include <QtQml>

// Определяем статические переменные класса
const char* BallisticCalculatorSettings::name = "BallisticCalculator";
const char* BallisticCalculatorSettings::settingsGroup = "BallisticCalculator";

// Определяем имена для всех Fact
const char* BallisticCalculatorSettings::PayloadMassName = "PayloadMass";
const char* BallisticCalculatorSettings::VerticalDragCoefficientName = "VerticalDragCoefficient";
const char* BallisticCalculatorSettings::HorizontalDragCoefficientName = "HorizontalDragCoefficient";
const char* BallisticCalculatorSettings::VerticalCrossSectionName = "VerticalCrossSection";
const char* BallisticCalculatorSettings::HorizontalCrossSectionName = "HorizontalCrossSection";
const char* BallisticCalculatorSettings::WindSpeedName = "WindSpeed";
const char* BallisticCalculatorSettings::WindDirectionName = "WindDirection";
const char* BallisticCalculatorSettings::WindFilterEnabledName = "WindFilterEnabled";
const char* BallisticCalculatorSettings::WindFilterPeriodName = "WindFilterPeriod";
const char* BallisticCalculatorSettings::AuxChannelName = "AuxChannel";
const char* BallisticCalculatorSettings::AuxMinHeightName = "AuxMinHeight";
const char* BallisticCalculatorSettings::AuxMaxHeightName = "AuxMaxHeight";
const char* BallisticCalculatorSettings::DropHeightName = "DropHeight";
const char* BallisticCalculatorSettings::GimbalPitchName = "GimbalPitch";
const char* BallisticCalculatorSettings::MarkerSizeName = "MarkerSize";
const char* BallisticCalculatorSettings::MarkerOffsetXName = "MarkerOffsetX";
const char* BallisticCalculatorSettings::MarkerOffsetYName = "MarkerOffsetY";
const char* BallisticCalculatorSettings::ShowTrajectoryName = "ShowTrajectory";
const char* BallisticCalculatorSettings::ReadyToDropEnabledName = "ReadyToDropEnabled";
const char* BallisticCalculatorSettings::MaxDropWindSpeedName = "MaxDropWindSpeed";
const char* BallisticCalculatorSettings::ActiveProfileName = "ActiveProfile";
const char* BallisticCalculatorSettings::SavedProfilesName = "SavedProfiles";
const char* BallisticCalculatorSettings::WindAltitudeName = "WindAltitude";
const char* BallisticCalculatorSettings::DropAltitudeName = "DropAltitude";
const char* BallisticCalculatorSettings::DropSpeedName = "DropSpeed";
const char* BallisticCalculatorSettings::DropDirectionName = "DropDirection";

BallisticCalculatorSettings::BallisticCalculatorSettings(QObject* parent)
    : SettingsGroup(name, settingsGroup, parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterUncreatableType<BallisticCalculatorSettings>("QGroundControl.SettingsManager", 1, 0, "BallisticCalculatorSettings", "Reference only");
}

BallisticCalculatorSettings::~BallisticCalculatorSettings()
{
    // Пустой деструктор, все очистится автоматически
}

// Определяем методы доступа к Fact
Fact* BallisticCalculatorSettings::PayloadMass() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_PayloadMassFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_PayloadMassFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(PayloadMassName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_PayloadMassFact;
}

Fact* BallisticCalculatorSettings::VerticalDragCoefficient() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_VerticalDragCoefficientFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_VerticalDragCoefficientFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(VerticalDragCoefficientName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_VerticalDragCoefficientFact;
}

Fact* BallisticCalculatorSettings::HorizontalDragCoefficient() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_HorizontalDragCoefficientFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_HorizontalDragCoefficientFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(HorizontalDragCoefficientName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_HorizontalDragCoefficientFact;
}

Fact* BallisticCalculatorSettings::VerticalCrossSection() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_VerticalCrossSectionFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_VerticalCrossSectionFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(VerticalCrossSectionName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_VerticalCrossSectionFact;
}

Fact* BallisticCalculatorSettings::HorizontalCrossSection() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_HorizontalCrossSectionFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_HorizontalCrossSectionFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(HorizontalCrossSectionName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_HorizontalCrossSectionFact;
}

Fact* BallisticCalculatorSettings::WindSpeed() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_WindSpeedFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_WindSpeedFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(WindSpeedName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_WindSpeedFact;
}

Fact* BallisticCalculatorSettings::WindDirection() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_WindDirectionFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_WindDirectionFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(WindDirectionName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_WindDirectionFact;
}

Fact* BallisticCalculatorSettings::WindFilterEnabled()
{
    if (!_WindFilterEnabledFact) {
        _WindFilterEnabledFact = _createSettingsFact(WindFilterEnabledName);
    }
    return _WindFilterEnabledFact;
}

Fact* BallisticCalculatorSettings::WindFilterPeriod()
{
    if (!_WindFilterPeriodFact) {
        _WindFilterPeriodFact = _createSettingsFact(WindFilterPeriodName);
    }
    return _WindFilterPeriodFact;
}

Fact* BallisticCalculatorSettings::AuxChannel()
{
    if (!_AuxChannelFact) {
        _AuxChannelFact = _createSettingsFact(AuxChannelName);
    }
    return _AuxChannelFact;
}

Fact* BallisticCalculatorSettings::AuxMinHeight()
{
    if (!_AuxMinHeightFact) {
        _AuxMinHeightFact = _createSettingsFact(AuxMinHeightName);
    }
    return _AuxMinHeightFact;
}

Fact* BallisticCalculatorSettings::AuxMaxHeight()
{
    if (!_AuxMaxHeightFact) {
        _AuxMaxHeightFact = _createSettingsFact(AuxMaxHeightName);
    }
    return _AuxMaxHeightFact;
}

Fact* BallisticCalculatorSettings::DropHeight()
{
    if (!_DropHeightFact) {
        _DropHeightFact = _createSettingsFact(DropHeightName);
    }
    return _DropHeightFact;
}

Fact* BallisticCalculatorSettings::GimbalPitch()
{
    if (!_GimbalPitchFact) {
        _GimbalPitchFact = _createSettingsFact(GimbalPitchName);
    }
    return _GimbalPitchFact;
}

Fact* BallisticCalculatorSettings::MarkerSize() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_MarkerSizeFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_MarkerSizeFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(MarkerSizeName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_MarkerSizeFact;
}

Fact* BallisticCalculatorSettings::MarkerOffsetX() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetXFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetXFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(MarkerOffsetXName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetXFact;
}

Fact* BallisticCalculatorSettings::MarkerOffsetY() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetYFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetYFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(MarkerOffsetYName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_MarkerOffsetYFact;
}

Fact* BallisticCalculatorSettings::ShowTrajectory()
{
    if (!_ShowTrajectoryFact) {
        _ShowTrajectoryFact = _createSettingsFact(ShowTrajectoryName);
    }
    return _ShowTrajectoryFact;
}

Fact* BallisticCalculatorSettings::ReadyToDropEnabled() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_ReadyToDropEnabledFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_ReadyToDropEnabledFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(ReadyToDropEnabledName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_ReadyToDropEnabledFact;
}

Fact* BallisticCalculatorSettings::MaxDropWindSpeed() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_MaxDropWindSpeedFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_MaxDropWindSpeedFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(MaxDropWindSpeedName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_MaxDropWindSpeedFact;
}

Fact* BallisticCalculatorSettings::ActiveProfile()
{
    if (!_ActiveProfileFact) {
        _ActiveProfileFact = _createSettingsFact(ActiveProfileName);
    }
    return _ActiveProfileFact;
}

Fact* BallisticCalculatorSettings::SavedProfiles() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_SavedProfilesFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_SavedProfilesFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(SavedProfilesName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_SavedProfilesFact;
}

Fact* BallisticCalculatorSettings::WindAltitude() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_WindAltitudeFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_WindAltitudeFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(WindAltitudeName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_WindAltitudeFact;
}

Fact* BallisticCalculatorSettings::DropAltitude() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_DropAltitudeFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_DropAltitudeFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(DropAltitudeName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_DropAltitudeFact;
}

Fact* BallisticCalculatorSettings::DropSpeed() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_DropSpeedFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_DropSpeedFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(DropSpeedName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_DropSpeedFact;
}

Fact* BallisticCalculatorSettings::DropDirection() const
{
    if (!const_cast<BallisticCalculatorSettings*>(this)->_DropDirectionFact) {
        const_cast<BallisticCalculatorSettings*>(this)->_DropDirectionFact = const_cast<BallisticCalculatorSettings*>(this)->_createSettingsFact(DropDirectionName);
    }
    return const_cast<BallisticCalculatorSettings*>(this)->_DropDirectionFact;
}

void BallisticCalculatorSettings::saveCurrentProfile(const QString& name)
{
    QJsonDocument doc = QJsonDocument::fromJson(SavedProfiles()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    profiles[name] = getCurrentProfileData();
    SavedProfiles()->setRawValue(QString(QJsonDocument(profiles).toJson()));
    ActiveProfile()->setRawValue(name);
}

void BallisticCalculatorSettings::loadProfile(const QString& name)
{
    QJsonObject profile = loadProfileFromJson(name);
    if (!profile.isEmpty()) {
        applyProfileData(profile);
        ActiveProfile()->setRawValue(name);
    }
}

void BallisticCalculatorSettings::deleteProfile(const QString& name)
{
    if (name == "Default") return;

    QJsonDocument doc = QJsonDocument::fromJson(SavedProfiles()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    profiles.remove(name);
    SavedProfiles()->setRawValue(QString(QJsonDocument(profiles).toJson()));
    
    if (ActiveProfile()->rawValue().toString() == name) {
        ActiveProfile()->setRawValue("Default");
    }
}

bool BallisticCalculatorSettings::isReadyToDrop() const
{
    if (!ReadyToDropEnabled()->rawValue().toBool()) {
        return true;
    }

    double windSpeed = WindSpeed()->rawValue().toDouble();
    double maxWindSpeed = MaxDropWindSpeed()->rawValue().toDouble();

    return windSpeed <= maxWindSpeed;
}

QStringList BallisticCalculatorSettings::getProfileList() const
{
    QJsonDocument doc = QJsonDocument::fromJson(SavedProfiles()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    QStringList result;
    result << "Default";
    result.append(profiles.keys());
    return result;
}

QJsonObject BallisticCalculatorSettings::getCurrentProfileData() const
{
    QJsonObject profile;
    
    profile["payloadMass"] = PayloadMass()->rawValue().toDouble();
    profile["verticalDragCoefficient"] = VerticalDragCoefficient()->rawValue().toDouble();
    profile["horizontalDragCoefficient"] = HorizontalDragCoefficient()->rawValue().toDouble();
    profile["verticalCrossSection"] = VerticalCrossSection()->rawValue().toDouble();
    profile["horizontalCrossSection"] = HorizontalCrossSection()->rawValue().toDouble();
    
    profile["markerSize"] = MarkerSize()->rawValue().toInt();
    profile["markerOffsetX"] = MarkerOffsetX()->rawValue().toInt();
    profile["markerOffsetY"] = MarkerOffsetY()->rawValue().toInt();
    
    return profile;
}

QJsonObject BallisticCalculatorSettings::loadProfileFromJson(const QString& name) const
{
    QJsonDocument doc = QJsonDocument::fromJson(SavedProfiles()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    return profiles.value(name).toObject();
}

void BallisticCalculatorSettings::applyProfileData(const QJsonObject& profile)
{
    if (profile.contains("payloadMass"))
        PayloadMass()->setRawValue(profile["payloadMass"].toDouble());
    if (profile.contains("verticalDragCoefficient"))
        VerticalDragCoefficient()->setRawValue(profile["verticalDragCoefficient"].toDouble());
    if (profile.contains("horizontalDragCoefficient"))
        HorizontalDragCoefficient()->setRawValue(profile["horizontalDragCoefficient"].toDouble());
    if (profile.contains("verticalCrossSection"))
        VerticalCrossSection()->setRawValue(profile["verticalCrossSection"].toDouble());
    if (profile.contains("horizontalCrossSection"))
        HorizontalCrossSection()->setRawValue(profile["horizontalCrossSection"].toDouble());
    
    if (profile.contains("markerSize"))
        MarkerSize()->setRawValue(profile["markerSize"].toInt());
    if (profile.contains("markerOffsetX"))
        MarkerOffsetX()->setRawValue(profile["markerOffsetX"].toInt());
    if (profile.contains("markerOffsetY"))
        MarkerOffsetY()->setRawValue(profile["markerOffsetY"].toInt());
} 