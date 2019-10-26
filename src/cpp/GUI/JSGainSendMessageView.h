//------------------------------------------------------------------------------------------------------------
// This file defines the view representing the button that sends whatever text is entered in the input
// text field to the the RT processing. This example demonstrates how to extend a Jamba custom control
// (momentary button) and get access to the state. Note how the view simply uses the state and does not
// register any parameter as nothing needs to happen when the values of the parameter it uses changes.
//------------------------------------------------------------------------------------------------------------
#pragma once

#include <pongasoft/VST/GUI/Views/TextButtonView.h>
#include "../JSGainModel.h"
#include "../JSGainPlugin.h"

namespace pongasoft::VST::JSGain::GUI {

using namespace pongasoft::VST::GUI::Views;
using namespace pongasoft::VST::GUI;
using namespace VSTGUI;

//------------------------------------------------------------------------------------------------------------
// By inheriting from StateAwareView, this view will have access to fState and fParams
// for dealing for parameters/state as well as the behavior of the text view button.
// Note that this view is actually not tied to any vst parameter (including for the control boolean toggle
// which manages the state of the button itself).
//------------------------------------------------------------------------------------------------------------
class JSGainSendMessageView : public StateAwareView<TextButtonView, JSGainGUIState>
{
public:
  // Constructor
  explicit JSGainSendMessageView(const CRect &iSize) : StateAwareView(iSize) {}

  //------------------------------------------------------------------------
  // onClick - this is the callback to implement to handle what to do
  // when the button is pressed
  //------------------------------------------------------------------------
  void onClick() override;

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
  class Creator : public CustomViewCreator<JSGainSendMessageView, TextButtonView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) noexcept :
      CustomViewCreator(iViewName, iDisplayName)
    {
    }
  };
};

}
