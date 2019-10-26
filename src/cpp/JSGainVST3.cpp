//------------------------------------------------------------------------------------------------------------
// This file contains the standard boilerplate code that VST3 sdk requires to instantiate the plugin
// components: it uses the unique IDs declared in JSGainCIDs.h and references the factory methods
// (createInstance) for the processor and the controller. BEGIN_FACTORY_DEF, and other macros are coming from
// the vst3 sdk
//------------------------------------------------------------------------------------------------------------

#include "JSGainCIDs.h"

#include <pluginterfaces/vst/ivstcomponent.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <public.sdk/source/main/pluginfactoryvst3.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>

#include "version.h"
#include "RT/JSGainProcessor.h"
#include "GUI/JSGainController.h"

using namespace Steinberg::Vst;

#ifndef NDEBUG
#define stringPluginName "JambaSampleGain_Debug"
#else
#define stringPluginName "JambaSampleGain"
#endif

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule()
{
  return true;
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule()
{
  return true;
}

using namespace pongasoft::VST::JSGain;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------
BEGIN_FACTORY_DEF ("pongasoft",
                   "https://www.pongasoft.com",
                   "mailto:support@pongasoft.com")

    // JSGainProcessor processor
    DEF_CLASS2 (INLINE_UID_FROM_FUID(JSGainProcessorUID),
                PClassInfo::kManyInstances,  // cardinality
                kVstAudioEffectClass,    // the component category (do not changed this)
                stringPluginName,      // here the Plug-in name (to be changed)
                Vst::kDistributable,  // means that component and controller could be distributed on different computers
                "Fx",          // Subcategory for this Plug-in (to be changed)
                FULL_VERSION_STR,    // Plug-in version (to be changed)
                kVstVersionString,    // the VST 3 SDK version (do not changed this, use always this define)
                RT::JSGainProcessor::createInstance)  // function pointer called when this component should be instantiated

    // JSGainController controller
    DEF_CLASS2 (INLINE_UID_FROM_FUID(JSGainControllerUID),
                PClassInfo::kManyInstances,  // cardinality
                kVstComponentControllerClass,// the Controller category (do not changed this)
                stringPluginName
                "Controller",  // controller name (could be the same than component name)
                0,            // not used here
                "",            // not used here
                FULL_VERSION_STR,    // Plug-in version (to be changed)
                kVstVersionString,    // the VST 3 SDK version (do not changed this, use always this define)
                GUI::JSGainController::createInstance)// function pointer called when this component should be instantiated

END_FACTORY
