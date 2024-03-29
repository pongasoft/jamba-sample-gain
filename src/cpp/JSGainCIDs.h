//------------------------------------------------------------------------
// This file contains the various IDs used by the plugin.
//------------------------------------------------------------------------

#pragma once

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/vsttypes.h>

namespace pongasoft::VST::JSGain {

//------------------------------------------------------------------------
// These 2 IDs are used in JSGainVST3.cpp to create
// the processor (RT) and controller (GUI). Note that those IDs must be
// unique. You can use an online generator like https://www.guidgenerator.com
// to generate them. Those were generated with java UUID.randomUUID().
// Using different ids for Debug/Release targets so that both plugins can live
// side by side.
//------------------------------------------------------------------------
#ifndef NDEBUG
static const ::Steinberg::FUID JSGainProcessorUID(0xf4be5771, 0x557944a9, 0x8fa175ab, 0xaddf4c8a);
static const ::Steinberg::FUID JSGainControllerUID(0xabda718f, 0x5d564c10, 0xb1921f2f, 0xcf2b8241);
#else
static const ::Steinberg::FUID JSGainProcessorUID(0x7788b64f, 0xe2d646df, 0x95297fa4, 0x6dd07e91);
static const ::Steinberg::FUID JSGainControllerUID(0x7c69960c, 0x09e44de8, 0x8557bcb6, 0x086f8a64);
#endif

//------------------------------------------------------------------------
// Thanks to the sharing of parameters with the Jamba framework, this
// enumeration is used in only one place. It is still a good practice to
// see all the IDs (which must be unique) in one place.
// Check JSGainPlugin.h where they are used.
//------------------------------------------------------------------------
enum EJSGainParamID : Steinberg::Vst::ParamID
{
  // although NOT a requirement, I like to start at 1000 so that they are all 4 digits.
  // the grouping and numbering is arbitrary and you can use whatever makes sense for your case.

  // the bypass parameter which has a special meaning to the host
  kBypass = 1000,

  // 2000s represent the Vst parameters
  kVuPPM = 2000,

  kLeftGain = 2010,
  kRightGain = 2011,
  kLink = 2012,

  kResetMax = 2020,

  kInputText = 2030,

  // 3000s represent the Jmb (Jamba) parameters
  kStats = 3000,
  kUIMessage = 3010,
};

} // namespace pongasoft
