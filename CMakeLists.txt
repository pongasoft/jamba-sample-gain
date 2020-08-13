cmake_minimum_required(VERSION 3.12)

# build VST 2?
option(JAMBA_ENABLE_VST2 "Use VST2" ON)

# build Audio Unit?
option(JAMBA_ENABLE_AUDIO_UNIT "Enable Audio Unit" ON)

# Download VST SDK if not installed?
option(JAMBA_DOWNLOAD_VSTSDK "Download VST SDK if not installed" ON)

set(PLUGIN_MAJOR_VERSION 1)
set(PLUGIN_MINOR_VERSION 2)
set(PLUGIN_PATCH_VERSION 5)
set(PLUGIN_VERSION "${PLUGIN_MAJOR_VERSION}.${PLUGIN_MINOR_VERSION}.${PLUGIN_PATCH_VERSION}")

project(JambaSampleGain)

# To disable download/update, you can set those 2 variables to local folders
set(JAMBA_ROOT_DIR "")
#set(JAMBA_ROOT_DIR ../../pongasoft/jamba)
set(GOOGLETEST_ROOT_DIR "")
#set(GOOGLETEST_ROOT_DIR ../../google/googletest)

# download jamba framework
include(jamba.cmake)

set(CPP_SOURCES src/cpp)

configure_file(${CPP_SOURCES}/version.h.in ${CMAKE_BINARY_DIR}/generated/version.h)
include_directories(${CMAKE_BINARY_DIR}/generated/)

if (JAMBA_ENABLE_VST2)
  set(vst2_sources
      ${CPP_SOURCES}/JSGainVST2.cpp
      )
endif ()

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

# VST2 is only defined for macOS and Windows
if (MAC OR WIN)
  set(vst_sources ${vst_sources} ${vst2_sources})
endif ()

# Defining the list of resources
set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/resource)

set(vst_resources
  "${RES_DIR}/background.png"
  "${RES_DIR}/slider_background.png"
  "${RES_DIR}/slider_handle.png"
  "${RES_DIR}/slider_handle_2.0x.png"
  "${RES_DIR}/vu_off.png"
  "${RES_DIR}/vu_on.png"
  )

jamba_add_vst3_plugin(
    TARGET pongasoft_JambaSampleGain
    RELEASE_FILENAME JambaSampleGain
    TARGETS_PREFIX jmb_
    VST_SOURCES ${vst_sources}
    UIDESC "${RES_DIR}/JSGain.uidesc"
    RESOURCES ${vst_resources}
)

####################################################
## Testing
####################################################
file(GLOB_RECURSE TEST_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} test/cpp/*cpp)
set(test_sources ${test_sources}
		${LOGURU_IMPL}
		${CPP_SOURCES}/JSGainModel.cpp # add whichever cpp you need or ${vst_sources} for all
  )
set(test_libs base sdk vstgui_support jamba)

jamba_add_test(pongasoft_JambaSampleGain_test "${TEST_SRC_FILES}" "${test_sources}" "${test_libs}")
