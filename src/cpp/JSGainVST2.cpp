#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"
#include "JSGainCIDs.h"

//------------------------------------------------------------------------
::AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return Steinberg::Vst::Vst2Wrapper::create(GetPluginFactory(),
                                             pongasoft::VST::JSGain::JSGainProcessorUID,
                                             'TBDx',
                                             audioMaster);
}
