import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.ScreenTools  1.0
import QGroundControl.Controls     1.0

Item {
    id: root
    
    property bool isActive: false
    property var targetPoint: QtPositioning.coordinate()
    property real dropTime: 0.0
    property bool isCalibrationMode: QGroundControl.settingsManager.ballisticCalculatorSettings.calibrationMode.rawValue
    property real calibrationStep: QGroundControl.settingsManager.ballisticCalculatorSettings.calibrationStep.rawValue

    visible: isActive

    // Обработка клавиш для калибровки
    Keys.onPressed: {
        if (!isCalibrationMode) return;

        var settings = QGroundControl.settingsManager.ballisticCalculatorSettings;
        var step = calibrationStep;

        switch (event.key) {
            case Qt.Key_Left:
                settings.markerOffsetX.rawValue = settings.markerOffsetX.rawValue - step;
                break;
            case Qt.Key_Right:
                settings.markerOffsetX.rawValue = settings.markerOffsetX.rawValue + step;
                break;
            case Qt.Key_Up:
                settings.markerOffsetY.rawValue = settings.markerOffsetY.rawValue - step;
                break;
            case Qt.Key_Down:
                settings.markerOffsetY.rawValue = settings.markerOffsetY.rawValue + step;
                break;
            case Qt.Key_Plus:
            case Qt.Key_Equal:
                settings.markerSize.rawValue = Math.min(settings.markerSize.rawValue + step, 128);
                break;
            case Qt.Key_Minus:
                settings.markerSize.rawValue = Math.max(settings.markerSize.rawValue - step, 16);
                break;
            case Qt.Key_R:
                // Сброс калибровки
                settings.markerOffsetX.rawValue = 0;
                settings.markerOffsetY.rawValue = 0;
                settings.markerSize.rawValue = 32;
                break;
            case Qt.Key_S:
                // Сохраняем текущий профиль
                if (settings.activeProfile.rawValue !== "Default") {
                    settings.saveCurrentProfile(settings.activeProfile.rawValue);
                }
                break;
        }
        event.accepted = true;
    }

    // Прицельная метка
    Rectangle {
        id: crosshair
        width: QGroundControl.settingsManager.ballisticCalculatorSettings.markerSize.rawValue
        height: width
        color: "transparent"
        border.color: isCalibrationMode ? "yellow" : "red"
        border.width: isCalibrationMode ? 3 : 2
        radius: width / 2

        // Применяем калибровочные смещения и учитываем угол гимбала
        transform: [
            Translate {
                x: QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetX.rawValue
                y: QGroundControl.settingsManager.ballisticCalculatorSettings.markerOffsetY.rawValue + 
                   Math.tan(QGroundControl.settingsManager.ballisticCalculatorSettings.gimbalPitch.rawValue * Math.PI / 180.0) * height
            }
        ]

        anchors.centerIn: parent

        // Вертикальная линия
        Rectangle {
            width: 2
            height: parent.height * 0.6
            color: isCalibrationMode ? "yellow" : "red"
            anchors.centerIn: parent
        }

        // Горизонтальная линия
        Rectangle {
            width: parent.width * 0.6
            height: 2
            color: isCalibrationMode ? "yellow" : "red"
            anchors.centerIn: parent
        }

        // Индикатор направления ветра
        Rectangle {
            width: 2
            height: parent.width * 0.4
            color: "yellow"
            anchors.centerIn: parent
            transform: Rotation {
                origin.x: 1
                origin.y: 0
                angle: QGroundControl.settingsManager.ballisticCalculatorSettings.windDirection.rawValue
            }
        }
    }

    // Информация о точке падения
    Column {
        anchors {
            left: crosshair.right
            leftMargin: ScreenTools.defaultFontPixelWidth
            verticalCenter: crosshair.verticalCenter
        }
        spacing: ScreenTools.defaultFontPixelHeight / 2

        QGCLabel {
            text: targetPoint.isValid ? 
                  qsTr("Lat: %1°").arg(targetPoint.latitude.toFixed(6)) : 
                  qsTr("Lat: --")
            font.pointSize: ScreenTools.smallFontPointSize
        }

        QGCLabel {
            text: targetPoint.isValid ? 
                  qsTr("Lon: %1°").arg(targetPoint.longitude.toFixed(6)) : 
                  qsTr("Lon: --")
            font.pointSize: ScreenTools.smallFontPointSize
        }

        QGCLabel {
            text: qsTr("Drop time: %1 s").arg(dropTime.toFixed(1))
            font.pointSize: ScreenTools.smallFontPointSize
        }

        QGCLabel {
            text: qsTr("Wind: %1 м/с %2°").arg(
                QGroundControl.settingsManager.ballisticCalculatorSettings.windSpeed.rawValue.toFixed(1)
            ).arg(
                QGroundControl.settingsManager.ballisticCalculatorSettings.windDirection.rawValue.toFixed(0)
            )
            font.pointSize: ScreenTools.smallFontPointSize
            color: "yellow"
        }
    }

    // Инструкции по калибровке
    Rectangle {
        visible: isCalibrationMode
        color: Qt.rgba(0, 0, 0, 0.7)
        radius: ScreenTools.defaultFontPixelWidth
        anchors {
            left: parent.left
            top: parent.top
            margins: ScreenTools.defaultFontPixelWidth
        }
        width: calibrationHelp.width + ScreenTools.defaultFontPixelWidth * 2
        height: calibrationHelp.height + ScreenTools.defaultFontPixelWidth * 2

        Column {
            id: calibrationHelp
            anchors.centerIn: parent
            spacing: ScreenTools.defaultFontPixelHeight / 2

            QGCLabel {
                text: qsTr("Режим калибровки")
                font.bold: true
                color: "yellow"
            }

            QGCLabel {
                text: qsTr("Стрелки: перемещение метки")
                color: "white"
            }

            QGCLabel {
                text: qsTr("+/-: изменение размера")
                color: "white"
            }

            QGCLabel {
                text: qsTr("R: сброс калибровки")
                color: "white"
            }

            QGCLabel {
                text: qsTr("S: сохранить профиль")
                color: "white"
                visible: QGroundControl.settingsManager.ballisticCalculatorSettings.activeProfile.rawValue !== "Default"
            }

            QGCLabel {
                text: qsTr("Шаг: %1").arg(calibrationStep)
                color: "white"
            }

            QGCLabel {
                text: qsTr("Профиль: %1").arg(QGroundControl.settingsManager.ballisticCalculatorSettings.activeProfile.rawValue)
                color: "white"
            }
        }
    }

    // Траектория падения
    Canvas {
        id: trajectoryCanvas
        anchors.fill: parent
        visible: QGroundControl.settingsManager.ballisticCalculatorSettings.showTrajectory.rawValue

        property var points: []
        property real scale: 1.0

        Component.onCompleted: {
            QGroundControl.ballisticCalculator.trajectoryPointsChanged.connect(function(newPoints) {
                points = newPoints;
                requestPaint();
            });
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            
            if (points.length < 2) return;
            
            ctx.beginPath();
            ctx.strokeStyle = "yellow";
            ctx.lineWidth = 2;
            ctx.setLineDash([5, 5]);
            
            // Находим максимальное смещение для масштабирования
            var maxOffset = 0;
            for (var i = 0; i < points.length; i++) {
                var point = points[i];
                maxOffset = Math.max(maxOffset, Math.abs(point.x), Math.abs(point.y));
            }
            
            // Рассчитываем масштаб, чтобы траектория занимала не более 80% экрана
            scale = Math.min(width, height) * 0.4 / maxOffset;
            
            // Рисуем траекторию
            ctx.moveTo(points[0].x * scale + width/2, points[0].y * scale + height/2);
            for (var i = 1; i < points.length; i++) {
                ctx.lineTo(points[i].x * scale + width/2, points[i].y * scale + height/2);
            }
            
            ctx.stroke();
        }
    }

    // Индикатор готовности к сбросу
    Rectangle {
        visible: QGroundControl.settingsManager.ballisticCalculatorSettings.readyToDropEnabled.rawValue
        width: readyLabel.width + ScreenTools.defaultFontPixelWidth * 2
        height: readyLabel.height + ScreenTools.defaultFontPixelHeight
        color: QGroundControl.settingsManager.ballisticCalculatorSettings.isReadyToDrop() ? "#80008000" : "#80800000"
        radius: height / 4
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: ScreenTools.defaultFontPixelWidth
        }

        QGCLabel {
            id: readyLabel
            anchors.centerIn: parent
            text: QGroundControl.settingsManager.ballisticCalculatorSettings.isReadyToDrop() ? 
                  qsTr("ГОТОВ К СБРОСУ") : qsTr("НЕ ГОТОВ")
            color: "white"
        }
    }

    // Диалог управления профилями
    Rectangle {
        id: profileDialog
        visible: false
        color: Qt.rgba(0, 0, 0, 0.8)
        radius: ScreenTools.defaultFontPixelWidth
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.8, ScreenTools.defaultFontPixelWidth * 40)
        height: profileColumn.height + ScreenTools.defaultFontPixelHeight * 2

        MouseArea {
            anchors.fill: parent
            // Предотвращаем клики "сквозь" диалог
        }

        Column {
            id: profileColumn
            anchors.margins: ScreenTools.defaultFontPixelHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: ScreenTools.defaultFontPixelHeight / 2

            QGCLabel {
                text: qsTr("Управление профилями")
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Список профилей
            Rectangle {
                width: parent.width
                height: profileList.height
                color: Qt.rgba(0, 0, 0, 0.3)

                ListView {
                    id: profileList
                    width: parent.width
                    height: Math.min(contentHeight, ScreenTools.defaultFontPixelHeight * 10)
                    model: QGroundControl.settingsManager.ballisticCalculatorSettings.getProfileList()
                    delegate: Rectangle {
                        width: parent.width
                        height: profileLabel.height + ScreenTools.defaultFontPixelHeight
                        color: modelData === QGroundControl.settingsManager.ballisticCalculatorSettings.activeProfile.rawValue ?
                               Qt.rgba(0.2, 0.4, 0.6, 0.5) : "transparent"

                        QGCLabel {
                            id: profileLabel
                            text: modelData
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: ScreenTools.defaultFontPixelWidth
                        }

                        QGCButton {
                            visible: modelData !== "Default"
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: qsTr("Удалить")
                            onClicked: QGroundControl.settingsManager.ballisticCalculatorSettings.deleteProfile(modelData)
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: QGroundControl.settingsManager.ballisticCalculatorSettings.loadProfile(modelData)
                        }
                    }
                }
            }

            // Создание нового профиля
            Row {
                spacing: ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter: parent.horizontalCenter

                QGCTextField {
                    id: newProfileName
                    placeholderText: qsTr("Имя нового профиля")
                    width: profileDialog.width * 0.6
                }

                QGCButton {
                    text: qsTr("Сохранить")
                    enabled: newProfileName.text.length > 0
                    onClicked: {
                        QGroundControl.settingsManager.ballisticCalculatorSettings.saveCurrentProfile(newProfileName.text)
                        newProfileName.text = ""
                    }
                }
            }

            QGCButton {
                text: qsTr("Закрыть")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: profileDialog.visible = false
            }
        }
    }

    // Кнопка открытия диалога профилей
    Rectangle {
        width: profileButton.width + ScreenTools.defaultFontPixelWidth * 2
        height: profileButton.height + ScreenTools.defaultFontPixelHeight
        color: Qt.rgba(0, 0, 0, 0.5)
        radius: height / 4
        anchors {
            right: parent.right
            top: parent.top
            margins: ScreenTools.defaultFontPixelWidth
        }

        QGCButton {
            id: profileButton
            anchors.centerIn: parent
            text: qsTr("Профили")
            onClicked: profileDialog.visible = true
        }
    }

    // Захватываем фокус для обработки клавиш
    Component.onCompleted: {
        forceActiveFocus()
    }
} 