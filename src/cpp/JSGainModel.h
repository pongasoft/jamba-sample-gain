#pragma once

#include <pongasoft/VST/AudioUtils.h>
#include <pongasoft/Utils/Clock/Clock.h>
#include <pongasoft/VST/ParamSerializers.h>

#include <pluginterfaces/base/ustring.h>
#include <string>

namespace pongasoft {
namespace VST {
namespace JSGain {

/**
 * Gain
 */
class Gain
{
public:
  static constexpr double Unity = 1.0;
  static constexpr double Factor = 0.7;

  constexpr explicit Gain(double iValue = Unity) noexcept : fValue{iValue} {}

  inline double getValueInSample() const { return fValue; }
  inline double getValueInDb() const { return sampleToDb(fValue); }

private:
  double fValue;
};

constexpr Gain UNITY_GAIN = Gain{};

//------------------------------------------------------------------------
// toDbString
//------------------------------------------------------------------------
std::string toDbString(double iSample, int iPrecision = 2);

/**
 * Gain
 */
class GainParamConverter
{
public:
  using ParamType = Gain;

  /**
   * Gain uses an x^3 curve with 0.7 (Param Value) being unity gain
   */
  static Gain denormalize(ParamValue value)
  {
    if(std::fabs(value - Gain::Factor) < 1e-5)
      return Gain{};

    // gain = (value / 0.7) ^ 3
    double correctedGain = value / Gain::Factor;
    return Gain{correctedGain * correctedGain * correctedGain};
  }

  // normalize
  static ParamValue normalize(Gain const &iGain)
  {
    // value = (gain ^ 1/3) * 0.7
    return std::pow(iGain.getValueInSample(), 1.0/3) * Gain::Factor;
  }

  // toString
  inline static void toString(ParamType const &iValue, String128 iString, int32 iPrecision)
  {
    auto s = toDbString(iValue.getValueInSample(), iPrecision);
    Steinberg::UString wrapper(iString, str16BufferSize (String128));
    wrapper.fromAscii(s.c_str());
  }
};

struct Stats
{
  double fSampleRate{44100};
  double fMaxSinceReset{0};
  int64 fResetTime{Clock::getCurrentTimeMillis()};
};

class StatsParamSerializer
{
public:
  using ParamType = Stats;

  inline static tresult readFromStream(IBStreamer &iStreamer, ParamType &oValue)
  {
    tresult res = kResultOk;

    res |= IBStreamHelper::readDouble(iStreamer, oValue.fSampleRate);
    res |= IBStreamHelper::readDouble(iStreamer, oValue.fMaxSinceReset);
    res |= IBStreamHelper::readInt64(iStreamer, oValue.fResetTime);
    return res;
  }

  inline static tresult writeToStream(const ParamType &iValue, IBStreamer &oStreamer)
  {
    oStreamer.writeDouble(iValue.fSampleRate);
    oStreamer.writeDouble(iValue.fMaxSinceReset);
    oStreamer.writeInt64(iValue.fResetTime);
    return kResultOk;
  }
};

}
}
}
