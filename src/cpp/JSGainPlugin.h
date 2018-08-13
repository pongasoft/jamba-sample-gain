#pragma once

#include "JSGainCIDs.h"
#include "JSGainModel.h"

#include <pongasoft/VST/Parameters.h>
#include <pongasoft/VST/RT/RTState.h>
#include <pongasoft/VST/GUI/GUIState.h>
#include <pongasoft/VST/GUI/Params/GUIParamSerializers.h>

#include <pluginterfaces/vst/ivstaudioprocessor.h>

namespace pongasoft {
namespace VST {
namespace JSGain {

using namespace GUI::Params;

using UTF8StringSerializer = UTF8StringParamSerializer<128>;

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

    // toggle to reset max
    fResetMaxParam =
      vst<BooleanParamConverter>(EJSGainParamID::kResetMax, STR16 ("Reset Max"))
        .defaultValue(false)
        .shortTitle(STR16 ("Reset"))
        .add();

    // vuPPM
    fVuPPMParam =
      vst<RawParamConverter>(EJSGainParamID::kVuPPM, STR16 ("VuPPM"))
        .transient()
        .shortTitle(STR16 ("VuPPM"))
        .add();

    // stats
    fStatsParam =
      jmb<StatsParamSerializer>(EJSGainParamID::kStats, STR16("Stats"))
        .transient()
        .rtOwned()
        .shared()
        .add();

    // the free form input text
    fInputTextParam =
      jmb<UTF8StringSerializer>(EJSGainParamID::kInputText, STR16("Input Text"))
        .guiOwned()
        .defaultValue("Hello from GUI")
        .add();

    // UI Message
    fUIMessageParam =
      jmb<UIMessageParamSerializer>(EJSGainParamID::kUIMessage, STR16("UIMessage"))
        .guiOwned()
        .transient()
        .shared()
        .add();
  }

  // saved
  VstParam<bool> fBypassParam;
  VstParam<Gain> fLeftGainParam;
  VstParam<Gain> fRightGainParam;
  VstParam<bool> fResetMaxParam;

  // transient
  VstParam<ParamValue> fVuPPMParam;

  // UI Only
  VstParam<bool> fLinkParam;
  JmbParam<UTF8String> fInputTextParam;

  // used to communicate data from RT to UI
  JmbParam<Stats> fStatsParam;

  // used to communicate data from UI to RT
  JmbParam<UIMessage> fUIMessageParam;
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
    fVuPPM{add(iParams.fVuPPMParam)},
    fResetMax{add(iParams.fResetMaxParam)},
    fStats{addJmbOut(iParams.fStatsParam)},
    fUIMessage{addJmbIn(iParams.fUIMessageParam)}
  {
  }

public:
  // saved state
  RTVstParam<bool> fBypass;
  RTVstParam<Gain> fLeftGain;
  RTVstParam<Gain> fRightGain;
  RTVstParam<bool> fResetMax;

  // transient state
  RTVstParam<ParamValue> fVuPPM;

  // messaging
  RTJmbOutParam<Stats> fStats;
  RTJmbInParam<UIMessage> fUIMessage;
};

using namespace GUI;

class JSGainGUIState : public GUIPluginState<JSGainParameters>
{
public:
  explicit JSGainGUIState(JSGainParameters const &iParams) :
    GUIPluginState(iParams),
    fInputText{add(iParams.fInputTextParam)},
    fStats{add(iParams.fStatsParam)},
    fUIMessage{add(iParams.fUIMessageParam)}
  {};

public:
  // GUI owned / saved to GUI state
  GUIJmbParam<UTF8String> fInputText;

  // messaging
  GUIJmbParam<Stats> fStats;
  GUIJmbParam<UIMessage> fUIMessage;
};

}
}
}