#include "JSGainModel.h"

#include <sstream>

namespace pongasoft::VST::JSGain {

//------------------------------------------------------------------------
// toDbString
//------------------------------------------------------------------------
std::string toDbString(double iSample, int iPrecision)
{
  if(iSample < 0)
    iSample = -iSample;

  std::ostringstream s;

  if(iSample >= VST::Sample64SilentThreshold)
  {
    s.precision(iPrecision);
    s.setf(std::ios::fixed);
    s << std::showpos << sampleToDb(iSample) << "dB";
  }
  else
    s << "-oo";
  return s.str();
}


}