cmake_minimum_required (VERSION 3.2)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project (SCIEditor)

# Widgets finds its own dependencies.s
find_package(Qt5Widgets REQUIRED) #!!!
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON) #!!!
# As moc files are generated in the binary dir, tell CMake to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON) #!!!

# All binaries are located in the bin - folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source directory variable
set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# User's interface directory variable
set(UI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ui) #!!!
# GUI implementation directory variable
set(IMPLEMENT_UI_PATH ${SOURCE_PATH}/ui) #!!!

find_package(Qt5Core REQUIRED) #!!!

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    # project directories
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
#                    ${IMPLEMENT_UI_PATH}
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/tools/File.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/tools/cDecode.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/ResourcePage.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/ResourceMap.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/ResourcePackage.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/ResourceImpl.cpp
    # -----
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources/entity/Font.cpp
#    ${IMPLEMENT_UI_PATH}/MainFrame.cpp
)

#set(FORMS
#    ${UI_PATH}/MainFrame.ui
#    ${UI_PATH}/widgets/ResourceStack.ui
#)

#qt5_wrap_ui(UI_HEADERS ${FORMS})
#add_executable(${PROJECT_NAME} ${UI_HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})
qt5_use_modules(${PROJECT_NAME} Widgets)

include("${CMAKE_CURRENT_LIST_DIR}/precompile.cmake")
ADD_PRECOMPILED_HEADER(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h" FORCEINCLUDE)
