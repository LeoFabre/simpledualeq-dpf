cmake_minimum_required(VERSION 3.22)
project(SimpleDualEq VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SIMPLEDUALEQ_BUILD_UI    "Build the optional NanoVG UI" ON)
option(SIMPLEDUALEQ_BUILD_TESTS "Build offline DSP tests"      ON)

add_subdirectory(dpf)

if(SIMPLEDUALEQ_BUILD_UI)
    dpf_add_plugin(SimpleDualEq
        TARGETS vst3 lv2
        UI_TYPE opengl
        FILES_DSP plugin/SimpleDualEqPlugin.cpp
        FILES_UI  plugin/SimpleDualEqUI.cpp)
    target_compile_definitions(SimpleDualEq PUBLIC DISTRHO_PLUGIN_HAS_UI=1)
else()
    dpf_add_plugin(SimpleDualEq
        TARGETS vst3 lv2
        FILES_DSP plugin/SimpleDualEqPlugin.cpp)
    target_compile_definitions(SimpleDualEq PUBLIC DISTRHO_PLUGIN_HAS_UI=0)
endif()

target_include_directories(SimpleDualEq PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp)

if(SIMPLEDUALEQ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
