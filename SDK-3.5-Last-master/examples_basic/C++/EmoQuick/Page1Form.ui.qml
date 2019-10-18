import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    property alias textField1: textField1
    property alias button1: button1
    property alias button2: button2

    property alias textGyroX: textGyroX
    property alias textGyroY: textGyroY

    property alias textTheta: textTheta
    property alias textAlpha: textAlpha
    property alias textLowBeta: textLowBeta
    property alias textHighBeta: textHighBeta
    property alias textGamma: textGamma

    property alias textMems0: textMems0
    property alias textMems1: textMems1
    property alias textMems2: textMems2
    property alias textMems3: textMems3
    property alias textMems4: textMems4
    property alias textMems5: textMems5
    property alias textMems6: textMems6
    property alias textMems7: textMems7
    property alias textMems8: textMems8
    property alias textMems9: textMems9


    property alias textEeg2: textEeg2
    property alias textEeg3: textEeg3
    property alias textEeg4: textEeg4
    property alias textEeg5: textEeg5
    property alias textEeg6: textEeg6
    property alias textEeg7: textEeg7


    ColumnLayout {
    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        anchors.top: parent.top

        TextField {
            id: textField1
            placeholderText: qsTr("Text Field")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }

        Button {
            id: button1
            text: qsTr("Start")
        }
        Button {
            id: button2
            text: qsTr("Stop")
        }
    }
    RowLayout {
        Label {
            id: textGyro
            text: qsTr("Gyro")
            color: "#21be2b"
            width: 40
            height: 20
            font.pixelSize: 15
        }
        TextField {
            id: textGyroX
            placeholderText: qsTr("GyroX")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textGyroY
            placeholderText: qsTr("GyroY")
            background: Rectangle {
              implicitWidth: 40
              implicitHeight: 25
              border.color: "#353637"
            }
        }
    }
    RowLayout {
        Label {
            id: textBandPower
            text: qsTr("BandPower")
            color: "#21be2b"
            width: 40
            height: 20
            font.pixelSize: 15
        }
        TextField {
            id: textTheta
            placeholderText: qsTr("Theta")
            background: Rectangle {
                implicitWidth: 150
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textAlpha
            placeholderText: qsTr("Alpha")
            background: Rectangle {
              implicitWidth: 150
              implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textLowBeta
            placeholderText: qsTr("Low Beta")
            background: Rectangle {
                implicitWidth: 150
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textHighBeta
            placeholderText: qsTr("High Beta")
            background: Rectangle {
              implicitWidth: 150
              implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textGamma
            placeholderText: qsTr("Gamma")
            background: Rectangle {
              implicitWidth: 150
              implicitHeight: 25
              border.color: "#353637"
            }
        }
    }
    RowLayout {
        Label {
            id: textMems
            text: qsTr("MEMS")
            color: "#21be2b"
            width: 40
            height: 20
        }
        TextField {
            id: textMems0
            placeholderText: qsTr("Number")
            background: Rectangle {
              implicitWidth: 40
              implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textMems1
            placeholderText: qsTr("GYROX")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textMems2
            placeholderText: qsTr("GYROY")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textMems3
            placeholderText: qsTr("GYROZ")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textMems4
            placeholderText: qsTr("ACCX")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textMems5
            placeholderText: qsTr("ACCY")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textMems6
            placeholderText: qsTr("ACCZ")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textMems7
            placeholderText: qsTr("MAGX")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textMems8
            placeholderText: qsTr("MAGY")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textMems9
            placeholderText: qsTr("MAGZ")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
    }

    RowLayout {
        Label {
            id: textEeg
            text: qsTr("EEG")
            color: "#21be2b"
            width: 40
            height: 20
        }
        TextField {
            id: textEeg0
            placeholderText: qsTr("Number")
            background: Rectangle {
              implicitWidth: 40
              implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textEeg2
            placeholderText: qsTr("CQ")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textEeg3
            placeholderText: qsTr("AF3")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textEeg4
            placeholderText: qsTr("T7")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
        TextField {
            id: textEeg5
            placeholderText: qsTr("O1")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textEeg6
            placeholderText: qsTr("P8")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#21be2b"
            }
        }
        TextField {
            id: textEeg7
            placeholderText: qsTr("AF4")
            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 25
              border.color: "#353637"
            }
        }
    }
    }


}
