import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Gyro - BandPower - MEMS - EEG ")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        Page {
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
/*
    Connections {
            target: EmotivEngine
            onHeadsetConnected: {
                console.log("EmotivEngine=>onHeasetConnected:" + headset);
            }
            onHeadsetDisconnected: {
                console.log("EmotivEngine=>onHeadsetDisconnected:" + headset);
            }
            onUpdateGyro: {
                console.log("EmotivEngine=>onUpdateGyro:" + headset + ":gyroY:" + gyroX + ",gyroY:" + gyroY);
            }
            onUpdateMems: {
                //console.log("EmotivEngine=>onUpdateMems, headset:" + headset+",samples:" + samples + ", dataLength:" + data.length);
            }
            onUpdateEeg: {
                console.log("EmotivEngine=>onUpdateEeg, headset:" + headset+",samples:" + samples + ", dataLength:" + data.length);
            }
            onUpdateBandPower: {
                //console.log("EmotivEngine=>onUpdateBandPower:" + headset+ ":" + bpInfo.ichannel+ "," + bpInfo.theta + "," + bpInfo.alpha + "," + bpInfo.low_beta + "," + bpInfo.high_beta + "," + bpInfo.gamma);
                //console.log("EmotivEngine=>onUpdateBandPower");
            }
            onUpdatePm: {
                console.log("EmotivEngine=>onUpdatePm:" + headset);
            }
            onUpdateFETraining: {
                console.log("EmotivEngine=>onUpdateFETraining:" + headset);
            }
            onUpdateMCTraining: {
                console.log("EmotivEngine=>onUpdateMCTraining:" + headset);
            }
        }
*/
}
