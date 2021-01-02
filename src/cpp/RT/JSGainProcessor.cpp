//------------------------------------------------------------------------
// This file contains the RT (Real Time) processor code
//------------------------------------------------------------------------
#include <pongasoft/VST/AudioBuffer.h>
#include <pongasoft/VST/Debug/ParamTable.h>
#include <pongasoft/VST/Debug/ParamLine.h>


#include "JSGainProcessor.h"

#include "version.h"
#include "jamba_version.h"

namespace pongasoft::VST::JSGain::RT {

//------------------------------------------------------------------------
// Constructor
// Note how the super constructor is expecting the UID of the controller
// that way you won't forget to initialize it!
//------------------------------------------------------------------------
JSGainProcessor::JSGainProcessor() : RTProcessor(JSGainControllerUID),
                                     fParameters{},
                                     fState{fParameters}
{
  DLOG_F(INFO, "[%s] JambaSampleGainProcessor() - jamba: %s - plugin: v%s (%s)",
         stringPluginName,
         JAMBA_GIT_VERSION_STR,
         FULL_VERSION_STR,
         BUILD_ARCHIVE_ARCHITECTURE);

  // in Debug mode we display the parameters in a table
#ifndef NDEBUG
  DLOG_F(INFO, "Parameters ---> \n%s", Debug::ParamTable::from(fParameters).full().toString().c_str());
#endif
}

//------------------------------------------------------------------------
// Destructor - purely for debugging purposes
//------------------------------------------------------------------------
JSGainProcessor::~JSGainProcessor()
{
  DLOG_F(INFO, "~JSGainProcessor()");
}

//------------------------------------------------------------------------
// JSGainProcessor::initialize - define your input/outputs
//------------------------------------------------------------------------
tresult JSGainProcessor::initialize(FUnknown *context)
{
  DLOG_F(INFO, "JSGainProcessor::initialize()");

  //------------------------------------------------------------------------
  // DO NOT FORGET TO INITIALIZE THE SUPERCLASS! It initializes the state
  //------------------------------------------------------------------------
  tresult result = RTProcessor::initialize(context);
  if(result != kResultOk)
  {
    return result;
  }

  //------------------------------------------------------------------------
  // This is where you define inputs and outputs
  //------------------------------------------------------------------------
  addAudioInput(STR16 ("Stereo In"), SpeakerArr::kStereo);
  addAudioOutput(STR16 ("Stereo Out"), SpeakerArr::kStereo);

  //------------------------------------------------------------------------
  // In debug mode this code displays the order in which the RT parameters
  // will be saved
  //------------------------------------------------------------------------
#ifndef NDEBUG
  using Key = Debug::ParamDisplay::Key;
  DLOG_F(INFO, "RT Save State - Version=%d --->\n%s",
         fParameters.getRTSaveStateOrder().fVersion,
         Debug::ParamTable::from(getRTState(), true).keys({Key::kID, Key::kTitle}).full().toString().c_str());
#endif

  return result;
}

//------------------------------------------------------------------------
// JSGainProcessor::terminate - purely for debugging purposes
//------------------------------------------------------------------------
tresult JSGainProcessor::terminate()
{
  DLOG_F(INFO, "JSGainProcessor::terminate()");

  return RTProcessor::terminate();
}

//------------------------------------------------------------------------
// JSGainProcessor::setupProcessing
// This is where the sample rate is being assigned => we copy the value
// into the stats
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
// JSGainProcessor::setActive
//------------------------------------------------------------------------
tresult JSGainProcessor::setActive(TBool iActive)
{
  tresult result = RTProcessor::setActive(iActive);

  if(result != kResultOk)
    return result;

  //------------------------------------------------------------------------
  // The DAW calls this method before calling process when the plugin becomes
  // active. This is a good place to initialize the stats
  //------------------------------------------------------------------------
  if(iActive)
  {
    resetStats();
  }

  return result;
}

//------------------------------------------------------------------------
// JSGainProcessor::resetStats
//------------------------------------------------------------------------
void JSGainProcessor::resetStats()
{
  // we reset the max
  fState.fMaxSinceReset = 0;

  Stats stats{};
  stats.fSampleRate = processSetup.sampleRate;
  stats.fMaxSinceReset = fState.fMaxSinceReset;
  stats.fResetTime = Clock::getCurrentTimeMillis();

  //------------------------------------------------------------------------
  // This is how easy it is to send the stats to the GUI...
  //------------------------------------------------------------------------
  fState.fStats.broadcast(stats);
}

//------------------------------------------------------------------------
// processChannel => implements the business logic on a single channel
// (left or right). Uses the AudioBuffers and Channel helper classes
// provided by Jamba. The logic is pretty simple: iterate over each sample,
// multiply by the gain and keep track of the absolute max (peak value) and
// silence flag.
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

  // use convenient call on the buffer to set the silence flag appropriately
  iOut.setSilenceFlag(silent);

  return max;
}

//------------------------------------------------------------------------
// JSGainProcessor::processInputs
//------------------------------------------------------------------------
tresult JSGainProcessor::processInputs(ProcessData &data)
{
  // Detect the fact that the GUI has sent a message to the RT.
  auto uiMessage = fState.fUIMessage.pop();
  if(uiMessage)
  {
    DLOG_F(INFO, "Received message from UI <%s> / timestamp = %lld", uiMessage->fText, uiMessage->fTimestamp);

    //------------------------------------------------------------------------
    // For a bit of "fun", the message is interpreted as a command to display
    // the current RT state. Note how this block is being executed only in
    // Debug mode as this is allocating memory in RT!
    //------------------------------------------------------------------------
#ifndef NDEBUG
    auto command = std::string(uiMessage->fText);

    if(command == "$state" || command == "$rtState")
    {
      DLOG_F(INFO, "rt - command=%s --->\n%s",
             uiMessage->fText,
             Debug::ParamTable::from(getRTState()).full().toString().c_str());
    }
#endif
  }

  return RTProcessor::processInputs(data);
}

//------------------------------------------------------------------------
// JSGainProcessor::genericProcessInputs
// Implementation of the generic (32 and 64 bits) logic.
// Note how the Vst params (fState.fBypass and fState.fxxxGain are being
// used naturally as if they were their underlying type (bool and Gain).
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
                                                  *fState.fBypass ? UNITY_GAIN : *fState.fLeftGain);
  SampleType rightMax = 0;
  if(in.getNumChannels() == 2 && out.getNumChannels() == 2)
  {
    rightMax = processChannel<SampleType>(in.getRightChannel(),
                                          out.getRightChannel(),
                                          *fState.fBypass ? UNITY_GAIN : *fState.fRightGain);
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

  //------------------------------------------------------------------------
  // Vst params keep the previous (meaning the value the last time the
  // process method was called) so that it is easy to see if it has changed
  // during this frame. Jamba automatically update previous at the end of
  // the current frame.
  //------------------------------------------------------------------------

  if(fState.fVuPPM.hasChanged())
    fState.fVuPPM.addToOutput(data);

  if(*fState.fResetMax)
  {
    resetStats();
  }
  else
  {
    if(fState.fMaxSinceReset < iCurrentMax)
    {
      fState.fMaxSinceReset = iCurrentMax;

      //------------------------------------------------------------------------
      // This is another way to use the broadcast API which does not incur an
      // additional copy (like the one used in resetStats)
      //------------------------------------------------------------------------
      fState.fStats.broadcast([this](Stats *oStats) {
        oStats->fSampleRate = processSetup.sampleRate;
        oStats->fMaxSinceReset = fState.fMaxSinceReset;
        oStats->fResetTime = Clock::getCurrentTimeMillis();
      });
    }
  }
}



}