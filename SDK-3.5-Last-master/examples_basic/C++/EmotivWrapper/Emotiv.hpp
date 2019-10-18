/*

Filename: Emotiv.hpp
C++ Wrapper for Emotiv SDK
Emotiv 2017
Author: Dinh Duong Ha ha@emotiv.com

*/

#ifndef __EMOTIV_HPP__
#define __EMOTIV_HPP__

#include <map>
#include <vector>
#include <memory>

#define HAVE_EEG    0   // Have eeg data API
#define HAVE_PM     0   // Have Performance Metrics API
#define HAVE_CLOUD  0   // Have Cloud API

#ifndef HAVE_EEG
#   define HAVE_EEG   0
#endif

#ifndef HAVE_PM
#   define HAVE_PM    0
#endif

#ifndef HAVE_CLOUD
#   define HAVE_CLOUD 0
#endif

#include "Iedk.h"

namespace Emotiv {

class EmotivHeadsetPrivate;
class EmotivStatePrivate;
class EmotivEventHandlePrivate;
class EmotivEnginePrivate;

class EmotivHeadset;
class EmotivState;
class EmotivEventHandle;
class EmotivEngine;

struct HeadsetConfig
{
    unsigned int mode = 0;
    unsigned int eegRate = 0;
    unsigned int eegRes = 0;
    unsigned int memsRate = 0;
    unsigned int memsRes = 0;
};

/// Signal
//! Wireless Signal Strength
enum class SignalStrength {
    SS_NO_SIG = 0,
    SS_BAD_SIG,
    SS_GOOD_SIG
};

//! Logical input channel identifiers
enum class InputChannel {

    CHAN_CMS = 0,
    CHAN_DRL,
    CHAN_FP1,
    CHAN_AF3,
    CHAN_F7,
    CHAN_F3,
    CHAN_FC5,
    CHAN_T7,
    CHAN_P7,
    CHAN_Pz,
    CHAN_O1 = CHAN_Pz,
    CHAN_O2,
    CHAN_P8,
    CHAN_T8,
    CHAN_FC6,
    CHAN_F4,
    CHAN_F8,
    CHAN_AF4,
    CHAN_FP2,
};

//! EEG Electrode Contact Quality enumeration
enum class ContactQuality {

    CQ_NO_SIGNAL,
    CQ_VERY_BAD,
    CQ_POOR,
    CQ_FAIR,
    CQ_GOOD
};

////////////////////////////////////////////////////////////////////////
/// Facial Expression Functions

//! FacialExpression Suite threshold type enumerator
enum class FEThreshold {
    FET_SENSITIVITY  //!< Sensitivity of each facial expression
};

//! FacialExpression Suite signature type enumerator
enum class FESignature {
    FES_UNIVERSAL = 0,   //!< Use built-in universal signature
    FES_TRAINED          //!< Use custom trained signature
};

//! FacialExpression Suite training control enumerator
enum class FacialExpressionTrainingControl {
    FEC_NONE = 0,    //!< No action
    FEC_START,       //!< Start a new training
    FEC_ACCEPT,      //!< Accept current training
    FEC_REJECT,      //!< Reject current training
    FEC_ERASE,       //!< Erase training data for a facial expression
    FEC_RESET        //!< Reset current training
};

//! FacialExpression event types
enum class FETrainingEvent{
    FEE_TrainingNoEvent = 0,        //!< No new event
    FEE_TrainingStarted,    //!< The training has begun after FE_START is sent.
    FEE_TrainingSucceeded,  //!< The training is succeeded, waiting for FE_ACCEPT or FE_REJECT.
    FEE_TrainingFailed,     //!< The training is failed due to signal issues. Please restart training.
    FEE_TrainingCompleted,  //!< The training is completed after FE_ACCEPT is sent.
    FEE_TrainingDataErased, //!< The training data for a particular facial expression has been erased by FE_ERASE.
    FEE_TrainingRejected,   //!< The training is rejected after FE_REJECT is sent.
    FEE_TrainingReset       //!< The training has been reset after FE_RESET is sent.
};

//! FacialExpression facial expression type enumerator
enum class FacialExpressionAlgo {

    FEA_NEUTRAL    = 0x0001,
    FEA_BLINK      = 0x0002,
    FEA_WINK_LEFT  = 0x0004,
    FEA_WINK_RIGHT = 0x0008,
    FEA_HORIEYE    = 0x0010,
    FEA_SURPRISE   = 0x0020,
    FEA_FROWN      = 0x0040,
    FEA_SMILE      = 0x0080,
    FEA_CLENCH     = 0x0100,

    FEA_LAUGH      = 0x0200,
    FEA_SMIRK_LEFT = 0x0400,
    FEA_SMIRK_RIGHT= 0x0800

};

////////////////////////////////////////////////////////////////////////
/// Mental Command Functions

//! MentalCommand action type enumerator
enum class MentalCommandAction {
    MCA_NEUTRAL                  = 0x0001,
    MCA_PUSH                     = 0x0002,
    MCA_PULL                     = 0x0004,
    MCA_LIFT                     = 0x0008,
    MCA_DROP                     = 0x0010,
    MCA_LEFT                     = 0x0020,
    MCA_RIGHT                    = 0x0040,
    MCA_ROTATE_LEFT              = 0x0080,
    MCA_ROTATE_RIGHT             = 0x0100,
    MCA_ROTATE_CLOCKWISE         = 0x0200,
    MCA_ROTATE_COUNTER_CLOCKWISE = 0x0400,
    MCA_ROTATE_FORWARDS          = 0x0800,
    MCA_ROTATE_REVERSE           = 0x1000,
    MCA_DISAPPEAR                = 0x2000

};

//! MentalCommand Suite training control enumerator
enum class MentalCommandTrainingControl {
    MCC_NONE = 0,    //!< No action
    MCC_START,       //!< Start a new training
    MCC_ACCEPT,      //!< Accept current training
    MCC_REJECT,      //!< Reject current training
    MCC_ERASE,       //!< Erase training data for an action
    MCC_RESET        //!< Reset current training
};


//! MentalCommand event types
enum class MCTrainingEvent{
    MCE_TrainingNoEvent = 0,                   //!< No new event
    MCE_TrainingStarted,               //!< The training has begun after MC_START is sent.
    MCE_TrainingSucceeded,             //!< The training is succeeded, waiting for MC_ACCEPT or MC_REJECT.
    MCE_TrainingFailed,                //!< The training is failed due to signal issues. Please restart training.
    MCE_TrainingCompleted,             //!< The training is completed after MC_ACCEPT is sent.
    MCE_TrainingDataErased,            //!< The training data for a particular action has been erased by MC_ERASE.
    MCE_TrainingRejected,              //!< The training is rejected after MC_REJECT is sent.
    MCE_TrainingReset,                 //!< The training has been reset after MC_RESET is sent.
    MCE_TrainingAutoSamplingNeutral,  //!< The neutral training is completed.
    MCE_TrainingSignatureUpdated               //!< The mental command signature has been updated for new actions.
};

//! PerformanceMetric emotional type enumerator
enum class PerformanceMetricAlgo {

    PMA_EXCITEMENT = 0x0001,
    PMA_RELAXATION = 0x0002,
    PMA_STRESS     = 0x0004,
    PMA_ENGAGEMENT = 0x0008,

    PMA_INTEREST   = 0x0010,
    PMA_FOCUS      = 0x0020

};

////////////////////////////////////////////////////////////////////////
/// Motion/Eeg/BandPower Data Functions

//! Motion data channel description
enum class MotionDataChannel {
    MD_COUNTER = 0,        //!< Sample counter
    MD_GYROX,              //!< Gyroscope X-axis
    MD_GYROY,              //!< Gyroscope Y-axis
    MD_GYROZ,              //!< Gyroscope Z-axis
    MD_ACCX,               //!< Accelerometer X-axis
    MD_ACCY,               //!< Accelerometer Y-axis
    MD_ACCZ,               //!< Accelerometer Z-axis
    MD_MAGX,               //!< Magnetometer X-axis
    MD_MAGY,               //!< Magnetometer Y-axis
    MD_MAGZ,               //!< Magnetometer Z-axis
    MD_TIMESTAMP           //!< Timestamp of the motion data stream
};


//! EEG and system data channel description
enum class EegDataChannel {
    ED_COUNTER = 0,        //!< Sample counter
    ED_INTERPOLATED,       //!< Indicate if data is interpolated
    ED_RAW_CQ,             //!< Raw contact quality value
    ED_AF3,                //!< Channel AF3
    ED_F7,                 //!< Channel F7
    ED_F3,                 //!< Channel F3
    ED_FC5,                //!< Channel FC5
    ED_T7,                 //!< Channel T7
    ED_P7,                 //!< Channel P7
    ED_Pz,                 //!< Channel Pz
    ED_O1 = ED_Pz,        //!< Channel O1
    ED_O2,                 //!< Channel O2
    ED_P8,                 //!< Channel P8
    ED_T8,                 //!< Channel T8
    ED_FC6,                //!< Channel FC6
    ED_F4,                 //!< Channel F4
    ED_F8,                 //!< Channel F8
    ED_AF4,                //!< Channel AF4
    ED_GYROX,              //!< Gyroscope X-axis
    ED_GYROY,              //!< Gyroscope Y-axis
    ED_TIMESTAMP,          //!< System timestamp
    ED_MARKER_HARDWARE,    //!< Marker from extender
    ED_ES_TIMESTAMP,       //!< EmoState timestamp
    ED_FUNC_ID,            //!< Reserved function id
    ED_FUNC_VALUE,         //!< Reserved function value
    ED_MARKER,             //!< Marker value from hardware
    ED_SYNC_SIGNAL         //!< Synchronisation signal
};


//! Windowing types enum for Fast Fourier Transform
enum class FFTWindowingTypes {
    FFT_WT_HANNING   = 0,      //!< Hanning Window
    FFT_WT_HAMMING   = 1,      //!< Hamming Window
    FFT_WT_HANN      = 2,      //!< Hann Window
    FFT_WT_BLACKMAN  = 3,      //!< Blackman-Harris Window
    FFT_WT_RECTANGLE = 4       //!< Uniform/rectangular Window
};


struct BandPowerData
{
    EegDataChannel channel = EegDataChannel::ED_AF3;
    double theta = 0;
    double alpha = 0;
    double low_beta = 0;
    double high_beta = 0;
    double gamma = 0;
};

////////////////////////////////////////////////////////////////////////
/// \brief The EmotivCallback class
///
///
class EmotivCallback
{
public:
    explicit EmotivCallback();
    virtual ~EmotivCallback();
    virtual void onHeasetConnected( const std::shared_ptr<EmotivHeadset>);
    virtual void onHeasetDisconnected( const std::shared_ptr<EmotivHeadset>);
    virtual void onEmoStateUpdate( const std::shared_ptr<EmotivHeadset>, const std::shared_ptr<EmotivState> emoState);
    virtual void onPMEvent( const std::shared_ptr<EmotivHeadset>, const std::shared_ptr<EmotivState> emoState);

    virtual void onFETrainingEvent( const std::shared_ptr<EmotivHeadset>, FETrainingEvent);
    virtual void onMCTrainingEvent( const std::shared_ptr<EmotivHeadset>, MCTrainingEvent);

    virtual void onGyroData( const std::shared_ptr<EmotivHeadset>, int gyroX, int gyroY);
    virtual void onBandPowerData( const std::shared_ptr<EmotivHeadset>, const std::vector<BandPowerData>& data);
    virtual void onMotionData( const std::shared_ptr<EmotivHeadset>, unsigned int nSamples);
    virtual void onEEGData( const std::shared_ptr<EmotivHeadset>, unsigned int nSamples);
};

class EmotivHeadset
{
public:

    EmotivHeadset(int id);
    int HeadsetId();

    int GetConfig(HeadsetConfig &config);
    int SetConfig(const HeadsetConfig &config);

    int GetHardwareVersion(unsigned long &verion);
    int GetSerialNumber(std::string& serial);

    int GyroGetDelta(int& pXOut, int& pYOut);
    int GyroRezero();
    int FFTSetWindowingType(FFTWindowingTypes type);
    int FFTGetWindowingType(FFTWindowingTypes& type);


    int FacialExpressionSetThreshold(FacialExpressionAlgo algoName, FEThreshold thresholdName, int value);
    int FacialExpressionGetThreshold(FacialExpressionAlgo algoName, FEThreshold thresholdName, int &pValueOut);
    int FacialExpressionSetTrainingControl(FacialExpressionTrainingControl control);
    int FacialExpressionSetTrainingAction(FacialExpressionAlgo action);
    int FacialExpressionGetTrainingAction(FacialExpressionAlgo &pActionOut);
    int FacialExpressionGetTrainingTime(unsigned int &pTrainingTimeOut);
    int FacialExpressionGetTrainedSignatureActions(unsigned long &pTrainedActionsOut);
    int FacialExpressionGetTrainedSignatureAvailable(int &pfAvailableOut);
    int FacialExpressionSetSignatureType(FESignature sigType);
    int FacialExpressionGetSignatureType(FESignature &pSigTypeOut);

    int MentalCommandSetActiveActions(unsigned long activeActions);
    int MentalCommandGetActiveActions(unsigned long &pActiveActionsOut);
    int MentalCommandGetTrainingTime(unsigned int &pTrainingTimeOut);
    int MentalCommandSetTrainingControl(MentalCommandTrainingControl control);
    int MentalCommandSetTrainingAction(MentalCommandAction action);
    int MentalCommandGetTrainingAction(MentalCommandAction &pActionOut);
    int MentalCommandGetTrainedSignatureActions(unsigned long &pTrainedActionsOut);
    int MentalCommandGetOverallSkillRating(float &pOverallSkillRatingOut);
    int MentalCommandGetActionSkillRating(MentalCommandAction action, float &pActionSkillRatingOut);
    int MentalCommandSetActivationLevel( int level);
    int MentalCommandSetActionSensitivity(int action1Sensitivity, int action2Sensitivity, int action3Sensitivity, int action4Sensitivity);
    int MentalCommandGetActivationLevel(int &pLevelOut);
    int MentalCommandGetActionSensitivity(int &pAction1SensitivityOut, int &pAction2SensitivityOut, int &pAction3SensitivityOut, int &pAction4SensitivityOut);
    int MentalCommandStartSamplingNeutral();
    int MentalCommandStopSamplingNeutral();
    int MentalCommandSetSignatureCaching(unsigned int enabled);
    int MentalCommandGetSignatureCaching(unsigned int &pEnabledOut);
    int MentalCommandSetSignatureCacheSize(unsigned int size);
    int MentalCommandGetSignatureCacheSize(unsigned int &pSizeOut);

    int MotionSamplingRate(unsigned int &samplingRateOut);
    int EEGSamplingRate(unsigned int &samplingRateOut);

    int DataAcquisitionEnable(bool enable);
    int DataAcquisitionIsEnabled(bool &pEnableOut);
    int DataSetSychronizationSignal(int signal);
    int DataSetMarker(int marker);

    std::shared_ptr<EmotivState> getEmoState();

    static int SetMotionBufferSizeInSec(float secs);
    static int GetMotionBufferSizeInSec(float &secs);

    static int SetEEGBufferSizeInSec(float secs);
    static int GetEEGBufferSizeInSec(float &secs);

    int UpdateMotionData(unsigned int &nSamples);
    int GetMotionData(MotionDataChannel channel, double buffer[], unsigned int bufferSizeInSample);
    int GetMotionMultiChannelsData(MotionDataChannel channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample);

    int UpdateEEGData(unsigned int &nSamples);
    int GetEEGData(EegDataChannel channel, double buffer[], unsigned int bufferSizeInSample);
    int GetEEGMultiChannelsData(EegDataChannel channels[], unsigned int nChannels, double * buffer[], unsigned int bufferSizeInSample);
    int AverageBandPowers(EegDataChannel channel, double& theta, double& alpha, double& low_beta, double& high_beta, double& gamma);
private:
    friend class EmotivEngine;
    EmotivHeadsetPrivate *d_ptr = nullptr;

};

class EmotivState
{
public:
    EmotivState(unsigned int headsetId = 0);
    virtual ~EmotivState();

    int GetBatteryChargeLevel(int& level, int& maxLevel);
    SignalStrength GetWirelessSignalStatus();
    ContactQuality GetContactQuality(InputChannel channel);

    float GetTimeFromStart();
    int GetHeadsetOn();
    int GetNumContactQualityChannels();

    int FacialExpressionIsActive(FacialExpressionAlgo type);
    int FacialExpressionIsBlink();
    int FacialExpressionIsLeftWink();
    int FacialExpressionIsRightWink();

    int FacialExpressionIsEyesOpen();
    int FacialExpressionIsLookingUp();
    int FacialExpressionIsLookingDown();
    int FacialExpressionIsLookingLeft();
    int FacialExpressionIsLookingRight();
    void FacialExpressionGetEyelidState(float& leftEye, float& rightEye);
    void FacialExpressionGetEyeLocation(float& x, float& y);

    float FacialExpressionGetEyebrowExtent();
    float FacialExpressionGetSmileExtent();
    float FacialExpressionGetClenchExtent();

    FacialExpressionAlgo FacialExpressionGetUpperFaceAction();
    float FacialExpressionGetUpperFaceActionPower();
    FacialExpressionAlgo FacialExpressionGetLowerFaceAction();
    float FacialExpressionGetLowerFaceActionPower();

    int MentalCommandIsActive();
    MentalCommandAction MentalCommandGetCurrentAction();
    float MentalCommandGetCurrentActionPower();
    float MentalCommandGetCurrentLevelRating();
    void MentalCommandGetActionSkills(int &action1Skill, int &action2Skill, int &action3Skill, int &action4Skill);

#if HAVE_PM
    int PerformanceMetricIsActive(PerformanceMetricAlgo type);
    int PerformanceMetricEqual(EmotivState& other);

    void PerformanceMetricGetExcitementBands(float *band1, float *band2);
    float PerformanceMetricGetExcitementLongTermScore();

    float PerformanceMetricGetInstantaneousExcitementScore();    
    float PerformanceMetricGetRelaxationScore();
    float PerformanceMetricGetEngagementBoredomScore();
    float PerformanceMetricGetStressScore();    
    float PerformanceMetricGetInterestScore();
    float PerformanceMetricGetFocusScore();

    void PerformanceMetricGetInstantaneousExcitementModelParams(double &rawScore, double &minScale, double &maxScale);    
    void PerformanceMetricGetRelaxationModelParams(double &rawScore, double &minScale, double &maxScale);
    void PerformanceMetricGetEngagementBoredomModelParams(double &rawScore, double &minScale, double &maxScale);
    void PerformanceMetricGetStressModelParams(double &rawScore, double &minScale, double &maxScale);
    void PerformanceMetricGetInterestModelParams(double &rawScore, double &minScale, double &maxScale);
    void PerformanceMetricGetFocusModelParams(double &rawScore, double &minScale, double &maxScale);

#endif
protected:
    EmoStateHandle getHandle();
private:
    friend class EmotivEventHandle;
    EmotivStatePrivate *d_ptr = nullptr;

    int FacialExpressionEqual(EmotivState& other);
    int MentalCommandEqual(EmotivState& other);
    int EmoEngineEqual(EmotivState& other);
    int Equal(EmotivState& other);
    void Copy(EmotivState& other);
};

class EmotivEventHandle
{
public:
    EmotivEventHandle();
    virtual ~EmotivEventHandle();

    int NextEvent();

    IEE_Event_t getEventType();
    MCTrainingEvent getMCEventType();
    FETrainingEvent getFEEventType();
    unsigned int getHeadsetId(); // Get user id    
    int updateState(std::shared_ptr<EmotivState> emoState);
protected:

private:
    EmotivEventHandlePrivate *d_ptr = nullptr;
};

class EmotivEngine
{
    public:
        EmotivEngine(EmotivCallback* const cbk);
        EmotivEngine();
        virtual ~EmotivEngine();

        int Connect(const std::string& devname = "Emotiv Systems-5");
        int RemoteConnect(const std::string host, unsigned short port);
        int Disconnect();        
        int HeadsetCount();

        std::shared_ptr<EmotivHeadset> getHeadset(int id);
        bool addHeadset(std::shared_ptr<EmotivHeadset>);
        std::shared_ptr<EmotivHeadset> removeHeadset(int id);

        //IEE_Detection_enum
        void EnableDetections(unsigned long value);
        unsigned long GetDetectionsEnabled();

        virtual int EventLoop();
        virtual void DataLoop();
        virtual void run();

        /// Facial Expresion Training Functions
        int FEGetThreshold(unsigned int id, FacialExpressionAlgo algoName, FEThreshold thresholdName);
        int FESetThreshold(unsigned int id, FacialExpressionAlgo algoName, FEThreshold thresholdName, int value);
        virtual FESignature FEGetSignatureType(unsigned int headset);
        virtual int FESetSignatureType(unsigned int headset, FESignature sign);
        virtual unsigned long FEGetSignatureActions(unsigned int headset);
        virtual int FEGetSignatureAvailable(unsigned int headset);

        virtual unsigned int FEGetTrainingTime(unsigned int headset);
        virtual FacialExpressionAlgo FEGetTrainingAction(unsigned int headset);
        virtual int FESetTrainingAction(unsigned int headset, FacialExpressionAlgo action);
        virtual int FETrainingControl(unsigned int headset, FacialExpressionTrainingControl control);

#if 0
        // Mental Command Training
        int MCActiveActions(unsigned long &pActiveActionsOut);
        int MCSetActiveActions(unsigned long activeActions);

        int MCTrainingTime(unsigned int &pTrainingTimeOut);
        int MCTrainingAction(MentalCommandAction &pActionOut);
        int MCTrainingControl(MentalCommandTrainingControl control);
        int MCSetTrainingAction(MentalCommandAction action);

        int MCTrainedSignatureActions(unsigned long &pTrainedActionsOut);
        int MCOverallSkillRating(float &pOverallSkillRatingOut);
        int MCActionSkillRating(MentalCommandAction action, float &pActionSkillRatingOut);
        int MCActivationLevel(int &pLevelOut);
        int MCSetActivationLevel( int level);
        int MCSetActionSensitivity(int action1Sensitivity, int action2Sensitivity, int action3Sensitivity, int action4Sensitivity);
        int MCActionSensitivity(int &pAction1SensitivityOut, int &pAction2SensitivityOut, int &pAction3SensitivityOut, int &pAction4SensitivityOut);
        int MCStartSamplingNeutral();
        int MCStopSamplingNeutral();
        int MCSetSignatureCaching(unsigned int enabled);
        int MCSignatureCaching(unsigned int &pEnabledOut);
        int MCSetSignatureCacheSize(unsigned int size);
        int MCSignatureCacheSize(unsigned int &pSizeOut);
#endif
    protected:
        EmotivCallback* const callback = nullptr;
        EmotivEnginePrivate* d_ptr = nullptr;
};

struct EmotivTrainingProfile
{
    int id = 0;
    std::string name = "";
};

class EmotivUserPrivate;
class EmotivUser
{
    public:
    EmotivUser();
    EmotivUser(std::string &username, std::string &password);
    virtual ~EmotivUser();
    void setUserInfo(std::string &username, std::string &password);
    int login();
    void logout();
    int getUserId();
    bool isLogged();
    int FetchProfile();
    // EC_DownloadProfileFile
    // EC_UploadProfileFile
    // EC_UpdateUserProfile
    // EC_LoadUserProfile
    // EC_SaveUserProfile
private:
    EmotivUserPrivate* d_ptr = nullptr;
};

}

#endif //__EMOTIV_HPP__
