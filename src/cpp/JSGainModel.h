//------------------------------------------------------------------------------------------------------------
// This file contains the "model" for this plugin: a set of classes and constants used in both the RT and GUI
// that model the specifics of this plugin. For example it defines what the Gain type is as well as the Stats
// and UIMessage used for communication.
//------------------------------------------------------------------------------------------------------------
#pragma once

#include <pongasoft/VST/AudioUtils.h>
#include <pongasoft/Utils/Clock/Clock.h>
#include <pongasoft/VST/ParamSerializers.h>

#include <pluginterfaces/base/ustring.h>
#include <string>
#include <pongasoft/VST/ParamConverters.h>

namespace pongasoft {
namespace VST {
namespace JSGain {

//------------------------------------------------------------------------
// The Gain class encapsulates the concept of gain which is the multiplier
// factor that will be used on each sample to raise or lower the volume.
// A Unity gain (1.0) will not affect the signal. Note that this class
// supports gain > 1.0 (which is not the case in the again example that
// comes with the sdk).
//
// Note how Gain is a class, not a primitive: Jamba handles the conversion
// for you via the converter (see below)
//------------------------------------------------------------------------
class Gain
{
public:
  static constexpr double Unity = 1.0;
  static constexpr double Factor = 0.7; // GainParamConverter

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

//------------------------------------------------------------------------
// This class is the param converter used in JSGainPlugin.h which defines
// how to convert from the native ParamValue (double in [0.0, 1.0] range)
// to the Gain class. The mapping is NOT linear and use a more realistic
// x^3 curve with the convention that unity gain maps to a ParamValue of
// 0.7 (more room for lowering than raising) and is pretty standard for
// gain knobs in general.
//
// This gives the range [-oo, +9.29dB]
//------------------------------------------------------------------------
class GainParamConverter : public IParamConverter<Gain>
{
public:
  // makes toString available
  using IParamConverter<Gain>::toString;

  /**
   * Gain uses an x^3 curve with 0.7 (Param Value) being unity gain
   */
  Gain denormalize(ParamValue value) const override
  {
    if(std::fabs(value - Gain::Factor) < 1e-5)
      return Gain{};

    // gain = (value / 0.7) ^ 3
    double correctedGain = value / Gain::Factor;
    return Gain{correctedGain * correctedGain * correctedGain};
  }

  // normalize
  ParamValue normalize(Gain const &iGain) const override
  {
    // value = (gain ^ 1/3) * 0.7
    return std::pow(iGain.getValueInSample(), 1.0/3) * Gain::Factor;
  }

  //------------------------------------------------------------------------
  // This method is called by the GUI to generate the string representation
  // of the value.
  //------------------------------------------------------------------------
  inline void toString(ParamType const &iValue, String128 iString, int32 iPrecision) const override
  {
    auto s = toDbString(iValue.getValueInSample(), iPrecision);
    Steinberg::UString wrapper(iString, str16BufferSize(String128));
    wrapper.fromAscii(s.c_str());
  }
};

//------------------------------------------------------------------------
// This structure is the information that the RT sends to the GUI whenever
// the value changes.
//
// Note that the code simply deal with values of this type and the
// framework takes care of the messaging details
// (see JSGainProcess.cpp - fState.fStats.broadcast() )
//------------------------------------------------------------------------
struct Stats
{
  double fSampleRate{44100};
  double fMaxSinceReset{0};
  int64 fResetTime{Clock::getCurrentTimeMillis()};
};

//------------------------------------------------------------------------
// This class is the param serializer used in JSGainPlugin.h which defines
// how to serialize/deserialize the Stats object so that it can be sent
// in a message.
//------------------------------------------------------------------------
class StatsParamSerializer : public IParamSerializer<Stats>
{
public:
  // deserialize / readFromStream
  inline tresult readFromStream(IBStreamer &iStreamer, ParamType &oValue) const override
  {
    tresult res = kResultOk;

    // using helper class with don't modify the value when error
    res |= IBStreamHelper::readDouble(iStreamer, oValue.fSampleRate);
    res |= IBStreamHelper::readDouble(iStreamer, oValue.fMaxSinceReset);
    res |= IBStreamHelper::readInt64(iStreamer, oValue.fResetTime);
    return res;
  }

  // serialize / writeToStream
  inline tresult writeToStream(const ParamType &iValue, IBStreamer &oStreamer) const override
  {
    oStreamer.writeDouble(iValue.fSampleRate);
    oStreamer.writeDouble(iValue.fMaxSinceReset);
    oStreamer.writeInt64(iValue.fResetTime);
    return kResultOk;
  }

  //------------------------------------------------------------------------
  // This optional method implementation allows the param to be displayed
  // (see Debug::ParamTable or Debug::ParamLine classes)
  //------------------------------------------------------------------------
  void writeToStream(ParamType const &iValue, std::ostream &oStream) const override
  {
    oStream << toDbString(iValue.fMaxSinceReset);
  }
};

//------------------------------------------------------------------------
// This structure is the message that the GUI sends to the RT whenever
// the user presses the "Send" button
//
// Note that the code simply deal with values of this type and the
// framework takes care of the messaging details
// (see JSGainSendMessageView.cpp - fUIMessageParam.broadcast(msg) )
//
// Also note that fText is a static array NOT a std::string or UTF8String
// (like fInputTextParam). This is due to the fact that this structure
// is used in the RT code and there should never be memory allocation in RT
// which would happen if using std::string or UTF8String.
//------------------------------------------------------------------------
struct UIMessage
{
  int64 fTimestamp{Clock::getCurrentTimeMillis()};
  char fText[64]{}; // NO memory allocation for RT!!
};

//------------------------------------------------------------------------
// This class is the param serializer used in JSGainPlugin.h which defines
// how to serialize/deserialize the UIMessage object so that it can be sent
// in a message. Note how it delegates to another serializer
// (part of Jamba) for the "C" type string.
//------------------------------------------------------------------------
class UIMessageParamSerializer : public IParamSerializer<UIMessage>
{
public:
  // deserialize / readFromStream
  inline tresult readFromStream(IBStreamer &iStreamer, ParamType &oValue) const override
  {
    tresult res = kResultOk;

    res |= IBStreamHelper::readInt64(iStreamer, oValue.fTimestamp);
    // fTextSerializer.readFromStream will ensure that the string is properly null terminated!
    res |= fTextSerializer.readFromStream(iStreamer, oValue.fText);
    return res;
  }

  // serialize / writeToStream
  inline tresult writeToStream(const ParamType &iValue, IBStreamer &oStreamer) const override
  {
    oStreamer.writeInt64(iValue.fTimestamp);
    fTextSerializer.writeToStream(iValue.fText, oStreamer);
    return kResultOk;
  }

  //------------------------------------------------------------------------
  // This optional method implementation allows the param to be displayed
  // (see Debug::ParamTable or Debug::ParamLine classes)
  //------------------------------------------------------------------------
  inline void writeToStream(ParamType const &iValue, std::ostream &oStream) const override
  {
    fTextSerializer.writeToStream(iValue.fText, oStream);
  }

private:
  CStringParamSerializer<64> fTextSerializer{};
};

}
}
}
