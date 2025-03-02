/****************************************************************************
 *
 * (c) 2009-2023 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtQuick.Controls         2.4
import QtQuick.Layouts          1.12

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0

// Простой OSD модуль, который показывает кастомную информацию
Rectangle {
    id:                     customOSDRoot
    width:                  customOSDColumn.width + ScreenTools.defaultFontPixelWidth * 2
    height:                 customOSDColumn.height + ScreenTools.defaultFontPixelHeight
    color:                  qgcPal.window
    radius:                 ScreenTools.defaultFontPixelHeight / 2
    opacity:                _osdSettings ? _osdSettings.opacity.rawValue : 0.8
    
    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property bool   _lightWidgetBorders:    qgcPal.globalTheme === QGCPalette.Light
    property var    _osdSettings:           QGroundControl.settingsManager.customOSDSettings
    
    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }
    
    // Позиционирование OSD в зависимости от настроек
    Component.onCompleted: {
        if (_osdSettings) {
            var position = _osdSettings.position.rawValue
            switch(position) {
            case 0: // Верхний правый угол
                anchors.right = parent.right
                anchors.top = parent.top
                break
            case 1: // Верхний левый угол
                anchors.left = parent.left
                anchors.top = parent.top
                break
            case 2: // Нижний правый угол
                anchors.right = parent.right
                anchors.bottom = parent.bottom
                break
            case 3: // Нижний левый угол
                anchors.left = parent.left
                anchors.bottom = parent.bottom
                break
            case 4: // Центр
                anchors.centerIn = parent
                break
            }
        }
    }
    
    Column {
        id:                     customOSDColumn
        anchors.centerIn:       parent
        spacing:                ScreenTools.defaultFontPixelHeight / 2
        
        QGCLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text:                     "Кастомный OSD"
            color:                    qgcPal.text
            font.pointSize:           ScreenTools.defaultFontPointSize
            font.bold:                true
        }
        
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width:                    helloWorldLabel.width + ScreenTools.defaultFontPixelWidth * 2
            height:                   helloWorldLabel.height + ScreenTools.defaultFontPixelHeight
            color:                    qgcPal.windowShade
            border.color:             _lightWidgetBorders ? qgcPal.text : qgcPal.windowShade
            border.width:             1
            radius:                   ScreenTools.defaultFontPixelHeight / 4
            visible:                  _osdSettings ? _osdSettings.showHelloWorld.rawValue : true
            
            QGCLabel {
                id:                     helloWorldLabel
                anchors.centerIn:       parent
                text:                   "Hello World"
                color:                  qgcPal.text
                font.pointSize:         ScreenTools.defaultFontPointSize
            }
        }
        
        // Информация о полете
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width:                    flightInfoColumn.width + ScreenTools.defaultFontPixelWidth * 2
            height:                   flightInfoColumn.height + ScreenTools.defaultFontPixelHeight
            color:                    qgcPal.windowShade
            border.color:             _lightWidgetBorders ? qgcPal.text : qgcPal.windowShade
            border.width:             1
            radius:                   ScreenTools.defaultFontPixelHeight / 4
            visible:                  _activeVehicle && (_osdSettings ? _osdSettings.showFlightInfo.rawValue : true)
            
            Column {
                id:                     flightInfoColumn
                anchors.centerIn:       parent
                spacing:                ScreenTools.defaultFontPixelHeight / 4
                
                Row {
                    spacing:            ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text:           "Высота:"
                        color:          qgcPal.text
                        font.pointSize: ScreenTools.defaultFontPointSize
                    }
                    
                    QGCLabel {
                        text:           _activeVehicle ? _activeVehicle.altitudeRelative.toFixed(1) + " м" : ""
                        color:          qgcPal.text
                        font.pointSize: ScreenTools.defaultFontPointSize
                    }
                }
                
                Row {
                    spacing:            ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text:           "Скорость:"
                        color:          qgcPal.text
                        font.pointSize: ScreenTools.defaultFontPointSize
                    }
                    
                    QGCLabel {
                        text:           _activeVehicle ? _activeVehicle.groundSpeed.toFixed(1) + " м/с" : ""
                        color:          qgcPal.text
                        font.pointSize: ScreenTools.defaultFontPointSize
                    }
                }
                
                Row {
                    spacing:            ScreenTools.defaultFontPixelWidth
                    
                    QGCLabel {
                        text:           "Режим полета:"
                        color:          qgcPal.text
                        font.pointSize: ScreenTools.defaultFontPointSize
                    }
                    
                    QGCLabel {
                        text:           _activeVehicle ? _activeVehicle.flightMode : ""
                        color:          qgcPal.colorGreen
                        font.pointSize: ScreenTools.defaultFontPointSize
                        font.bold:      true
                    }
                }
            }
        }
    }
    
    // Таймер для обновления данных
    Timer {
        interval:   500  // Обновление каждые 500 мс
        running:    true
        repeat:     true
        onTriggered: {
            // Обновление данных происходит автоматически через привязки свойств
        }
    }
} 