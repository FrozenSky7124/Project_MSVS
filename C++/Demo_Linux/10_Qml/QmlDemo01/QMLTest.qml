import QtQuick 2.0

Image {
    id: root
    source: "Resources/bg_chuyin.jpg"

    Image {
        id: wheel
        source: "Resources/img_wh.jpg"
        anchors.centerIn: parent
        /**
         *@param: anchors 锚点
         * anchors.verticalCenterOffset:
         * anchors.horizontalCenterOffset:
         * anchors.centerIn:
         */
        Behavior on rotation {
            NumberAnimation {
                duration: 250
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: wheel.rotation += 90
    }
}
