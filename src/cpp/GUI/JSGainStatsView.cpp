#include <pongasoft/VST/GUI/DrawContext.h>
#include "JSGainStatsView.h"

#include <chrono>
#include <sstream>

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace GUI {

using namespace pongasoft::VST::GUI;

//------------------------------------------------------------------------
// JSGainStatsView::registerParameters
//------------------------------------------------------------------------
void JSGainStatsView::registerParameters()
{
  fStatsParam = registerJmbParam(fState->fStats);
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
// JSGainStatsView::draw
//------------------------------------------------------------------------
void JSGainStatsView::draw(CDrawContext *iContext)
{
  CustomView::draw(iContext);

  auto rdc = RelativeDrawContext{this, iContext};

  std::ostringstream s;

  s << "Rate=" << fStatsParam->fSampleRate
    << "| Max=" << toDbString(fStatsParam->fMaxSinceReset)
    << "| Dur.=" << computeDurationString(Clock::getCurrentTimeMillis() - fStatsParam->fResetTime);

  StringDrawContext sdc{};
  sdc.fHorizTxtAlign = kCenterText;
  sdc.fTextInset = {2, 2};
  sdc.fFontColor = fTextColor;
  sdc.fFont = fFont;

  rdc.drawString(s.str(), sdc);
}

//------------------------------------------------------------------------
// JSGainStatsView::onTimer
//------------------------------------------------------------------------
void JSGainStatsView::onTimer(Timer *timer)
{
  markDirty();
}

JSGainStatsView::Creator __gJSGainStatsCreator("JSGain::Stats", "JSGain - Stats");

}
}
}
}