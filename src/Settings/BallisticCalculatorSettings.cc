#include "BallisticCalculatorSettings.h"
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>
#include <QtQml>

// Определяем новый макрос, который определяет только функцию, но не константу имени
#define DECLARE_SETTINGSFACT_FUNC_ONLY(CLASS, NAME) \
    Fact* CLASS::NAME() \
    { \
        if (!_ ## NAME ## Fact) { \
            _ ## NAME ## Fact = _createSettingsFact(NAME ## Name); \
        } \
        return _ ## NAME ## Fact; \
    }

// Правильное использование макроса DECLARE_SETTINGGROUP
// Первый параметр должен быть "BallisticCalculator", чтобы получилось "BallisticCalculatorSettings"
DECLARE_SETTINGGROUP(BallisticCalculator, "BallisticCalculator")
{
    qmlRegisterUncreatableType<BallisticCalculatorSettings>("QGroundControl.SettingsManager", 1, 0, "BallisticCalculatorSettings", "Reference only");
}

// Объявление констант для имен настроек
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

// Деструктор
BallisticCalculatorSettings::~BallisticCalculatorSettings()
{
}

// Используем макрос DECLARE_SETTINGSFACT_FUNC_ONLY только для методов, объявленных с DEFINE_SETTINGFACT
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, WindFilterEnabled)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, WindFilterPeriod)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, AuxChannel)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, AuxMinHeight)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, AuxMaxHeight)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, DropHeight)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, GimbalPitch)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, ShowTrajectory)
DECLARE_SETTINGSFACT_FUNC_ONLY(BallisticCalculatorSettings, ActiveProfile)

// Константные методы доступа к фактам
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