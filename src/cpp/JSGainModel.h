#pragma once

#include <pongasoft/VST/AudioUtils.h>
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

  inline double getValue() const { return fValue; }
  inline double getValueInDb() const { return sampleToDb(fValue); }
  inline ParamValue getNormalizedValue() const
  {
    // value = (gain ^ 1/3) * 0.7
    return std::pow(fValue, 1.0/3) * Factor;
  }

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
    return iGain.getNormalizedValue();
  }

  // toString
  inline static void toString(ParamType const &iValue, String128 iString, int32 iPrecision)
  {
    auto s = toDbString(iValue.getValue(), iPrecision);
    Steinberg::UString wrapper(iString, str16BufferSize (String128));
    wrapper.fromAscii(s.c_str());
  }
};


}
}
}
