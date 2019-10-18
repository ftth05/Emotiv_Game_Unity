#ifndef __MY_EMOTIV_CALLBACK__
#define MY_EMOTIV_CALLBACK
#include <memory>
#include <vector>
#include "Emotiv.hpp"

class MyEmotivCallback: public Emotiv::EmotivCallback
{
public:
    MyEmotivCallback();
    virtual ~MyEmotivCallback();

    void UpdateEmoState(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::shared_ptr<Emotiv::EmotivState>);

    virtual void onHeasetConnected(const std::shared_ptr<Emotiv::EmotivHeadset>) override;
    virtual void onHeasetDisconnected(const std::shared_ptr<Emotiv::EmotivHeadset>) override;
    virtual void onEmoStateUpdate(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::shared_ptr<Emotiv::EmotivState>) override;
    virtual void onPMEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::shared_ptr<Emotiv::EmotivState> emoState) override;

//    virtual void onFETrainingEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, Emotiv::FETrainingEvent) override;
//    virtual void onMCTrainingEvent(const std::shared_ptr<Emotiv::EmotivHeadset>, Emotiv::MCTrainingEvent) override;

    virtual void onBandPowerData(const std::shared_ptr<Emotiv::EmotivHeadset>, const std::vector<Emotiv::BandPowerData>& data) override;
    virtual void onMotionData(const std::shared_ptr<Emotiv::EmotivHeadset>, unsigned int nSamples) override;
    virtual void onEEGData(const std::shared_ptr<Emotiv::EmotivHeadset>, unsigned int nSamples) override;
protected:
    std::map<unsigned int, std::shared_ptr<Emotiv::EmotivHeadset> > headsets;
};

#endif

