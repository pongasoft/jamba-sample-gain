//------------------------------------------------------------------------
// This file contains the implementation of the JSGainSendMessageView
//------------------------------------------------------------------------
#include "JSGainSendMessageView.h"

namespace pongasoft::VST::JSGain::GUI {

/*
 * This is how this view is defined in the XML file.
 * <view back-color="#c8c8c8ff" class="JSGain::SendMessage" editor-mode="false" mouse-enabled="true"
 *       on-color="~ GreyCColor" opacity="1" origin="220, 120" size="20, 20" transparent="false" wants-focus="true"/>
 */

//------------------------------------------------------------------------
// onClick - this is the callback to implement to handle what to do
// when the button is pressed
//------------------------------------------------------------------------
void JSGainSendMessageView::onClick()
{
  UIMessage msg{};
  //------------------------------------------------------------------------
  // We simply use the state directly
  //------------------------------------------------------------------------
  auto const &text = fState->fInputText.getValue();
  text.copy(msg.fText, sizeof(msg.fText) / sizeof(msg.fText[0]));

  //------------------------------------------------------------------------
  // This is how easy it is to send the message to the RT...
  // broadcast(msg) is a shortcut: setValue(msg) then broadcast()
  //------------------------------------------------------------------------
  fState->fUIMessage.broadcast(msg);

  //------------------------------------------------------------------------
  // For a bit of "fun", the message is interpreted as a command to display
  // the current GUI state.
  //------------------------------------------------------------------------
  auto command = std::string(msg.fText);
  if(command == "$state" || command == "$guiState")
  {
    DLOG_F(INFO, "gui - command=%s --->\n%s",
           msg.fText,
           Debug::ParamTable::from(fState).full().toString().c_str());
  }
}

//------------------------------------------------------------------------
// This makes the JSGainSendMessageView class available to the editor (and
// required for loading the XML) => the first parameter is what is
// stored in the XML [class="JSGain::SendMessage"], the 2nd one is what
// you see in the editor in the "Views" tab
//------------------------------------------------------------------------

JSGainSendMessageView::Creator __gJSGainSendMessageCreator("JSGain::SendMessage", "JSGain - Send Message");

}