#pragma once

#include <pongasoft/VST/GUI/Views/CustomView.h>
#include <pongasoft/VST/Timer.h>
#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI::Views;
using namespace VSTGUI;

class JSGainStatsView : public PluginCustomView<JSGainGUIState>, public ITimerCallback
{
public:
  // Constructor
  explicit JSGainStatsView(const CRect &iSize) : PluginCustomView(iSize)
  {}

  // get/setNoDataColor
  const CColor &getTextColor() const { return fTextColor;  }
  void setTextColor(const CColor &iColor) { fTextColor = iColor; }

  // get/setFont
  FontPtr getFont() const { return fFont; }
  void setFont(FontPtr iFont) { fFont = iFont; }

  // registerParameters
  void registerParameters() override;

  void draw(CDrawContext *iContext) override;

  void onTimer(Timer *timer) override;

  CLASS_METHODS_NOCOPY(JSGainStatsView, CustomView)

protected:
  CColor fTextColor{};
  FontSPtr fFont{nullptr};

  GUIJmbParam<Stats> fStatsParam{};

  std::unique_ptr<AutoReleaseTimer> fTimer{};

public:
  class Creator : public CustomViewCreator<JSGainStatsView, CustomView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) :
      CustomViewCreator(iViewName, iDisplayName)
    {
      registerColorAttribute("text-color",
                             &JSGainStatsView::getTextColor,
                             &JSGainStatsView::setTextColor);
      registerFontAttribute("font",
                            &JSGainStatsView::getFont,
                            &JSGainStatsView::setFont);
    }
  };

};

}
}
}
}
