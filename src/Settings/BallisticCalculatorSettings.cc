#include "BallisticCalculatorSettings.h"
#include <QJsonDocument>
#include <QJsonObject>

IMPLEMENT_SETTING_NAME_GROUP(BallisticCalculatorSettings)
{
    settingsGroup = QStringLiteral("BallisticCalculator");
}

BallisticCalculatorSettings::BallisticCalculatorSettings(QObject* parent)
    : SettingsGroup(settingsGroup, QString() /* root settings group */, parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    // Регистрация всех настроек
    _auxChannelFact =                 _createSettingsFact(AuxChannelName);
    _auxMinHeightFact =               _createSettingsFact(AuxMinHeightName);
    _auxMaxHeightFact =               _createSettingsFact(AuxMaxHeightName);
    _markerSizeFact =                 _createSettingsFact(MarkerSizeName);
    _markerOffsetXFact =              _createSettingsFact(MarkerOffsetXName);
    _markerOffsetYFact =              _createSettingsFact(MarkerOffsetYName);
    _gimbalPitchFact =                _createSettingsFact(GimbalPitchName);
    _windSpeedFact =                  _createSettingsFact(WindSpeedName);
    _windDirectionFact =              _createSettingsFact(WindDirectionName);
    _payloadMassFact =                _createSettingsFact(PayloadMassName);
    _verticalDragCoefficientFact =    _createSettingsFact(VerticalDragCoefficientName);
    _horizontalDragCoefficientFact =  _createSettingsFact(HorizontalDragCoefficientName);
    _verticalCrossSectionFact =       _createSettingsFact(VerticalCrossSectionName);
    _horizontalCrossSectionFact =     _createSettingsFact(HorizontalCrossSectionName);
    _cameraOffsetFact =               _createSettingsFact(CameraOffsetName);
    _heightModeFact =                 _createSettingsFact(HeightModeName);
    _fixedHeightFact =                _createSettingsFact(FixedHeightName);
}

void BallisticCalculatorSettings::saveCurrentProfile(const QString& name)
{
    QJsonObject profile = getCurrentProfileData();
    saveProfileToJson(name, profile);
    activeProfiFact()->setRawValue(name);
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
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    if (profiles.contains(name)) {
        profiles.remove(name);
        savedProfilesFact()->setRawValue(QString(QJsonDocument(profiles).toJson()));
        
        // Если удаляем активный профиль, переключаемся на Default
        if (activeProfileFact()->rawValue().toString() == name) {
            activeProfileFact()->setRawValue("Default");
        }
    }
}

QStringList BallisticCalculatorSettings::getProfileList() const
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    return doc.object().keys();
}

bool BallisticCalculatorSettings::isReadyToDrop() const
{
    if (!readyToDropEnabledFact()->rawValue().toBool()) {
        return true; // Если проверка отключена, всегда готов
    }

    // Проверяем скорость ветра
    double currentWindSpeed = windSpeedFact()->rawValue().toDouble();
    double maxWindSpeed = maxDropWindSpeedFact()->rawValue().toDouble();
    
    return currentWindSpeed <= maxWindSpeed;
}

void BallisticCalculatorSettings::saveProfileToJson(const QString& name, const QJsonObject& profile)
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    profiles[name] = profile;
    savedProfilesFact()->setRawValue(QString(QJsonDocument(profiles).toJson()));
}

QJsonObject BallisticCalculatorSettings::loadProfileFromJson(const QString& name) const
{
    QJsonDocument doc = QJsonDocument::fromJson(savedProfilesFact()->rawValue().toString().toUtf8());
    QJsonObject profiles = doc.object();
    
    return profiles.value(name).toObject();
}

QJsonObject BallisticCalculatorSettings::getCurrentProfileData() const
{
    QJsonObject profile;
    
    // Сохраняем все параметры груза
    profile["payloadMass"] = payloadMassFact()->rawValue().toDouble();
    profile["verticalDragCoefficient"] = verticalDragCoefficientFact()->rawValue().toDouble();
    profile["horizontalDragCoefficient"] = horizontalDragCoefficientFact()->rawValue().toDouble();
    profile["verticalCrossSection"] = verticalCrossSectionFact()->rawValue().toDouble();
    profile["horizontalCrossSection"] = horizontalCrossSectionFact()->rawValue().toDouble();
    profile["cameraOffset"] = cameraOffsetFact()->rawValue().toDouble();
    
    // Сохраняем калибровку OSD
    profile["markerSize"] = markerSizeFact()->rawValue().toInt();
    profile["markerOffsetX"] = markerOffsetXFact()->rawValue().toInt();
    profile["markerOffsetY"] = markerOffsetYFact()->rawValue().toInt();
    
    return profile;
}

void BallisticCalculatorSettings::applyProfileData(const QJsonObject& profile)
{
    // Применяем параметры груза
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
    if (profile.contains("cameraOffset"))
        cameraOffsetFact()->setRawValue(profile["cameraOffset"].toDouble());
    
    // Применяем калибровку OSD
    if (profile.contains("markerSize"))
        markerSizeFact()->setRawValue(profile["markerSize"].toInt());
    if (profile.contains("markerOffsetX"))
        markerOffsetXFact()->setRawValue(profile["markerOffsetX"].toInt());
    if (profile.contains("markerOffsetY"))
        markerOffsetYFact()->setRawValue(profile["markerOffsetY"].toInt());
} 