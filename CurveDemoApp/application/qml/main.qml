import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

import "qrc:/qml/components"
import "components/createpoint.js" as PointScript

import SceneGraphRendering 1.0

Item {
    id: root

    signal pointClicked(int x, int y, int box)
    signal toggleHidBindView
    signal pointsClear


    onToggleHidBindView: {
        hid_bind_view.toggle()
    }

    Component.onCompleted: {
        PointScript.createComponent()
    }

    onPointsClear: {
        for (var i = 5; i< root.children.length; i++){
            console.log(root.children[i])
            root.children[i].destroy()
        }


    }

    Renderer {
        id: renderer

        anchors.fill: parent

        rcpair_name: rc_pair_cb.currentText

        ComboBox {
            id: rc_pair_cb
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5

            width: 128

            opacity: 0.7

            model: rc_name_model
            textRole: "display"
        }

        Button {
            text: "?"
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 5

            width: height
            opacity: 0.7

            onClicked: hid_bind_view.toggle()
        }

        HidBindingView {
            id: hid_bind_view
            anchors.fill: parent
            anchors.margins: 50
            visible:false

            states: [
                State{
                    name: "show"
                    PropertyChanges {
                        target: hid_bind_view
                        visible: true
                    }
                }
            ]

            function toggle() {
                if(state === "") state = "show"
                else state = ""
            }
        }

        Button {
            text: "Clear colours"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 5

            opacity: 0.7

            onClicked: pointsClear()
        }
    }

    Rectangle {
        id: redrec
        width:  100
        height: 100
        opacity: 1.0

        anchors {
            top: parent.top
            right: parent.right
            margins: 30
            topMargin: 50
        }

        gradient: Gradient {
            GradientStop { position: 1.0; color: "white" }
            GradientStop { position: 0.0; color: "red" }
        }

        MouseArea {
            anchors.fill: parent
            function handleMouseRed(mouse) {
                if(mouse.button & Qt.LeftButton) {
                    pointlist.append(PointScript.createPoint(redrec.x + mouse.x - 5, redrec.y + mouse.y -5))
                    root.pointClicked(mouse.x, mouse.y, 1)
                }
            }
            onPressed: handleMouseRed(mouse)
        }
    }


    Rectangle {
        id: greenrec
        width:  100
        height: 100
        opacity: 1.0

        anchors {
            top: redrec.bottom
            right: parent.right
            margins: 30
        }

        gradient: Gradient {
            GradientStop { position: 1.0; color: "white" }
            GradientStop { position: 0.0; color: "green" }
        }

        MouseArea {
            anchors.fill: parent
            function handleMouseGreen(mouse) {
                if(mouse.button & Qt.LeftButton) {
                    pointlist.append(PointScript.createPoint(greenrec.x + mouse.x - 5, greenrec.y + mouse.y -5))
                    root.pointClicked(mouse.x, mouse.y, 2)
                }
            }
            onPressed: handleMouseGreen(mouse)
        }
    }

    Rectangle {
        id: bluerec
        width:  100
        height: 100
        opacity: 1.0

        anchors {
            top: greenrec.bottom
            right: parent.right
            margins: 30
        }

        gradient: Gradient {
            GradientStop { position: 1.0; color: "white" }
            GradientStop { position: 0.0; color: "blue" }
        }

        MouseArea {
            anchors.fill: parent
            function handleMouseBlue(mouse) {
                if(mouse.button & Qt.LeftButton) {
                    pointlist.append(PointScript.createPoint(bluerec.x + mouse.x - 5, bluerec.y + mouse.y -5))
                    root.pointClicked(mouse.x, mouse.y, 3)
                }
            }
            onPressed: handleMouseBlue(mouse)
        }
    }

    Rectangle {
        id: alpharec
        width:  100
        height: 100
        opacity: 1.0

        anchors {
            top: bluerec.bottom
            right: parent.right
            margins: 30
        }

        gradient: Gradient {
            GradientStop { position: 1.0; color: "white" }
            GradientStop { position: 0.0; color: "black" }
        }

        MouseArea {
            anchors.fill: parent
            function handleMouseAlpha(mouse) {
                if(mouse.button & Qt.LeftButton) {
                    pointlist.append(PointScript.createPoint(alpharec.x + mouse.x - 5, alpharec.y + mouse.y -5))
                    root.pointClicked(mouse.x, mouse.y, 4)
                }
            }
            onPressed: handleMouseAlpha(mouse)
        }
    }

    ListModel {
        id: pointlist
    }
}

