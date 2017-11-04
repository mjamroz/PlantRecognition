/*
# Copyright (C) 2016 Michal Jamroz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
import QtQuick.Layouts 1.3
import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {

    id: item

    ListModel {
        id: sampleModel
    }
    ListView {
        anchors.fill: parent
        spacing: 15
        anchors.margins: 12
        id: sampleListView
        model: MXNet.things
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        orientation: ListView.Horizontal

        add: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0
                to: 1.0
                duration: 400
            }
            NumberAnimation {
                property: "scale"
                from: 0
                to: 1.0
                duration: 400
            }
        }
        displaced: Transition {
            NumberAnimation {
                properties: "x,y"
                duration: 400
                easing.type: Easing.OutBounce
            }
        }
        delegate: Pane {
            width: item.width - 40
            height: item.height - 40
            id: recta
            Material.elevation: 6

            Column {
                id: column
                width: parent.width
                spacing: 8

                Label {
                    id: name_latinska
                    font.weight: Font.ExtraBold
                    font.pixelSize: 22
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: modelData.name_lat
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    width: parent.width
                    height: 0.71 * item.height
                    id: mainimg
                    clip: false
                    cache: false
                    asynchronous: false
                    source: modelData.img

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //stackView.push(Qt.resolvedUrl("PlantView.qml"), {plant_id: modelData.plant_id})
                        }
                    }
                }
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.weight: Font.ExtraLight
                    font.pixelSize: 8
                    text: 'Photo author: ' + modelData.author
                }
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.weight: Font.ExtraLight
                    font.pixelSize: 8
                    text: 'Licence: '+modelData.lic
                }
            }
        }
    }
}
