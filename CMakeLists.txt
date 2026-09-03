cmake_minimum_required(VERSION 3.12)
project("moonloader_for_cleo5" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
add_compile_definitions(NOMINMAX)

if(MSVC)
    add_compile_options(/MP)
    add_compile_options(/MT)
endif()

add_library(${PROJECT_NAME} SHARED Main.cpp HookInstall.cpp PatternFinder.cpp CTheScriptsPatcher.cpp VersionFinder.cpp)