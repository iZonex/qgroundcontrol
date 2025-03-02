#include "BallisticCalculator.h"
#include "Vehicle.h"
#include "QGCApplication.h"
#include "Settings/SettingsManager.h"
#include "Settings/BallisticCalculatorSettings.h"
#include <cmath>

BallisticCalculator::BallisticCalculator(Vehicle* vehicle, QObject* parent)
    : QObject(parent)
    , _vehicle(vehicle)
    , _ballisticSettings(nullptr)
    , _dropTime(0.0)
    , _isActive(false)
{
    _ballisticSettings = qgcApp()->toolbox()->settingsManager()->ballisticCalculatorSettings();
    
    if (_vehicle) {
        connect(_vehicle, &Vehicle::rcChannelsChanged, this, &BallisticCalculator::_rcChannelsChanged);
        connect(_vehicle, &Vehicle::coordinateChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->heading(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->altitudeRelative(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->pitch(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
        connect(_vehicle->roll(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
    }

    if (_ballisticSettings) {
        connect(_ballisticSettings->WindSpeed(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->WindDirection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->PayloadMass(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->VerticalDragCoefficient(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->HorizontalDragCoefficient(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->VerticalCrossSection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->HorizontalCrossSection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->GimbalPitch(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        
        // Связываем настройку Enabled с активацией калькулятора
        connect(_ballisticSettings->Enabled(), &Fact::rawValueChanged, this, [this]() {
            this->setIsActive(_ballisticSettings->Enabled()->rawValue().toBool());
        });
        
        // Устанавливаем начальное состояние активности
        setIsActive(_ballisticSettings->Enabled()->rawValue().toBool());
    }
}

BallisticCalculator::BallisticCalculator(QObject* parent)
    : QObject(parent)
    , _vehicle(nullptr)
    , _ballisticSettings(nullptr)
    , _dropTime(0.0)
    , _isActive(false)
{
    _ballisticSettings = qgcApp()->toolbox()->settingsManager()->ballisticCalculatorSettings();
    
    if (_ballisticSettings) {
        connect(_ballisticSettings->WindSpeed(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->WindDirection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->PayloadMass(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->VerticalDragCoefficient(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->HorizontalDragCoefficient(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->VerticalCrossSection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->HorizontalCrossSection(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->GimbalPitch(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        
        // Связываем настройку Enabled с активацией калькулятора
        connect(_ballisticSettings->Enabled(), &Fact::rawValueChanged, this, [this]() {
            this->setIsActive(_ballisticSettings->Enabled()->rawValue().toBool());
        });
        
        // Устанавливаем начальное состояние активности
        setIsActive(_ballisticSettings->Enabled()->rawValue().toBool());
    }
}

BallisticCalculator::~BallisticCalculator()
{
    if (_vehicle) {
        disconnect(_vehicle, nullptr, this, nullptr);
    }
    if (_ballisticSettings) {
        disconnect(_ballisticSettings, nullptr, this, nullptr);
    }
}

void BallisticCalculator::setIsActive(bool active)
{
    if (_isActive != active) {
        _isActive = active;
        emit isActiveChanged(active);
        if (active) {
            _updateTrajectory();
        }
    }
}

void BallisticCalculator::updateCalculations()
{
    if (!_isActive || !_vehicle) {
        return;
    }
    calculateTrajectory();
}

void BallisticCalculator::calculateTrajectory()
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    // Получаем текущие параметры полета
    double altitude = _vehicle->altitudeRelative()->rawValue().toDouble();

    // Получаем данные о ветре из настроек
    double windSpeed = _ballisticSettings->WindSpeed()->rawValue().toDouble();
    double windDirection = _ballisticSettings->WindDirection()->rawValue().toDouble() * M_PI / 180.0;

    // Рассчитываем точку падения
    QGeoCoordinate vehiclePosition = _vehicle->coordinate();
    QGeoCoordinate newTargetPoint = calculateImpactPoint(vehiclePosition, altitude, windSpeed, windDirection);

    if (_targetPoint != newTargetPoint) {
        _targetPoint = newTargetPoint;
        emit targetPointChanged(_targetPoint);
    }

    // Обновляем траекторию
    emit trajectoryPointsChanged(calculateTrajectoryPoints());
}

QGeoCoordinate BallisticCalculator::calculateImpactPoint(const QGeoCoordinate& vehiclePosition,
                                                        double altitude,
                                                        double windSpeed,
                                                        double windDirection)
{
    if (!_ballisticSettings) {
        return vehiclePosition;
    }
    
    // Параметры груза
    double mass = _ballisticSettings->PayloadMass()->rawValue().toDouble() / 1000.0;
    double vertDragCoef = _ballisticSettings->VerticalDragCoefficient()->rawValue().toDouble();
    double horizDragCoef = _ballisticSettings->HorizontalDragCoefficient()->rawValue().toDouble();
    double vertCrossSection = _ballisticSettings->VerticalCrossSection()->rawValue().toDouble() / 10000.0;
    double horizCrossSection = _ballisticSettings->HorizontalCrossSection()->rawValue().toDouble() / 10000.0;

    // Константы для расчета
    const double dt = 0.01;  // шаг времени для интегрирования
    const double g = GRAVITY;
    const double rho = AIR_DENSITY;

    // Начальные условия
    double x = 0;
    double y = 0;
    double z = altitude;
    double vx = windSpeed * cos(windDirection);
    double vy = windSpeed * sin(windDirection);
    double vz = 0;
    
    _dropTime = 0;

    // Интегрирование методом Эйлера с учетом сопротивления воздуха
    while (z > 0) {
        // Скорость относительно воздуха
        double v_rel_x = vx - windSpeed * cos(windDirection);
        double v_rel_y = vy - windSpeed * sin(windDirection);
        double v_rel_z = vz;
        
        // Модуль относительной скорости
        double v_rel = sqrt(v_rel_x * v_rel_x + v_rel_y * v_rel_y + v_rel_z * v_rel_z);

        // Силы сопротивления воздуха
        double Fx = -0.5 * rho * horizDragCoef * horizCrossSection * v_rel * v_rel_x;
        double Fy = -0.5 * rho * horizDragCoef * horizCrossSection * v_rel * v_rel_y;
        double Fz = -0.5 * rho * vertDragCoef * vertCrossSection * v_rel * v_rel_z;

        // Ускорения
        double ax = Fx / mass;
        double ay = Fy / mass;
        double az = Fz / mass - g;

        // Интегрирование
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;
        
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
        
        _dropTime += dt;
    }

    emit dropTimeChanged(_dropTime);

    // Конвертируем смещение в метрах в координаты
    double metersPerLat = 111111.0;
    double metersPerLon = metersPerLat * cos(vehiclePosition.latitude() * M_PI / 180.0);

    double latOffset = y / metersPerLat;
    double lonOffset = x / metersPerLon;

    return QGeoCoordinate(
        vehiclePosition.latitude() + latOffset,
        vehiclePosition.longitude() + lonOffset,
        0  // высота точки приземления
    );
}

QVariantList BallisticCalculator::calculateTrajectoryPoints()
{
    QVariantList points;
    if (!_vehicle || !_ballisticSettings) return points;

    // Параметры груза
    double mass = _ballisticSettings->PayloadMass()->rawValue().toDouble() / 1000.0;
    double vertDragCoef = _ballisticSettings->VerticalDragCoefficient()->rawValue().toDouble();
    double horizDragCoef = _ballisticSettings->HorizontalDragCoefficient()->rawValue().toDouble();
    double vertCrossSection = _ballisticSettings->VerticalCrossSection()->rawValue().toDouble() / 10000.0;
    double horizCrossSection = _ballisticSettings->HorizontalCrossSection()->rawValue().toDouble() / 10000.0;

    // Получаем данные о ветре
    double windSpeed = _ballisticSettings->WindSpeed()->rawValue().toDouble();
    double windDirection = _ballisticSettings->WindDirection()->rawValue().toDouble() * M_PI / 180.0;

    // Начальные условия
    double x = 0;
    double y = 0;
    double z = _vehicle->altitudeRelative()->rawValue().toDouble();
    double vx = windSpeed * cos(windDirection);
    double vy = windSpeed * sin(windDirection);
    double vz = 0;

    const double dt = 0.1;  // больший шаг для визуализации
    const double g = GRAVITY;
    const double rho = AIR_DENSITY;

    while (z > 0) {
        // Добавляем текущую точку
        QVariantMap point;
        point["x"] = x;
        point["y"] = y;
        point["z"] = z;
        points.append(point);

        // Скорость относительно воздуха
        double v_rel_x = vx - windSpeed * cos(windDirection);
        double v_rel_y = vy - windSpeed * sin(windDirection);
        double v_rel_z = vz;
        
        double v_rel = sqrt(v_rel_x * v_rel_x + v_rel_y * v_rel_y + v_rel_z * v_rel_z);

        // Силы сопротивления
        double Fx = -0.5 * rho * horizDragCoef * horizCrossSection * v_rel * v_rel_x;
        double Fy = -0.5 * rho * horizDragCoef * horizCrossSection * v_rel * v_rel_y;
        double Fz = -0.5 * rho * vertDragCoef * vertCrossSection * v_rel * v_rel_z;

        // Ускорения
        double ax = Fx / mass;
        double ay = Fy / mass;
        double az = Fz / mass - g;

        // Интегрирование
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;
        
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
    }

    return points;
}

void BallisticCalculator::_rcChannelsChanged(int channelCount, int pwmValues[])
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    int auxChannel = _ballisticSettings->AuxChannel()->rawValue().toInt() - 1;
    if (auxChannel >= 0 && auxChannel < channelCount) {
        int pwmValue = pwmValues[auxChannel];
        double minHeight = _ballisticSettings->AuxMinHeight()->rawValue().toDouble();
        double maxHeight = _ballisticSettings->AuxMaxHeight()->rawValue().toDouble();
        double normalizedPwm = (pwmValue - 1000.0) / 1000.0;
        double height = minHeight + normalizedPwm * (maxHeight - minHeight);
        
        _ballisticSettings->DropHeight()->setRawValue(height);
    }
}

void BallisticCalculator::_updateWindFromAttitude()
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    double pitch = _vehicle->pitch()->rawValue().toDouble() * M_PI / 180.0;
    double roll = _vehicle->roll()->rawValue().toDouble() * M_PI / 180.0;
    
    double windX = -pitch * (180.0 / M_PI);
    double windY = -roll * (180.0 / M_PI);
    
    double windSpeed = sqrt(windX * windX + windY * windY);
    double windDirection = atan2(windY, windX) * 180.0 / M_PI;
    
    if (windDirection < 0) {
        windDirection += 360.0;
    }
    
    _ballisticSettings->WindSpeed()->setRawValue(windSpeed);
    _ballisticSettings->WindDirection()->setRawValue(windDirection);
}

void BallisticCalculator::_updateTrajectory()
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    _updateWindFromAttitude();
    calculateTrajectory();
}

void BallisticCalculator::setVehicle(Vehicle* vehicle)
{
    // Отключаем соединения со старым транспортным средством, если оно существует
    if (_vehicle) {
        disconnect(_vehicle, &Vehicle::rcChannelsChanged, this, &BallisticCalculator::_rcChannelsChanged);
        disconnect(_vehicle, &Vehicle::coordinateChanged, this, &BallisticCalculator::_updateTrajectory);
        if (_vehicle->heading()) {
            disconnect(_vehicle->heading(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        }
        if (_vehicle->altitudeRelative()) {
            disconnect(_vehicle->altitudeRelative(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        }
        if (_vehicle->pitch()) {
            disconnect(_vehicle->pitch(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
        }
        if (_vehicle->roll()) {
            disconnect(_vehicle->roll(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
        }
    }

    // Устанавливаем новое транспортное средство
    _vehicle = vehicle;

    // Подключаем сигналы к новому транспортному средству, если оно существует
    if (_vehicle) {
        connect(_vehicle, &Vehicle::rcChannelsChanged, this, &BallisticCalculator::_rcChannelsChanged);
        connect(_vehicle, &Vehicle::coordinateChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->heading(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->altitudeRelative(), &Fact::valueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle->pitch(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
        connect(_vehicle->roll(), &Fact::valueChanged, this, &BallisticCalculator::_updateWindFromAttitude);
    }

    // Обновляем траекторию с новым транспортным средством
    _updateTrajectory();
} 