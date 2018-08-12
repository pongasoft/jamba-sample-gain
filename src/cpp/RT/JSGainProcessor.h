#pragma once

#include <pongasoft/VST/RT/RTProcessor.h>
#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace RT {

using namespace pongasoft::VST::RT;

class JSGainProcessor : public RTProcessor
{
public:
  //--- ---------------------------------------------------------------------
  // create function required for Plug-in factory,
  // it will be called to create new instances of this Plug-in
  //--- ---------------------------------------------------------------------
  static FUnknown *createInstance(void * /*context*/) { return (IAudioProcessor *) new JSGainProcessor(); }

public:
  // Constructor
  JSGainProcessor();

  // Destructor
  ~JSGainProcessor() override;

  // getRTState
  RTState *getRTState() override { return &fState; }

  /** Called at first after constructor (setup input/output) */
  tresult PLUGIN_API initialize(FUnknown *context) override;

  /** Called at the end before destructor */
  tresult PLUGIN_API terminate() override;

  // This is where the setup happens which depends on sample rate, etc..
  tresult PLUGIN_API setupProcessing(ProcessSetup &setup) override;

  tresult setActive(TBool state) override;

protected:
  /**
   * Processes inputs (step 2 always called after processing the parameters)
   */
  template<typename SampleType>
  tresult genericProcessInputs(ProcessData &data);

  // processInputs32Bits
  tresult processInputs32Bits(ProcessData &data) override { return genericProcessInputs<Sample32>(data); }

  // processInputs64Bits
  tresult processInputs64Bits(ProcessData &data) override { return genericProcessInputs<Sample64>(data); }

  // handleMax
  void handleMax(ProcessData &data, double iCurrentMax);

private:
  JSGainParameters fParameters;
  JSGainRTState fState;
};

}
}
}
}

