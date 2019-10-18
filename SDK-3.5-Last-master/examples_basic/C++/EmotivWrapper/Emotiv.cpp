/*

Filename: Emotiv.cpp
C++ Wrapper for Emotiv SDK
Emotiv 2017
Author: Dinh Duong Ha ha@emotiv.com

*/

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#include "Emotiv.hpp"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"
#include "FacialExpressionDetection.h"
#include "MentalCommandDetection.h"

#if HAVE_EEG
#   include "IEegData.h"
#endif

#if HAVE_PM
#   include "IEmoStatePerformanceMetric.h"
#endif

#if HAVE_CLOUD
#   include "EmotivCloudClient.h"
#endif

namespace Emotiv {

class EmotivHeadsetPrivate
{
public:
    EmotivHeadsetPrivate(int headsetId = 0)
        : _emoState(std::shared_ptr<EmotivState>(new EmotivState(headsetId)))
    {
        _headsetId = headsetId;
        _motionHandle = IEE_MotionDataCreate();
#if HAVE_EEG
        _eegHandle = IEE_DataCreate();
#endif
    }
    virtual ~EmotivHeadsetPrivate()
    {
        if (_motionHandle) IEE_MotionDataFree(_motionHandle); _motionHandle = nullptr;
#if HAVE_EEG
        if (_eegHandle) IEE_DataFree(_eegHandle); _eegHandle = nullptr;
#endif
    }

    int _headsetId = 0; // UserID
    DataHandle _motionHandle = nullptr;
    DataHandle _eegHandle = nullptr;
    std::shared_ptr<EmotivState> _emoState;
    HeadsetConfig _config;
    unsigned long _version;
    unsigned long _serial;
    std::string _serialStr;
    unsigned int _gyroX; // Last gyroX value
    unsigned int _gyroY; // Last gyroY value
    unsigned long long _lastGyro = 0;
    bool _gyroStop = true;
};

class EmotivStatePrivate
{
public:
    EmotivStatePrivate(int headsetId)
    {
        _headsetId = headsetId;
       _emoStateHandle = IS_Create();
    }
    virtual ~EmotivStatePrivate()
    {
        if (_emoStateHandle) {
            IS_Free(_emoStateHandle);
        }
        _emoStateHandle = nullptr;
        _headsetId = 0;
    }
    int _headsetId = 0;
    EmoStateHandle _emoStateHandle = nullptr;
};




class EmotivEventHandlePrivate
{
public:
    EmotivEventHandlePrivate()
    {
        _eventHandle =  IEE_EmoEngineEventCreate();
    }
    virtual ~EmotivEventHandlePrivate()
    {
        if (_eventHandle) IEE_EmoEngineEventFree(_eventHandle);
    }
    EmoEngineEventHandle _eventHandle = nullptr;
};


EmotivCallback::EmotivCallback()
{
}

EmotivCallback::~EmotivCallback()
{
}
void EmotivCallback::onHeasetConnected(const std::shared_ptr<EmotivHeadset> headset)
{
    if (headset == nullptr) return;
}
void EmotivCallback::onHeasetDisconnected(const std::shared_ptr<EmotivHeadset> headset)
{
    if (headset == nullptr) return;
}

void EmotivCallback::onEmoStateUpdate(const std::shared_ptr<EmotivHeadset> headset, const std::shared_ptr<EmotivState> emoState)
{
    if (headset == nullptr) return;
}
void EmotivCallback::onFETrainingEvent(const std::shared_ptr<EmotivHeadset> headset, FETrainingEvent feEvent)
{
    if (headset == nullptr) return;
}

void EmotivCallback::onMCTrainingEvent(const std::shared_ptr<EmotivHeadset> headset, MCTrainingEvent mcEvent)
{
    if (headset == nullptr) return;
}

void EmotivCallback::onPMEvent(const std::shared_ptr<EmotivHeadset>headset, const std::shared_ptr<EmotivState> emoState)
{    
    if (headset == nullptr) return;
}

void EmotivCallback::onGyroData(const std::shared_ptr<EmotivHeadset> headset, int gyroX, int gyroY)
{
    if (headset == nullptr) return;
}

void EmotivCallback::onBandPowerData(const std::shared_ptr<EmotivHeadset> headset, const std::vector<BandPowerData> &bpData)
{
    if (headset == nullptr) return;
}

void EmotivCallback::onMotionData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
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
#if 0
    double *data = new double[nSamples];
    for (int i = 0; i < channelList.size(); ++i) {
        if (headset->GetMotionData(channelList.at(i), data, nSamples) == EDK_OK) {
            if (channelList.at(i) == MotionDataChannel::MD_COUNTER && nSamples > 0 ) {
                for (int j = 0; j < nSamples; ++j) {                    
                }
            }
        }
    }
    delete []data;
#endif
}


void EmotivCallback::onEEGData(const std::shared_ptr<EmotivHeadset> headset, unsigned int nSamples)
{
    if ((nSamples == 0) || headset == nullptr) return;
#if HAVE_EEG
    double *data = new double[nSamples];
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

    for (auto channel: channelList) {
        if (headset->GetEEGData(channel, data, nSamples) == EDK_OK) {
            if (channel == EegDataChannel::ED_COUNTER && nSamples > 0 ) {
                for (int j = 0; j < nSamples; ++j) {                    
                }
            }
        }
    }
    delete []data;
#endif
}

/*

EmotivHeadset

*/


EmotivHeadset::EmotivHeadset(int id)
    :d_ptr(new EmotivHeadsetPrivate(id))
{
}

int EmotivHeadset::HeadsetId()
{
    return d_ptr->_headsetId;
}

std::shared_ptr<EmotivState> EmotivHeadset::getEmoState()
{
    return d_ptr->_emoState;
}

int EmotivHeadset::GetConfig(HeadsetConfig &config)
{
    int headsetId = d_ptr->_headsetId;
    return IEE_GetHeadsetSettings(headsetId, &config.mode, &config.eegRate, &config.eegRes, &config.memsRate, &config.memsRes);
}

int EmotivHeadset::SetConfig(const HeadsetConfig &config)
{
    int headsetId = d_ptr->_headsetId;
    return IEE_SetHeadsetSettings(headsetId, config.mode,  config.eegRate, config.eegRes, config.memsRate, config.memsRes);
}

int EmotivHeadset::GetHardwareVersion(unsigned long &verison)
{
    return IEE_HardwareGetVersion(d_ptr->_headsetId, &verison);
}

int EmotivHeadset::GetSerialNumber(std::string& serial)
{
    char buff[50] = {0};
    char* pHwSerialOut = buff;
    int state = IEE_HeadsetGetSerialNumber(d_ptr->_headsetId, &pHwSerialOut);
    if (state == EDK_OK) {
        serial = std::string(pHwSerialOut, strlen(pHwSerialOut));
    }

    return state;
}

int EmotivHeadset::GyroGetDelta(int& pXOut, int& pYOut)
{
    int pX = 0;
    int pY = 0;
    int res = IEE_HeadsetGetGyroDelta(d_ptr->_headsetId, &pX, &pY);
    if (res == EDK_OK) {
        auto now = std::chrono::system_clock::now();
        unsigned long long millisecs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        if ((pX == 0) && (pY == 0)) {
            if (millisecs > d_ptr->_lastGyro + 1000) {
                if (d_ptr->_gyroStop == false) {
                    d_ptr->_gyroStop = true;
                    pXOut = 0;
                    pYOut = 0;
                } else {
                    return -1;
                }
            } else {
                return -1;
            }
        } else {
            d_ptr->_lastGyro = millisecs;
            d_ptr->_gyroStop = false;
        }
        d_ptr->_gyroX = pX;
        d_ptr->_gyroY = pY;
        pXOut = pX;
        pYOut = pY;
    }
    return res;
}

int EmotivHeadset::GyroRezero()
{
    return IEE_HeadsetGyroRezero(d_ptr->_headsetId);
}

int EmotivHeadset::FFTSetWindowingType(FFTWindowingTypes type)
{
    return IEE_FFTSetWindowingType(d_ptr->_headsetId, (IEE_WindowingTypes_enum)type);
}

int EmotivHeadset::FFTGetWindowingType(FFTWindowingTypes& type)
{
    IEE_WindowingTypes_enum typeTmp;
    int result = IEE_FFTGetWindowingType(d_ptr->_headsetId, (IEE_WindowingTypes_enum*)&typeTmp);
    type = (FFTWindowingTypes)typeTmp;
    return result;
}


int EmotivHeadset::FacialExpressionSetThreshold(FacialExpressionAlgo algoName, FEThreshold thresholdName, int value)
{
    return IEE_FacialExpressionSetThreshold(d_ptr->_headsetId, (IEE_FacialExpressionAlgo_t)algoName, (IEE_FacialExpressionThreshold_t)thresholdName, value);
}

int EmotivHeadset::FacialExpressionGetThreshold(FacialExpressionAlgo algoName, FEThreshold thresholdName, int &pValueOut)
{
    return IEE_FacialExpressionGetThreshold( d_ptr->_headsetId, (IEE_FacialExpressionAlgo_t)algoName, (IEE_FacialExpressionThreshold_t)thresholdName, &pValueOut);
}

int EmotivHeadset::FacialExpressionSetTrainingControl(FacialExpressionTrainingControl control)
{
    return IEE_FacialExpressionSetTrainingControl(d_ptr->_headsetId, (IEE_FacialExpressionTrainingControl_t)control);
}

int EmotivHeadset::FacialExpressionSetTrainingAction(FacialExpressionAlgo action)
{
    return IEE_FacialExpressionSetTrainingAction(d_ptr->_headsetId, (IEE_FacialExpressionAlgo_t)action);
}

int EmotivHeadset::FacialExpressionGetTrainingAction(FacialExpressionAlgo &pActionOut)
{
    return IEE_FacialExpressionGetTrainingAction(d_ptr->_headsetId, (IEE_FacialExpressionAlgo_t*)&pActionOut);
}

int EmotivHeadset::FacialExpressionGetTrainingTime(unsigned int &pTrainingTimeOut)
{
    return IEE_FacialExpressionGetTrainingTime(d_ptr->_headsetId, &pTrainingTimeOut);
}

int EmotivHeadset::FacialExpressionGetTrainedSignatureActions(unsigned long &pTrainedActionsOut)
{
    return IEE_FacialExpressionGetTrainedSignatureActions( d_ptr->_headsetId, &pTrainedActionsOut);
}

int EmotivHeadset::FacialExpressionGetTrainedSignatureAvailable(int &pfAvailableOut)
{
    return IEE_FacialExpressionGetTrainedSignatureAvailable(d_ptr->_headsetId, &pfAvailableOut);
}

int EmotivHeadset::FacialExpressionSetSignatureType(FESignature sigType)
{
    return IEE_FacialExpressionSetSignatureType( d_ptr->_headsetId, (IEE_FacialExpressionSignature_t)sigType);
}

int EmotivHeadset::FacialExpressionGetSignatureType(FESignature &pSigTypeOut)
{
    return IEE_FacialExpressionGetSignatureType(d_ptr->_headsetId, (IEE_FacialExpressionSignature_t*)&pSigTypeOut);
}

//////
int EmotivHeadset::MentalCommandSetActiveActions(unsigned long activeActions)
{
    return IEE_MentalCommandSetActiveActions(d_ptr->_headsetId, activeActions);
}
int EmotivHeadset::MentalCommandGetActiveActions(unsigned long &pActiveActionsOut)
{
    return IEE_MentalCommandGetActiveActions(d_ptr->_headsetId, &pActiveActionsOut);
}

int EmotivHeadset::MentalCommandGetTrainingTime(unsigned int &pTrainingTimeOut)
{
    return IEE_MentalCommandGetTrainingTime(d_ptr->_headsetId, &pTrainingTimeOut);
}
int EmotivHeadset::MentalCommandSetTrainingControl(MentalCommandTrainingControl control)
{
    return IEE_MentalCommandSetTrainingControl(d_ptr->_headsetId, (IEE_MentalCommandTrainingControl_t)control);
}
int EmotivHeadset::MentalCommandSetTrainingAction(MentalCommandAction action)
{
    return IEE_MentalCommandSetTrainingAction(d_ptr->_headsetId, (IEE_MentalCommandAction_t)action);
}

int EmotivHeadset::MentalCommandGetTrainingAction(MentalCommandAction &pActionOut)
{
    return IEE_MentalCommandGetTrainingAction(d_ptr->_headsetId, (IEE_MentalCommandAction_t*)&pActionOut);
}

int EmotivHeadset::MentalCommandGetTrainedSignatureActions(unsigned long &pTrainedActionsOut)
{
    return IEE_MentalCommandGetTrainedSignatureActions(d_ptr->_headsetId, &pTrainedActionsOut);
}

int EmotivHeadset::MentalCommandGetOverallSkillRating(float &pOverallSkillRatingOut)
{
    return IEE_MentalCommandGetOverallSkillRating(d_ptr->_headsetId, &pOverallSkillRatingOut);
}

int EmotivHeadset::MentalCommandGetActionSkillRating(MentalCommandAction action, float &pActionSkillRatingOut)
{
    return IEE_MentalCommandGetActionSkillRating(d_ptr->_headsetId, (IEE_MentalCommandAction_t)action, &pActionSkillRatingOut);
}

int EmotivHeadset::MentalCommandSetActivationLevel( int level)
{
    return IEE_MentalCommandSetActivationLevel(d_ptr->_headsetId, level);
}

int EmotivHeadset::MentalCommandSetActionSensitivity( int action1Sensitivity, int action2Sensitivity, int action3Sensitivity, int action4Sensitivity)
{
    return IEE_MentalCommandSetActionSensitivity(d_ptr->_headsetId, action1Sensitivity,  action2Sensitivity, action3Sensitivity, action4Sensitivity);
}

int EmotivHeadset::MentalCommandGetActivationLevel(int &pLevelOut)
{
    return IEE_MentalCommandGetActivationLevel(d_ptr->_headsetId, &pLevelOut);
}

int EmotivHeadset::MentalCommandGetActionSensitivity(int &pAction1SensitivityOut, int &pAction2SensitivityOut, int &pAction3SensitivityOut, int &pAction4SensitivityOut)
{
    return IEE_MentalCommandGetActionSensitivity(d_ptr->_headsetId, &pAction1SensitivityOut, &pAction2SensitivityOut, &pAction3SensitivityOut, &pAction4SensitivityOut);
}

int EmotivHeadset::MentalCommandStartSamplingNeutral()
{
    return IEE_MentalCommandStartSamplingNeutral(d_ptr->_headsetId);
}

int EmotivHeadset::MentalCommandStopSamplingNeutral()
{
    return IEE_MentalCommandStopSamplingNeutral(d_ptr->_headsetId);
}
int EmotivHeadset::MentalCommandSetSignatureCaching(unsigned int enabled)
{
    return IEE_MentalCommandSetSignatureCaching(d_ptr->_headsetId, enabled);
}
int EmotivHeadset::MentalCommandGetSignatureCaching(unsigned int &pEnabledOut)
{
    return IEE_MentalCommandGetSignatureCaching(d_ptr->_headsetId, &pEnabledOut);
}

int EmotivHeadset::MentalCommandSetSignatureCacheSize(unsigned int size)
{
    return IEE_MentalCommandSetSignatureCacheSize(d_ptr->_headsetId, size);
}

int EmotivHeadset::MentalCommandGetSignatureCacheSize(unsigned int &pSizeOut)
{
    return IEE_MentalCommandGetSignatureCacheSize(d_ptr->_headsetId, &pSizeOut);
}

int EmotivHeadset::MotionSamplingRate(unsigned int &samplingRateOut)
{
    return IEE_MotionDataGetSamplingRate(d_ptr->_headsetId, &samplingRateOut);
}

int EmotivHeadset::EEGSamplingRate(unsigned int &samplingRateOut)
{
#if HAVE_EEG
    return IEE_DataGetSamplingRate(d_ptr->_headsetId, &samplingRateOut);
#endif
    samplingRateOut = 0;
    return -1;
}

int EmotivHeadset::DataAcquisitionEnable(bool enable)
{
#if HAVE_EEG
    return IEE_DataAcquisitionEnable(d_ptr->_headsetId, enable);
#endif
    return false;
}

int EmotivHeadset::DataAcquisitionIsEnabled(bool &pEnableOut)
{
    pEnableOut = false;
#if HAVE_EEG
    return IEE_DataAcquisitionIsEnabled(d_ptr->_headsetId, &pEnableOut);
#endif
    return false;
}

int EmotivHeadset::DataSetSychronizationSignal(int signal)
{
#if HAVE_EEG
    return IEE_DataSetSychronizationSignal(d_ptr->_headsetId, signal);
#endif
    return -1;
}
int EmotivHeadset::DataSetMarker( int marker)
{
#if HAVE_EEG
    return IEE_DataSetMarker(d_ptr->_headsetId, marker);
#endif
    return -1;
}

int EmotivHeadset::SetMotionBufferSizeInSec(float secs)
{
    return IEE_MotionDataSetBufferSizeInSec(secs);
}
int EmotivHeadset::GetMotionBufferSizeInSec(float &secs)
{
    return IEE_MotionDataGetBufferSizeInSec(&secs);
}

int EmotivHeadset::SetEEGBufferSizeInSec(float secs)
{
#if HAVE_EEG
    return IEE_DataSetBufferSizeInSec(secs);
#endif
    return -1;
}
int EmotivHeadset::GetEEGBufferSizeInSec(float &secs)
{
#if HAVE_EEG
    return IEE_DataGetBufferSizeInSec(&secs);
#endif
    secs = 0;
    return -1;
}

int EmotivHeadset::UpdateMotionData(unsigned int &nSamples)
{
    nSamples = 0;
    int result = IEE_MotionDataUpdateHandle(d_ptr->_headsetId, d_ptr->_motionHandle);
    if (result == EDK_OK) {
        return IEE_MotionDataGetNumberOfSample(d_ptr->_motionHandle, &nSamples);
    }
    return result;
}

int EmotivHeadset::GetMotionData(MotionDataChannel channel, double buffer[], unsigned int bufferSizeInSample)
{
    return IEE_MotionDataGet(d_ptr->_motionHandle, (IEE_MotionDataChannel_t)channel, buffer, bufferSizeInSample);
}

int EmotivHeadset::UpdateEEGData(unsigned int &nSamples)
{
    int result = -1;
    nSamples = 0;
#if HAVE_EEG
    result =  IEE_DataUpdateHandle(d_ptr->_headsetId, d_ptr->_eegHandle);
    if (result == EDK_OK) {
        return IEE_DataGetNumberOfSample(d_ptr->_eegHandle, &nSamples);
    }
#endif
    return result;
}

int EmotivHeadset::GetEEGData(EegDataChannel channel, double buffer[], unsigned int bufferSizeInSample)
{
#if HAVE_EEG
    return IEE_DataGet(d_ptr->_eegHandle, (IEE_DataChannel_t)channel, buffer, bufferSizeInSample);
#endif
    return -1;
}

int EmotivHeadset::AverageBandPowers(EegDataChannel channel, double& theta, double& alpha, double& low_beta, double& high_beta, double& gamma)
{
    int result = -1;
    double _theta, _alpha, _low_beta, _high_beta, _gamma;
    result = IEE_GetAverageBandPowers(d_ptr->_headsetId, (IEE_DataChannel_t)channel, &_theta, &_alpha, &_low_beta, &_high_beta, &_gamma);
    theta = _theta ; alpha =_alpha; low_beta = _low_beta; high_beta = _high_beta; gamma = _gamma;
    return result;
}

//////////////////////////////////////////////////////////////////////////
/*

EmotivState

*/

EmotivState::EmotivState(unsigned int headsetId)
    :d_ptr(new EmotivStatePrivate(headsetId))
{

}

EmotivState::~EmotivState()
{
    if (d_ptr) {
        delete []d_ptr;
    }
}

EmoStateHandle EmotivState::getHandle()
{
    return d_ptr->_emoStateHandle;
}

int EmotivState::GetBatteryChargeLevel(int& level, int& maxLevel)
{
    int _level = 0, _max = 0;
    IS_GetBatteryChargeLevel(d_ptr->_emoStateHandle, &level, &_max);
    level = _level; maxLevel = _max;
    return level;
}

SignalStrength EmotivState::GetWirelessSignalStatus()
{
    return (SignalStrength)IS_GetWirelessSignalStatus(d_ptr->_emoStateHandle);
}

int EmotivState::GetNumContactQualityChannels()
{
    return IS_GetNumContactQualityChannels(d_ptr->_emoStateHandle);
}

ContactQuality EmotivState::GetContactQuality(InputChannel channel)
{
    return (ContactQuality)IS_GetContactQuality(d_ptr->_emoStateHandle, (IEE_InputChannels_t )channel);
}

float EmotivState::GetTimeFromStart()
{
    return IS_GetTimeFromStart(d_ptr->_emoStateHandle);
}

int EmotivState::GetHeadsetOn()
{
    return IS_GetHeadsetOn(d_ptr->_emoStateHandle);
}

int EmotivState::MentalCommandEqual(EmotivState& other)
{
    return IS_MentalCommandEqual(d_ptr->_emoStateHandle, other.getHandle());
}

MentalCommandAction EmotivState::MentalCommandGetCurrentAction()
{
    return (MentalCommandAction)IS_MentalCommandGetCurrentAction(d_ptr->_emoStateHandle);
}

float EmotivState::MentalCommandGetCurrentActionPower()
{
    return IS_MentalCommandGetCurrentActionPower(d_ptr->_emoStateHandle);
}
int EmotivState::MentalCommandIsActive()
{
    return IS_MentalCommandIsActive(d_ptr->_emoStateHandle);
}

float EmotivState::MentalCommandGetCurrentLevelRating()
{
    //return IS_MentalCommandGetCurrentLevelRating(d_ptr->_emoStateHandle);
    return 0;
}

void EmotivState::MentalCommandGetActionSkills(int &action1Skill, int &action2Skill, int &action3Skill, int & action4Skill)
{
    return;
    //return IS_MentalCommandGetActionSkills(d_ptr->_emoStateHandle, action1Skill, action2Skill, action3Skill, action4Skill);
}

int EmotivState::FacialExpressionIsBlink()
{
    return IS_FacialExpressionIsBlink(d_ptr->_emoStateHandle);
}
int EmotivState::FacialExpressionIsLeftWink()
{
    return IS_FacialExpressionIsLeftWink(d_ptr->_emoStateHandle);
}
int EmotivState::FacialExpressionIsRightWink()
{
    return IS_FacialExpressionIsRightWink(d_ptr->_emoStateHandle);
}

int EmotivState::FacialExpressionIsEyesOpen()
{
    return IS_FacialExpressionIsEyesOpen(d_ptr->_emoStateHandle);
}

int EmotivState::FacialExpressionIsLookingUp()
{
    return IS_FacialExpressionIsLookingUp(d_ptr->_emoStateHandle);
}
int EmotivState::FacialExpressionIsLookingDown()
{
    return IS_FacialExpressionIsLookingDown(d_ptr->_emoStateHandle);
}

int EmotivState::FacialExpressionIsLookingLeft()
{
    return IS_FacialExpressionIsLookingLeft(d_ptr->_emoStateHandle);
}

int EmotivState::FacialExpressionIsLookingRight()
{
    return IS_FacialExpressionIsLookingRight(d_ptr->_emoStateHandle);
}

void EmotivState::FacialExpressionGetEyelidState(float& leftEye, float& rightEye)
{
    IS_FacialExpressionGetEyelidState(d_ptr->_emoStateHandle, &leftEye, &rightEye);
}

void EmotivState::FacialExpressionGetEyeLocation(float& x, float& y)
{
    IS_FacialExpressionGetEyeLocation(d_ptr->_emoStateHandle, &x, &y);
}
float EmotivState::FacialExpressionGetEyebrowExtent()
{
    return IS_FacialExpressionGetEyebrowExtent(d_ptr->_emoStateHandle);
}
float EmotivState::FacialExpressionGetSmileExtent()
{
    return IS_FacialExpressionGetSmileExtent(d_ptr->_emoStateHandle);
}

float EmotivState::FacialExpressionGetClenchExtent()
{
    return IS_FacialExpressionGetClenchExtent(d_ptr->_emoStateHandle);
}

FacialExpressionAlgo EmotivState::FacialExpressionGetUpperFaceAction()
{
    return (FacialExpressionAlgo)IS_FacialExpressionGetUpperFaceAction(d_ptr->_emoStateHandle);
}

float EmotivState::FacialExpressionGetUpperFaceActionPower()
{
    return IS_FacialExpressionGetUpperFaceActionPower(d_ptr->_emoStateHandle);
}

FacialExpressionAlgo EmotivState::FacialExpressionGetLowerFaceAction()
{
    return (FacialExpressionAlgo)IS_FacialExpressionGetLowerFaceAction(d_ptr->_emoStateHandle);
}

float EmotivState::FacialExpressionGetLowerFaceActionPower()
{
    return IS_FacialExpressionGetLowerFaceActionPower(d_ptr->_emoStateHandle);
}

int EmotivState::FacialExpressionIsActive(FacialExpressionAlgo type)
{
    return IS_FacialExpressionIsActive(d_ptr->_emoStateHandle, (IEE_FacialExpressionAlgo_t)type);
}


int EmotivState::FacialExpressionEqual(EmotivState& other)
{
    return IS_FacialExpressionEqual(d_ptr->_emoStateHandle, other.getHandle());
}
int EmotivState::EmoEngineEqual(EmotivState& other)
{
    return IS_EmoEngineEqual(d_ptr->_emoStateHandle, other.getHandle());
}
int EmotivState::Equal(EmotivState& other)
{
    return IS_Equal(d_ptr->_emoStateHandle, other.getHandle());
}

void EmotivState::Copy(EmotivState& other)
{
    IS_Copy(d_ptr->_emoStateHandle, other.getHandle());
}

#if HAVE_PM

int EmotivState::PerformanceMetricIsActive(PerformanceMetricAlgo type)
{
    return IS_PerformanceMetricIsActive(d_ptr->_emoStateHandle, (IEE_PerformanceMetricAlgo_t)type);
}

void EmotivState::PerformanceMetricGetExcitementBands(float* band1, float* band2)
{
    *band1 = 0;
    *band2 = 0;
    //IS_PerformanceMetricGetExcitementBands(d_ptr->_emoStateHandle, band1, band2);
}

float EmotivState::PerformanceMetricGetExcitementLongTermScore()
{
    return IS_PerformanceMetricGetExcitementLongTermScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetInstantaneousExcitementScore()
{
    return IS_PerformanceMetricGetInstantaneousExcitementScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetRelaxationScore()
{
    return IS_PerformanceMetricGetRelaxationScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetEngagementBoredomScore()
{
    return IS_PerformanceMetricGetEngagementBoredomScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetStressScore()
{
    return IS_PerformanceMetricGetStressScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetInterestScore()
{
    return IS_PerformanceMetricGetInterestScore(d_ptr->_emoStateHandle);
}

float EmotivState::PerformanceMetricGetFocusScore()
{
    return IS_PerformanceMetricGetFocusScore(d_ptr->_emoStateHandle);
}

void EmotivState::PerformanceMetricGetInstantaneousExcitementModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetInstantaneousExcitementModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}
void EmotivState::PerformanceMetricGetRelaxationModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetRelaxationModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}
void EmotivState::PerformanceMetricGetEngagementBoredomModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetEngagementBoredomModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}
void EmotivState::PerformanceMetricGetStressModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetStressModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}
void EmotivState::PerformanceMetricGetInterestModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetInterestModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}
void EmotivState::PerformanceMetricGetFocusModelParams(double &rawScore, double &minScale, double &maxScale)
{
    IS_PerformanceMetricGetFocusModelParams(d_ptr->_emoStateHandle, &rawScore, &minScale, &maxScale);
}

int EmotivState::PerformanceMetricEqual(EmotivState& other)
{
    return IS_PerformanceMetricEqual(d_ptr->_emoStateHandle, other.getHandle());
}

#endif

//////////////////////////////////////////////////////////////////////////
/*

EmotivEventHandle

*/

EmotivEventHandle::EmotivEventHandle()
    :d_ptr(new EmotivEventHandlePrivate())
{

}
EmotivEventHandle::~EmotivEventHandle()
{
    if (d_ptr) {
        delete  []d_ptr;
    }
}

//EmoEngineEventHandle EmotivEventHandle::getHandle()
//{
//    if (d_ptr) {
//        return d_ptr->_eventHandle;
//    }
//    return nullptr;
//}

int EmotivEventHandle::NextEvent()
{
    return IEE_EngineGetNextEvent(d_ptr->_eventHandle);
}

unsigned int EmotivEventHandle::getHeadsetId()
{
    unsigned int id = 0xFFFFFFFF;
    IEE_EmoEngineEventGetUserId(d_ptr->_eventHandle, &id);
    return id;
}

int EmotivEventHandle::updateState(std::shared_ptr<EmotivState> emoState)
{
    int result = IEE_EmoEngineEventGetEmoState(d_ptr->_eventHandle, emoState->getHandle());
    return result;
}

IEE_Event_t EmotivEventHandle::getEventType()
{
    return IEE_EmoEngineEventGetType(d_ptr->_eventHandle);
}

MCTrainingEvent EmotivEventHandle::getMCEventType()
{
    return (MCTrainingEvent)IEE_MentalCommandEventGetType(d_ptr->_eventHandle);
}

FETrainingEvent EmotivEventHandle::getFEEventType()
{
    return (FETrainingEvent)IEE_FacialExpressionEventGetType(d_ptr->_eventHandle);
}

//////////////////////////////////////////////////////////////////////////
#if 0
class EmotivMotionDataPrivate
{
public:
    EmotivMotionDataPrivate(){}
    virtual ~EmotivMotionDataPrivate(){}
};
class EmotivEEGDataPrivate
{
public:
    EmotivEEGDataPrivate(){}
    virtual ~EmotivEEGDataPrivate(){}
};

class EmotivMotionData
{
public:
    EmotivMotionData();
    virtual ~EmotivMotionData();

    int Update(unsigned int headset);
    int GetNumberOfSample(unsigned int &nSampleOut);
    int GetData(IEE_MotionDataChannel_t channel, double buffer[], unsigned int bufferSizeInSample);
    int GetDataMultiChannels(IEE_MotionDataChannel_t channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample);

    static int SetBufferSizeInSec(float secs);
    static int GetBufferSizeInSec(float &secs);

private:
    DataHandle _motionHandle = nullptr;
};

#if HAVE_EEG
class EmotivEEGData
{
public:
    EmotivEEGData();
    virtual ~EmotivEEGData();

    int Update(unsigned int headset);
    int GetNumberOfSample(unsigned int &nSampleOut);
    int GetData(IEE_DataChannel_t channel, double buffer[], unsigned int bufferSizeInSample);
    int GetDataMultiChannels(IEE_DataChannel_t channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample);

    int SetBufferSizeInSec(float secs);
    int GetBufferSizeInSec(float &secs);


private:
    DataHandle _eegHandle = nullptr;
};
#endif
/*

Motion Data

*/
EmotivMotionData::EmotivMotionData()
{
    _motionHandle = IEE_MotionDataCreate();
}

EmotivMotionData::~EmotivMotionData()
{
    if (_motionHandle) IEE_MotionDataFree(_motionHandle); _motionHandle = nullptr;
}

int EmotivMotionData::SetBufferSizeInSec(float secs)
{
    return IEE_MotionDataSetBufferSizeInSec(secs);
}
int EmotivMotionData::GetBufferSizeInSec(float &secs)
{
    return IEE_MotionDataGetBufferSizeInSec(&secs);
}

int EmotivMotionData::Update(unsigned int headset)
{
    return IEE_MotionDataUpdateHandle(headset, _motionHandle);
}
int EmotivMotionData::GetNumberOfSample(unsigned int &nSampleOut)
{
    return IEE_MotionDataGetNumberOfSample(_motionHandle, &nSampleOut);
}

int EmotivMotionData::GetData(IEE_MotionDataChannel_t channel, double buffer[], unsigned int bufferSizeInSample)
{
    return IEE_MotionDataGet(_motionHandle, channel, buffer, bufferSizeInSample);
}

int EmotivMotionData::GetDataMultiChannels(IEE_MotionDataChannel_t channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample)
{
    return IEE_MotionDataGetMultiChannels(_motionHandle, channels, nChannels, buffer, bufferSizeInSample);
}


//////////////////////////////////////////////////////////////////////////
/*

EEG Data

*/
#if HAVE_EEG
EmotivEEGData::EmotivEEGData()
{
    _eegHandle = IEE_DataCreate();
}

EmotivEEGData::~EmotivEEGData()
{
    if (_eegHandle) IEE_DataFree(_eegHandle); _eegHandle = nullptr;
}

int EmotivEEGData::Update(unsigned int headset)
{
    return IEE_DataUpdateHandle(headset, _eegHandle);
}
int EmotivEEGData::GetNumberOfSample(unsigned int &nSampleOut)
{
    return IEE_DataGetNumberOfSample(_eegHandle, &nSampleOut);
}

int EmotivEEGData::GetData(IEE_DataChannel_t channel, double buffer[], unsigned int bufferSizeInSample)
{
    return IEE_DataGet(_eegHandle, channel, buffer, bufferSizeInSample);
}

int EmotivEEGData::GetDataMultiChannels(IEE_DataChannel_t channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample)
{
    return IEE_DataGetMultiChannels(_eegHandle, channels, nChannels, buffer, bufferSizeInSample);
}

int EmotivEEGData::SetBufferSizeInSec(float secs)
{
    return IEE_DataSetBufferSizeInSec(secs);
}
int EmotivEEGData::GetBufferSizeInSec(float &secs)
{
    return IEE_DataGetBufferSizeInSec(&secs);
}


#endif

#endif

//////////////////////////////////////////////////////////////////////////
/*

EmotivEngine

*/
class EmotivEnginePrivate
{
public:
    EmotivEnginePrivate(){}
    virtual ~EmotivEnginePrivate(){}
    EmotivEventHandle eventHandle;
    std::map<unsigned int, std::shared_ptr<EmotivHeadset> > emoHeadsets;
    std::mutex _mutex;
};

EmotivEngine::EmotivEngine(EmotivCallback* const cbk)
    :callback(cbk),d_ptr(new EmotivEnginePrivate())
{    
}

EmotivEngine::EmotivEngine()
    :d_ptr(new EmotivEnginePrivate())
{    
}

EmotivEngine::~EmotivEngine()
{
    Disconnect();
    if (d_ptr) {
        //delete []d_ptr;
    }
}

int EmotivEngine::Connect(const std::string& devname)
{
    int result =  IEE_EngineConnect(devname.c_str());
#if HAVE_CLOUD
    if (result == EDK_OK)
    {
        EC_Disconnect();
        int result = EC_Connect();
    }
#endif
    return result;
}

int EmotivEngine::RemoteConnect(const std::string host, unsigned short port)
{
    return IEE_EngineRemoteConnect(host.c_str(), port);
}


int EmotivEngine::Disconnect()
{
#if HAVE_CLOUD
    EC_Disconnect();
#endif
    return IEE_EngineDisconnect();
}

int EmotivEngine::HeadsetCount()
{
    unsigned int count = 0;
    IEE_EngineGetNumUser(&count);
    return count;

}

void EmotivEngine::EnableDetections(unsigned long value)
{
    IEE_EnableDetections(value);
}

unsigned long EmotivEngine::GetDetectionsEnabled()
{
    unsigned long value = 0;
    IEE_CheckDetectionsEnabled(&value);
    return value;
}

/*
IEE_EnableDiagnostics
IEE_GetEditionName
IEE_HeadsetGetSensorDetails
IEE_SoftwareGetVersion
IEE_SetHardwarePlayerDisplay
*/

std::shared_ptr<EmotivHeadset> EmotivEngine::getHeadset(int id)
{
    std::lock_guard<std::mutex> _lock(d_ptr->_mutex);
    return d_ptr->emoHeadsets[id];
}
bool EmotivEngine::addHeadset(std::shared_ptr<EmotivHeadset> headset)
{
    std::lock_guard<std::mutex> _lock(d_ptr->_mutex);
    d_ptr->emoHeadsets[headset->d_ptr->_headsetId] = headset;
    return true;
}

std::shared_ptr<EmotivHeadset> EmotivEngine::removeHeadset(int id)
{
    std::lock_guard<std::mutex> _lock(d_ptr->_mutex);
    std::shared_ptr<EmotivHeadset> tmp = d_ptr->emoHeadsets[id];
    d_ptr->emoHeadsets.erase(id);
    return tmp;
}

int EmotivEngine::EventLoop()
{

    int state = d_ptr->eventHandle.NextEvent();
    if (state == EDK_OK ) {
        return -1;
    }
    IEE_Event_t eventType = d_ptr->eventHandle.getEventType();
    unsigned int headsetId = d_ptr->eventHandle.getHeadsetId();
    switch(eventType) {
    case IEE_UnknownEvent:      //!< An unknown event.
        break;
    case IEE_EmulatorError:     //!< Error event from emulator. Connection to EmoComposer could be lost.
        break;
    case IEE_ReservedEvent:     //!< Reserved event.
        break;
    case IEE_InternalStateChanged://!< Reserved for internal use.
        break;
    case IEE_ProfileEvent:      //!< A profile has been returned from EmoEngine.
        break;
    case IEE_UserAdded:         //!< A headset is connected.
    {
        // Don't care
    }
    break;
    case IEE_UserRemoved:       //!< A headset has been disconnected.
    {
        std::shared_ptr<EmotivHeadset> headset  = getHeadset(headsetId);
        if (callback) {
            callback->onHeasetDisconnected(headset);
        }
        removeHeadset(headsetId);
    }
    break;
    case IEE_EmoStateUpdated:   //!< Detection results have been updated from EmoEngine.
    {
        std::shared_ptr<EmotivHeadset> headset  = getHeadset(headsetId);
        if (headset == nullptr) {
            unsigned long version = 0;
            int result = IEE_HardwareGetVersion(headsetId, &version);
            if ((result != EDK_OK) || version == 0) {
                break;
            }
            HeadsetConfig config;
            //IEE_HardwareGetVersion
            //IEE_GetHeadsetSettings
            headset = std::shared_ptr<EmotivHeadset>(new EmotivHeadset(headsetId));
            if (headset->GetConfig(config) != EDK_OK) {
                break;
            }            
            std::string serial;
            headset->GetSerialNumber(serial);
            if (serial.size() == 0) {
                break;
            }
            headset->d_ptr->_config = config;
            headset->d_ptr->_version = version;
            headset->d_ptr->_serialStr = serial;
            addHeadset(headset);
            headset->DataAcquisitionEnable(true);
            if (callback) {
                callback->onHeasetConnected(headset);
            }
        }
        if (headset) {
            std::shared_ptr<EmotivState> emoState = headset->getEmoState();
            d_ptr->eventHandle.updateState(emoState);
            if (callback) {
                callback->onEmoStateUpdate(headset, emoState);
                callback->onPMEvent(headset, emoState);
            }
        }
    }
    break;
    case IEE_MentalCommandEvent://!< A IEE_MentalCommandEvent_t has been returned from EmoEngine.
    {
        std::shared_ptr<EmotivHeadset> headset  = getHeadset(headsetId);
        if (headset) {
            auto mcEvent = d_ptr->eventHandle.getMCEventType();
            if (callback) {
                callback->onMCTrainingEvent(headset, mcEvent);
            }
        }
    }
    break;
    case IEE_FacialExpressionEvent://!< A IEE_FacialExpressionEvent_t has been returned from EmoEngine.
    {
        std::shared_ptr<EmotivHeadset> headset  = getHeadset(headsetId);
        if (headset != nullptr) {
            auto feEvent = d_ptr->eventHandle.getFEEventType();
            if (callback) {
                callback->onFETrainingEvent(headset, feEvent);
            }
        }
    }
    break;
    default:
        break;
    }
    return headsetId;
}
void EmotivEngine::DataLoop()
{
    for (auto it: d_ptr->emoHeadsets) {
        int gyroX = 0, gyroY = 0;
        std::shared_ptr<EmotivHeadset> headset = it.second;
        if (headset == nullptr) return;
        int err = headset->GyroGetDelta(gyroX, gyroY);
        if (err == EDK_OK) {
            if (callback) {
                callback->onGyroData(headset, gyroX, gyroY);
            }
        }
        /// 2. Band Power Data Examples
        std::vector<EegDataChannel> channelList={
            EegDataChannel::ED_AF3,
            EegDataChannel::ED_T7,
            EegDataChannel::ED_Pz,
            EegDataChannel::ED_T8,
            EegDataChannel::ED_AF4
        };

        std::vector<BandPowerData> bpDatas;
        bpDatas.reserve(channelList.size());
        for (auto channel: channelList) {
            //double theta = 0, alpha = 0, low_beta = 0, high_beta = 0, gamma = 0;
            BandPowerData data;
            if (headset->AverageBandPowers(channel, data.theta, data.alpha, data.low_beta, data.high_beta, data.gamma)==EDK_OK) {
                data.channel = channel;
                bpDatas.push_back(data);
            }
        }
        if (bpDatas.size() && callback) {
            callback->onBandPowerData(headset, bpDatas);
        }
        /// 3. Motion Data
        unsigned int nSamples = 0;
        if ((headset->UpdateMotionData(nSamples) == 0) && (nSamples > 0)) {
            if (callback) {
                callback->onMotionData(headset, nSamples);
            }
        }

        /// 4. Eeg Data
        nSamples = 0;
        if ((headset->UpdateEEGData(nSamples) == EDK_OK) && (nSamples > 0)) {
            if (callback) {
                callback->onEEGData(headset, nSamples);
            }
        }
    }
}

/// Facial Expression Training
int EmotivEngine::FEGetThreshold(unsigned int id, FacialExpressionAlgo algoName, FEThreshold thresholdName)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    int value = 0;
    headset->FacialExpressionGetThreshold((FacialExpressionAlgo) algoName, (FEThreshold) thresholdName, value);
    return value;
}

int EmotivEngine::FESetThreshold(unsigned int id, FacialExpressionAlgo algoName, FEThreshold thresholdName, int value)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    return headset->FacialExpressionSetThreshold((FacialExpressionAlgo) algoName, (FEThreshold) thresholdName, value);
}

FESignature EmotivEngine::FEGetSignatureType(unsigned int id)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return FESignature::FES_UNIVERSAL;
    }
    FESignature sign;
    headset->FacialExpressionGetSignatureType(sign);
    return sign;
}

int EmotivEngine::FESetSignatureType(unsigned int id, FESignature sign)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    return headset->FacialExpressionSetSignatureType((FESignature) sign);
}

unsigned long  EmotivEngine::FEGetSignatureActions(unsigned int id)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    unsigned long trained = 0;
    headset->FacialExpressionGetTrainedSignatureActions(trained);
    return trained;
}
int EmotivEngine::FEGetSignatureAvailable(unsigned int id)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    int value = 0;
    headset->FacialExpressionGetTrainedSignatureAvailable(value);
    return value;
}

unsigned int EmotivEngine::FEGetTrainingTime(unsigned int id)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    unsigned int value = 0;
    headset->FacialExpressionGetTrainingTime(value);
    return value;
}

FacialExpressionAlgo EmotivEngine::FEGetTrainingAction(unsigned int id)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return FacialExpressionAlgo::FEA_NEUTRAL;
    }
    FacialExpressionAlgo value;
    headset->FacialExpressionGetTrainingAction(value);
    return value;
}
int EmotivEngine::FESetTrainingAction(unsigned int id, FacialExpressionAlgo action)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    return headset->FacialExpressionSetTrainingAction(action);
}

int EmotivEngine::FETrainingControl(unsigned int id, FacialExpressionTrainingControl control)
{
    std::shared_ptr<EmotivHeadset> headset = getHeadset(id);
    if (headset == nullptr) {
        return -1;
    }
    return headset->FacialExpressionSetTrainingControl(control);
}

void EmotivEngine::run()
{
    while (1) {
        EventLoop();
        DataLoop();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

class EmotivUserPrivate
{
public:
    EmotivUserPrivate(){}
    virtual ~EmotivUserPrivate(){}
    std::map<int, std::shared_ptr<EmotivTrainingProfile>> trainingProfiles;
    std::string _username;
    std::string _password;
    std::string _token;
    std::string _clientId;
    std::string _clientPassword;
    int _userId = 0;
    bool _logged = false;
};

EmotivUser::EmotivUser()
    :d_ptr(new EmotivUserPrivate())
{

}

EmotivUser::EmotivUser(std::string &username, std::string &password)
    :d_ptr(new EmotivUserPrivate())
{
    d_ptr->_username = username;
    d_ptr->_password = password;
}

EmotivUser::~EmotivUser()
{
    if (d_ptr)
        delete []d_ptr;
    d_ptr =  nullptr;
}

void EmotivUser::setUserInfo(std::string &username, std::string &password)
{
    d_ptr->_username = username;
    d_ptr->_password = password;
}

int EmotivUser::login()
{
#if HAVE_CLOUD
    int result = 0;
    d_ptr->_logged = false;
    d_ptr->_token = "";
    d_ptr->_userId = 0;
    int cloudUserID = 0;
    result = EC_Login(d_ptr->_username.c_str(), d_ptr->_password.c_str());
    if (result != EDK_OK) {
        return result;
    }
    result = EC_GetUserDetail(&cloudUserID);
    if (result == EDK_OK) {
        d_ptr->_logged = true;
        d_ptr->_userId = cloudUserID;
    }
    return result;
#else
    return -1;
#endif
}

void EmotivUser::logout()
{
#if HAVE_CLOUD
    EC_Logout(d_ptr->_userId);
#endif
    d_ptr->_logged = false;
    d_ptr->_token = "";
    d_ptr->_userId = 0;

}

int EmotivUser::getUserId()
{
    return d_ptr->_userId;
}

bool EmotivUser::isLogged()
{
    return d_ptr->_logged;
}

int EmotivUser::FetchProfile()
{
#if HAVE_CLOUD
    int totalProfile = EC_GetAllProfileName(d_ptr->_userId);
    if (totalProfile <= 0) return 0;

    return totalProfile;
#else
    return -1;
#endif
}



} // namespace

