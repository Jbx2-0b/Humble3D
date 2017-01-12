import QtQuick 2.6
import QtQuick.Controls 2.1
import Humble3D 1.0


ApplicationWindow {
    id: window
    width: 1000
    height: 520
    visible: true
    title: "Test Qml"

    Widget3D {
        width: parent.width / 2
        height: parent.height

        eyePosition: Qt.vector3d(12, 12, 8)
        center: Qt.vector3d(0, 0, 0)
    }

    Widget3D {
        width: parent.width / 2
        height: parent.height
        x: parent.width / 2

        eyePosition: Qt.vector3d(30, 30, 12)
        center: Qt.vector3d(0, 0, 0)
    }

}
