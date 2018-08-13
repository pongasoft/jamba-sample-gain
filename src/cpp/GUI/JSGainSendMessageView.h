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

class JSGainSendMessageView : public MomentaryButtonView, PluginAccessor<JSGainGUIState>
{
public:
  // Constructor
  explicit JSGainSendMessageView(const CRect &iSize) : MomentaryButtonView(iSize) {}

  // setControlValue - overridden to handle button press
  void setControlValue(const bool &iControlValue) override;

  // registerParameters
  void registerParameters() override;

private:
  // initState
  void initState(GUIState *iGUIState) override;

public:
  CLASS_METHODS_NOCOPY(JSGainSendMessageView, MomentaryButtonView)

protected:
  GUIJmbParam<UTF8String> fInputTextParam{};
  GUIJmbParam<UIMessage> fUIMessageParam{};

public:
  class Creator : public CustomViewCreator<JSGainSendMessageView, MomentaryButtonView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) :
      CustomViewCreator(iViewName, iDisplayName)
    {
    }
  };
};

}
}
}
}
