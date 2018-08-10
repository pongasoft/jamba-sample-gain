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

}
}
}
}
