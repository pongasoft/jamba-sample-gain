cmake_minimum_required (VERSION 3.9)

# build VST 2 by default
option(SMTG_CREATE_VST2_VERSION "Use VST2" ON)
option(SMTG_CREATE_VST3_LINK "" OFF)

set(JAMBA_GIT_REPO "https://github.com/pongasoft/jamba" CACHE STRING "Jamba git repository url" FORCE)
set(JAMBA_GIT_TAG 09c340734785fbe469fa3b4a678ed7b934d2665a CACHE STRING "Jamba git tag" FORCE)

set(PLUGIN_MAJOR_VERSION 1)
set(PLUGIN_MINOR_VERSION 0)
set(PLUGIN_PATCH_VERSION 0)
set(PLUGIN_VERSION "${PLUGIN_MAJOR_VERSION}.${PLUGIN_MINOR_VERSION}.${PLUGIN_PATCH_VERSION}")

project(JambaSampleGain)

# for working on the framework at the same time...
set(JAMBA_ROOT_DIR ../jamba)

if(NOT JAMBA_ROOT_DIR)
	include(jamba.cmake)
endif()

include(${JAMBA_ROOT_DIR}/jamba.cmake)

set(CPP_SOURCES src/cpp)

configure_file(${CPP_SOURCES}/version.h.in ${CMAKE_BINARY_DIR}/generated/version.h)
include_directories(${CMAKE_BINARY_DIR}/generated/)

if (SMTG_CREATE_VST2_VERSION)
	set(vst2_sources
			${CPP_SOURCES}/JSGainVST2.cpp
			)
endif()

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
if(MAC OR WIN)
	set(vst_sources ${vst_sources} ${vst2_sources})
endif()

set(target pongasoft_JambaSampleGain)
smtg_add_vst3plugin(${target} ${VST3_SDK_ROOT} ${vst_sources})
target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
target_link_libraries(${target} PRIVATE base sdk vstgui_support jamba)

smtg_add_vst3_resource(${target} "resource/background.png")
smtg_add_vst3_resource(${target} "resource/slider_background.png")
smtg_add_vst3_resource(${target} "resource/slider_handle_2.0x.png")
smtg_add_vst3_resource(${target} "resource/slider_handle.png")
smtg_add_vst3_resource(${target} "resource/vu_off.png")
smtg_add_vst3_resource(${target} "resource/vu_on.png")
smtg_add_vst3_resource(${target} "resource/JSGain.uidesc")

if(MAC)
	smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS)
elseif(WIN)
	target_sources(${target} PRIVATE resource/JSGain.rc)
endif()

# fix for vst2
jamba_fix_vst2(${target})

###################################################
# Create archive (.tgz)
###################################################
jamba_create_archive(${target} JambaSampleGain)

####################################################
## Testing
####################################################
file(GLOB_RECURSE TEST_SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} test/cpp/*cpp)
set(test_sources ${test_sources}
		${LOGURU_IMPL}
  )

jamba_add_test(${PROJECT_NAME}_test "${TEST_SRC_FILES}" "${test_sources}" "")

###################################################
# Dev help
###################################################
jamba_dev_scripts(${target})
