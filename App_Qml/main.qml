import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    visible: true
    width: 800
    height: 480
    title: qsTr("Vehicle Monitor")

    property int speed: 0
    property int rpm: 0
    property int odo: 0
    property int trip: 0
    property string gear: "D"

    Rectangle {
        anchors.fill: parent
        color: "#202020"

        // Mặt đồng hồ tốc độ
        Image {
            id: speedImage
            source: "qrc:/images/SpeedMetter.png"
            width: 300
            height: 300
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        // Mặt đồng hồ vòng tua
        Image {
            id: rpmImage
            source: "qrc:/images/TachoMetter.png"
            width: 300
            height: 300
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        // Kim tốc độ
        Image {
            id: speedNeedle
            source: "qrc:/images/Indicator.png"
            width: 110
            height: 20
            anchors.centerIn: speedImage
            transform: Rotation {
                origin.x: 36
                origin.y: speedNeedle.height / 4
                angle: speed *1.1 + 143  // scale và dịch về góc bắt đầu = 0
            }
        }

        Image {
            id: rpmNeedle
            source: "qrc:/images/Indicator.png"
            width: 110
            height: 20
            anchors.centerIn: rpmImage
            transform: Rotation {
                origin.x: 36
                origin.y: rpmNeedle.height / 4
                angle: rpm * 30 + 143   // RPM 0 → -120°, RPM 8 → 120°
            }
        }


        Rectangle {
            width: parent.width
            height: 140
            anchors.bottom: parent.bottom
            color: "#303030"
            radius: 10
            border.color: "#505050"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 40

                // Gear
                Column {
                    spacing: 4
                    Layout.preferredWidth: 100
                    Text {
                        text: "Gear"
                        font.pixelSize: 20
                        color: "#AAAAAA"
                    }
                    Text {
                        text: gear
                        font.pixelSize: 26
                        font.bold: true
                        color: "#00BFFF"
                    }
                }

                // Speed
                Column {
                    spacing: 4
                    Layout.preferredWidth: 150
                    Text {
                        text: "Speed (km/h)"
                        font.pixelSize: 20
                        color: "#AAAAAA"
                    }
                    Text {
                        text: speed
                        font.pixelSize: 26
                        font.bold: true
                        color: "white"
                    }
                }

                // RPM
                Column {
                    spacing: 4
                    Layout.preferredWidth: 150
                    Text {
                        text: "RPM (x1000)"
                        font.pixelSize: 20
                        color: "#AAAAAA"
                    }
                    Text {
                        text: rpm
                        font.pixelSize: 26
                        font.bold: true
                        color: "white"
                    }
                }

                // ODO
                Column {
                    spacing: 4
                    Layout.preferredWidth: 150
                    Text {
                        text: "ODO (km)"
                        font.pixelSize: 20
                        color: "#AAAAAA"
                    }
                    Text {
                        text: odo
                        font.pixelSize: 26
                        font.bold: true
                        color: "lightgreen"
                    }
                }

                // Trip
                Column {
                    spacing: 4
                    Layout.preferredWidth: 150
                    Text {
                        text: "Trip (km)"
                        font.pixelSize: 20
                        color: "#AAAAAA"
                    }
                    Text {
                        text: trip
                        font.pixelSize: 26
                        font.bold: true
                        color: "orange"
                    }
                }
            }
        }

    }

    // Đọc dữ liệu từ ESP32
    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: SerialHandler.readData()
    }

    // Nhận tín hiệu cập nhật từ SerialHandler
    Connections {
        target: SerialHandler
        onUpdateData: function(speedVal, rpmVal, odoVal, tripVal, gearVal) {
            speed = speedVal
            rpm = rpmVal
            odo = odoVal
            trip = tripVal
            gear = gearVal
        }
    }
}
