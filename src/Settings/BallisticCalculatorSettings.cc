#include "BallisticCalculatorSettings.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>

// Объявление группы настроек
const char* BallisticCalculatorSettings::name = "BallisticCalculator";
const char* BallisticCalculatorSettings::settingsGroup = "BallisticCalculator";

// Макрос для объявления константных методов доступа к Fact
#define DECLARE_SETTINGSFACT_CONST(CLASS, NAME) \
    const char* CLASS::NAME ## Name = #NAME; \
    Fact* CLASS::NAME() const \
    { \
        if (!_ ## NAME ## Fact) { \
            const_cast<CLASS*>(this)->_ ## NAME ## Fact = const_cast<CLASS*>(this)->_createSettingsFact(NAME ## Name); \
        } \
        return _ ## NAME ## Fact; \
    }

// Реализация методов доступа к Fact
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, PayloadMass)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, VerticalDragCoefficient)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, HorizontalDragCoefficient)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, VerticalCrossSection)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, HorizontalCrossSection)

DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, WindSpeed)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, WindDirection)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindFilterEnabled)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindFilterPeriod)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxChannel)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxMinHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxMaxHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, DropHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, GimbalPitch)

DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, MarkerSize)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, MarkerOffsetX)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, MarkerOffsetY)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, ShowTrajectory)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, ReadyToDropEnabled)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, MaxDropWindSpeed)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, ActiveProfile)
DECLARE_SETTINGSFACT_CONST(BallisticCalculatorSettings, SavedProfiles)

BallisticCalculatorSettings::BallisticCalculatorSettings(QObject* parent)
    : SettingsGroup(name, settingsGroup, parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Создаем факты при инициализации
    PayloadMass();
    VerticalDragCoefficient();
    HorizontalDragCoefficient();
    VerticalCrossSection();
    HorizontalCrossSection();
    
    WindSpeed();
    WindDirection();
    WindFilterEnabled();
    WindFilterPeriod();
    
    AuxChannel();
    AuxMinHeight();
    AuxMaxHeight();
    DropHeight();
    GimbalPitch();
    
    MarkerSize();
    MarkerOffsetX();
    MarkerOffsetY();
    ShowTrajectory();
    ReadyToDropEnabled();
    MaxDropWindSpeed();
    
    ActiveProfile();
    SavedProfiles();
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