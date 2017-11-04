import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import Qt.labs.settings 1.0 as Stgs
import QtGraphicalEffects 1.0
ApplicationWindow {
    visible: true
    width: 360
    height: 520
    title: "Plant Recognition"
    id: window

    Shortcut {
        sequence: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
        }
    }
    onClosing: {
        if (stackView !== null && stackView.depth !== 'undefined'
        && stackView.depth > 1) {
            close.accepted = false
            stackView.pop()
        }
    }
    Dialog {
        width: parent.width - 20
        margins: 10
        standardButtons: Dialog.Ok
        id: snackbarOK
        Column {
            width: parent.width
            Label {
                id: snackbarOKT
                wrapMode: Label.Wrap
                width: parent.width
                text: ''
            }
        }
    }
    Dialog {
        width: parent.width - 20
        margins: 10
        standardButtons: Dialog.Ok
        id: snackbar
        title: "Ups..."
        Column {
            width: parent.width
            Label {
                id: snackbarT
                wrapMode: Label.Wrap
                width: parent.width
                text: ''
            }
        }
    }
    Dialog {
        id: aboutDialog
        width: parent.width - 20
        margins: 10
        standardButtons: Dialog.Ok
        Column {
            width: parent.width

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: 'Plant image recognition'
                font.weight: Font.Light
                font.pixelSize: 16
            }
            Rectangle {
                color: 'transparent'
                width: 1
                height: 10
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                onLinkActivated: Qt.openUrlExternally(link)
                text: "<a href='https://atlas.roslin.pl'>https://atlas.roslin.pl</a>"
            }
            Rectangle {
                color: 'transparent'
                width: 1
                height: 10
            }
            Label {
                wrapMode: Label.Wrap
                horizontalAlignment: Text.AlignJustify
                width: parent.width
                text: "App uses MXNet and resnet CNN model trained over plant images available in Internet."
            }
            Label {
                width: parent.width
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignJustify
                font.pixelSize: 12
                onLinkActivated: Qt.openUrlExternally(link)
                text: "Source code find on <a href='https://github.com/mjamroz/PlantRecognition'>https://github.com/mjamroz/PlantRecognition</a>"
            }
            Rectangle {
                color: 'transparent'
                width: 1
                height: 10
            }
            Label {
                width: parent.width
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignJustify
                onLinkActivated: Qt.openUrlExternally(link)
                font.pixelSize: 12
                text: "App may contains some bugs, let me know by <a href='mailto:robot@atlas.roslin.pl'>robot@atlas.roslin.pl</a>, and I'll try to fix it."
            }
        }
    }

    header: ToolBar {
        id: hdr
        RowLayout {
            spacing: 20
            anchors.fill: parent

            Label {
                id: titleLabel
                text: "Plant image recognition"
                font.pixelSize: 20
                color: 'white'
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                id: tb
                contentItem: Image {
                    id: neigh
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Recognize plant"
                        onTriggered: stackView.push(Qt.resolvedUrl("pages/IdentifyPlant.qml"))
                    }
                    MenuItem {
                        text: "Download data"
                        onTriggered: stackView.push(Qt.resolvedUrl("pages/DownloadOffline.qml"))
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: (MXNet.modelExists()) ? Qt.resolvedUrl("pages/IdentifyPlant.qml") :  stackView.push(Qt.resolvedUrl("pages/DownloadOffline.qml"))
    }
}
