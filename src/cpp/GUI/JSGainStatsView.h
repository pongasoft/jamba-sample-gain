//------------------------------------------------------------------------------------------------------------
// This file defines a custom view which doesn't inherit from any control from the vst sdk. It will display
// the stats in a little window. This class gives access to fParams and fState and shows how to create a
// "Creator" which allows the view to be accessible in the "editor" like all other VST SDK views with custom
// attributes. It also shows how to create a timer in the GUI layer.
//------------------------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------------------------
// By inheriting from PluginCustomView<JSGainGUIState>, this view will inherit the default behaviors of
// pongasoft::VST::GUI::Views::CustomView and add fState and fParams for dealing for parameters/state.
//------------------------------------------------------------------------------------------------------------
class JSGainStatsView : public PluginCustomView<JSGainGUIState>, public ITimerCallback
{
public:
  // Constructor
  explicit JSGainStatsView(const CRect &iSize) : PluginCustomView(iSize)
  {}

  //------------------------------------------------------------------------
  // tied to custom attribute "text-color" (see Creator below)
  //------------------------------------------------------------------------
  const CColor &getTextColor() const { return fTextColor;  }
  void setTextColor(const CColor &iColor) { fTextColor = iColor; }

  //------------------------------------------------------------------------
  // tied to custom attribute "font" (see Creator below)
  //------------------------------------------------------------------------
  FontPtr getFont() const { return fFont; }
  void setFont(FontPtr iFont) { fFont = iFont; }

  //------------------------------------------------------------------------
  // This is the method that registers the parameter with the framework: the
  // place where fStatsParam is assigned and registered to be notified for
  // changes.
  //------------------------------------------------------------------------
  void registerParameters() override;

  //------------------------------------------------------------------------
  // This view will handle its own drawing
  //------------------------------------------------------------------------
  void draw(CDrawContext *iContext) override;

  //------------------------------------------------------------------------
  // Callback (from ITimerCallback) when the timer fires
  //------------------------------------------------------------------------
  void onTimer(Timer *timer) override;

  CLASS_METHODS_NOCOPY(JSGainStatsView, CustomView)

protected:
  CColor fTextColor{};
  FontSPtr fFont{nullptr};

  //------------------------------------------------------------------------
  // Using a Jmb param to get to the stats (see registerParameters)
  // GUIJmbParam is a wrapper class (similar to the other ones) which gives
  // access to the param and make it behave like the underlying type (Stats
  // in this case).
  //------------------------------------------------------------------------
  GUIJmbParam<Stats> fStatsParam{};

  //------------------------------------------------------------------------
  // Using the Jamba AutoReleaseTimer class which takes care of releasing
  // the timer automatically (RAII concept)
  //------------------------------------------------------------------------
  std::unique_ptr<AutoReleaseTimer> fTimer{};

public:
  //------------------------------------------------------------------------
  // The Creator class is what makes this new view accessible in the editor.
  // It has 2 custom attributes (text-color and font).
  // The 1st type in CustomViewCreator is this view
  // The 2nd type is the parent view so that it automatically adds all the
  // attributes of the parent view (like back-color, custom-view-tag,
  // editor-mode).
  // Check the .cpp to see where/how the creator is instantiated.
  //------------------------------------------------------------------------
  class Creator : public CustomViewCreator<JSGainStatsView, CustomView>
  {
  public:
    explicit Creator(char const *iViewName = nullptr, char const *iDisplayName = nullptr) noexcept :
      CustomViewCreator(iViewName, iDisplayName)
    {
      //------------------------------------------------------------------------
      // Each type of attribute has its own register<Type>Attribute method
      // Note how you simply references the getter and setter in the call
      //------------------------------------------------------------------------
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
