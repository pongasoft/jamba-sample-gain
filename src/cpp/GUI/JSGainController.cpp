#include "JSGainController.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

//------------------------------------------------------------------------
// JSGainController::JSGainController
//------------------------------------------------------------------------
JSGainController::JSGainController() : GUIController("JSGain.uidesc"),
                                       fParameters{},
                                       fState{fParameters}
{
  DLOG_F(INFO, "JSGainController()");
}

//------------------------------------------------------------------------
// JSGainController::JSGainController
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
