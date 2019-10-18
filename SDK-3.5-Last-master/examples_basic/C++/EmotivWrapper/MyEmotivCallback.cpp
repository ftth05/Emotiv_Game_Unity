#include <string.h>
#include <memory>
#include <iostream>
#include <string>
#include "MyEmotivCallback.hpp"
using namespace Emotiv;

MyEmotivCallback::MyEmotivCallback()
    :Emotiv::EmotivCallback()
{

}

MyEmotivCallback::~MyEmotivCallback()
{

}

void MyEmotivCallback::onHeasetConnected(const std::shared_ptr<EmotivHeadset> headset)
{
    if (headset == nullptr) return;
    std::cout << "Headset " << headset->HeadsetId() << " MyEmotivCallback::onHeasetConnected\r\n";
    headsets[headset->HeadsetId()] = headset;
    headset->DataAcquisitionEnable(true);
    headset->FFTSetWindowingType(FFTWindowingTypes::FFT_WT_HAMMING);
}

void MyEmotivCallback::onHeasetDisconnected(const std::shared_ptr<EmotivHeadset> headset)
{
    if (headset == nullptr) return;
    std::cout << "Headset " << headset->HeadsetId() << " MyEmotivCallback::onHeasetDisconnected\r\n";
    headsets.erase(headset->HeadsetId());
}

void MyEmotivCallback::UpdateEmoState(const std::shared_ptr<EmotivHeadset> headset, std::shared_ptr<EmotivState> emoState)
{

    /// EmoStateLogger Functions
    /// 1. Headset Information
    auto systemUpTime = emoState->GetTimeFromStart();
    auto wirelessStrength = emoState->GetWirelessSignalStatus();
    int batteryLevel = 0, maxBatteryLevel = 0;
    std::map<InputChannel, ContactQuality> cq;
    if (wirelessStrength != SignalStrength::SS_NO_SIG)
    {
        // Get battery level
        emoState->GetBatteryChargeLevel(batteryLevel, maxBatteryLevel);
        std::vector<InputChannel> channelList={
            InputChannel::CHAN_AF3,
            InputChannel::CHAN_T7,
            InputChannel::CHAN_Pz,
            InputChannel::CHAN_T8,
            InputChannel::CHAN_AF4
        };
        for (auto channel: channelList) {
            // Get contact quality per channel
            cq[channel] = emoState->GetContactQuality(channel);
        }
    }

    /// 2. Get Current Mental Command
    auto mcAction = emoState->MentalCommandGetCurrentAction();
    auto mcPower = emoState->MentalCommandGetCurrentActionPower();
    auto mcRate = emoState->MentalCommandGetCurrentLevelRating();
    int action1Skill = 0, action2Skill = 0, action3Skill = 0, action4Skill = 0;
    emoState->MentalCommandGetActionSkills(action1Skill, action2Skill, action3Skill, action4Skill);
    unsigned int mcTrainingTime;
    headset->MentalCommandGetTrainingTime(mcTrainingTime);

    /// 3. Get Current Facial Expression
    unsigned int feTrainingTime;
    headset->FacialExpressionGetTrainingTime(feTrainingTime);
    auto feLowerAction = emoState->FacialExpressionGetLowerFaceAction();
    auto feLowerPower = emoState->FacialExpressionGetLowerFaceActionPower();
    auto feUpperAction = emoState->FacialExpressionGetUpperFaceAction();
    auto feUpperPower = emoState->FacialExpressionGetUpperFaceActionPower();

    float eyeLeft = 0, eyeRight = 0;
    float eyeLidLeft = 0, eyeLidRight = 0;
    emoState->FacialExpressionGetEyeLocation(eyeLeft, eyeRight);
    emoState->FacialExpressionGetEyelidState(eyeLidLeft, eyeLidRight);

    auto feClench = emoState->FacialExpressionGetClenchExtent();
    auto feEyebrow = emoState->FacialExpressionGetEyebrowExtent();
    auto feSmile = emoState->FacialExpressionGetSmileExtent();

    auto isBlink = emoState->FacialExpressionIsBlink();
    auto isLeftWink = emoState->FacialExpressionIsLeftWink();
    auto isRightWink = emoState->FacialExpressionIsRightWink();
    auto isEyeOpen = emoState->FacialExpressionIsEyesOpen();
    auto isLookingUp = emoState->FacialExpressionIsLookingUp();
    auto isLookingDown = emoState->FacialExpressionIsLookingDown();
    auto isLookingLeft = emoState->FacialExpressionIsLookingLeft();
    auto isLookingRight = emoState->FacialExpressionIsLookingRight();
}

void MyEmotivCallback::onEmoStateUpdate(const std::shared_ptr<EmotivHeadset> headset, const std::shared_ptr<EmotivState> emoState)
{
    // Headset Information / EmoStateLogger / Facial Expression / Mental Command Examples
    if (headset == nullptr) return;
    UpdateEmoState(headset, emoState);
}

void MyEmotivCallback::onPMEvent(const std::shared_ptr<EmotivHeadset>headset, const std::shared_ptr<EmotivState> emoState)
{
    if (headset == nullptr) return;
    std::cout << "Headset " << headset->HeadsetId() << " MyEmotivCallback::onPMEvent\r\n";
#if HAVE_PM
    emoState->PerformanceMetricGetInstantaneousExcitementScore();
    emoState->PerformanceMetricGetRelaxationScore();
    emoState->PerformanceMetricGetEngagementBoredomScore();
    emoState->PerformanceMetricGetStressScore();
    emoState->PerformanceMetricGetInterestScore();
    emoState->PerformanceMetricGetFocusScore();
#endif
}

void MyEmotivCallback::onBandPowerData(const std::shared_ptr<Emotiv::EmotivHeadset> headset, const std::vector<Emotiv::BandPowerData>& data)
{
    if (headset == nullptr) return;
    std::cout << "Headset " << headset->HeadsetId() << " MyEmotivCallback::onBandPowerData\r\n";
}

void MyEmotivCallback::onMotionData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
{
    if ((nSamples == 0) || headset == nullptr) return;
   std::vector<MotionDataChannel> channelList={
       MotionDataChannel::MD_COUNTER,        //!< Sample counter
       MotionDataChannel::MD_GYROX,              //!< Gyroscope X-axis
       MotionDataChannel::MD_GYROY,              //!< Gyroscope Y-axis
       MotionDataChannel::MD_GYROZ,              //!< Gyroscope Z-axis
       MotionDataChannel::MD_ACCX,               //!< Accelerometer X-axis
       MotionDataChannel::MD_ACCY,               //!< Accelerometer Y-axis
       MotionDataChannel::MD_ACCZ,               //!< Accelerometer Z-axis
       MotionDataChannel::MD_MAGX,               //!< Magnetometer X-axis
       MotionDataChannel::MD_MAGY,               //!< Magnetometer Y-axis
       MotionDataChannel::MD_MAGZ,               //!< Magnetometer Z-axis
       MotionDataChannel::MD_TIMESTAMP           //!< Timestamp of the motion data stream
   };
   int cols = channelList.size();
   double *data = new double[nSamples*channelList.size()];
   memset(data, 0, sizeof(double)*nSamples*channelList.size());
   double *dataTmp = new double[nSamples];
   for (auto i = 0; i < channelList.size(); ++i) {
       memset(dataTmp, 0, sizeof(double)*nSamples);
       if (headset->GetMotionData(channelList.at(i), dataTmp, nSamples) == EDK_OK) {
           for (auto j = 0; j < nSamples; ++j) {
               data[j*cols+i] = dataTmp[j];
           }
       }
   }
   // Log Motion Data to screen
#if 1
   for (auto row = 0; row < nSamples; ++row) {
       for (auto col = 0; col < channelList.size(); ++col) {
           std::cout << data[row*cols + col] << ",";
       }
       std::cout << "\r\n";
   }
#endif
   delete []dataTmp;
   delete []data;
}

void MyEmotivCallback::onEEGData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
{
    if ((nSamples == 0) || headset == nullptr) return;
#if HAVE_EEG
    std::vector<EegDataChannel> channelList={
        EegDataChannel::ED_COUNTER,        //!< Sample counter
        EegDataChannel::ED_INTERPOLATED,       //!< Indicate if data is interpolated
        EegDataChannel::ED_RAW_CQ,             //!< Raw contact quality value
        EegDataChannel::ED_AF3,                //!< Channel AF3
        EegDataChannel::ED_T7,                 //!< Channel T7
        EegDataChannel::ED_O1,                 //!< Channel O1
        EegDataChannel::ED_P8,                 //!< Channel P8
        EegDataChannel::ED_AF4,                //!< Channel AF4
        EegDataChannel::ED_TIMESTAMP,          //!< System timestamp
        EegDataChannel::ED_MARKER_HARDWARE    //!< Marker from extender
    };
    int cols = channelList.size();
    double *data = new double[nSamples*channelList.size()];
    memset(data, 0, sizeof(double)*nSamples*channelList.size());
    double *dataTmp = new double[nSamples];

    for (int i = 0; i < channelList.size(); ++i) {
        memset(dataTmp, 0, sizeof(double)*nSamples);
        if (headset->GetEEGData(channelList.at(i), dataTmp, nSamples) == EDK_OK) {
            for (int j = 0; j < nSamples; ++j) {
                data[j*cols+i] = dataTmp[j];
            }
        }
    }
    // Log EEG Data to screen
    for (int row = 0; row < nSamples; row++) {
        for (int col = 0; col < channelList.size(); col++) {
            std::cout << data[row*cols + col] << ",";
        }
        std::cout << "\r\n";
    }
    delete []dataTmp;
    delete []data;
#endif
}
