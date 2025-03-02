#include "BallisticCalculatorSettings.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>

DECLARE_SETTINGGROUP(BallisticCalculatorSettings, "BallisticCalculator")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

// Реализация методов доступа к Fact
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, PayloadMass)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, VerticalDragCoefficient)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, HorizontalDragCoefficient)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, VerticalCrossSection)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, HorizontalCrossSection)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindSpeed)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindDirection)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindFilterEnabled)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, WindFilterPeriod)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxChannel)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxMinHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, AuxMaxHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, DropHeight)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, GimbalPitch)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, MarkerSize)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, MarkerOffsetX)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, MarkerOffsetY)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, ShowTrajectory)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, ReadyToDropEnabled)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, MaxDropWindSpeed)

DECLARE_SETTINGSFACT(BallisticCalculatorSettings, ActiveProfile)
DECLARE_SETTINGSFACT(BallisticCalculatorSettings, SavedProfiles)

BallisticCalculatorSettings::BallisticCalculatorSettings(QObject* parent)
    : SettingsGroup(settingsGroup, QString() /* root settings group */, parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    _payloadMassFact =                _createSettingsFact(PayloadMassName);
    _verticalDragCoefficientFact =    _createSettingsFact(VerticalDragCoefficientName);
    _horizontalDragCoefficientFact =  _createSettingsFact(HorizontalDragCoefficientName);
    _verticalCrossSectionFact =       _createSettingsFact(VerticalCrossSectionName);
    _horizontalCrossSectionFact =     _createSettingsFact(HorizontalCrossSectionName);

    _windSpeedFact =                  _createSettingsFact(WindSpeedName);
    _windDirectionFact =              _createSettingsFact(WindDirectionName);
    _windFilterEnabledFact =          _createSettingsFact(WindFilterEnabledName);
    _windFilterPeriodFact =           _createSettingsFact(WindFilterPeriodName);

    _auxChannelFact =                 _createSettingsFact(AuxChannelName);
    _auxMinHeightFact =               _createSettingsFact(AuxMinHeightName);
    _auxMaxHeightFact =               _createSettingsFact(AuxMaxHeightName);
    _dropHeightFact =                 _createSettingsFact(DropHeightName);
    _gimbalPitchFact =                _createSettingsFact(GimbalPitchName);

    _markerSizeFact =                 _createSettingsFact(MarkerSizeName);
    _markerOffsetXFact =              _createSettingsFact(MarkerOffsetXName);
    _markerOffsetYFact =              _createSettingsFact(MarkerOffsetYName);
    _showTrajectoryFact =             _createSettingsFact(ShowTrajectoryName);
    _readyToDropEnabledFact =         _createSettingsFact(ReadyToDropEnabledName);
    _maxDropWindSpeedFact =           _createSettingsFact(MaxDropWindSpeedName);

    _activeProfileFact =              _createSettingsFact(ActiveProfileName);
    _savedProfilesFact =              _createSettingsFact(SavedProfilesName);
}

void BallisticCalculatorSettings::saveCurrentProfile(const QString& name)
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    profiles[name] = getCurrentProfileData();
    savedProfilesFact()->setRawValue(QString(QJsonDocument(profiles).toJson()));
    activeProfileFact()->setRawValue(name);
}

void BallisticCalculatorSettings::loadProfile(const QString& name)
{
    QJsonObject profile = loadProfileFromJson(name);
    if (!profile.isEmpty()) {
        applyProfileData(profile);
        activeProfileFact()->setRawValue(name);
    }
}

void BallisticCalculatorSettings::deleteProfile(const QString& name)
{
    if (name == "Default") return;

    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    profiles.remove(name);
    savedProfilesFact()->setRawValue(QString(QJsonDocument(profiles).toJson()));
    
    if (activeProfileFact()->rawValue().toString() == name) {
        activeProfileFact()->setRawValue("Default");
    }
}

bool BallisticCalculatorSettings::isReadyToDrop() const
{
    if (!readyToDropEnabledFact()->rawValue().toBool()) {
        return true;
    }

    double windSpeed = windSpeedFact()->rawValue().toDouble();
    double maxWindSpeed = maxDropWindSpeedFact()->rawValue().toDouble();

    return windSpeed <= maxWindSpeed;
}

QStringList BallisticCalculatorSettings::getProfileList() const
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    QStringList result;
    result << "Default";
    result.append(profiles.keys());
    return result;
}

QJsonObject BallisticCalculatorSettings::getCurrentProfileData() const
{
    QJsonObject profile;
    
    profile["payloadMass"] = payloadMassFact()->rawValue().toDouble();
    profile["verticalDragCoefficient"] = verticalDragCoefficientFact()->rawValue().toDouble();
    profile["horizontalDragCoefficient"] = horizontalDragCoefficientFact()->rawValue().toDouble();
    profile["verticalCrossSection"] = verticalCrossSectionFact()->rawValue().toDouble();
    profile["horizontalCrossSection"] = horizontalCrossSectionFact()->rawValue().toDouble();
    
    profile["markerSize"] = markerSizeFact()->rawValue().toInt();
    profile["markerOffsetX"] = markerOffsetXFact()->rawValue().toInt();
    profile["markerOffsetY"] = markerOffsetYFact()->rawValue().toInt();
    
    return profile;
}

QJsonObject BallisticCalculatorSettings::loadProfileFromJson(const QString& name) const
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    return profiles.value(name).toObject();
}

void BallisticCalculatorSettings::applyProfileData(const QJsonObject& profile)
{
    if (profile.contains("payloadMass"))
        payloadMassFact()->setRawValue(profile["payloadMass"].toDouble());
    if (profile.contains("verticalDragCoefficient"))
        verticalDragCoefficientFact()->setRawValue(profile["verticalDragCoefficient"].toDouble());
    if (profile.contains("horizontalDragCoefficient"))
        horizontalDragCoefficientFact()->setRawValue(profile["horizontalDragCoefficient"].toDouble());
    if (profile.contains("verticalCrossSection"))
        verticalCrossSectionFact()->setRawValue(profile["verticalCrossSection"].toDouble());
    if (profile.contains("horizontalCrossSection"))
        horizontalCrossSectionFact()->setRawValue(profile["horizontalCrossSection"].toDouble());
    
    if (profile.contains("markerSize"))
        markerSizeFact()->setRawValue(profile["markerSize"].toInt());
    if (profile.contains("markerOffsetX"))
        markerOffsetXFact()->setRawValue(profile["markerOffsetX"].toInt());
    if (profile.contains("markerOffsetY"))
        markerOffsetYFact()->setRawValue(profile["markerOffsetY"].toInt());
} 