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

  return result;
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
  if(in.getNumChannels() < 1 || in.getNumChannels() > 2 || out.getNumChannels() < 1 || out.getNumChannels() > 2)
    return kNotImplemented;

  out.copyFrom(in);

  return kResultOk;
}

}
}
}
}