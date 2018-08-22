//------------------------------------------------------------------------------------------------------------
// This file defines the GUI controller which is the entry point to the GUI. Besides providing access to
// the state to Jamba and logging some debug messages it doesn't do much since the framework takes care of
// most of the details for you. The views, which are the UI components representing the look and feel of the
// plugin, are defined in an XML file (resource/JSGain.uidesc). This XML file is not manually generated: in
// debug mode, you can simply right click on the plugin and open the editor where you can drag and drop views,
// move them around, change their properties, etc...
//------------------------------------------------------------------------------------------------------------
#pragma once

#include <pongasoft/VST/GUI/GUIController.h>
#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI;

//------------------------------------------------------------------------
// Inherits from GUIController which takes care of most of the details.
// Note that you can override many methods to enhance and/or bypass what
// the framework is doing.
//------------------------------------------------------------------------
class JSGainController : public GUIController
{
public:
  //------------------------------------------------------------------------
  // Factory method used in JSGainVST3.cpp to create the controller
  //------------------------------------------------------------------------
  static FUnknown *createInstance(void * /*context*/) { return (IEditController *) new JSGainController(); }

public:
  // Constructor
  JSGainController();

  // Destructor -- overridden for debugging purposes only
  ~JSGainController() override;

  //------------------------------------------------------------------------
  // This is the only method that needs to be implemented from GUIController
  // It gives access to the state as defined in JSGainPlugin.h
  //------------------------------------------------------------------------
  GUIState *getGUIState() override { return &fState; }

protected:
  tresult initialize(FUnknown *context) override;

private:
  // The controller gets its own copy of the parameters (defined in JSGainPlugin.h)
  JSGainParameters fParameters;

  // The state (also defined in JSGainPlugin.h) is readily accessible in the views (see views for usage)
  JSGainGUIState fState;
};

}
}
}
}

