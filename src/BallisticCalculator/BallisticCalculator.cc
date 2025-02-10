#include "BallisticCalculator.h"
#include "QGCApplication.h"
#include "Settings/SettingsManager.h"
#include <cmath>

BallisticCalculator::BallisticCalculator(Vehicle* vehicle, QObject* parent)
    : QObject(parent)
    , _vehicle(vehicle)
    , _ballisticSettings(qgcApp()->toolbox()->settingsManager()->ballisticCalculatorSettings())
    , _dropTime(0.0)
    , _isActive(false)
{
    if (_vehicle) {
        connect(_vehicle, &Vehicle::rcChannelsChanged, this, &BallisticCalculator::_rcChannelsChanged);
        connect(_vehicle, &Vehicle::coordinateChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle, &Vehicle::headingChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_vehicle, &Vehicle::altitudeRelativeChanged, this, &BallisticCalculator::_updateTrajectory);
        
        // Подключаемся к изменениям углов наклона
        connect(_vehicle, &Vehicle::pitchChanged, this, &BallisticCalculator::_updateWindFromAttitude);
        connect(_vehicle, &Vehicle::rollChanged, this, &BallisticCalculator::_updateWindFromAttitude);
    }

    if (_ballisticSettings) {
        connect(_ballisticSettings->windSpeedFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->windDirectionFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->dropHeightFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->payloadMassFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->dragCoefficientFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->crossSectionAreaFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
        connect(_ballisticSettings->gimbalPitchFact(), &Fact::rawValueChanged, this, &BallisticCalculator::_updateTrajectory);
    }
}

BallisticCalculator::~BallisticCalculator()
{
    // Disconnect all signals
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
    auto* settingsManager = qgcApp()->toolbox()->settingsManager();
    auto* ballisticSettings = settingsManager->ballisticCalculatorSettings();

    // Получаем текущие параметры
    double mass = ballisticSettings->payloadMassFact()->rawValue().toDouble() / 1000.0; // конвертируем граммы в кг
    double vertDragCoef = ballisticSettings->verticalDragCoefficientFact()->rawValue().toDouble();
    double horizDragCoef = ballisticSettings->horizontalDragCoefficientFact()->rawValue().toDouble();
    double vertCrossSection = ballisticSettings->verticalCrossSectionFact()->rawValue().toDouble() / 10000.0; // конвертируем см² в м²
    double horizCrossSection = ballisticSettings->horizontalCrossSectionFact()->rawValue().toDouble() / 10000.0;

    // Получаем текущие параметры полета
    double altitude;
    switch (ballisticSettings->heightModeFact()->rawValue().toInt()) {
        case BallisticCalculatorSettings::HeightModeFixed:
            altitude = ballisticSettings->fixedHeightFact()->rawValue().toDouble();
            break;
        case BallisticCalculatorSettings::HeightModeBarometric:
            altitude = _vehicle->altitudeRelative()->rawValue().toDouble();
            break;
        case BallisticCalculatorSettings::HeightModeRemote:
            // TODO: Реализовать получение высоты с пульта
            altitude = _vehicle->altitudeRelative()->rawValue().toDouble();
            break;
        default:
            altitude = _vehicle->altitudeRelative()->rawValue().toDouble();
    }

    // Получаем данные о ветре из настроек
    double windSpeed = ballisticSettings->windSpeedFact()->rawValue().toDouble();
    double windDirection = ballisticSettings->windDirectionFact()->rawValue().toDouble() * M_PI / 180.0; // конвертируем градусы в радианы

    // Рассчитываем точку падения
    QGeoCoordinate vehiclePosition = _vehicle->coordinate();
    QGeoCoordinate newTargetPoint = calculateImpactPoint(vehiclePosition, altitude, windSpeed, windDirection);

    if (_targetPoint != newTargetPoint) {
        _targetPoint = newTargetPoint;
        emit targetPointChanged();
    }
}

QGeoCoordinate BallisticCalculator::calculateImpactPoint(const QGeoCoordinate& vehiclePosition,
                                                        double altitude,
                                                        double windSpeed,
                                                        double windDirection)
{
    auto* ballisticSettings = qgcApp()->toolbox()->settingsManager()->ballisticCalculatorSettings();
    
    // Параметры груза
    double mass = ballisticSettings->payloadMassFact()->rawValue().toDouble() / 1000.0;
    double vertDragCoef = ballisticSettings->verticalDragCoefficientFact()->rawValue().toDouble();
    double horizDragCoef = ballisticSettings->horizontalDragCoefficientFact()->rawValue().toDouble();
    double vertCrossSection = ballisticSettings->verticalCrossSectionFact()->rawValue().toDouble() / 10000.0;
    double horizCrossSection = ballisticSettings->horizontalCrossSectionFact()->rawValue().toDouble() / 10000.0;

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

    emit dropTimeChanged();

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

void BallisticCalculator::_rcChannelsChanged(int channelCount, int pwmValues[])
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    int auxChannel = _ballisticSettings->auxChannelFact()->rawValue().toInt() - 1; // Convert from 1-based to 0-based
    if (auxChannel >= 0 && auxChannel < channelCount) {
        int pwmValue = pwmValues[auxChannel];
        // Convert PWM (1000-2000) to height range
        double minHeight = _ballisticSettings->auxMinHeightFact()->rawValue().toDouble();
        double maxHeight = _ballisticSettings->auxMaxHeightFact()->rawValue().toDouble();
        double normalizedPwm = (pwmValue - 1000.0) / 1000.0; // 0.0 to 1.0
        double height = minHeight + normalizedPwm * (maxHeight - minHeight);
        
        _ballisticSettings->dropHeightFact()->setRawValue(height);
    }
}

void BallisticCalculator::_updateWindFromAttitude()
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    // Получаем углы наклона дрона
    double pitch = _vehicle->pitch()->rawValue().toDouble() * M_PI / 180.0;
    double roll = _vehicle->roll()->rawValue().toDouble() * M_PI / 180.0;
    
    // Примерное соотношение: 1 градус наклона = 1 м/с ветра
    // Отрицательный pitch означает наклон вперед, поэтому ветер дует спереди
    double windX = -pitch * (180.0 / M_PI); // конвертируем обратно в м/с
    double windY = -roll * (180.0 / M_PI);  // правый крен означает ветер слева
    
    // Рассчитываем скорость и направление ветра
    double windSpeed = sqrt(windX * windX + windY * windY);
    double windDirection = atan2(windY, windX) * 180.0 / M_PI;
    
    // Нормализуем направление ветра к 0-360°
    if (windDirection < 0) {
        windDirection += 360.0;
    }
    
    // Обновляем значения в настройках
    _ballisticSettings->windSpeedFact()->setRawValue(windSpeed);
    _ballisticSettings->windDirectionFact()->setRawValue(windDirection);
}

void BallisticCalculator::_updateTrajectory()
{
    if (!_vehicle || !_ballisticSettings) {
        return;
    }

    // Обновляем данные о ветре на основе углов наклона дрона
    _updateWindFromAttitude();

    // Get current vehicle position and attitude
    QGeoCoordinate vehiclePosition = _vehicle->coordinate();
    double vehicleAltitude = _vehicle->altitudeRelative()->rawValue().toDouble();
    double vehicleHeading = _vehicle->heading()->rawValue().toDouble();
    
    // Get wind parameters (теперь они обновляются автоматически из углов наклона)
    double windSpeed = _ballisticSettings->windSpeedFact()->rawValue().toDouble();
    double windDirection = _ballisticSettings->windDirectionFact()->rawValue().toDouble() * M_PI / 180.0;
    
    // Get drop height (either from AUX channel or fixed setting)
    double dropHeight = _ballisticSettings->dropHeightFact()->rawValue().toDouble();
    
    // Get gimbal pitch if available
    double gimbalPitch = _ballisticSettings->gimbalPitchFact()->rawValue().toDouble() * M_PI / 180.0;
    
    // Physical parameters
    double mass = _ballisticSettings->payloadMassFact()->rawValue().toDouble();
    double dragCoeff = _ballisticSettings->dragCoefficientFact()->rawValue().toDouble();
    double area = _ballisticSettings->crossSectionAreaFact()->rawValue().toDouble();
    
    // Initial conditions
    double x = 0.0;
    double y = 0.0;
    double z = dropHeight;
    double vx = 0.0;
    double vy = 0.0;
    double vz = 0.0;
    
    // Air density (approximate at sea level)
    const double rho = 1.225; // kg/m^3
    
    // Time step
    const double dt = 0.01;
    double totalTime = 0.0;
    
    // Integrate trajectory using Euler method
    while (z > 0.0) {
        // Relative velocity components including wind
        double vrx = vx - windSpeed * cos(windDirection);
        double vry = vy - windSpeed * sin(windDirection);
        double vrz = vz;
        
        // Total relative velocity
        double vr = sqrt(vrx*vrx + vry*vry + vrz*vrz);
        
        // Drag force
        double fd = 0.5 * rho * dragCoeff * area * vr * vr;
        
        // Acceleration components
        double ax = -fd * vrx / (mass * vr);
        double ay = -fd * vry / (mass * vr);
        double az = -9.81 - fd * vrz / (mass * vr);
        
        // Update velocities
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;
        
        // Update positions
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;
        
        totalTime += dt;
    }
    
    // Calculate impact point coordinates
    double distance = sqrt(x*x + y*y);
    double bearing = atan2(y, x) + vehicleHeading * M_PI / 180.0;
    
    QGeoCoordinate impactPoint = vehiclePosition.atDistanceAndAzimuth(distance, bearing * 180.0 / M_PI);
    
    // Update UI
    emit targetPointChanged(impactPoint);
    emit dropTimeChanged(totalTime);
} 