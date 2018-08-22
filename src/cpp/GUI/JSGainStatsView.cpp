//------------------------------------------------------------------------------------------------------------
// Implementation of the view. Note that the view does not override onParameterChange because the default
// implementation is to mark the view dirty (so that it gets redrawn) which is exactly what we want when the
// stats get updated.
//------------------------------------------------------------------------------------------------------------
#include <pongasoft/VST/GUI/DrawContext.h>
#include "JSGainStatsView.h"

#include <chrono>
#include <sstream>

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI;

/*
 * This is how this view is defined in the XML file.
 * <view back-color="~ BlackCColor" class="JSGain::Stats" editor-mode="false" font="~ NormalFontVerySmall"
 *       mouse-enabled="true" opacity="1" origin="120, 20" size="190, 20" text-color="~ WhiteCColor"
 *       transparent="false" wants-focus="true"/>
 * - font and text-color are defined by this view
 * - back-color and editor-mode are defined by CustomView
 * - the other attributes are defined by CView and the framework
 */

//------------------------------------------------------------------------
// JSGainStatsView::registerParameters
//------------------------------------------------------------------------
void JSGainStatsView::registerParameters()
{
  //------------------------------------------------------------------------
  // Because it is a Jmb param you register it by using the param in the
  // state.
  //------------------------------------------------------------------------
  fStatsParam = registerJmbParam(fState->fStats);

  //------------------------------------------------------------------------
  // Create a timer that will fire every 200ms. Note that because it is
  // a std::unique_ptr<AutoReleaseTimer> and AutoReleaseTimer handles
  // release on destruction, this call is safe no matter how many time you
  // do it
  //------------------------------------------------------------------------
  fTimer = AutoReleaseTimer::create(this, 200);
}

//------------------------------------------------------------------------
// computeDurationString
//------------------------------------------------------------------------
std::string computeDurationString(long iDuration)
{
  using namespace std::chrono;

  milliseconds dms(iDuration);

  std::ostringstream s;

  auto dMinutes = duration_cast<minutes>(dms);
  if(dMinutes.count() > 0)
    s << dMinutes.count() << "m";

  dms -= dMinutes;

  auto dSeconds = duration_cast<seconds>(dms);
  if(dSeconds.count() > 0)
    s << dSeconds.count() << "s";
  else
  {
    s.precision(3);
    s.setf(std::ios::fixed);
    s << dms.count();
  }

  return s.str();
}

//------------------------------------------------------------------------
// JSGainStatsView::draw - "Main" method called when the view needs to be
// drawn
//------------------------------------------------------------------------
void JSGainStatsView::draw(CDrawContext *iContext)
{
  //------------------------------------------------------------------------
  // The parent view takes care of drawing the background
  //------------------------------------------------------------------------
  CustomView::draw(iContext);

  //------------------------------------------------------------------------
  // RelativeDrawContext is a convenient class provided by Jamba which makes
  // some APIs easier to use.
  //------------------------------------------------------------------------
  auto rdc = RelativeDrawContext{this, iContext};

  std::ostringstream s;

  //------------------------------------------------------------------------
  // Note how the param is being used as if it was the Stats pointer
  //------------------------------------------------------------------------
  s << "Rate=" << fStatsParam->fSampleRate
    << "| Max=" << toDbString(fStatsParam->fMaxSinceReset)
    << "| Dur.=" << computeDurationString(Clock::getCurrentTimeMillis() - fStatsParam->fResetTime);

  StringDrawContext sdc{};
  sdc.fHorizTxtAlign = kCenterText;
  sdc.fTextInset = {2, 2};
  sdc.fFontColor = fTextColor;
  sdc.fFont = fFont;

  // after generating the string, simply draw it
  rdc.drawString(s.str(), sdc);
}

//------------------------------------------------------------------------
// JSGainStatsView::onTimer
//------------------------------------------------------------------------
void JSGainStatsView::onTimer(Timer *timer)
{
  // simply mark the view dirty so that it gets redrawn (the duration keeps on changing...)
  markDirty();
}

//------------------------------------------------------------------------
// This makes the JSGainStatsView class available to the editor (and
// required for loading the XML) => the first parameter is what is
// stored in the XML [class="JSGain::Stats"], the 2nd one is what
// you see in the editor in the "Views" tab
//------------------------------------------------------------------------

JSGainStatsView::Creator __gJSGainStatsCreator("JSGain::Stats", "JSGain - Stats");

}
}
}
}