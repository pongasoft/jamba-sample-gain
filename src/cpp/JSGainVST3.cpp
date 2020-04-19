//------------------------------------------------------------------------------------------------------------
// This file contains the standard boilerplate code that VST3 sdk requires to instantiate the plugin
// components: it uses the unique IDs declared in JSGainCIDs.h and references the factory methods
// (createInstance) for the processor and the controller. BEGIN_FACTORY_DEF, and other macros are coming from
// the vst3 sdk
//------------------------------------------------------------------------------------------------------------

#include "JSGainCIDs.h"

#include "version.h"
#include "RT/JSGainProcessor.h"
#include "GUI/JSGainController.h"

#include <pongasoft/VST/PluginFactory.h>

using namespace pongasoft::VST;

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

//------------------------------------------------------------------------
// VST3 Plugin Main entry point
// This is equivalent to the 'main()' method for an executable in the
// case of a VST3 plugin. The implementation simply delegates to
// JambaPluginFactory and use types and conventions to build and wire the right
// components. Because this plugin is an effect, the category  is set to
// Vst::PlugType::kFx and can obviously be changed to other values depending
// on the type of plugin
//------------------------------------------------------------------------
EXPORT_FACTORY Steinberg::IPluginFactory* PLUGIN_API GetPluginFactory()
{
  return JambaPluginFactory::GetVST3PluginFactory<
    pongasoft::VST::JSGain::RT::JSGainProcessor,  // processor class (Real Time)
    pongasoft::VST::JSGain::GUI::JSGainController // controller class (GUI)
  >("pongasoft",                    // company/vendor
    "https://www.pongasoft.com",    // url
    "mailto:support@pongasoft.com", // email
    stringPluginName,               // plugin name (defined in version.h and depend on Release vs Debug build)
    FULL_VERSION_STR,               // plugin version (defined in version.h and is built from version value in CMakeLists.txt)
    Vst::PlugType::kFx              // plugin category (can be changed to other like kInstrument, etc...)
  );
}
