#pragma once

#include <pongasoft/VST/GUI/Views/CustomView.h>
#include <vstgui4/vstgui/lib/controls/cslider.h>

#include "../JSGainPlugin.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI::Views;
using namespace VSTGUI;

class LinkedSliderView : public PluginCustomViewAdapter<CSlider, JSGainGUIState>
{
public:
  // Constructor
  explicit LinkedSliderView(const CRect &iSize) : PluginCustomViewAdapter(iSize, nullptr, -1, 0, 0, nullptr, nullptr)
  {}

  // setTag => overridden to be able to change the param (in the editor)
  void setTag(int32_t val) override;

  // registerParameters
  void registerParameters() override;

  // onParameterChange
  void onParameterChange(ParamID iParamID) override;

  CLASS_METHODS_NOCOPY(LinkedSliderView, (PluginCustomViewAdapter<CSlider, JSGainGUIState>) )

protected:
  GUIVstParam<bool> fLink{};
  GUIRawVstParam fGain{};
  GUIRawVstParam fLinkedGain{};

public:
  class Creator : public TCustomViewCreator<LinkedSliderView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) :
      TCustomViewCreator(iViewName, iDisplayName, VSTGUI::UIViewCreator::kCSlider)
    {
    }
  };

};

}
}
}
}
