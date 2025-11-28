import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    objectName: "mainPage"
    allowedOrientations: Orientation.All

    PageHeader {
        objectName: "pageHeader"
        title: qsTr("ФруктAIметр")
        extraContent.children: [
            IconButton {
                objectName: "aboutButton"
                icon.source: "image://theme/icon-m-about"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        ]
    }

    Column {
        anchors.top: parent.top
        anchors.topMargin: header.height + Theme.paddingLarge
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: Theme.paddingLarge

        // Заголовок и описание
        Column {
            width: parent.width - 2*Theme.horizontalPageMargin
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Theme.paddingMedium

            Image {
                source: "../icons/da (1).png"
                anchors.horizontalCenter: parent.horizontalCenter
                height: 64
                width: 64
            }

            Label {
                width: parent.width
                text: qsTr("Apple Mind")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeHuge
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                width: parent.width
                text: qsTr("Определение свежести яблок с помощью ИИ")
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }

        // Основная кнопка действия
        Button {
            width: parent.width - 2*Theme.horizontalPageMargin
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("🔍 Начать анализ яблока")
            onClicked: pageStack.push(Qt.resolvedUrl("CameraPage.qml"))
        }

        // Возможности приложения
        Rectangle {
            width: parent.width - 2*Theme.horizontalPageMargin
            anchors.horizontalCenter: parent.horizontalCenter
            height: featuresColumn.height + 2*Theme.paddingMedium
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.1)
            radius: Theme.paddingSmall

            Column {
                id: featuresColumn
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: Theme.paddingMedium
                }
                spacing: Theme.paddingSmall

                Label {
                    width: parent.width
                    text: qsTr("🌟 Возможности:")
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                }

                Row {
                    width: parent.width
                    spacing: Theme.paddingMedium

                    Column {
                        width: (parent.width - Theme.paddingMedium) / 2
                        spacing: Theme.paddingSmall

                        Label {
                            width: parent.width
                            text: qsTr("📷 Камера")
                            color: Theme.primaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }

                        Label {
                            width: parent.width
                            text: qsTr("Мгновенный анализ через камеру")
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: Text.WordWrap
                        }
                    }

                    Column {
                        width: (parent.width - Theme.paddingMedium) / 2
                        spacing: Theme.paddingSmall

                        Label {
                            width: parent.width
                            text: qsTr("🤖 ИИ-анализ")
                            color: Theme.primaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }

                        Label {
                            width: parent.width
                            text: qsTr("Точное определение свежести")
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: Text.WordWrap
                        }
                    }
                }

                Row {
                    width: parent.width
                    spacing: Theme.paddingMedium

                    Column {
                        width: (parent.width - Theme.paddingMedium) / 2
                        spacing: Theme.paddingSmall

                        Label {
                            width: parent.width
                            text: qsTr("⚡ Автономно")
                            color: Theme.primaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }

                        Label {
                            width: parent.width
                            text: qsTr("Работа без интернета")
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: Text.WordWrap
                        }
                    }

                    Column {
                        width: (parent.width - Theme.paddingMedium) / 2
                        spacing: Theme.paddingSmall

                        Label {
                            width: parent.width
                            text: qsTr("🎯 Точно")
                            color: Theme.primaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }

                        Label {
                            width: parent.width
                            text: qsTr("Профессиональная оценка")
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }

        // Инструкция
        Rectangle {
            width: parent.width - 2*Theme.horizontalPageMargin
            anchors.horizontalCenter: parent.horizontalCenter
            height: instructionColumn.height + 2*Theme.paddingMedium
            color: Theme.rgba(Theme.highlightColor, 0.1)
            radius: Theme.paddingSmall

            Column {
                id: instructionColumn
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: Theme.paddingMedium
                }
                spacing: Theme.paddingSmall

                Label {
                    width: parent.width
                    text: qsTr("📝 Как использовать:")
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeSmall
                    font.bold: true
                }

                Label {
                    width: parent.width
                    text: qsTr("1. Нажмите 'Начать анализ'\n2. Сфотографируйте яблоко или выберите демо-режим\n3. Получите детальный анализ свежести")
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
