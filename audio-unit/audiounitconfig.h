#include "version.h"

// Check https://developer.apple.com/library/archive/documentation/General/Conceptual/ExtensibilityPG/AudioUnit.html for various types

/* Bundle Identifier */
#define kAudioUnitBundleIdentifier	org.pongasoft.vst.JambaSampleGain.audiounit

/* Version Number (computed in version.h from version in CMakeLists.txt) */
#define kAudioUnitVersion			AU_VERSION_INT

/* Company Name + Effect Name */
#define kAUPluginName 				pongasoft: JambaSampleGain

/* A product name for the audio unit, such as TremoloUnit */
#define kAUPluginDescription 		JambaSampleGain

/*
  The specific variant of the Audio Unit. The four possible types and their values are:
  Effect (aufx), Generator (augn), Instrument (aumu), and Music Effect (aufm).
 */
#define kAUPluginType 				aufx

/* A subtype code for the audio unit, such as tmlo. This value must be exactly 4 alphanumeric characters. */
#define kAUPluginSubType 			gain

/* A manufacturer code for the audio unit, such as Aaud. This value must be exactly 4 alphanumeric characters.
 * Manufacturer OSType should have at least one non-lower case character */
#define kAUPluginManufacturer 		Psft

// Definitions for the resource file
#define kAudioUnitName				      "pongasoft: JambaSampleGain" // same as kAUPluginName
#define kAudioUnitDescription	      "JambaSampleGain" // same as kAUPluginDescription
#define kAudioUnitType				      'aufx' // same as kAUPluginType
#define kAudioUnitComponentSubType	'gain' // same as kAUPluginSubType
#define kAudioUnitComponentManuf    'Psft' // same as kAUPluginManufacturer

#define kAudioUnitCarbonView		1		// if 0 no Carbon view support will be added
