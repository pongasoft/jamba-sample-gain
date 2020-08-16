cmake_minimum_required(VERSION 3.12)

# build VST 2?
option(JAMBA_ENABLE_VST2 "Use VST2" ON)

# build Audio Unit?
option(JAMBA_ENABLE_AUDIO_UNIT "Enable Audio Unit" ON)

set(PLUGIN_MAJOR_VERSION 1)
set(PLUGIN_MINOR_VERSION 2)
set(PLUGIN_PATCH_VERSION 5)
set(PLUGIN_VERSION "${PLUGIN_MAJOR_VERSION}.${PLUGIN_MINOR_VERSION}.${PLUGIN_PATCH_VERSION}")

project("jamba-sample-gain")

# To disable download/update, you can set those 2 variables to local folders
set(JAMBA_ROOT_DIR "")
#set(JAMBA_ROOT_DIR ../../pongasoft/jamba)
set(GOOGLETEST_ROOT_DIR "")
#set(GOOGLETEST_ROOT_DIR ../../google/googletest)

# download jamba framework
include(jamba.cmake)

# Location of the source files
set(CPP_SOURCES src/cpp)

# Generating the version.h header file which contains the plugin version (to make sure it is in sync with the version
# defined here)
set(VERSION_DIR "${CMAKE_BINARY_DIR}/generated")
configure_file(${CPP_SOURCES}/version.h.in "${VERSION_DIR}/version.h")

# Source files for VST2
if (JAMBA_ENABLE_VST2)
  set(vst2_sources
      ${CPP_SOURCES}/JSGainVST2.cpp
      )
endif ()

# Source files for VST3
set(vst_sources
  	${CPP_SOURCES}/JSGainCIDs.h
		${CPP_SOURCES}/JSGainModel.h
		${CPP_SOURCES}/JSGainModel.cpp
		${CPP_SOURCES}/JSGainPlugin.h
		${CPP_SOURCES}/JSGainVST3.cpp

		${CPP_SOURCES}/RT/JSGainProcessor.h
		${CPP_SOURCES}/RT/JSGainProcessor.cpp

		${CPP_SOURCES}/GUI/JSGainController.h
		${CPP_SOURCES}/GUI/JSGainController.cpp
		${CPP_SOURCES}/GUI/JSGainSendMessageView.h
		${CPP_SOURCES}/GUI/JSGainSendMessageView.cpp
		${CPP_SOURCES}/GUI/JSGainStatsView.h
		${CPP_SOURCES}/GUI/JSGainStatsView.cpp
		${CPP_SOURCES}/GUI/LinkedSliderView.h
		${CPP_SOURCES}/GUI/LinkedSliderView.cpp
  )

# Location of resources
set(RES_DIR "${CMAKE_CURRENT_LIST_DIR}/resource")

# List of resources (images)
set(vst_resources
  "${RES_DIR}/background.png"
  "${RES_DIR}/slider_background.png"
  "${RES_DIR}/slider_handle.png"
  "${RES_DIR}/slider_handle_2.0x.png"
  "${RES_DIR}/vu_off.png"
  "${RES_DIR}/vu_on.png"
  )

# Location of the test cases
set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test/cpp")

# List of test cases
set(test_case_sources
  "${TEST_DIR}/test-JSGain.cpp"
)

# Finally invoke jamba_add_vst3_plugin
jamba_add_vst3_plugin(
    TARGET              "pongasoft_JambaSampleGain"        # name of CMake target for the plugin
    RELEASE_FILENAME    "JambaSampleGain"                  # filename for the plugin (JambaSampleGain.vst3)
    TARGETS_PREFIX      "jsg_"                             # prefix used by all targets directly linked to this plugin
    VST_SOURCES         "${vst_sources}" "${vst2_sources}" # the source files that compose the plugin
    INCLUDE_DIRECTORIES "${VERSION_DIR}"                   # we add the version folder to the list of includes
    UIDESC              "${RES_DIR}/JSGain.uidesc"         # the main xml file for the GUI
    RESOURCES           "${vst_resources}"                 # the resources for the GUI (png files)
    TEST_CASE_SOURCES   "${test_case_sources}"             # the source files containing the test cases
    TEST_SOURCES        "${CPP_SOURCES}/JSGainModel.cpp"   # we only need this file but we could add ${vst_sources} if we needed more
    TEST_LINK_LIBRARIES "jamba"                            # the library needed for linking the tests
)
