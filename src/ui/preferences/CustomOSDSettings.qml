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
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts          1.2

import QGroundControl                       1.0
import QGroundControl.FactSystem            1.0
import QGroundControl.FactControls          1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import QGroundControl.Controllers           1.0
import QGroundControl.SettingsManager       1.0

Rectangle {
    id:                 _root
    color:              qgcPal.window
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    property Fact _showHelloWorld:          QGroundControl.settingsManager.customOSDSettings.showHelloWorld
    property Fact _showFlightInfo:          QGroundControl.settingsManager.customOSDSettings.showFlightInfo
    property Fact _position:                QGroundControl.settingsManager.customOSDSettings.position
    property Fact _opacity:                 QGroundControl.settingsManager.customOSDSettings.opacity

    property real   _labelWidth:            ScreenTools.defaultFontPixelWidth * 20
    property real   _valueWidth:            ScreenTools.defaultFontPixelWidth * 10
    property real   _margins:               ScreenTools.defaultFontPixelHeight

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    QGCFlickable {
        clip:               true
        anchors.fill:       parent
        contentHeight:      settingsColumn.height
        contentWidth:       settingsColumn.width

        Column {
            id:                         settingsColumn
            anchors.horizontalCenter:   parent.horizontalCenter
            spacing:                    _margins

            QGCLabel {
                id:             customOSDSectionLabel
                text:           qsTr("Кастомный OSD")
                font.family:    ScreenTools.demiboldFontFamily
            }

            Rectangle {
                Layout.preferredHeight: customOSDCol.height + (_margins * 2)
                Layout.preferredWidth:  customOSDCol.width + (_margins * 2)
                color:                  qgcPal.windowShade
                Layout.fillWidth:       true

                Column {
                    id:                         customOSDCol
                    anchors.margins:            _margins
                    anchors.top:                parent.top
                    anchors.horizontalCenter:   parent.horizontalCenter
                    spacing:                    _margins

                    GridLayout {
                        columns:        2
                        columnSpacing:  _margins
                        rowSpacing:     _margins

                        QGCLabel {
                            text:               qsTr("Показывать Hello World")
                            Layout.fillWidth:   true
                        }
                        FactCheckBox {
                            fact:               _showHelloWorld
                            Layout.alignment:   Qt.AlignHCenter
                        }

                        QGCLabel {
                            text:               qsTr("Показывать информацию о полете")
                            Layout.fillWidth:   true
                        }
                        FactCheckBox {
                            fact:               _showFlightInfo
                            Layout.alignment:   Qt.AlignHCenter
                        }

                        QGCLabel {
                            text:               qsTr("Позиция OSD")
                            Layout.fillWidth:   true
                        }
                        FactComboBox {
                            fact:               _position
                            indexModel:         false
                            Layout.alignment:   Qt.AlignHCenter
                            Layout.minimumWidth: ScreenTools.defaultFontPixelWidth * 15
                        }

                        QGCLabel {
                            text:               qsTr("Прозрачность OSD")
                            Layout.fillWidth:   true
                        }
                        FactTextField {
                            fact:               _opacity
                            Layout.alignment:   Qt.AlignHCenter
                            Layout.minimumWidth: ScreenTools.defaultFontPixelWidth * 10
                        }
                    }

                    Item {
                        width:  1
                        height: ScreenTools.defaultFontPixelHeight
                    }

                    QGCLabel {
                        text:       qsTr("Предпросмотр OSD")
                        font.bold:  true
                    }

                    Rectangle {
                        width:          ScreenTools.defaultFontPixelWidth * 40
                        height:         ScreenTools.defaultFontPixelHeight * 15
                        color:          qgcPal.window
                        border.color:   qgcPal.text
                        border.width:   1

                        CustomOSDPreview {
                            anchors.fill:       parent
                            anchors.margins:    _margins
                            showHelloWorld:     _showHelloWorld.rawValue
                            showFlightInfo:     _showFlightInfo.rawValue
                            position:           _position.rawValue
                            opacity:            _opacity.rawValue
                        }
                    }
                }
            }
        }
    }
} 