#pragma once

#include "JSGainCIDs.h"

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
  JSGainParameters() {}

  // saved

  // transient

  // UI Only

  // used to communicate data from the processing to the UI
};

using namespace RT;

class JSGainRTState : public RTState
{
public:
  explicit JSGainRTState(JSGainParameters const &iParams) :
    RTState(iParams)
  {
  }

public:
  // saved state

  // transient state

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