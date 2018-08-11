#pragma once

#include "JSGainCIDs.h"
#include "JSGainModel.h"

#include <pongasoft/VST/Parameters.h>
#include <pongasoft/VST/RT/RTState.h>
#include <pongasoft/VST/GUI/GUIState.h>

#include <pluginterfaces/vst/ivstaudioprocessor.h>

namespace pongasoft {
namespace VST {
namespace JSGain {

class JSGainParameters : public Parameters
{
public:
  JSGainParameters()
  {
    // YP Note: registration order is important as it defines the order in which
    // they will be registered to the vst world hence for example the order in which they will be displayed
    // by Maschine

    // bypass
    fBypassParam =
      vst<BooleanParamConverter>(EJSGainParamID::kBypass, STR16 ("Bypass"))
        .defaultValue(false)
        .flags(ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass)
        .shortTitle(STR16 ("Bypass"))
        .add();

    // the Left Gain slider
    fLeftGainParam =
      vst<GainParamConverter>(EJSGainParamID::kLeftGain, STR16 ("Left Gain"))
        .defaultValue(UNITY_GAIN)
        .shortTitle(STR16 ("GainL"))
        .precision(2)
        .add();

    // the Right Gain slider
    fRightGainParam =
      vst<GainParamConverter>(EJSGainParamID::kRightGain, STR16 ("Right Gain"))
        .defaultValue(UNITY_GAIN)
        .shortTitle(STR16 ("GainR"))
        .precision(2)
        .add();

    // the Link toggle
    fLinkParam =
      vst<BooleanParamConverter>(EJSGainParamID::kLink, STR16 ("Link"))
        .defaultValue(true)
        .shortTitle(STR16 ("Link"))
        .guiOwned()
        .add();

    // vuPPM
    fVuPPMParam =
      vst<RawParamConverter>(EJSGainParamID::kVuPPM, STR16 ("VuPPM"))
        .transient()
        .shortTitle(STR16 ("VuPPM"))
        .add();

  }

  // saved
  VstParam<bool> fBypassParam;
  VstParam<Gain> fLeftGainParam;
  VstParam<Gain> fRightGainParam;

  // transient
  VstParam<ParamValue> fVuPPMParam;

  // UI Only
  VstParam<bool> fLinkParam;

  // used to communicate data from the processing to the UI
};

using namespace RT;

class JSGainRTState : public RTState
{
public:
  explicit JSGainRTState(JSGainParameters const &iParams) :
    RTState(iParams),
    fBypass{add(iParams.fBypassParam)},
    fLeftGain{add(iParams.fLeftGainParam)},
    fRightGain{add(iParams.fRightGainParam)},
    fVuPPM{add(iParams.fVuPPMParam)}
  {
  }

public:
  // saved state
  RTVstParam<bool> fBypass;
  RTVstParam<Gain> fLeftGain;
  RTVstParam<Gain> fRightGain;

  // transient state
  RTVstParam<ParamValue> fVuPPM;

  // messaging
};

using namespace GUI;

class JSGainGUIState : public GUIPluginState<JSGainParameters>
{
public:
  explicit JSGainGUIState(JSGainParameters const &iParams) :
    GUIPluginState(iParams)
  {};

public:
  // messaging
};

}
}
}