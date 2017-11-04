import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3

Item {
    Component.onCompleted: {
                if(MXNet.modelExists()) {
                    lbl1.text = "It seems you already downloaded the data. If by some reason image recognition doesn't work - try to remove and download again the data."
                }
    }

    Timer {
        interval: 1000
        id: checkProgress
        repeat: true
        running: false
        onTriggered: {
            var tot = downloader.getDownloadedCounter()
            total.text = "Downloaded " + tot + " files"
            if (tot === "4/4") {
                checkProgress.running = false
                snackbarOKT.text = "Data downloaded! Now you can use plant recognition algorithm totally offline"
                snackbarOK.open()
            }


        }
    }

    Column {
        width: parent.width
        Rectangle {
            height:80
            width:1
            color: "transparent"
        }

        Label {
            id: lbl1
            width: parent.width - 30
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignJustify
            font.pixelSize: 12
            text: "By pressing <strong>Download</strong>, your phone will download about <strong>385MB</strong> of data, required for plant recognition algorithm."
        }
        BusyIndicator {
            id: busy
            anchors.horizontalCenter: parent.horizontalCenter
            running: checkProgress.running
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: btn
                text: "Download"

                onClicked: {
                    checkProgress.running = true
                    downloader.removeFiles()
                    downloader.run()
                }

            }
            Rectangle {
                height:1
                width:10
                color: "transparent"
            }
            Button {
                id: btRemoven
                text: "Remove"

                onClicked: {
                    if (downloader.removeFiles()) {
                        snackbarOKT.text = "Data removed!"
                        snackbarOK.open()
                    }  else {
                        snackbarOKT.text = "Cannot remove the data :-()"
                        snackbarOK.open()

                    }
                }
            }
        }

        Label {
            id: total
            width: parent.width
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            text: ""
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
