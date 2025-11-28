import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    objectName: "defaultCover"

    CoverTemplate {
        objectName: "applicationCover"
        primaryText: "Apple Mind"
        secondaryText: qsTr("ФруктAIметр")
        icon {
            source: Qt.resolvedUrl("../icons/da (1).png")
            sourceSize { width: icon.width; height: icon.height }
        }
    }
}
