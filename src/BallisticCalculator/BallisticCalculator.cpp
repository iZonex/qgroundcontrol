QVariantList BallisticCalculator::calculateTrajectoryPoints()
{
    QVariantList points;
    if (!_vehicle || !_ballisticSettings) return points;

    const double g = 9.81;  // ускорение свободного падения
    const double dt = 0.1;  // шаг по времени
    const double mass = _ballisticSettings->mass()->rawValue().toDouble();
    const double cd = _ballisticSettings->dragCoefficient()->rawValue().toDouble();
    const double area = _ballisticSettings->crossSectionArea()->rawValue().toDouble();
    const double rho = 1.225;  // плотность воздуха на уровне моря

    // Начальные условия
    double x = 0;
    double y = 0;
    double z = _vehicle->altitudeRelative()->rawValue().toDouble();
    double vx = 0;
    double vy = 0;
    double vz = 0;

    // Получаем компоненты ветра
    double windSpeed = _ballisticSettings->windSpeed()->rawValue().toDouble();
    double windDirection = _ballisticSettings->windDirection()->rawValue().toDouble() * M_PI / 180.0;
    double windX = windSpeed * cos(windDirection);
    double windY = windSpeed * sin(windDirection);

    while (z > 0) {
        // Относительная скорость с учетом ветра
        double vrx = vx - windX;
        double vry = vy - windY;
        double vrz = vz;
        double vr = sqrt(vrx*vrx + vry*vry + vrz*vrz);

        // Сила сопротивления воздуха
        double fd = 0.5 * rho * cd * area * vr * vr;
        
        // Компоненты ускорения
        double ax = -fd * vrx / (mass * vr);
        double ay = -fd * vry / (mass * vr);
        double az = -g - fd * vrz / (mass * vr);

        // Обновляем скорости
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;

        // Обновляем координаты
        x += vx * dt;
        y += vy * dt;
        z += vz * dt;

        // Добавляем точку в список
        QVariantMap point;
        point["x"] = x;
        point["y"] = y;
        point["z"] = z;
        points.append(point);
    }

    return points;
}

void BallisticCalculator::_updateTrajectory()
{
    if (!_vehicle || !_ballisticSettings) return;

    // Обновляем ветер на основе положения дрона
    _updateWindFromAttitude();

    // Рассчитываем точки траектории
    QVariantList trajectoryPoints = calculateTrajectoryPoints();
    emit trajectoryPointsChanged(trajectoryPoints);

    // Рассчитываем точку падения
    if (!trajectoryPoints.isEmpty()) {
        QVariantMap lastPoint = trajectoryPoints.last().toMap();
        QGeoCoordinate currentPos = _vehicle->coordinate();
        
        // Преобразуем смещение в метрах в координаты
        double metersPerLat = 111319.9;  // примерное количество метров в одном градусе широты
        double metersPerLon = metersPerLat * cos(currentPos.latitude() * M_PI / 180.0);
        
        QGeoCoordinate targetPos(
            currentPos.latitude() + lastPoint["y"].toDouble() / metersPerLat,
            currentPos.longitude() + lastPoint["x"].toDouble() / metersPerLon,
            0  // высота над уровнем моря
        );
        
        emit targetPointChanged(targetPos);
        
        // Обновляем время падения
        double dropTime = trajectoryPoints.size() * 0.1;  // dt = 0.1
        emit dropTimeChanged(dropTime);
    }
} 