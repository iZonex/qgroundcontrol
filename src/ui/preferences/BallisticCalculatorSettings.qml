/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.3
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
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

    property var    _ballisticCalculatorSettings:  QGroundControl.settingsManager.ballisticCalculatorSettings
    property Fact   _enabled:                      _ballisticCalculatorSettings.enabled
    property Fact   _payloadMass:                  _ballisticCalculatorSettings.payloadMass
    property Fact   _verticalDragCoefficient:      _ballisticCalculatorSettings.verticalDragCoefficient
    property Fact   _horizontalDragCoefficient:    _ballisticCalculatorSettings.horizontalDragCoefficient
    property Fact   _verticalCrossSection:         _ballisticCalculatorSettings.verticalCrossSection
    property Fact   _horizontalCrossSection:       _ballisticCalculatorSettings.horizontalCrossSection
    property Fact   _windSpeed:                    _ballisticCalculatorSettings.windSpeed
    property Fact   _windDirection:                _ballisticCalculatorSettings.windDirection
    property Fact   _heightMode:                   _ballisticCalculatorSettings.heightMode
    property Fact   _fixedHeight:                  _ballisticCalculatorSettings.fixedHeight
    property Fact   _auxChannel:                   _ballisticCalculatorSettings.auxChannel
    property Fact   _auxMinHeight:                 _ballisticCalculatorSettings.auxMinHeight
    property Fact   _auxMaxHeight:                 _ballisticCalculatorSettings.auxMaxHeight
    property Fact   _cameraOffset:                 _ballisticCalculatorSettings.cameraOffset
    property Fact   _showTrajectory:               _ballisticCalculatorSettings.showTrajectory
    property Fact   _calibrationMode:              _ballisticCalculatorSettings.calibrationMode
    property Fact   _calibrationStep:              _ballisticCalculatorSettings.calibrationStep
    property Fact   _markerSize:                   _ballisticCalculatorSettings.markerSize
    property Fact   _markerOffsetX:                _ballisticCalculatorSettings.markerOffsetX
    property Fact   _markerOffsetY:                _ballisticCalculatorSettings.markerOffsetY
    property Fact   _gimbalPitch:                  _ballisticCalculatorSettings.gimbalPitch

    property real   _labelWidth:                   ScreenTools.defaultFontPixelWidth * 20
    property real   _valueFieldWidth:              ScreenTools.defaultFontPixelWidth * 20
    property real   _margins:                      ScreenTools.defaultFontPixelWidth

    readonly property real _internalWidthRatio:    0.8

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    QGCFlickable {
        clip:               true
        anchors.fill:       parent
        contentHeight:      outerItem.height
        contentWidth:       outerItem.width

        Item {
            id:     outerItem
            width:  Math.max(_root.width, settingsColumn.width)
            height: settingsColumn.height

            ColumnLayout {
                id:                         settingsColumn
                anchors.horizontalCenter:   parent.horizontalCenter
                spacing:                    _margins

                QGCLabel {
                    id:         ballisticCalculatorLabel
                    text:       qsTr("Баллистический калькулятор")
                    font.family: ScreenTools.demiboldFontFamily
                }

                Rectangle {
                    Layout.preferredHeight: enabledRow.height + (_margins * 2)
                    Layout.preferredWidth:  enabledRow.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    Layout.fillWidth:       true

                    RowLayout {
                        id:                         enabledRow
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        spacing:                    _margins

                        QGCLabel {
                            text:           qsTr("Включить баллистический калькулятор")
                            font.bold:      true
                        }

                        QGCCheckBox {
                            checked:        _enabled.rawValue
                            onClicked: {
                                _enabled.rawValue = checked
                            }
                        }
                    }
                }

                // Параметры ветра
                QGCLabel {
                    text:       qsTr("Параметры ветра")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: windParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  windParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         windParamsGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Скорость ветра (м/с)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _windSpeed
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите скорость ветра")
                        }

                        QGCLabel {
                            text:           qsTr("Направление ветра (градусы)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _windDirection
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите направление ветра (0-360)")
                        }
                    }
                }

                // Параметры груза
                QGCLabel {
                    text:       qsTr("Параметры груза")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: payloadParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  payloadParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         payloadParamsGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Масса груза (кг)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _payloadMass
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите массу груза")
                        }

                        QGCLabel {
                            text:           qsTr("Смещение от камеры (м)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _cameraOffset
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение от камеры")
                        }
                    }
                }

                // Коэффициенты сопротивления
                QGCLabel {
                    text:       qsTr("Коэффициенты сопротивления")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: dragCoefficientsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  dragCoefficientsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         dragCoefficientsGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Вертикальный коэффициент сопротивления")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _verticalDragCoefficient
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите вертикальный коэффициент")
                        }

                        QGCLabel {
                            text:           qsTr("Горизонтальный коэффициент сопротивления")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalDragCoefficient
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите горизонтальный коэффициент")
                        }
                    }
                }

                // Площади сечения
                QGCLabel {
                    text:       qsTr("Площади сечения")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: crossSectionalGrid.height + (_margins * 2)
                    Layout.preferredWidth:  crossSectionalGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         crossSectionalGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Вертикальная площадь сечения (м²)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _verticalCrossSection
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите вертикальную площадь")
                        }

                        QGCLabel {
                            text:           qsTr("Горизонтальная площадь сечения (м²)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalCrossSection
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите горизонтальную площадь")
                        }
                    }
                }

                // Параметры высоты
                QGCLabel {
                    text:       qsTr("Параметры высоты")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: heightParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  heightParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         heightParamsGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Режим определения высоты")
                            Layout.fillWidth: true
                        }
                        QGCComboBox {
                            id:             heightModeCombo
                            model:          [qsTr("Фиксированная высота"), qsTr("Высота от барометра"), qsTr("Высота от AUX канала")]
                            currentIndex:   _heightMode.rawValue
                            onActivated: {
                                _heightMode.rawValue = index
                            }
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           qsTr("Фиксированная высота (м)")
                            visible:        _heightMode.rawValue === 0
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _fixedHeight
                            visible:        _heightMode.rawValue === 0
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите фиксированную высоту")
                        }

                        QGCLabel {
                            text:           qsTr("Номер AUX канала")
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxChannel
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите номер канала")
                        }

                        QGCLabel {
                            text:           qsTr("Минимальная высота AUX (м)")
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMinHeight
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите минимальную высоту")
                        }

                        QGCLabel {
                            text:           qsTr("Максимальная высота AUX (м)")
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMaxHeight
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите максимальную высоту")
                        }
                    }
                }

                // Параметры отображения
                QGCLabel {
                    text:       qsTr("Параметры отображения")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: displayOptionsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  displayOptionsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         displayOptionsGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Показывать траекторию")
                            Layout.fillWidth: true
                        }
                        QGCCheckBox {
                            checked:        _showTrajectory.rawValue
                            onClicked: {
                                _showTrajectory.rawValue = checked
                            }
                        }
                    }
                }

                // Калибровка OSD
                QGCLabel {
                    text:       qsTr("Калибровка OSD")
                    visible:    _enabled.rawValue
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: calibrationGrid.height + (_margins * 2)
                    Layout.preferredWidth:  calibrationGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.rawValue
                    Layout.fillWidth:       true

                    GridLayout {
                        id:                         calibrationGrid
                        anchors.margins:            _margins
                        anchors.top:                parent.top
                        anchors.horizontalCenter:   parent.horizontalCenter
                        columns:                    2
                        rowSpacing:                 _margins
                        columnSpacing:              _margins

                        QGCLabel {
                            text:           qsTr("Режим калибровки")
                            Layout.fillWidth: true
                        }
                        QGCCheckBox {
                            id:             calibrationModeCheckbox
                            checked:        _calibrationMode.rawValue
                            onClicked: {
                                _calibrationMode.rawValue = checked
                            }
                        }

                        QGCLabel {
                            text:           qsTr("Шаг калибровки (пиксели)")
                            visible:        calibrationModeCheckbox.checked
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _calibrationStep
                            visible:        calibrationModeCheckbox.checked
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите шаг калибровки")
                        }

                        QGCLabel {
                            text:           qsTr("Размер маркера (пиксели)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerSize
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите размер маркера")
                        }

                        QGCLabel {
                            text:           qsTr("Смещение маркера по X (пиксели)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetX
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение по X")
                        }

                        QGCLabel {
                            text:           qsTr("Смещение маркера по Y (пиксели)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetY
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение по Y")
                        }

                        QGCLabel {
                            text:           qsTr("Наклон подвеса (градусы)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _gimbalPitch
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите наклон подвеса")
                        }

                        QGCButton {
                            text:           qsTr("Сброс калибровки")
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            onClicked: {
                                _markerSize.rawValue = 32
                                _markerOffsetX.rawValue = 0
                                _markerOffsetY.rawValue = 0
                                _gimbalPitch.rawValue = 0
                            }
                        }
                    }
                }
            }
        }
    }
} 