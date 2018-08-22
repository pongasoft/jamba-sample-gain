//------------------------------------------------------------------------------------------------------------
// This file defines the view representing the button that sends whatever text is entered in the input
// text field to the the RT processing. This example demonstrates how to extend a Jamba custom control
// (momentary button) and get access to the state. Note how the view simply uses the state and does not
// register any parameter as nothing needs to happen when the values of the parameter it uses changes.
//------------------------------------------------------------------------------------------------------------
#pragma once

#include <pongasoft/VST/GUI/Views/MomentaryButtonView.h>
#include "../JSGainModel.h"
#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI::Views;
using namespace pongasoft::VST::GUI;
using namespace VSTGUI;

//------------------------------------------------------------------------------------------------------------
// By inheriting from PluginAccessor<JSGainGUIState>, this view will have access to fState and fParams
// for dealing for parameters/state. By inheriting it inherits the behavior of the momentary button.
// Note that this view is actually not tied to any vst parameter (including for the control boolean toggle
// which manages the state of the button itself).
//------------------------------------------------------------------------------------------------------------
class JSGainSendMessageView : public MomentaryButtonView, PluginAccessor<JSGainGUIState>
{
public:
  // Constructor
  explicit JSGainSendMessageView(const CRect &iSize) : MomentaryButtonView(iSize) {}

  //------------------------------------------------------------------------
  // setControlValue - overridden to handle button press
  // In the case of MomentaryButtonView the value of the control is a
  // boolean which specifies whether the button is pressed (true) or not
  // (false).
  //------------------------------------------------------------------------
  void setControlValue(const bool &iControlValue) override;

private:
  //------------------------------------------------------------------------
  // Need to call it on both inherited classes
  //------------------------------------------------------------------------
  void initState(GUIState *iGUIState) override;

public:
  CLASS_METHODS_NOCOPY(JSGainSendMessageView, MomentaryButtonView)

public:
  //------------------------------------------------------------------------
  // The Creator class is what makes this new view accessible in the editor
  // The JSGainSendMessageView does not have any attribute to add.
  // The 1st type in CustomViewCreator is this view
  // The 2nd type is the parent view so that it automatically adds all the
  // attributes of the parent view (like on-color, button-image) as well
  // as its parents (all the way to the top: CView)
  // Check the .cpp to see where/how the creator is instantiated.
  //------------------------------------------------------------------------
  class Creator : public CustomViewCreator<JSGainSendMessageView, MomentaryButtonView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) noexcept :
      CustomViewCreator(iViewName, iDisplayName)
    {
    }
  };
};

}
}
}
}
