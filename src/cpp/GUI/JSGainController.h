#pragma once

#include <pongasoft/VST/GUI/GUIController.h>
#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI;

class JSGainController : public GUIController
{
public:
  // Constructor
  JSGainController();

  // Destructor
  ~JSGainController() override;

  // getGUIState
  GUIState *getGUIState() override { return &fState; }

protected:
  tresult initialize(FUnknown *context) override;

public:
  //--- ---------------------------------------------------------------------
  // create function required for Plug-in factory,
  // it will be called to create new instances of this controller
  //--- ---------------------------------------------------------------------
  static FUnknown *createInstance(void * /*context*/)
  {
    return (IEditController *) new JSGainController();
  }

private:
  JSGainParameters fParameters;
  JSGainGUIState fState;
};

}
}
}
}

