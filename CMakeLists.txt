cmake_minimum_required(VERSION 3.12)

# build VST 2?
option(JAMBA_ENABLE_VST2 "Use VST2" ON)

# build Audio Unit?
option(JAMBA_ENABLE_AUDIO_UNIT "Enable Audio Unit" ON)

# Download VST SDK if not installed?
option(JAMBA_DOWNLOAD_VSTSDK "Download VST SDK if not installed" ON)

set(PLUGIN_MAJOR_VERSION 1)
set(PLUGIN_MINOR_VERSION 2)
set(PLUGIN_PATCH_VERSION 3)
set(PLUGIN_VERSION "${PLUGIN_MAJOR_VERSION}.${PLUGIN_MINOR_VERSION}.${PLUGIN_PATCH_VERSION}")

project(JambaSampleGain)

# To disable download/update, you can set those 2 variables to local folders
unset(JAMBA_ROOT_DIR)
#set(JAMBA_ROOT_DIR ../../pongasoft/jamba)
unset(GOOGLETEST_ROOT_DIR)
#set(GOOGLETEST_ROOT_DIR ../../google/googletest)

# check for existence of default VST3.6.9 install and install it if not found
if(JAMBA_DOWNLOAD_VSTSDK)
  include(vst369.cmake)
endif()

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

set(target pongasoft_JambaSampleGain)
jamba_add_vst3plugin(${target} "${vst_sources}")
target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
target_link_libraries(${target} PRIVATE base sdk vstgui_support jamba)

###################################################
# Resources - add your resources here...
###################################################
jamba_add_vst3_resource(${target} DATA "JSGain.uidesc")
jamba_add_vst3_resource(${target} PNG "background.png")
jamba_add_vst3_resource(${target} PNG "slider_background.png")
jamba_add_vst3_resource(${target} PNG "slider_handle_2.0x.png")
jamba_add_vst3_resource(${target} PNG "slider_handle.png")
jamba_add_vst3_resource(${target} PNG "vu_off.png")
jamba_add_vst3_resource(${target} PNG "vu_on.png")

# this must be called after you are done calling jamba_add_vst3_resource for all resources
jamba_gen_vst3_resources(${target} "JSGain")

###################################################
# Create archive (.tgz)
###################################################
jamba_create_archive(${target} "JambaSampleGain")

####################################################
## Testing
####################################################
file(GLOB_RECURSE TEST_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} test/cpp/*cpp)
set(test_sources ${test_sources}
		${LOGURU_IMPL}
		${CPP_SOURCES}/JSGainModel.cpp # add whichever cpp you need or ${vst_sources} for all
  )
set(test_libs base sdk vstgui_support jamba)

jamba_add_test(${target}_test "${TEST_SRC_FILES}" "${test_sources}" "${test_libs}")

###################################################
# Dev help
###################################################
jamba_dev_scripts(${target})
