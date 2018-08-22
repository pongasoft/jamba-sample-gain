//------------------------------------------------------------------------------------------------------------
// This file is an example on how to write unit tests for any class in the project. It uses google test
// framework so check the documentation for this testing framework for more details.
//------------------------------------------------------------------------------------------------------------
#include <pongasoft/logging/logging.h>
#include <gtest/gtest.h>

#include "src/cpp/JSGainModel.h"

namespace pongasoft {
namespace VST {
namespace JSGain {
namespace Test {

///////////////////////////////////////////
// JSGain tests
///////////////////////////////////////////

// JSGainModelTest - GainParamConverter
TEST(JSGainModelTest, GainParamConverter)
{
  DLOG_F(INFO, "Demonstrating test capability");
  GainParamConverter converter{};
  
  Gain unityGain = converter.denormalize(0.7);
  ASSERT_EQ(1.0, unityGain.getValueInSample());
  ASSERT_EQ(std::string{"+0.00dB"}, converter.toString(unityGain, 2));
}

}
}
}
}