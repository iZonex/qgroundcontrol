import QGroundControl.Vehicle       1.0

Item {
    id: root

    BallisticTargetIndicator {
        id: ballisticTargetIndicator
        anchors.fill: parent
        visible: QGroundControl.settingsManager.ballisticCalculatorSettings.Enabled.rawValue
        targetPoint: QGroundControl.ballisticCalculator.targetPoint
        dropTime: QGroundControl.ballisticCalculator.dropTime
        isActive: QGroundControl.ballisticCalculator.isActive
    }
} 