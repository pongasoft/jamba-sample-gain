//------------------------------------------------------------------------------------------------------------
// This file defines a slider which is linked to another one via a toggle. This class demonstrates how to
// extend a class from the VST SDK (CSlider), how to give it access to the state and how to use the state and
// params defined in JSGainPlugin.h. It also shows how to create a "Creator" which allows the view to be
// accessible in the "editor" like all other VST SDK views. Check the .cpp file for a Generic implementation
// of this class which does not use the state.
//------------------------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------------------------
// The PluginCustomViewAdapter templated class is the class that links the VST SDK world to the Jamba world
// when you want to inherit the behavior of a class from the SDK. The 1st template parameter is the
// class you are inheriting from and the 2nd one is the actual state class (which similarly to the RTProcessor
// code, makes the state available in the code with a member variable called fState and the params with a
// member variable called fParams)
//------------------------------------------------------------------------------------------------------------
class LinkedSliderView : public PluginCustomViewAdapter<CSlider, JSGainGUIState>
{
public:
  //------------------------------------------------------------------------
  // Constructor which delegates to the adapter. The long list of arguments
  // comes from the CSlider constructor but the adapter is smart enough
  // to pass them through.
  //------------------------------------------------------------------------
  explicit LinkedSliderView(const CRect &iSize) : PluginCustomViewAdapter(iSize, nullptr, -1, 0, 0, nullptr, nullptr)
  {}

  //------------------------------------------------------------------------
  // This method is overridden so that when it is changed in the editor we
  // can set the fGain and fLinkedGain appropriately
  //------------------------------------------------------------------------
  void setTag(int32_t val) override;

  //------------------------------------------------------------------------
  // This is the method that registers the parameter with the framework: the
  // place where fLink, fGain and fLinkedGain are assigned and registered
  // to be notified for changes
  //------------------------------------------------------------------------
  void registerParameters() override;

  //------------------------------------------------------------------------
  // The callback when one of the registered parameter value changes
  //------------------------------------------------------------------------
  void onParameterChange(ParamID iParamID) override;

  CLASS_METHODS_NOCOPY(LinkedSliderView, (PluginCustomViewAdapter<CSlider, JSGainGUIState>) )

protected:
  //------------------------------------------------------------------------
  // These are the 3 parameters that this view will use:
  // - fLink is the toggle that specifies whether the 2 sliders are linked
  //   or not
  // - fGain represents the value tied to "this" slider. Although the CSlider
  //   class manages this value, this shows that you can use a parameter as
  //   well
  // - fLinkedGain represents the value tied to the other/linked slider
  // Note that we are using GUIRawVstParam instead of GUIVstParam<Gain>
  // because we are not using the denormalized value: we are simply copying
  // it so there is no need to denormalize and normalize it. This shows that
  // you can use the raw/native value if it is more convenient...
  //------------------------------------------------------------------------
  GUIVstParam<bool> fLink{};
  GUIRawVstParam fGain{};
  GUIRawVstParam fLinkedGain{};

public:
  //------------------------------------------------------------------------
  // The Creator class is what makes this new view accessible in the editor
  // The LinkedSliderView does not have any attribute to add. Please pay
  // attention to how the super constructor is called with
  // VSTGUI::UIViewCreator::kCSlider to ensure that the CSlider attributes
  // are properly initialized. Finally, check the .cpp to see where/how
  // the creator is instantiated.
  //------------------------------------------------------------------------
  class Creator : public TCustomViewCreator<LinkedSliderView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) noexcept :
      TCustomViewCreator(iViewName, iDisplayName, VSTGUI::UIViewCreator::kCSlider)
    {
    }
  };
};

}
}
}
}
