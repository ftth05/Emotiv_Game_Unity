import QtQuick 2.7

Page1Form {

    button1.onClicked: {
        //console.log("Button Pressed. Entered text: " + textField1.text);
        console.log("Start Emotiv Engine");
        EmotivEngine.start();

    }
    button2.onClicked: {
        console.log("Stop Emotiv Engine");
        EmotivEngine.stop();
    }

    Connections {
            target: EmotivEngine
            onHeadsetConnected: {
                //console.log("EmotivEngine=>onHeasetConnected:" + headset);
                textField1.text = "Connected";
            }
            onHeadsetDisconnected: {
                //console.log("EmotivEngine=>onHeadsetDisconnected:" + headset);
                textField1.text = "Disconnected"
            }
            onUpdateGyro: {
                textGyroX.text = gyroX;
                textGyroY.text = gyroY;
            }
            onUpdateBandPower: {
                if (data[0] === 3.0) { //ED_AF3
                    textTheta.text = data[1];
                    textAlpha.text = data[2];
                    textLowBeta.text = data[3];
                    textHighBeta.text = data[4];
                    textGamma.text = data[5];
                }
            }
            onUpdateMems: {
                textMems0.text = data[0];
                textMems1.text = data[1];
                textMems2.text = data[2];
                textMems3.text = data[3];
                textMems4.text = data[4];
                textMems5.text = data[5];
                textMems6.text = data[6];
                textMems7.text = data[7];
                textMems8.text = data[8];
                textMems9.text = data[9];
                //console.log("EmotivEngine=>onUpdateMems, headset:" + headset+",samples:" + data[0] + ", dataLength:" + data.length);
            }
            onUpdateEeg: {
                textEeg0.text = data[0];    // Number
                //textEeg1.text = data[1];  //
                textEeg2.text = data[2];    // CQ
                textEeg3.text = data[3];    // AF3
                textEeg4.text = data[4];    // T7
                textEeg5.text = data[5];    // O1
                textEeg5.text = data[6];    // P8
                textEeg5.text = data[7];    // AF4
                //console.log("EmotivEngine=>onUpdateEeg, headset:" + headset+",samples:" + data[0] + ", dataLength:" + data.length);
            }

//            onUpdatePm: {
//                console.log("EmotivEngine=>onUpdatePm:" + headset);
//            }
//            onUpdateFETraining: {
//                console.log("EmotivEngine=>onUpdateFETraining:" + headset);
//            }
//            onUpdateMCTraining: {
//                console.log("EmotivEngine=>onUpdateMCTraining:" + headset);
//            }
        }
}
