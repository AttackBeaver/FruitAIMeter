import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    objectName: "aboutPage"
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("О приложении")
            }

            DetailItem {
                label: qsTr("Версия")
                value: "1.7.3"
            }

            DetailItem {
                label: qsTr("Разработчик")
                value: "404: Имя не найдено"
            }

            SectionHeader {
                text: qsTr("Описание")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.WordWrap
                text: qsTr("ФруктAIметр Apple Mind - приложение для определения свежести яблок с помощью компьютерного зрения.")
                color: Theme.primaryColor
            }
        }
    }
}
