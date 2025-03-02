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

                        FactCheckBox {
                            fact:           _enabled
                            Layout.fillWidth: true
                        }
                    }
                }

                // Параметры ветра
                QGCLabel {
                    text:       qsTr("Параметры ветра")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: windParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  windParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _windSpeed.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _windSpeed
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _windDirection.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _windDirection
                            width:          _valueFieldWidth
                        }
                    }
                }

                // Параметры груза
                QGCLabel {
                    text:       qsTr("Параметры груза")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: payloadParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  payloadParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _payloadMass.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _payloadMass
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _cameraOffset.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _cameraOffset
                            width:          _valueFieldWidth
                        }
                    }
                }

                // Коэффициенты сопротивления
                QGCLabel {
                    text:       qsTr("Коэффициенты сопротивления")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: dragCoefficientsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  dragCoefficientsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _verticalDragCoefficient.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _verticalDragCoefficient
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _horizontalDragCoefficient.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalDragCoefficient
                            width:          _valueFieldWidth
                        }
                    }
                }

                // Площади сечения
                QGCLabel {
                    text:       qsTr("Площади сечения")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: crossSectionalGrid.height + (_margins * 2)
                    Layout.preferredWidth:  crossSectionalGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _verticalCrossSection.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _verticalCrossSection
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _horizontalCrossSection.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalCrossSection
                            width:          _valueFieldWidth
                        }
                    }
                }

                // Параметры высоты
                QGCLabel {
                    text:       qsTr("Параметры высоты")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: heightParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  heightParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _heightMode.shortDescription
                            Layout.fillWidth: true
                        }
                        FactComboBox {
                            fact:           _heightMode
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _fixedHeight.shortDescription
                            visible:        _heightMode.rawValue === 0
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _fixedHeight
                            visible:        _heightMode.rawValue === 0
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _auxChannel.shortDescription
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxChannel
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _auxMinHeight.shortDescription
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMinHeight
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _auxMaxHeight.shortDescription
                            visible:        _heightMode.rawValue === 2
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMaxHeight
                            visible:        _heightMode.rawValue === 2
                            width:          _valueFieldWidth
                        }
                    }
                }

                // Параметры отображения
                QGCLabel {
                    text:       qsTr("Параметры отображения")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: displayOptionsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  displayOptionsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _showTrajectory.shortDescription
                            Layout.fillWidth: true
                        }
                        FactCheckBox {
                            fact:           _showTrajectory
                            Layout.fillWidth: true
                        }
                    }
                }

                // Калибровка OSD
                QGCLabel {
                    text:       qsTr("Калибровка OSD")
                    visible:    _enabled.value
                }
                Rectangle {
                    Layout.preferredHeight: calibrationGrid.height + (_margins * 2)
                    Layout.preferredWidth:  calibrationGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _enabled.value
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
                            text:           _calibrationMode.shortDescription
                            Layout.fillWidth: true
                        }
                        FactCheckBox {
                            fact:           _calibrationMode
                            Layout.fillWidth: true
                        }

                        QGCLabel {
                            text:           _calibrationStep.shortDescription
                            visible:        _calibrationMode.value
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _calibrationStep
                            visible:        _calibrationMode.value
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _markerSize.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerSize
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _markerOffsetX.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetX
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _markerOffsetY.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetY
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           _gimbalPitch.shortDescription
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _gimbalPitch
                            width:          _valueFieldWidth
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