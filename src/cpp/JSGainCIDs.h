#pragma once

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/vsttypes.h>

namespace pongasoft {
namespace VST {
namespace JSGain {

// generated with java/groovy UUID.randomUUID()

static const ::Steinberg::FUID JSGainProcessorUID(0x7788b64f, 0xe2d646df, 0x95297fa4, 0x6dd07e91);
static const ::Steinberg::FUID JSGainControllerUID(0x7c69960c, 0x09e44de8, 0x8557bcb6, 0x086f8a64);

// tags associated to parameters
enum EJSGainParamID : Steinberg::Vst::ParamID
{
  kBypass = 1000,

};

// tags associated to custom views (not associated to params)
enum EJSGainCustomViewTag : Steinberg::Vst::ParamID
{
};

//------------------------------------------------------------------------
} // namespace JSGain
} // namespace VST
} // namespace pongasoft
