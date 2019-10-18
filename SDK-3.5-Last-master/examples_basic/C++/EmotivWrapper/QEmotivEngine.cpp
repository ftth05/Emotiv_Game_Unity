/*

Filename: QEmotivEngine.hpp
QT C++ Wrapper for Emotiv SDK
Emotiv 2017
Author: Dinh Duong Ha ha@emotiv.com

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>    
#endif

#include <QVector>

#include "Emotiv.hpp"
#include "QEmotivEngine.hpp"

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

#if HAVE_EEG
#   include "IEegData.h"
#endif

#if HAVE_PM
#   include "IEmoStatePerformanceMetric.h"
#endif

#if HAVE_CLOUD
#include "EmotivCloudClient.h"
#endif

using namespace Emotiv;

namespace Emotiv {
class QEmotivEngine;
class QEmotivEnginePrivate
{
    //Q_DECLARE_PUBLIC(QEmotivEngine)
public:
    explicit QEmotivEnginePrivate(QEmotivEngine * const callback);
    virtual ~QEmotivEnginePrivate();
    //QEmotivEngine * const q_ptr;
    Emotiv::EmotivEngine emoEngine;
    std::map<unsigned int, std::shared_ptr<EmotivHeadset>> headsets;
    int timerId = 0;
    bool started = false;
    int headsetId = -1;
};
}
QEmotivEnginePrivate::QEmotivEnginePrivate(QEmotivEngine* const callback)
    :emoEngine(callback)
{
}

QEmotivEnginePrivate::~QEmotivEnginePrivate()
{

}
QEmotivEngine::QEmotivEngine(QObject* parent)
    :QObject(parent), d_ptr(new QEmotivEnginePrivate(this))
{

}

QEmotivEngine::~QEmotivEngine()
{

}

void QEmotivEngine::onHeasetConnected(const std::shared_ptr<EmotivHeadset> headset)
{
    Q_D(QEmotivEngine);
    if (headset == nullptr) return;
    d->headsets[headset->HeadsetId()] = headset;
    d->headsetId = headset->HeadsetId();
    headset->DataAcquisitionEnable(true);
    headset->FFTSetWindowingType(FFTWindowingTypes::FFT_WT_HAMMING);

    emit headsetConnected(headset->HeadsetId());
}

void QEmotivEngine::onHeasetDisconnected(const std::shared_ptr<EmotivHeadset> headset)
{
    Q_D(QEmotivEngine);
    if (headset == nullptr) return;
    d->headsets.erase(headset->HeadsetId());
    d->headsetId = -1;
    emit headsetDisconnected(headset->HeadsetId());
}

void QEmotivEngine::UpdateEmoState(const std::shared_ptr<EmotivHeadset> headset, const std::shared_ptr<EmotivState> emoState)
{
    Q_D(QEmotivEngine);
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

void QEmotivEngine::onEmoStateUpdate(const std::shared_ptr<EmotivHeadset> headset, const std::shared_ptr<EmotivState> emoState)
{
    Q_D(QEmotivEngine);
    // Headset Information / EmoStateLogger / Facial Expression / Mental Command Examples
    if (headset == nullptr) return;
    UpdateEmoState(headset, emoState);
}

void QEmotivEngine::onGyroData(const std::shared_ptr<EmotivHeadset> headset, int gyroX, int gyroY)
{
    emit updateGyro(headset->HeadsetId(), gyroX, gyroY);
}


void QEmotivEngine::onBandPowerData(const std::shared_ptr<EmotivHeadset> headset, const std::vector<BandPowerData>& bpDatas)
{
    for (int i = 0; i < bpDatas.size(); ++i) {
        QVector<double> vectorData;
        vectorData.resize(6);
        BandPowerData data = bpDatas.at(i);
        vectorData[0] = (double)data.channel;
        vectorData[1] = data.theta;
        vectorData[2] = data.alpha;
        vectorData[3] = data.low_beta;
        vectorData[4] = data.high_beta;
        vectorData[5] = data.gamma;
        emit updateBandPower(headset->HeadsetId(), vectorData);
    }
}

void QEmotivEngine::onMotionData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
{
    Q_D(QEmotivEngine);
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
   QVector<double> vectorData;
   vectorData.resize(nSamples*channelList.size());
   double *dataTmp = new double[nSamples];
   for (auto i = 0; i < channelList.size(); ++i) {
       memset(dataTmp, 0, sizeof(double)*nSamples);
       if (headset->GetMotionData(channelList.at(i), dataTmp, nSamples) == EDK_OK) {
           for (auto j = 0; j < nSamples; ++j) {
               vectorData[j*cols+i] = dataTmp[j];
           }
       }
   }   
   for (auto row = 0; row < nSamples; ++row) {
       QVector<double> data;
       data.resize(channelList.size());
       for (auto col = 0; col < channelList.size(); ++col) {
           data[col] = vectorData[row*cols + col];
       }
       // Samples One by one
       emit updateMems(headset->HeadsetId(), data);
   }
   delete []dataTmp;

}

void QEmotivEngine::onEEGData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
{
    Q_D(QEmotivEngine);
    if ((nSamples == 0) || headset == nullptr) return;
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
    QVector<double> vectorData;
    vectorData.resize(nSamples*channelList.size());
    double *dataTmp = new double[nSamples];
#if HAVE_EEG
    int cols = channelList.size();
    for (int i = 0; i < channelList.size(); ++i) {
        memset(dataTmp, 0, sizeof(double)*nSamples);
        if (headset->GetEEGData(channelList.at(i), dataTmp, nSamples) == EDK_OK) {
            for (int j = 0; j < nSamples; ++j) {
                vectorData[j*cols+i] = dataTmp[j];
            }
        }
    }
    for (auto row = 0; row < nSamples; ++row) {
        QVector<double> data;
        data.resize(channelList.size());
        for (auto col = 0; col < channelList.size(); ++col) {
            data[col] = vectorData[row*cols + col];
        }
        emit updateEeg(headset->HeadsetId(), data);
    }
#endif
    delete []dataTmp;
}



void QEmotivEngine::onFETrainingEvent(std::shared_ptr<EmotivHeadset> headset, FETrainingEvent feEvent)
{
    Q_D(QEmotivEngine);
    if (headset == nullptr) return;
}

void QEmotivEngine::onMCTrainingEvent(std::shared_ptr<EmotivHeadset> headset, MCTrainingEvent mcEvent)
{
    Q_D(QEmotivEngine);
    if (headset == nullptr) return;
}

void QEmotivEngine::onPMEvent(std::shared_ptr<EmotivHeadset>headset, std::shared_ptr<EmotivState> emoState)
{
    Q_D(QEmotivEngine);
    if (headset == nullptr) return;
#if HAVE_PM
    //std::cout << "Headset " << headset->HeadsetId() << " PM event\r\n";
    emoState->PerformanceMetricGetInstantaneousExcitementScore();
    emoState->PerformanceMetricGetRelaxationScore();
    emoState->PerformanceMetricGetEngagementBoredomScore();
    emoState->PerformanceMetricGetStressScore();
    emoState->PerformanceMetricGetInterestScore();
    emoState->PerformanceMetricGetFocusScore();
#endif
}


void QEmotivEngine::loop()
{
    Q_D(QEmotivEngine);
    int headsetId = d->emoEngine.EventLoop();
    d->emoEngine.DataLoop();
    std::shared_ptr<EmotivHeadset> headset = d->emoEngine.getHeadset(headsetId);
    if (headset != nullptr) {

    }
}

void QEmotivEngine::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    Q_D(QEmotivEngine);
    loop();
}

void QEmotivEngine::start()
{
    Q_D(QEmotivEngine);
    if (!d->started) {
        if (d->emoEngine.Connect() != EDK_OK) {
            return ;
        }
        d->timerId = startTimer(5, Qt::CoarseTimer);
        d->started =  true;
    }
}

void QEmotivEngine::stop()
{
    Q_D(QEmotivEngine);
    if (d->started) {
        killTimer(d->timerId);
        d->started = false;
        d->emoEngine.Disconnect();
    }
}

/// Facial Expression Training
int QEmotivEngine::feGetThreshold(unsigned int id, int algoName, int thresholdName)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FEGetThreshold(d->headsetId, (FacialExpressionAlgo)algoName, (FEThreshold)thresholdName);
}

int QEmotivEngine::feSetThreshold(unsigned int id, int algoName, int thresholdName, int value)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FESetThreshold(d->headsetId, (FacialExpressionAlgo)algoName, (FEThreshold)thresholdName, value);
}

int QEmotivEngine::feGetSignatureType(unsigned int id)
{
    Q_D(QEmotivEngine);
    return (int)d->emoEngine.FEGetSignatureType(d->headsetId);
}

int QEmotivEngine::feSetSignatureType(unsigned int id, int sign)
{
    Q_D(QEmotivEngine);
   return d->emoEngine.FESetSignatureType(d->headsetId, (FESignature)sign);
}

int QEmotivEngine::feGetSignatureActions(unsigned int id)
{
    Q_D(QEmotivEngine);
    return (int)d->emoEngine.FEGetSignatureActions(d->headsetId);
}

int QEmotivEngine::feGetSignatureAvailable(unsigned int id)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FEGetSignatureAvailable(d->headsetId);
}

unsigned int QEmotivEngine::feGetTrainingTime(unsigned int id)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FEGetTrainingTime(d->headsetId);
}
int QEmotivEngine::feGetTrainingAction(unsigned int id)
{
    Q_D(QEmotivEngine);
    return (int)d->emoEngine.FEGetTrainingAction(d->headsetId);
}
int QEmotivEngine::feSetTrainingAction(unsigned int id, int action)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FESetTrainingAction(d->headsetId, (FacialExpressionAlgo)action);
}
int QEmotivEngine::feTrainingControl(unsigned int id, int control)
{
    Q_D(QEmotivEngine);
    return d->emoEngine.FETrainingControl(d->headsetId, (FacialExpressionTrainingControl)control);
}


/// Mental Command Training
