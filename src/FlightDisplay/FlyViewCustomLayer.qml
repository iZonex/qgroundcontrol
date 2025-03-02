/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtQuick.Controls         2.4
import QtQuick.Dialogs          1.3
import QtQuick.Layouts          1.12

import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.BallisticCalculator 1.0

// To implement a custom overlay copy this code to your own control in your custom code source. Then override the
// FlyViewCustomLayer.qml resource with your own qml. See the custom example and documentation for details.
Item {
    id: _root

    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets:   _toolInsets // These are the insets for your custom overlay additions
    property var mapControl

    // since this file is a placeholder for the custom layer in a standard build, we will just pass through the parent insets
    QGCToolInsets {
        id:                     _toolInsets
        leftEdgeTopInset:       parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset:    parentToolInsets.leftEdgeCenterInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     parentToolInsets.topEdgeCenterInset
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset:   parentToolInsets.bottomEdgeRightInset
    }
    
    // Баллистический калькулятор - временное решение
    Rectangle {
        id: ballisticTargetIndicator
        anchors.fill: parent
        visible: QGroundControl.settingsManager.ballisticCalculatorSettings.Enabled.rawValue
        color: "transparent"
        
        // Прицельная метка
        Rectangle {
            id: crosshair
            width: 32
            height: width
            color: "transparent"
            border.color: "red"
            border.width: 2
            radius: width / 2
            anchors.centerIn: parent
            
            // Вертикальная линия
            Rectangle {
                width: 2
                height: parent.height * 0.6
                color: "red"
                anchors.centerIn: parent
            }
            
            // Горизонтальная линия
            Rectangle {
                width: parent.width * 0.6
                height: 2
                color: "red"
                anchors.centerIn: parent
            }
        }
        
        // Информация о точке падения
        Column {
            anchors {
                left: crosshair.right
                leftMargin: 10
                verticalCenter: crosshair.verticalCenter
            }
            spacing: 5
            
            Text {
                text: "Баллистический калькулятор"
                color: "white"
                font.pixelSize: 14
            }
            
            Text {
                text: "Активен"
                color: "green"
                font.pixelSize: 14
            }
        }
    }
    
    // Оригинальный код, закомментирован
    /*
    BallisticTargetIndicator {
        anchors.fill: parent
        visible: QGroundControl.settingsManager.ballisticCalculatorSettings.Enabled.rawValue
        targetPoint: QGroundControl.ballisticCalculator.targetPoint
        dropTime: QGroundControl.ballisticCalculator.dropTime
        isActive: QGroundControl.ballisticCalculator.isActive
    }
    */
}
