/*

Filename: QEmotivEngine.hpp
QT C++ Wrapper for Emotiv SDK
Emotiv 2017
Author: Dinh Duong Ha ha@emotiv.com

*/

#ifndef __QEMOTIV_ENGINE__
#define __QEMOTIV_ENGINE__

#include <map>
#include <QObject>
#include <QVariant>
#include <QVector>
#include "Emotiv.hpp"

namespace Emotiv {

class QEmotivEnginePrivate;
class QEmotivEngine: public  QObject, Emotiv::EmotivCallback
{
    Q_OBJECT
public:
    QEmotivEngine(QObject* parent = nullptr);
    virtual ~QEmotivEngine();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    virtual void onHeasetConnected(const std::shared_ptr<Emotiv::EmotivHeadset>) override;
    virtual void onHeasetDisconnected(const std::shared_ptr<Emotiv::EmotivHeadset>) override;
    virtual void onEmoStateUpdate(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::shared_ptr<Emotiv::EmotivState>) override;
    virtual void onPMEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::shared_ptr<Emotiv::EmotivState> emoState) override;

    virtual void onFETrainingEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, Emotiv::FETrainingEvent) override;
    virtual void onMCTrainingEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, Emotiv::MCTrainingEvent) override;

    virtual void onGyroData(const std::shared_ptr<Emotiv::EmotivHeadset>, int gyroX, int gyroY);
    virtual void onBandPowerData(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::vector<BandPowerData>& bpDatas);
    virtual void onMotionData(const std::shared_ptr<Emotiv::EmotivHeadset>, unsigned int nSamples) override;
    virtual void onEEGData(const std::shared_ptr<Emotiv::EmotivHeadset>, unsigned int nSamples) override;

    // Facial Expresion Training
    Q_INVOKABLE virtual int feGetThreshold(unsigned int headset, int algoName, int thresholdName);
    Q_INVOKABLE virtual int feSetThreshold(unsigned int id, int algoName, int thresholdName, int value);
    Q_INVOKABLE virtual int feGetSignatureType(unsigned int headset);
    Q_INVOKABLE virtual int feSetSignatureType(unsigned int headset, int sign);
    Q_INVOKABLE virtual int feGetSignatureActions(unsigned int headset);
    Q_INVOKABLE virtual int feGetSignatureAvailable(unsigned int headset);

    Q_INVOKABLE virtual unsigned int feGetTrainingTime(unsigned int headset);
    Q_INVOKABLE virtual int feGetTrainingAction(unsigned int headset);
    Q_INVOKABLE virtual int feSetTrainingAction(unsigned int headset, int action);
    //Q_INVOKABLE virtual int FETrainingControl(unsigned int headset, int control);
    Q_INVOKABLE virtual int feTrainingControl(unsigned int headset, int control);


    // Mental Command Training
//    Q_INVOKABLE int MCSetActiveActions(unsigned int headset, unsigned long activeActions);
//    Q_INVOKABLE int MCGetActiveActions(unsigned int headset, unsigned long &pActiveActionsOut);
//    Q_INVOKABLE int MCGetTrainingTime(unsigned int headset, unsigned int &pTrainingTimeOut);
//    Q_INVOKABLE int MCTrainingControl(unsigned int headset, MentalCommandTrainingControl control);
//    Q_INVOKABLE int MCSetTrainingAction(unsigned int headset, MentalCommandAction action);
//    Q_INVOKABLE int MCGetTrainingAction(unsigned int headset, MentalCommandAction &pActionOut);
//    Q_INVOKABLE int MCTrainedSignatureActions(unsigned int headset, unsigned long &pTrainedActionsOut);
//    Q_INVOKABLE int MCOverallSkillRating(unsigned int headset, float &pOverallSkillRatingOut);
//    Q_INVOKABLE int MCActionSkillRating(unsigned int headset, MentalCommandAction action, float &pActionSkillRatingOut);
//    Q_INVOKABLE int MCActivationLevel(unsigned int headset, int &pLevelOut);
//    Q_INVOKABLE int MCSetActivationLevel(unsigned int headset,  int level);
//    Q_INVOKABLE int MCSetActionSensitivity(unsigned int headset, int action1Sensitivity, int action2Sensitivity, int action3Sensitivity, int action4Sensitivity);
//    Q_INVOKABLE int MCGetActionSensitivity(unsigned int headset, int &pAction1SensitivityOut, int &pAction2SensitivityOut, int &pAction3SensitivityOut, int &pAction4SensitivityOut);
//    Q_INVOKABLE int MCStartSamplingNeutral();
//    Q_INVOKABLE int MCStopSamplingNeutral();
//    Q_INVOKABLE int MCSetSignatureCaching(unsigned int enabled);
//    Q_INVOKABLE int MCGetSignatureCaching(unsigned int &pEnabledOut);
//    Q_INVOKABLE int MCSetSignatureCacheSize(unsigned int size);
//    Q_INVOKABLE int MCGetSignatureCacheSize(unsigned int &pSizeOut);

protected:
    virtual void timerEvent(QTimerEvent *event) override;
    virtual void loop();
signals:
    void headsetConnected(int headset);
    void headsetDisconnected(int headset);

    void updatePm(unsigned int headset, float time, const struct EmotivPMInfo &pmInfo );
    void updateFETraining(unsigned  int headset, float time, Emotiv::FETrainingEvent event );
    void updateMCTraining(unsigned  int headset, float time, Emotiv::MCTrainingEvent event );

    void updateGyro(unsigned  int headset, int gyroX, int gyroY);
    void updateBandPower(unsigned int headset, const QVector<double> &data); //const EmotivBandPowerInfo& bpInfo
    void updateMems(unsigned int headset, const QVector<double> &data );
    void updateEeg(unsigned int headset, const QVector<double> &data );
private:
    void UpdateEmoState(std::shared_ptr<Emotiv::EmotivHeadset>, std::shared_ptr<Emotiv::EmotivState>);
private:
    Q_DECLARE_PRIVATE(QEmotivEngine)
    QScopedPointer<QEmotivEnginePrivate> d_ptr;
};

}

#endif

