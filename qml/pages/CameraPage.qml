import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.6
import FruitAIMeter 1.0

Page {
    id: cameraPage
    objectName: "cameraPage"
    allowedOrientations: Orientation.All

    property bool isCameraActive: false // должен быть false
    property string capturedImagePath: ""
    property bool isAnalyzing: false
    property bool modelsLoaded: false

    // Цвета для индикации состояния
    property color freshColor: "#4CAF50"
    property color normalColor: "#FF9800"
    property color spoiledColor: "#F44336"

    PageHeader {
        id: header
        objectName: "pageHeader"
        title: qsTr("Apple Mind - Анализ")
    }

    Label {
        id: statusText
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Theme.paddingMedium
        text: "Статус: Ожидание"
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        horizontalAlignment: Text.AlignHCenter
    }

    ImageAnalyzer {
        id: imageAnalyzer

        onAnalysisComplete: {
            console.log("Анализ завершен успешно")
            isAnalyzing = false
            resultText.text = result
            analysisProgress.visible = false
            resultArea.visible = true

            // Прокручиваем к результатам
            flickable.contentY = Math.max(0, resultArea.y - Theme.paddingLarge)
        }

        onAnalysisFailed: {
            console.log("Ошибка анализа:", error)
            isAnalyzing = false
            resultText.text = "❌ Ошибка анализа: " + error
            analysisProgress.visible = false
            resultArea.visible = true
        }
    }

    Flickable {
        id: flickable
        anchors.top: statusText.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentHeight: contentColumn.height
        clip: true

        Column {
            id: contentColumn
            width: parent.width
            spacing: Theme.paddingMedium

            // Статус системы
            Rectangle {
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                height: statusColumn.height + 2*Theme.paddingMedium
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.1)
                radius: Theme.paddingSmall

                Column {
                    id: statusColumn
                    width: parent.width - 2*Theme.paddingMedium
                    anchors.centerIn: parent
                    spacing: Theme.paddingSmall

                    Row {
                        width: parent.width
                        spacing: Theme.paddingMedium

                        Rectangle {
                            width: 12
                            height: 12
                            radius: 6
                            color: imageAnalyzer.modelsLoaded ? freshColor : spoiledColor
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Label {
                            width: parent.width
                            text: imageAnalyzer.modelsLoaded ?
                                "✅ Классификатор готов" :
                                "🔄 Загрузка классификатора..."
                            color: imageAnalyzer.modelsLoaded ? Theme.highlightColor : Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                            wrapMode: Text.WordWrap
                        }
                    }

                    Label {
                        width: parent.width
                        text: qsTr("Режим: Демо-анализ")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }

                    Label {
                        width: parent.width
                        text: qsTr("Камера: %1").arg(camera.availability === Camera.Available ? "Доступна" : "Недоступна")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
            }

            // Область камеры/изображения
            Rectangle {
                id: cameraContainer
                width: parent.width
                height: Math.min(width * 0.75, 400)
                color: "black"
                radius: Theme.paddingSmall

                Camera {
                    id: camera
                    objectName: "camera"

                    // Базовые настройки
                    captureMode: Camera.CaptureStillImage
                    focus.focusMode: CameraFocus.FocusContinuous

                    // Настройки для фото
                    imageCapture {
                        onImageCaptured: {
                            console.log("Изображение захвачено")
                            photoPreview.source = preview
                            capturedImagePath = preview
                            isCameraActive = false
                            camera.stop()
                        }

                        onImageSaved: {
                            console.log("Изображение сохранено:", path)
                            startAnalysis(path)
                        }
                    }        

                    onCameraStatusChanged: {
                           console.log("Статус камеры:", cameraStatus)
                           if (cameraStatus === Camera.LoadedStatus) {
                               statusText.text = "Камера готова"
                           } else if (cameraStatus === Camera.ActiveStatus) {
                               statusText.text = "Камера активна"
                           } else if (cameraStatus === Camera.StartingStatus) {
                               statusText.text = "Запуск камеры..."
                           }
                       }

                    onError: {
                        console.log("Ошибка камеры:", errorString)
                        statusText.text = "Ошибка камеры: " + errorString
                    }
                }

                VideoOutput {
                    id: videoOutput
                    source: camera
                    anchors.fill: parent
                    visible: isCameraActive
                    autoOrientation: true
                    //focus: visible
                }

                Image {
                    id: photoPreview
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    visible: !isCameraActive && capturedImagePath !== ""
                }

                // Сообщение когда камера неактивна
                Column {
                    anchors.centerIn: parent
                    spacing: Theme.paddingMedium
                    width: parent.width - 2*Theme.paddingLarge
                    visible: !isCameraActive && photoPreview.source == ""

                    Label {
                        width: parent.width
                        text: qsTr("📷 Готов к анализу")
                        color: "white"
                        font.pixelSize: Theme.fontSizeLarge
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        width: parent.width
                        text: qsTr("Нажмите 'Старт' для начала анализа")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }
                }

                // Индикатор активности камеры
                Rectangle {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: Theme.paddingSmall
                    width: 16
                    height: 16
                    radius: 8
                    color: isCameraActive ? "#FF4444" : "transparent"
                    border.color: "white"
                    border.width: 2
                    visible: isCameraActive

                    SequentialAnimation on opacity {
                        running: isCameraActive
                        loops: Animation.Infinite
                        PropertyAnimation { to: 0.3; duration: 500 }
                        PropertyAnimation { to: 1.0; duration: 500 }
                    }
                }
            }

            // Панель управления камерой
            Row {
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium

                Button {
                    width: (parent.width - Theme.paddingMedium) / 2
                    text: isCameraActive ? qsTr("⏹️ Стоп") : qsTr("📷 Старт")
                    enabled: camera.availability === Camera.Available && !isAnalyzing
                    onClicked: toggleCamera()
                }

                Button {
                    width: (parent.width - Theme.paddingMedium) / 2
                    text: qsTr("📸 Снять")
                    enabled: isCameraActive && !isAnalyzing
                    onClicked: captureImage()
                }
            }

            // Альтернативные способы
            Column {
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingSmall

                Label {
                    width: parent.width
                    text: qsTr("Или выберите демо-изображение:")
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }

                // Кнопка случайного выбора
                Button {
                    width: parent.width
                    text: qsTr("🎲 Случайное демо-изображение")
                    enabled: !isAnalyzing
                    onClicked: useRandomDemoImage()
                }
            }

            // Индикатор анализа
            Rectangle {
                id: analysisProgress
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                height: progressColumn.height + 2*Theme.paddingMedium
                color: Theme.rgba(Theme.highlightColor, 0.1)
                radius: Theme.paddingSmall
                visible: isAnalyzing

                Column {
                    id: progressColumn
                    anchors.centerIn: parent
                    spacing: Theme.paddingMedium
                    width: parent.width - 2*Theme.paddingLarge

                    BusyIndicator {
                        anchors.horizontalCenter: parent.horizontalCenter
                        size: BusyIndicatorSize.Medium
                        running: true
                    }

                    Label {
                        width: parent.width
                        text: qsTr("Идет анализ изображения...\nИИ определяет свежесть яблока")
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        color: Theme.highlightColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
            }

            // Область результатов
            Rectangle {
                id: resultArea
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                height: resultText.text !== "" ? (resultText.height + 2*Theme.paddingMedium) : 0
                color: getResultColor()
                radius: Theme.paddingSmall
                visible: resultText.text !== ""

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }

                Flickable {
                    anchors.fill: parent
                    contentHeight: resultText.height
                    clip: true

                    Label {
                        id: resultText
                        width: parent.width - 2*Theme.paddingMedium
                        anchors.centerIn: parent
                        text: ""
                        wrapMode: Text.WordWrap
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }

            // Информация о технологии
            Rectangle {
                width: parent.width - 2*Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                height: techInfoColumn.height + 2*Theme.paddingMedium
                color: Theme.rgba(Theme.secondaryColor, 0.1)
                radius: Theme.paddingSmall

                Column {
                    id: techInfoColumn
                    width: parent.width - 2*Theme.paddingMedium
                    anchors.centerIn: parent
                    spacing: Theme.paddingSmall

                    Label {
                        width: parent.width
                        text: qsTr("🔍 Технологии анализа:")
                        color: Theme.highlightColor
                        font.pixelSize: Theme.fontSizeSmall
                        font.bold: true
                    }

                    Label {
                        width: parent.width
                        text: qsTr("• YOLO11-segm - сегментация яблока\n• TensorFlow Lite - классификация\n• Автономная работа - без интернета")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    function toggleCamera() {
        if (isCameraActive) {
            console.log("Останавливаем камеру")
            camera.stop()
            isCameraActive = false
            statusText.text = "Камера остановлена"
        } else {
            console.log("Запускаем камеру, доступность:", camera.availability)

            if (camera.availability === Camera.Available) {
                camera.start()
                capturedImagePath = ""
                resultText.text = ""
                resultArea.visible = false
                isCameraActive = true
                statusText.text = "Запуск камеры..."
            } else {
                statusText.text = "Камера недоступна"
                console.log("Камера недоступна для запуска")
            }
        }
    }

    function captureImage() {
        if (isCameraActive && camera.cameraStatus === Camera.ActiveStatus) {
            console.log("Захват изображения...")
            camera.imageCapture.capture()
        }
    }

    function startAnalysis(imagePath, demoType) {
        if (isAnalyzing) {
            console.log("Анализ уже выполняется")
            return
        }

        isAnalyzing = true
        analysisProgress.visible = true
        resultArea.visible = false

        console.log("Запуск анализа для:", imagePath)

        // Загружаем модели если еще не загружены
        if (!modelsLoaded) {
            console.log("Загрузка моделей...")
            modelsLoaded = imageAnalyzer.loadModels()
        }

        imageAnalyzer.analyzeImage(imagePath)
    }

    function getResultColor() {
        var result = resultText.text
        if (!result) return Theme.rgba(Theme.highlightBackgroundColor, 0.1)

        var lowerResult = result.toLowerCase()
        if (lowerResult.includes("свежее") || lowerResult.includes("отличное")) {
            return freshColor
        } else if (lowerResult.includes("нормальное") || lowerResult.includes("хорошее")) {
            return normalColor
        } else if (lowerResult.includes("порченое") || lowerResult.includes("плохое")) {
            return spoiledColor
        }
        return Theme.rgba(Theme.highlightBackgroundColor, 0.1)
    }

    function useRandomDemoImage() {
        // Используем изображения из датасета: 1.jpg, 2.jpg, 4.jpg, 5.jpg, 6.jpg, 7.jpg
        var demoImages = ["1", "2", "4", "5", "6", "7", "43"];

        var randomIndex = Math.floor(Math.random() * demoImages.length);
        var randomImage = demoImages[randomIndex];

        useDemoImage(randomImage);
    }

    function useDemoImage(imageNumber) {
        var imageSource = "qrc:/images/" + imageNumber + ".jpg"

        capturedImagePath = imageSource
        photoPreview.source = imageSource
        resultText.text = ""
        resultArea.visible = false
        isCameraActive = false

        console.log("Используется демо-изображение:", imageNumber + ".jpg")
        startAnalysis(imageSource, "demo")
    }

    Component.onCompleted: {
        console.log("Инициализация камеры...")
        console.log("Доступность камеры:", camera.availability)
        console.log("Доступные камеры:", QtMultimedia.availableCameras.length)

        if (QtMultimedia.availableCameras.length > 0) {
            camera.deviceId = QtMultimedia.availableCameras[0].deviceId
            console.log("Выбрана камера:", QtMultimedia.availableCameras[0].displayName)
        }

        // Предварительная загрузка моделей
        console.log("Предварительная загрузка моделей...")
        modelsLoaded = imageAnalyzer.loadModels()

        statusText.text = "Готов к работе"
    }

    Component.onDestruction: {
        if (isCameraActive) {
            camera.stop()
        }
    }
}
