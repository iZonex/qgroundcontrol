import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.FactSystem   1.0
import QGroundControl.FactControls 1.0
import QGroundControl.Controls     1.0
import QGroundControl.ScreenTools  1.0

Item {
    id: root
    
    property var qgcView: null

    Column {
        spacing: ScreenTools.defaultFontPixelHeight

        QGCLabel {
            text: qsTr("Баллистический калькулятор")
            font.family: ScreenTools.demiboldFontFamily
        }

        Rectangle {
            height: ballisticColumn.height + (ScreenTools.defaultFontPixelHeight * 2)
            width: parent.width
            color: qgcPal.windowShade

            Column {
                id: ballisticColumn
                anchors.margins: ScreenTools.defaultFontPixelHeight
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: ScreenTools.defaultFontPixelHeight

                // Параметры ветра
                QGCLabel {
                    text: qsTr("Параметры ветра")
                    font.bold: true
                }

                GridLayout {
                    columns: 2
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    rowSpacing: ScreenTools.defaultFontPixelHeight / 2
                    width: parent.width

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.windSpeed
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.windDirection
                        Layout.fillWidth: true
                    }
                }

                // Параметры груза
                QGCLabel {
                    text: qsTr("Параметры груза")
                    font.bold: true
                }

                // Масса груза
                FactTextField {
                    fact: QGroundControl.settingsManager.ballisticCalculatorSettings.payloadMass
                    width: parent.width
                }

                // Коэффициенты сопротивления
                GridLayout {
                    columns: 2
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    rowSpacing: ScreenTools.defaultFontPixelHeight / 2
                    width: parent.width

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.verticalDragCoefficient
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.horizontalDragCoefficient
                        Layout.fillWidth: true
                    }
                }

                // Площади сечения
                GridLayout {
                    columns: 2
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    rowSpacing: ScreenTools.defaultFontPixelHeight / 2
                    width: parent.width

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.verticalCrossSection
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.horizontalCrossSection
                        Layout.fillWidth: true
                    }
                }

                // Смещение от камеры
                FactTextField {
                    fact: QGroundControl.settingsManager.ballisticCalculatorSettings.cameraOffset
                    width: parent.width
                }

                // Параметры высоты
                QGCLabel {
                    text: qsTr("Параметры высоты")
                    font.bold: true
                }

                // Режим определения высоты
                FactComboBox {
                    fact: QGroundControl.settingsManager.ballisticCalculatorSettings.heightMode
                    width: parent.width
                }

                // Фиксированная высота
                FactTextField {
                    fact: QGroundControl.settingsManager.ballisticCalculatorSettings.fixedHeight
                    width: parent.width
                    visible: QGroundControl.settingsManager.ballisticCalculatorSettings.heightMode.rawValue === 0
                }

                // Настройки AUX канала
                Column {
                    width: parent.width
                    visible: QGroundControl.settingsManager.ballisticCalculatorSettings.heightMode.rawValue === 2
                    spacing: ScreenTools.defaultFontPixelHeight / 2

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.auxChannel
                        width: parent.width
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.auxMinHeight
                        width: parent.width
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.auxMaxHeight
                        width: parent.width
                    }
                }

                // Калибровка OSD
                QGCLabel {
                    text: qsTr("Калибровка OSD")
                    font.bold: true
                }

                GridLayout {
                    columns: 2
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    rowSpacing: ScreenTools.defaultFontPixelHeight / 2
                    width: parent.width

                    QGCCheckBox {
                        id: calibrationModeCheckbox
                        text: qsTr("Режим калибровки")
                        checked: QGroundControl.settingsManager.ballisticCalculatorSettings.calibrationMode.rawValue
                        onClicked: {
                            QGroundControl.settingsManager.ballisticCalculatorSettings.calibrationMode.rawValue = checked
                        }
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.calibrationStep
                        Layout.fillWidth: true
                        enabled: calibrationModeCheckbox.checked
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.markerSize
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetX
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetY
                        Layout.fillWidth: true
                    }

                    FactTextField {
                        fact: QGroundControl.settingsManager.ballisticCalculatorSettings.gimbalPitch
                        Layout.fillWidth: true
                    }

                    // Кнопка сброса калибровки
                    QGCButton {
                        text: qsTr("Сброс калибровки")
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        onClicked: {
                            QGroundControl.settingsManager.ballisticCalculatorSettings.markerSize.rawValue = 32
                            QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetX.rawValue = 0
                            QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetY.rawValue = 0
                            QGroundControl.settingsManager.ballisticCalculatorSettings.gimbalPitch.rawValue = 0
                        }
                    }
                }
            }
        }
    }
} 