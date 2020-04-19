//------------------------------------------------------------------------------------------------------------
// This file defines the RT (Real Time) processor which is where the main business logic of the plugin resides.
// The main methods that you implement are processInputs32Bits and processInputs64Bits. This example shows
// how to write the code in a generic fashion. The code in this class must be very fast, do not allocate
// memory or lock. The framework takes care of the nitty-gritty details of saving/restoring the state and
// dealing with messaging.
//------------------------------------------------------------------------------------------------------------
#pragma once

#include <pongasoft/VST/RT/RTProcessor.h>
#include "../JSGainPlugin.h"

namespace pongasoft::VST::JSGain::RT {

using namespace pongasoft::VST::RT;

//------------------------------------------------------------------------
// Inherits from RTProcessor which takes care of most of the details.
// Note that you can override many methods to enhance and/or bypass what
// the framework is doing.
//------------------------------------------------------------------------
class JSGainProcessor : public RTProcessor
{
public:
  //------------------------------------------------------------------------
  // UUID() method used to create the processor
  //------------------------------------------------------------------------
  static inline ::Steinberg::FUID UUID() { return JSGainProcessorUID; };

  //------------------------------------------------------------------------
  // Factory method used to create the processor
  //------------------------------------------------------------------------
  static FUnknown *createInstance(void * /*context*/) { return (IAudioProcessor *) new JSGainProcessor(); }

public:
  // Constructor
  JSGainProcessor();

  // Destructor -- overridden for debugging purposes only
  ~JSGainProcessor() override;

  //------------------------------------------------------------------------
  // This is the only method that needs to be implemented from RTProcessor
  // It gives access to the state as defined in JSGainPlugin.h
  //------------------------------------------------------------------------
  RTState *getRTState() override { return &fState; }

  //------------------------------------------------------------------------
  // This method should be implemented as this is where you define the
  // inputs and outputs (addAudioInput / addAudioOutput)
  //------------------------------------------------------------------------
  /** Called at first after constructor (setup input/output) */
  tresult PLUGIN_API initialize(FUnknown *context) override;

  // Called at the end before destructor -- overridden for debugging purposes only
  tresult PLUGIN_API terminate() override;

  // This is where the setup happens which depends on sample rate, etc..
  tresult PLUGIN_API setupProcessing(ProcessSetup &setup) override;

  //------------------------------------------------------------------------
  // This method is called when the processor becomes active (state is true)
  // and inactive (state is false). In this case we implement it to
  // initialize the stats (see implementation)
  //------------------------------------------------------------------------
  tresult PLUGIN_API setActive(TBool iActive) override;

protected:

  //------------------------------------------------------------------------
  // This method dispatches to processInputs32Bits or processInputs64Bits
  // based on configuration (data.symbolicSampleSize). It is overridden here
  // to handle UIMessage (see implementation).
  //------------------------------------------------------------------------
  tresult processInputs(ProcessData &data) override;

  //------------------------------------------------------------------------
  // This pattern lets you implement only one version of the code
  // (SampleType) which will be picked at runtime depending on the DAW. If
  // for some reason you do not want to support 32bits or 64bits, simply
  // override canProcess32Bits (respectively canProcess64Bits) to return
  // false.
  //------------------------------------------------------------------------
  template<typename SampleType>
  tresult genericProcessInputs(ProcessData &data);

  // processInputs32Bits - simply delegate to generic implementation
  tresult processInputs32Bits(ProcessData &data) override { return genericProcessInputs<Sample32>(data); }

  // processInputs64Bits - simply delegate to generic implementation
  tresult processInputs64Bits(ProcessData &data) override { return genericProcessInputs<Sample64>(data); }

  // handleMax -- internal method which will update the stats
  void handleMax(ProcessData &data, double iCurrentMax);

  // internal call to reset the stats
  void resetStats();

private:
  // The processor gets its own copy of the parameters (defined in JSGainPlugin.h)
  JSGainParameters fParameters;

  // The state (also defined in JSGainPlugin.h) is readily accessible in the implementation
  JSGainRTState fState;
};

}

