#include <pongasoft/VST/AudioBuffer.h>

#include "JSGainProcessor.h"

#include "version.h"
#include "jamba_version.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace RT {

//------------------------------------------------------------------------
// JSGainProcessor::JSGainProcessor
//------------------------------------------------------------------------
JSGainProcessor::JSGainProcessor() : RTProcessor(JSGainControllerUID),
                                     fParameters{},
                                     fState{fParameters}
{
  DLOG_F(INFO, "JSGainProcessor() - jamba: %s - plugin: v%s", JAMBA_GIT_VERSION_STR, FULL_VERSION_STR);
}

//------------------------------------------------------------------------
// JSGainProcessor::~JSGainProcessor
//------------------------------------------------------------------------
JSGainProcessor::~JSGainProcessor()
{
  DLOG_F(INFO, "~JSGainProcessor()");
}

//------------------------------------------------------------------------
// JSGainProcessor::initialize
//------------------------------------------------------------------------
tresult JSGainProcessor::initialize(FUnknown *context)
{
  DLOG_F(INFO, "JSGainProcessor::initialize()");

  tresult result = RTProcessor::initialize(context);
  if(result != kResultOk)
  {
    return result;
  }

  addAudioInput(STR16 ("Stereo In"), SpeakerArr::kStereo);
  addAudioOutput(STR16 ("Stereo Out"), SpeakerArr::kStereo);

  return result;
}

//------------------------------------------------------------------------
// JSGainProcessor::terminate
//------------------------------------------------------------------------
tresult JSGainProcessor::terminate()
{
  DLOG_F(INFO, "JSGainProcessor::terminate()");

  return RTProcessor::terminate();
}

//------------------------------------------------------------------------
// JSGainProcessor::setupProcessing
//------------------------------------------------------------------------
tresult JSGainProcessor::setupProcessing(ProcessSetup &setup)
{
  tresult result = RTProcessor::setupProcessing(setup);

  if(result != kResultOk)
    return result;

  DLOG_F(INFO,
         "JSGainProcessor::setupProcessing(%s, %s, maxSamples=%d, sampleRate=%f)",
         setup.processMode == kRealtime ? "Realtime" : (setup.processMode == kPrefetch ? "Prefetch" : "Offline"),
         setup.symbolicSampleSize == kSample32 ? "32bits" : "64bits",
         setup.maxSamplesPerBlock,
         setup.sampleRate);

  fState.fStats->fSampleRate = setup.sampleRate;

  return result;
}

//------------------------------------------------------------------------
// processChannel
//------------------------------------------------------------------------
tresult JSGainProcessor::setActive(TBool state)
{
  tresult result = RTProcessor::setActive(state);

  if(result != kResultOk)
    return result;

  if(state)
  {
    fState.fStats->fMaxSinceReset = 0;
    fState.fStats->fResetTime = Clock::getCurrentTimeMillis();
    
    fState.fStats.enqueueUpdate();
  }

  return result;
}


//------------------------------------------------------------------------
// processChannel
//------------------------------------------------------------------------
template<typename SampleType>
SampleType processChannel(typename AudioBuffers<SampleType>::Channel const &iIn,
                          typename AudioBuffers<SampleType>::Channel iOut,
                          Gain const &iGain)
{
  DCHECK_F(iIn.getNumSamples() == iOut.getNumSamples(), "sanity check on number of samples");

  SampleType max = 0;

  bool silent = true;

  auto numSamples = iIn.getNumSamples();
  auto inPtr = iIn.getBuffer();
  auto outPtr = iOut.getBuffer();
  auto gain = iGain.getValueInSample();

  for(int i = 0; i < numSamples; ++i, inPtr++, outPtr++)
  {
    SampleType sample = *inPtr;

    if(gain != Gain::Unity)
      sample *= gain;

    if(silent && !pongasoft::VST::isSilent(sample))
      silent = false;

    *outPtr = sample;

    if(sample < 0)
      sample = -sample;

    max = std::max(sample, max);
  }

  iOut.setSilenceFlag(silent);

  return max;
}

//------------------------------------------------------------------------
// JSGainProcessor::genericProcessInputs
//------------------------------------------------------------------------
template<typename SampleType>
tresult JSGainProcessor::genericProcessInputs(ProcessData &data)
{
  if(data.numInputs == 0 || data.numOutputs == 0)
  {
    // nothing to do
    return kResultOk;
  }

  AudioBuffers<SampleType> in(data.inputs[0], data.numSamples);
  AudioBuffers<SampleType> out(data.outputs[0], data.numSamples);

  // Handling mono or stereo only
  if(in.getNumChannels() < 1 || in.getNumChannels() > 2 ||
     out.getNumChannels() < 1 || out.getNumChannels() > 2)
    return kNotImplemented;

  // in mono case there could be only one channel
  auto leftChannel = out.getLeftChannel();
  SampleType leftMax = processChannel<SampleType>(in.getLeftChannel(),
                                                  leftChannel,
                                                  fState.fBypass ? UNITY_GAIN : fState.fLeftGain);
  SampleType rightMax = 0;
  if(in.getNumChannels() == 2 && out.getNumChannels() == 2)
  {
    rightMax = processChannel<SampleType>(in.getRightChannel(),
                                          out.getRightChannel(),
                                          fState.fBypass ? UNITY_GAIN : fState.fRightGain);
  }

  handleMax(data, std::max(leftMax, rightMax));

  return kResultOk;
}

//------------------------------------------------------------------------
// JSGainProcessor::handleMax
//------------------------------------------------------------------------
void JSGainProcessor::handleMax(ProcessData &data, double iCurrentMax)
{
  fState.fVuPPM.update(iCurrentMax);
  if(fState.fVuPPM.hasChanged())
    fState.fVuPPM.addToOutput(data);

  if(fState.fResetMax)
  {
    fState.fStats->fMaxSinceReset = 0;
    fState.fStats->fResetTime = Clock::getCurrentTimeMillis();
    fState.fStats.enqueueUpdate();
  }
  else
  {
    if(fState.fStats->fMaxSinceReset < iCurrentMax)
    {
      fState.fStats->fMaxSinceReset = iCurrentMax;
      fState.fStats->fResetTime = Clock::getCurrentTimeMillis();
      fState.fStats.enqueueUpdate();
    }
  }
}

}
}
}
}