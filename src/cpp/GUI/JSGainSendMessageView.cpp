#include "JSGainSendMessageView.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

//------------------------------------------------------------------------
// JSGainSendMessageView::setControlValue
//------------------------------------------------------------------------
void JSGainSendMessageView::setControlValue(const bool &iControlValue)
{
  TCustomControlView::setControlValue(iControlValue);

  if(iControlValue)
  {
    if(fInputTextParam.exists() && fUIMessageParam.exists())
    {
      UIMessage msg{};
      auto const &text = fInputTextParam.getValue();
      text.copy(msg.fText, sizeof(msg.fText) / sizeof(msg.fText[0]));
      fUIMessageParam.broadcast(msg);

      auto command = std::string(msg.fText);
      if(command == "$state" || command == "$guiState")
      {
        DLOG_F(INFO, "gui - command=%s --->\n%s",
               msg.fText,
               Debug::ParamTable::from(fState).full().toString().c_str());
      }
    }
  }
}

//------------------------------------------------------------------------
// JSGainSendMessageView::registerParameters
//------------------------------------------------------------------------
void JSGainSendMessageView::registerParameters()
{
  TCustomControlView::registerParameters();
  fInputTextParam = registerJmbParam(fState->fInputText);
  fUIMessageParam = registerJmbParam(fState->fUIMessage);
}

//------------------------------------------------------------------------
// JSGainSendMessageView::initState
//------------------------------------------------------------------------
void JSGainSendMessageView::initState(VST::GUI::GUIState *iGUIState)
{
  MomentaryButtonView::initState(iGUIState);
  PluginAccessor::initState(iGUIState);
}

JSGainSendMessageView::Creator __gJSGainSendMessageCreator("JSGain::SendMessage", "JSGain - Send Message");

}
}
}
}