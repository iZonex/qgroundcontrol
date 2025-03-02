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

    // Проверяем наличие настроек баллистического калькулятора
    property bool   _settingsAvailable:         QGroundControl.settingsManager && 
                                               QGroundControl.settingsManager.ballisticCalculatorSettings
    property var    _ballisticCalculatorSettings: _settingsAvailable ? 
                                               QGroundControl.settingsManager.ballisticCalculatorSettings : null
    
    // Безопасное получение Fact объектов с проверкой на null и использованием правильных имен свойств
    property Fact   _enabled:                  _settingsAvailable && _ballisticCalculatorSettings.enabled ? 
                                               _ballisticCalculatorSettings.enabled : null
    property Fact   _payloadMass:              _settingsAvailable && _ballisticCalculatorSettings.payloadMass ? 
                                               _ballisticCalculatorSettings.payloadMass : null
    property Fact   _verticalDragCoefficient:  _settingsAvailable && _ballisticCalculatorSettings.verticalDragCoefficient ? 
                                               _ballisticCalculatorSettings.verticalDragCoefficient : null
    property Fact   _horizontalDragCoefficient: _settingsAvailable && _ballisticCalculatorSettings.horizontalDragCoefficient ? 
                                               _ballisticCalculatorSettings.horizontalDragCoefficient : null
    property Fact   _verticalCrossSection:     _settingsAvailable && _ballisticCalculatorSettings.verticalCrossSection ? 
                                               _ballisticCalculatorSettings.verticalCrossSection : null
    property Fact   _horizontalCrossSection:   _settingsAvailable && _ballisticCalculatorSettings.horizontalCrossSection ? 
                                               _ballisticCalculatorSettings.horizontalCrossSection : null
    property Fact   _windSpeed:                _settingsAvailable && _ballisticCalculatorSettings.windSpeed ? 
                                               _ballisticCalculatorSettings.windSpeed : null
    property Fact   _windDirection:            _settingsAvailable && _ballisticCalculatorSettings.windDirection ? 
                                               _ballisticCalculatorSettings.windDirection : null
    // Используем DropHeight вместо HeightMode, так как HeightMode не объявлен в BallisticCalculatorSettings.cc
    property int    _heightMode:               0  // Значение по умолчанию - фиксированная высота
    property Fact   _fixedHeight:              _settingsAvailable && _ballisticCalculatorSettings.dropHeight ? 
                                               _ballisticCalculatorSettings.dropHeight : null
    property Fact   _auxChannel:               _settingsAvailable && _ballisticCalculatorSettings.auxChannel ? 
                                               _ballisticCalculatorSettings.auxChannel : null
    property Fact   _auxMinHeight:             _settingsAvailable && _ballisticCalculatorSettings.auxMinHeight ? 
                                               _ballisticCalculatorSettings.auxMinHeight : null
    property Fact   _auxMaxHeight:             _settingsAvailable && _ballisticCalculatorSettings.auxMaxHeight ? 
                                               _ballisticCalculatorSettings.auxMaxHeight : null
    property Fact   _cameraOffset:             _settingsAvailable && _ballisticCalculatorSettings.cameraOffset ? 
                                               _ballisticCalculatorSettings.cameraOffset : null
    property Fact   _showTrajectory:           _settingsAvailable && _ballisticCalculatorSettings.showTrajectory ? 
                                               _ballisticCalculatorSettings.showTrajectory : null
    property Fact   _calibrationMode:          _settingsAvailable && _ballisticCalculatorSettings.calibrationMode ? 
                                               _ballisticCalculatorSettings.calibrationMode : null
    property Fact   _calibrationStep:          _settingsAvailable && _ballisticCalculatorSettings.calibrationStep ? 
                                               _ballisticCalculatorSettings.calibrationStep : null
    property Fact   _markerSize:               _settingsAvailable && _ballisticCalculatorSettings.markerSize ? 
                                               _ballisticCalculatorSettings.markerSize : null
    property Fact   _markerOffsetX:            _settingsAvailable && _ballisticCalculatorSettings.markerOffsetX ? 
                                               _ballisticCalculatorSettings.markerOffsetX : null
    property Fact   _markerOffsetY:            _settingsAvailable && _ballisticCalculatorSettings.markerOffsetY ? 
                                               _ballisticCalculatorSettings.markerOffsetY : null
    property Fact   _gimbalPitch:              _settingsAvailable && _ballisticCalculatorSettings.gimbalPitch ? 
                                               _ballisticCalculatorSettings.gimbalPitch : null

    property real   _labelWidth:                   ScreenTools.defaultFontPixelWidth * 20
    property real   _valueFieldWidth:              ScreenTools.defaultFontPixelWidth * 20
    property real   _margins:                      ScreenTools.defaultFontPixelWidth

    readonly property real _internalWidthRatio:    0.8

    // Константы для режимов высоты (из BallisticCalculatorSettings.h)
    readonly property int HeightModeFixed: 0
    readonly property int HeightModeBarometric: 1
    readonly property int HeightModeRemote: 2

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    // Функция для безопасного получения значения
    function safeRawValue(fact, defaultValue) {
        return fact ? fact.rawValue : defaultValue;
    }

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

                // Сообщение об ошибке, если настройки недоступны
                QGCLabel {
                    text:       qsTr("Ошибка: Настройки баллистического калькулятора недоступны")
                    color:      "red"
                    visible:    !_settingsAvailable
                }

                Rectangle {
                    Layout.preferredHeight: enabledRow.height + (_margins * 2)
                    Layout.preferredWidth:  enabledRow.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    Layout.fillWidth:       true
                    visible:                _settingsAvailable

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
                            checked:        _enabled ? _enabled.rawValue : false
                            onClicked: {
                                if (_enabled) {
                                    _enabled.rawValue = checked
                                }
                            }
                        }
                    }
                }

                // Параметры ветра
                QGCLabel {
                    text:       qsTr("Параметры ветра")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: windParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  windParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            visible:        _windSpeed !== null
                        }

                        QGCLabel {
                            text:           qsTr("Направление ветра (градусы)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _windDirection
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите направление ветра (0-360)")
                            visible:        _windDirection !== null
                        }
                    }
                }

                // Параметры груза
                QGCLabel {
                    text:       qsTr("Параметры груза")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: payloadParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  payloadParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            visible:        _payloadMass !== null
                        }

                        QGCLabel {
                            text:           qsTr("Смещение от камеры (м)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _cameraOffset
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение от камеры")
                            visible:        _cameraOffset !== null
                        }
                    }
                }

                // Коэффициенты сопротивления
                QGCLabel {
                    text:       qsTr("Коэффициенты сопротивления")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: dragCoefficientsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  dragCoefficientsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            visible:        _verticalDragCoefficient !== null
                        }

                        QGCLabel {
                            text:           qsTr("Горизонтальный коэффициент сопротивления")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalDragCoefficient
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите горизонтальный коэффициент")
                            visible:        _horizontalDragCoefficient !== null
                        }
                    }
                }

                // Площади сечения
                QGCLabel {
                    text:       qsTr("Площади сечения")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: crossSectionalGrid.height + (_margins * 2)
                    Layout.preferredWidth:  crossSectionalGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            visible:        _verticalCrossSection !== null
                        }

                        QGCLabel {
                            text:           qsTr("Горизонтальная площадь сечения (м²)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _horizontalCrossSection
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите горизонтальную площадь")
                            visible:        _horizontalCrossSection !== null
                        }
                    }
                }

                // Параметры высоты
                QGCLabel {
                    text:       qsTr("Параметры высоты")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: heightParamsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  heightParamsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            currentIndex:   _heightMode
                            onActivated: {
                                _heightMode = index
                            }
                            width:          _valueFieldWidth
                        }

                        QGCLabel {
                            text:           qsTr("Фиксированная высота (м)")
                            visible:        _heightMode === HeightModeFixed
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _fixedHeight
                            visible:        _heightMode === HeightModeFixed && _fixedHeight !== null
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите фиксированную высоту")
                        }

                        QGCLabel {
                            text:           qsTr("Номер AUX канала")
                            visible:        _heightMode === HeightModeRemote
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxChannel
                            visible:        _heightMode === HeightModeRemote && _auxChannel !== null
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите номер канала")
                        }

                        QGCLabel {
                            text:           qsTr("Минимальная высота AUX (м)")
                            visible:        _heightMode === HeightModeRemote
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMinHeight
                            visible:        _heightMode === HeightModeRemote && _auxMinHeight !== null
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите минимальную высоту")
                        }

                        QGCLabel {
                            text:           qsTr("Максимальная высота AUX (м)")
                            visible:        _heightMode === HeightModeRemote
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _auxMaxHeight
                            visible:        _heightMode === HeightModeRemote && _auxMaxHeight !== null
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите максимальную высоту")
                        }
                    }
                }

                // Параметры отображения
                QGCLabel {
                    text:       qsTr("Параметры отображения")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: displayOptionsGrid.height + (_margins * 2)
                    Layout.preferredWidth:  displayOptionsGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            checked:        _showTrajectory ? _showTrajectory.rawValue : false
                            onClicked: {
                                if (_showTrajectory) {
                                    _showTrajectory.rawValue = checked
                                }
                            }
                        }
                    }
                }

                // Калибровка OSD
                QGCLabel {
                    text:       qsTr("Калибровка OSD")
                    visible:    _settingsAvailable && _enabled && safeRawValue(_enabled, false)
                    font.family: ScreenTools.demiboldFontFamily
                }
                Rectangle {
                    Layout.preferredHeight: calibrationGrid.height + (_margins * 2)
                    Layout.preferredWidth:  calibrationGrid.width + (_margins * 2)
                    color:                  qgcPal.windowShade
                    visible:                _settingsAvailable && _enabled && safeRawValue(_enabled, false)
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
                            checked:        _calibrationMode ? _calibrationMode.rawValue : false
                            onClicked: {
                                if (_calibrationMode) {
                                    _calibrationMode.rawValue = checked
                                }
                            }
                        }

                        QGCLabel {
                            text:           qsTr("Шаг калибровки (пиксели)")
                            visible:        calibrationModeCheckbox.checked
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _calibrationStep
                            visible:        calibrationModeCheckbox.checked && _calibrationStep !== null
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
                            visible:        _markerSize !== null
                        }

                        QGCLabel {
                            text:           qsTr("Смещение маркера по X (пиксели)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetX
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение по X")
                            visible:        _markerOffsetX !== null
                        }

                        QGCLabel {
                            text:           qsTr("Смещение маркера по Y (пиксели)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _markerOffsetY
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите смещение по Y")
                            visible:        _markerOffsetY !== null
                        }

                        QGCLabel {
                            text:           qsTr("Наклон подвеса (градусы)")
                            Layout.fillWidth: true
                        }
                        FactTextField {
                            fact:           _gimbalPitch
                            width:          _valueFieldWidth
                            placeholderText: qsTr("Введите наклон подвеса")
                            visible:        _gimbalPitch !== null
                        }

                        QGCButton {
                            text:           qsTr("Сброс калибровки")
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            visible:        _markerSize !== null && _markerOffsetX !== null && 
                                           _markerOffsetY !== null && _gimbalPitch !== null
                            onClicked: {
                                if (_markerSize) _markerSize.rawValue = 32
                                if (_markerOffsetX) _markerOffsetX.rawValue = 0
                                if (_markerOffsetY) _markerOffsetY.rawValue = 0
                                if (_gimbalPitch) _gimbalPitch.rawValue = 0
                            }
                        }
                    }
                }
            }
        }
    }
} 