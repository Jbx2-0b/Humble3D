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
        id: view1
        width: parent.width / 2
        height: parent.height

        backgroundColor: Qt.vector4d(0.5, 0.5, 0.7, 1.0)

        eyePosition: Qt.vector3d(12, 12, 8)
        center: Qt.vector3d(0, 0, 0)

    }

    Widget3D {
        id: view2
        width: parent.width / 2
        height: parent.height
        x: parent.width / 2

        backgroundColor: Qt.vector4d(0.5, 0.5, 0.7, 1.0)

        eyePosition: Qt.vector3d(30, 30, 12)
        center: Qt.vector3d(0, 0, 0)
    }

    Switch {
        x: 10
        y: 10

        onCheckedChanged: {
            view1.enabled = checked
        }
    }

    Switch {
        x: parent.width / 2 + 10
        y: 10

        onCheckedChanged: {
            view2.enabled = checked
        }
    }
}
