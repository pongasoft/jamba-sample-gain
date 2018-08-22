//------------------------------------------------------------------------
// This file contains the controller code
//------------------------------------------------------------------------
#include "JSGainController.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

//------------------------------------------------------------------------
// Constructor
// Note how the super constructor is expecting the xml file which defines
// the layout and look and feel of the plugin
//------------------------------------------------------------------------
JSGainController::JSGainController() : GUIController("JSGain.uidesc"),
                                       fParameters{},
                                       fState{fParameters}
{
  DLOG_F(INFO, "JSGainController()");
}

//------------------------------------------------------------------------
// Destructor - purely for debugging purposes
//------------------------------------------------------------------------
JSGainController::~JSGainController()
{
  DLOG_F(INFO, "~JSGainController()");
}

//------------------------------------------------------------------------
// JSGainController::initialize
//------------------------------------------------------------------------
tresult JSGainController::initialize(FUnknown *context)
{
  tresult res = GUIController::initialize(context);

  //------------------------------------------------------------------------
  // In debug mode this code displays the order in which the GUI parameters
  // will be saved
  //------------------------------------------------------------------------
#ifndef NDEBUG
  if(res == kResultOk)
  {
    using Key = Debug::ParamDisplay::Key;
    DLOG_F(INFO, "GUI Save State - Version=%d --->\n%s",
           fParameters.getGUISaveStateOrder().fVersion,
           Debug::ParamTable::from(getGUIState(), true).keys({Key::kID, Key::kTitle}).full().toString().c_str());
  }
#endif

  return res;
}

}
}
}
}
