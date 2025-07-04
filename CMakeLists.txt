cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(RandomPatches VERSION 1.0.0)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

set(SOURCES
    src/portable/flipped-particles-fix.cpp
    src/portable/global-ship-drag-particles.cpp
    src/portable/wave-trail-on-death.cpp
    src/custom-percent-decimal.cpp
    src/portable/dash-particles-offset-fix.cpp
    src/portable/robot-spider-extra-parenting-fix.cpp
    src/portable/swing-default-mini-icon-fix.cpp
    src/portable/show-trigger-wave-particles-fix.cpp
    src/portable/logarithmic-volume-sliders.cpp
    src/portable/practice-music-sync-pulse-fix.cpp
    src/portable/icon-explosion-without-explosion.cpp
)
if (GEODE_TARGET_PLATFORM STREQUAL "Win64")
    set(SOURCES ${SOURCES}
        src/portable/options-keybind.cpp
#        src/background-on-unfocus.cpp
        src/optimized-grid.cpp
    )
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
    set(SOURCES ${SOURCES}
        src/portable/options-keybind.cpp
    )
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
    set(SOURCES ${SOURCES}
        src/portable/options-keybind.cpp
    )
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
    set(SOURCES ${SOURCES}
        src/portable/options-keybind.cpp
    )
elseif (GEODE_TARGET_PLATFORM STREQUAL "iOS")
    set(SOURCES ${SOURCES}
    )
endif ()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

CPMAddPackage("gh:prevter/sinaps#eba7941")
target_link_libraries(${PROJECT_NAME} sinaps)

set(GEODE_BUNDLE_PDB ON)
setup_geode_mod(${PROJECT_NAME})
