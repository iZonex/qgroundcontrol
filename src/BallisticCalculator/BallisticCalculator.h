#pragma once

#include <QObject>
#include <QGeoCoordinate>
#include <QVariantList>
class Vehicle;
#include "BallisticCalculatorSettings.h"

class BallisticCalculator : public QObject
{
    Q_OBJECT

public:
    explicit BallisticCalculator(Vehicle* vehicle, QObject* parent = nullptr);
    ~BallisticCalculator();

    Q_PROPERTY(QGeoCoordinate targetPoint READ targetPoint NOTIFY targetPointChanged)
    Q_PROPERTY(double dropTime READ dropTime NOTIFY dropTimeChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)

    QGeoCoordinate targetPoint() const { return _targetPoint; }
    double dropTime() const { return _dropTime; }
    bool isActive() const { return _isActive; }

public slots:
    void setIsActive(bool active);
    void updateCalculations();

signals:
    void targetPointChanged(QGeoCoordinate point);
    void dropTimeChanged(double time);
    void trajectoryPointsChanged(QVariantList points);
    void isActiveChanged(bool active);

private slots:
    void _updateTrajectory();
    void _updateWindFromAttitude();
    void _rcChannelsChanged(int channelCount, int pwmValues[]);

private:
    void calculateTrajectory();
    QGeoCoordinate calculateImpactPoint(const QGeoCoordinate& vehiclePosition,
                                      double altitude,
                                      double windSpeed,
                                      double windDirection);

    QVariantList calculateTrajectoryPoints();
    Vehicle* _vehicle;
    BallisticCalculatorSettings* _ballisticSettings;
    QGeoCoordinate _targetPoint;
    double _dropTime;
    bool _isActive;

    static constexpr double GRAVITY = 9.81;  // м/с²
    static constexpr double AIR_DENSITY = 1.225;  // кг/м³
}; 