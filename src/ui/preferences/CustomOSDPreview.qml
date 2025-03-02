/****************************************************************************
 *
 * (c) 2009-2023 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.3
import QtQuick.Controls         1.2
import QtQuick.Layouts          1.2

import QGroundControl                       1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.Palette               1.0

// Компонент предпросмотра для OSD модуля
Rectangle {
    id:                     customOSDPreview
    color:                  "transparent"
    
    property bool   showHelloWorld:     true
    property bool   showFlightInfo:     true
    property int    position:           0
    property real   previewOpacity:     0.8
    
    property bool   _lightWidgetBorders: qgcPal.globalTheme === QGCPalette.Light
    
    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }
    
    Rectangle {
        id:                     osdPreview
        width:                  osdColumn.width + ScreenTools.defaultFontPixelWidth * 2
        height:                 osdColumn.height + ScreenTools.defaultFontPixelHeight
        color:                  qgcPal.window
        radius:                 ScreenTools.defaultFontPixelHeight / 2
        opacity:                previewOpacity
        
        // Позиционирование OSD в зависимости от настроек
        Component.onCompleted: {
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
        
        Column {
            id:                     osdColumn
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
                visible:                  showHelloWorld
                
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
                visible:                  showFlightInfo
                
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
                            text:           "123.4 м"
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
                            text:           "15.2 м/с"
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
                            text:           "Стабилизация"
                            color:          qgcPal.colorGreen
                            font.pointSize: ScreenTools.defaultFontPointSize
                            font.bold:      true
                        }
                    }
                }
            }
        }
    }
} 