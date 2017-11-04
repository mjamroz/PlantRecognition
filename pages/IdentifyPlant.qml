import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import AndroidNative 1.0 as AN
import QtGraphicalEffects 1.0

Item {
    AN.ImagePicker {
        id: imagePicker
        multiple: false
    }
    Component.onCompleted: {
       // predictLocally()
    }
    id: item
    property bool camera: false
    property string path: ''

    ColumnLayout {
        id: column
        width: parent.width
        spacing: 10
        Rectangle {
            height: 20
            width:1
            color: "transparent"
        }
    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        smooth: true
        visible: imagePicker.imageUrl.length === 0
        height: 50
        source: "qrc:/icons/action_fingerprint.png"
        id: img
        layer.enabled: true
        layer.effect: ColorOverlay {
            anchors.fill: img
            source: img
            color: '#3F51B5'
        }
    }

        BusyIndicator {
            id: progress
            visible: imagePicker.imageUrl.length > 0
            anchors.horizontalCenter: parent.horizontalCenter
            running: true
        }
        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            smooth: true
            id: image
            source: imagePicker.imageUrl
            fillMode: Image.PreserveAspectFit
            Layout.maximumHeight: window.height * 0.6
            visible: imagePicker.imageUrl.length > 0
        }
        RowLayout {
            id: row
            visible: imagePicker.imageUrl.length === 0
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true

            spacing: 2
            anchors.margins: 10

            Button {
                id: b2
                text: "Take a photo"
                onClicked: {
                    imagePicker.takePhoto()
                    item.camera = true
                    check_selection.running = true
                }
            }
            Button {
                id: b1
                text: "Choose a photo"
                onClicked: {
                    imagePicker.pickImage()
                    item.camera = false
                    check_selection.running = true
                }
            }
        }
    }
    Timer {
        interval: 100
        id: check_selection
        repeat: true
        running: false
        onTriggered: {
            if (imagePicker.imageUrls.length > 0 && MXNet.modelExists())
                    predictLocally()
        }
    }

    function predictLocally() {
        check_selection.stop()
        progress.visible = true
        var path_ = decodeURIComponent(imagePicker.imageUrl).replace("file:/", "file://")
        //var path_ = "file://home/nme/QPredict/image.jpg"
        item.path = '/' + path_.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/, "")
        MXNet.predict(item.path)
        progress.visible = false
        stackView.push(Qt.resolvedUrl('LocalRecognitionResults.qml'), { })

    }
}
