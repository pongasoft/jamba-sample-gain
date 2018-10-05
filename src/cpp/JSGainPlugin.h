//------------------------------------------------------------------------------------------------------------
// This file defines the "plugin": it contains the definition of the parameters (a subclass of Parameters) and
// their usage in the RT (Real Time) (a subclass of RTState) and in the GUI (a subclass of GUIState).
// The RTState is used by the RTProcessor (see RT/JSGainProcessor.h)
// The GUIState is used by the GUIController (see GUI/JSGainController.h)
//------------------------------------------------------------------------------------------------------------

#pragma once

#include "JSGainCIDs.h"
#include "JSGainModel.h"

#include <pongasoft/VST/Parameters.h>
#include <pongasoft/VST/RT/RTState.h>
#include <pongasoft/VST/GUI/GUIState.h>
#include <pongasoft/VST/GUI/Params/GUIParamSerializers.h>

//------------------------------------------------------------------------
// The following includes will happen ONLY in Debug builds and ensure
// that we are not using them in Release mode (debug info, memory
// allocation in RT, etc...)
//------------------------------------------------------------------------
#ifndef NDEBUG
#include <pongasoft/VST/Debug/ParamLine.h>
#include <pongasoft/VST/Debug/ParamTable.h>
#endif

#include <pluginterfaces/vst/ivstaudioprocessor.h>

namespace pongasoft {
namespace VST {
namespace JSGain {

using namespace GUI::Params;

using UTF8StringSerializer = UTF8StringParamSerializer<128>;

// keeping track of the version of the state being saved so that it can be upgraded more easily later
// should be > 0
constexpr uint16 PROCESSOR_STATE_VERSION = 1;
constexpr uint16 CONTROLLER_STATE_VERSION = 1;

//------------------------------------------------------------------------------------------------------------
// This class which inherits from pongasoft::VST::Parameters defines ALL the parameters that the plugin will
// use.
//
// Vst parameters represent the parameters that DAWs know and understand. Internally they are represented
// by the type Steinberg::Vst::ParamValue which is a double in the range [0.0, 1.0]:
// - you can use them directly if it makes sense (see fVuPPMParam)
// - you can wrap them in a typed parameter for easier and more natural use in the code: the "conversion"
//   between the underlying ParamValue and the "type" is handled by a IParamConverter class (see all the other
//   vst parameters used by this plugin, from bool a primitive type to "Gain" an actual class)
//
// Although the plugin provides its own UI to manipulate the values of the params (knobs, sliders, etc...) the
// DAW is free to also manipulate the values outside the UI (for example automation or its own UI like Maschine
// which maps every parameter to a knob in its own UI (softare) and controllers (hardware)).
//
// Jmb (Jamba) parameters represent all the parameters that DAWs do not know and understand natively. They
// are typed (represent an actual class or primitive) and can be saved in the GUI state or used to communicate
// between the GUI and the RT (and vice versa). In order to be saved and/or used for communication they should
// provide a IParamSerializer class which takes care of the serialization/deserialization.
//
// The state of the plugin which the DAW store with the project/song (and restore when later open), is mostly
// comprised of the value of the parameters. Jamba lets you simply declare which parameter should be saved
// or not (marked transient to EXCLUDE from saved state) and will take care of the underlying details to
// save (and restore) the state in a thread safe manner. This works both for Vst and Jmb parameters!
//------------------------------------------------------------------------------------------------------------
class JSGainParameters : public Parameters
{
public:
  //------------------------------------------------------------------------
  // These parameters are used by the RT and saved part of the RT state
  // Note how they are typed (bool or Gain). Check JSGainModel.h for details
  // on the Gain type.
  //------------------------------------------------------------------------
  VstParam<bool> fBypassParam;    // the bypass toggle (bypasses gain multiplication when on/true)
  VstParam<Gain> fLeftGainParam;  // gain for left channel (typed because gain is not linear) - tied to GUI slider
  VstParam<Gain> fRightGainParam; // gain for right channel (typed because gain is not linear) - tied to GUI slider
  VstParam<bool> fResetMaxParam;  // the momentary button to reset the max value in the stats

  //------------------------------------------------------------------------
  // This parameter is transient, meaning it is NOT saved in the state
  // because it does not make sense to save the current peak value of the
  // signal in the state.
  // Note that this parameter does not require or need any kind of conversion
  // and as a result is untyped and use the "raw" version of the param.
  //------------------------------------------------------------------------
  RawVstParam fVuPPMParam; // used by a VUMeter view in the GUI to show the current peak value

  //------------------------------------------------------------------------
  // These parameters are used by the GUI only as the RT code does not care
  // about them and saved part of the GUI state (so that they are restored
  // when the plugin is loaded).
  //------------------------------------------------------------------------
  VstParam<bool> fLinkParam;            // links the left and right channel when on (implemented in UI only)
  JmbParam<UTF8String> fInputTextParam; // the input text field (Jmb param as it can't be mapped to ParamValue)

  //------------------------------------------------------------------------
  // This is an example of a Jmb param used to communicate data between
  // the RT and the GUI
  //------------------------------------------------------------------------
  JmbParam<Stats> fStatsParam; // Stats is a type defined in JSGainModel.h (as well as its serializer)

  //------------------------------------------------------------------------
  // This is an example of a Jmb param used to communicate data between
  // the GUI and the RT
  //------------------------------------------------------------------------
  JmbParam<UIMessage> fUIMessageParam; // UIMessage is a type defined in JSGainModel.h (as well as its serializer)

public:
  JSGainParameters()
  {
    //------------------------------------------------------------------------
    // After declaring the parameters, the constructor must initialize them and
    // use the various convenient methods to do so: raw, vst<>, jmb<>. Each of
    // this methods returns a builder to conveniently define only what needs to
    // be changed from their defaults and concludes with add which "registers"
    // the parameters.
    //
    // The registration order is important (for the vst params) because it
    // defines the order in which they will be registered to the vst world
    // (hence for example the order in which they will be displayed by Maschine
    // in its own UI/controllers)
    //------------------------------------------------------------------------

    // bypass
    fBypassParam =
      vst<BooleanParamConverter>(EJSGainParamID::kBypass, STR16 ("Bypass")) // templated by the converter
        .defaultValue(false)
        .flags(ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass) // need to provide kIsBypass flag
        .shortTitle(STR16 ("Bypass"))
        .add();

    // the Left Gain slider
    fLeftGainParam =
      vst<GainParamConverter>(EJSGainParamID::kLeftGain, STR16 ("Left Gain")) // templated by the converter
        .defaultValue(UNITY_GAIN) // note how the default value uses the actual type (Gain)
        .shortTitle(STR16 ("GainL"))
        .precision(2) // used by toString to display only 2 digits: ex -1.45dB
        .add();

    // the Right Gain slider
    fRightGainParam =
      vst<GainParamConverter>(EJSGainParamID::kRightGain, STR16 ("Right Gain"))
        .defaultValue(UNITY_GAIN)
        .shortTitle(STR16 ("GainR"))
        .precision(2)
        .add();

    // the Link toggle
    fLinkParam =
      vst<BooleanParamConverter>(EJSGainParamID::kLink, STR16 ("Link"))
        .defaultValue(true)
        .shortTitle(STR16 ("Link"))
        .guiOwned() // by default vst parameters are rtOwned => this makes this parameter owned by the GUI
        .add();

    // toggle to reset max
    fResetMaxParam =
      vst<BooleanParamConverter>(EJSGainParamID::kResetMax, STR16 ("Reset Max"))
        .defaultValue(false)
        .shortTitle(STR16 ("Reset"))
        .add();

    // vuPPM
    fVuPPMParam =
      raw(EJSGainParamID::kVuPPM, STR16 ("VuPPM"))
        .transient() // by default parameters are saved in the state of its owner => in this case it won't be saved
        .shortTitle(STR16 ("VuPPM"))
        .add();

    //------------------------------------------------------------------------
    // In order to be used for communication, the param must be marked
    // "shared". The communication direction is implied by who owns the param.
    // Since fStatsParam is rtOwned, the direction will be RT -> GUI
    //------------------------------------------------------------------------
    // stats
    fStatsParam =
      jmb<StatsParamSerializer>(EJSGainParamID::kStats, STR16("Stats"))
        .transient() // does not make much sense to be saved part of the state (and rtOwned Jmb param CANNOT be saved)
        .rtOwned()   // by default jmb parameters are guiOwned => this makes this parameter owned by the RT
        .shared()    // enables RT -> GUI communication (rtOwned)
        .add();

    // the free form input text - this param WILL be saved in its owner (GUI) state
    fInputTextParam =
      jmb<UTF8StringSerializer>(EJSGainParamID::kInputText, STR16("Input Text"))
        .guiOwned() // although it is the default, this can make it clearer who owns the parameter
        .defaultValue("Hello from GUI")
        .add();

    // UI Message
    fUIMessageParam =
      jmb<UIMessageParamSerializer>(EJSGainParamID::kUIMessage, STR16("UIMessage"))
        .guiOwned()  // although it is the default, this can make it clearer who owns the parameter
        .transient() // does not make much sense to be saved part of the state
        .shared()    // enables GUI -> RT communication (guiOwned)
        .add();

    //------------------------------------------------------------------------
    // Although this step is optional, it is HIGHLY recommended (and a warning
    // will be logged) if the order in which the state should be saved is not
    // explicitly specified. If not specified, the registration order will
    // be used and it is very brittle as the registration order can be changed
    // to alter the way the DAW shows the parameters.
    //
    // Note that it is ok to release a new version of the plugin with the SAME
    // PROCESSOR_STATE_VERSION/CONTROLLER_STATE_VERSION as long as parameters
    // have only been added to this list: added parameters that were not part
    // of a previously saved state will take on their default values. If you
    // alter this list by removing and/or swapping parameters, a new version
    // would have to be used. If you don't, then the order in which the state
    // will be restored will be wrong.
    //
    // Note that support for new version will be added at a later time in
    // the framework but handling "upgrades" would still be manual. As a result,
    // it is highly recommended to only add to this list once the plugin has
    // been released once.
    //------------------------------------------------------------------------
    setRTSaveStateOrder(PROCESSOR_STATE_VERSION,
                        fBypassParam,
                        fLeftGainParam,
                        fRightGainParam,
                        fResetMaxParam);

    // same for GUI - note that if the GUI does not save anything then you don't need this
    setGUISaveStateOrder(CONTROLLER_STATE_VERSION,
                         fLinkParam,
                         fInputTextParam);
  }
};

//------------------------------------------------------------------------------------------------------------
// This class which inherits from pongasoft::VST:RT::RTState defines the parameters that the RT processor code
// is going to use. Think about Parameters as an "interface" or declaration of the parameters (name, type
// etc...) and RTState as the implementation or usage of the parameters (actual value). This state is
// readily available in the RT processor code thus allowing to write code like if(fState.fBypass) or
// if(fState.fResetMax.hasChanged()), etc... Only rtOwned parameters can be used by this class.
//
// This class is used in RT/JSGainProcessor.h
//------------------------------------------------------------------------------------------------------------
using namespace RT;
class JSGainRTState : public RTState
{
public:
  //------------------------------------------------------------------------
  // These parameters are part of the saved state. Note how they are typed
  // by the underlying type (bool, Gain) and how the class is RTVstParam.
  // The RTVstParam is an actual class (not a pointer) and offers several
  // convenient overloads which makes it easy to use the parameter as the
  // underlying type. For example: if(fState.fBypass) or
  // fState.fLeftGain->getValueInSample().
  //------------------------------------------------------------------------
  RTVstParam<bool> fBypass;
  RTVstParam<Gain> fLeftGain;
  RTVstParam<Gain> fRightGain;
  RTVstParam<bool> fResetMax;

  //------------------------------------------------------------------------
  // This parameter which is transient is using the Raw flavor (untyped)
  // version. RTRawVstParam is also a class with convenient overloads.
  //------------------------------------------------------------------------
  RTRawVstParam fVuPPM;

  //------------------------------------------------------------------------
  // These parameters are Jmb parameters and as a result use a different
  // wrapper class. There are 2 different wrappers depending on how the
  // parameter is being used because the requirements and usage are
  // completely different.
  //------------------------------------------------------------------------
  RTJmbOutParam<Stats> fStats;         // RT sends the stats out (broadcast) => RTJmbOutParam
  RTJmbInParam<UIMessage> fUIMessage;  // RT receives UI message from GUI => RTJmbInParam

  //------------------------------------------------------------------------
  // This class is just a regular C++ class so you can also use it to store
  // other parts of the state necessary for the plugin. Note that Jamba
  // will not do anything about it.
  //------------------------------------------------------------------------
  double fMaxSinceReset{};

public:
  //------------------------------------------------------------------------
  // The constructor initializes each parameter by calling the appropriate
  // "addXXX" method and referencing the parameter previously declared.
  //------------------------------------------------------------------------
  explicit JSGainRTState(JSGainParameters const &iParams) :
    RTState(iParams),
    fBypass{add(iParams.fBypassParam)},
    fLeftGain{add(iParams.fLeftGainParam)},
    fRightGain{add(iParams.fRightGainParam)},
    fResetMax{add(iParams.fResetMaxParam)},
    fVuPPM{add(iParams.fVuPPMParam)},
    fStats{addJmbOut(iParams.fStatsParam)},
    fUIMessage{addJmbIn(iParams.fUIMessageParam)}
  {
  }

//------------------------------------------------------------------------
// The following overrides will happen only in debug mode and will log
// whenever the state is read or written in the RT. Note that the code
// uses the very powerfull Debug::ParamLine or Debug::ParamTable classes
// which can be configured to display the data the way you want.
//------------------------------------------------------------------------
#ifndef NDEBUG
protected:
  // afterReadNewState
  void afterReadNewState(NormalizedState const *iState) override
  {
    // swap the commented line to display either on a line or in a table
    DLOG_F(INFO, "RTState::read - %s", Debug::ParamLine::from(this, true).toString(*iState).c_str());
    //Debug::ParamTable::from(this, true).showCellSeparation().print(*iState, "RTState::read ---> ");
  }

  // beforeWriteNewState
  void beforeWriteNewState(NormalizedState const *iState) override
  {
    // swap the commented line to display either on a line or in a table
    DLOG_F(INFO, "RTState::write - %s", Debug::ParamLine::from(this, true).toString(*iState).c_str());
    //Debug::ParamTable::from(this, true).showCellSeparation().print(*iState, "RTState::write ---> ");
  }
#endif
};

using namespace GUI;

//------------------------------------------------------------------------------------------------------------
// This class which inherits from pongasoft::VST:GUI::GUIPluginState defines the parameters that the GUI code
// is going to use. Think about Parameters as an "interface" or declaration of the parameters (name, type
// etc...) and GUIState as the implementation or usage of the parameters (actual value). This state/class is
// readily available in the GUI code (views). Check the various view classes under GUI for examples.
// Note that (unlike RT) the subclass requires a template for the parameters. This is due to the fact that
// as you can see, none of the VST parameters are defined in this class: the GUI can use ANY and all of the
// declared VST parameters so there is no point in redeclaring them (they actually are managed by the vst sdk
// ui layer).
//
// Note that a parameter declared guiOwned cannot be used in the RT layer. But a parameter declared rtOwned
// is used in both layers: it just means that its state will be saved with the RT state.
//
// This class is used in GUI/JSGainController.h
//------------------------------------------------------------------------------------------------------------
class JSGainGUIState : public GUIPluginState<JSGainParameters>
{
public:
  //------------------------------------------------------------------------
  // This parameter is owned by the GUI and saved part of the GUI state
  // Note that it uses the GUIJmbParam class which is, similar to the RT
  // version, a wrapper class which lets you access the parameter
  // as the underlying type (UTF8String)
  //------------------------------------------------------------------------
  GUIJmbParam<UTF8String> fInputText;

  //------------------------------------------------------------------------
  // These 2 parameters are used for messaging. Note that, unlike the RT
  // version, they all use the same class.
  //------------------------------------------------------------------------
  GUIJmbParam<Stats> fStats;
  GUIJmbParam<UIMessage> fUIMessage;

public:
  //------------------------------------------------------------------------
  // The constructor initializes each parameter by calling the "add" method
  // and referencing the parameter previously declared.
  //------------------------------------------------------------------------
  explicit JSGainGUIState(JSGainParameters const &iParams) :
    GUIPluginState(iParams),
    fInputText{add(iParams.fInputTextParam)},
    fStats{add(iParams.fStatsParam)},
    fUIMessage{add(iParams.fUIMessageParam)}
  {};

//------------------------------------------------------------------------
// The following overrides will happen only in debug mode and will log
// whenever the state is read or written in the GUI. Note that you could
// read and write more data to the stream if you wish so by overriding
// these very methods.
//------------------------------------------------------------------------
#ifndef NDEBUG
protected:
  // readGUIState
  tresult readGUIState(IBStreamer &iStreamer) override
  {
    tresult res = GUIState::readGUIState(iStreamer);
    if(res == kResultOk)
    {
      // swap the commented line to display either on a line or in a table
      DLOG_F(INFO, "GUIState::read - %s", Debug::ParamLine::from(this, true).toString().c_str());
      //Debug::ParamTable::from(this, true).showCellSeparation().print("GUIState::read ---> ");
    }
    return res;
  }

  // writeGUIState
  tresult writeGUIState(IBStreamer &oStreamer) const override
  {
    tresult res = GUIState::writeGUIState(oStreamer);
    if(res == kResultOk)
    {
      // swap the commented line to display either on a line or in a table
      DLOG_F(INFO, "GUIState::write - %s", Debug::ParamLine::from(this, true).toString().c_str());
      //Debug::ParamTable::from(this, true).showCellSeparation().print("GUIState::write ---> ");
    }
    return res;
  }
#endif

};

}
}
}