project(voicemeeter LANGUAGES C CXX)

set(VOICEMEETER_SOURCES
    VoicemeeterRemote.h
    voicemeeter.c voicemeeter.h
    voicemeeterClient.hpp voicemeeterClient.cpp
)

add_library(voicemeeter STATIC ${VOICEMEETER_SOURCES})

target_include_directories(voicemeeter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})