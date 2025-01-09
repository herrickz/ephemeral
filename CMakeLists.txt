cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(2DSideScroller)

set(NAME "2d_sidescroller")

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
SET(CMAKE_ALL_ERRORS_FATAL TRUE) 

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

INCLUDE_DIRECTORIES(
  /System/Library/Frameworks
  /opt/homebrew/include
)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

if(APPLE)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
  find_path(RT_AUDIO_INCLUDE_DIRS rtaudio/RtAudio.h PATHS ${CMAKE_SOURCE_DIR}/dependencies)
  find_library(RT_AUDIO_LIBRARIES rtaudio PATHS ${CMAKE_SOURCE_DIR}/lib)
elseif(APPLE)
  find_path(RT_AUDIO_INCLUDE_DIRS rtaudio/RtAudio.h PATHS /usr/local/include /opt/homebrew/include)
  find_library(RT_AUDIO_LIBRARIES rtaudio PATHS /usr/local/lib /opt/homebrew/lib)
endif(WIN32)

message(STATUS "Found RtAudio include directory: ${RT_AUDIO_INCLUDE_DIRS}")
message(STATUS "Found RtAudio library: ${RT_AUDIO_LIBRARIES}")

if(WIN32)
  find_path(MPG_INCLUDE_DIRS mpg123.h PATHS ${CMAKE_SOURCE_DIR}/dependencies)
  find_library(MPG_LIBRARIES mpg123 PATHS ${CMAKE_SOURCE_DIR}/lib)
elseif(APPLE)
  find_path(MPG_INCLUDE_DIRS mpg123.h PATHS /usr/local/include /opt/homebrew/include)
  find_library(MPG_LIBRARIES mpg123 PATHS /usr/local/lib /opt/homebrew/lib)
endif(WIN32)

message(STATUS "Found MPG123 include directory: ${MPG_INCLUDE_DIRS}")
message(STATUS "Found MPG123 library: ${MPG_LIBRARIES}")

if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype irrKlang mpg123 rtaudio)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES} ${RT_AUDIO_LIBRARIES} ${MPG_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
endif(WIN32)

# first create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

set(SOURCE
  "src/main.cpp"
  "src/ephemeral/Shader.cpp"
  "src/ephemeral/TexturedSquare.cpp"
  "src/ephemeral/Camera.cpp"
  "src/ephemeral/Player.cpp"
  "src/ephemeral/TextRender.cpp"
  "src/ephemeral/StaticLine.cpp"
  "src/ephemeral/Texture.cpp"
  "src/ephemeral/LevelLoader.cpp"
  "src/ephemeral/GameManager.cpp"
  "src/ephemeral/AudioManager.cpp"
)

add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})

if(MSVC)
  target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
  target_link_options(${NAME} PUBLIC /ignore:4099)
endif(MSVC)

file(GLOB DLLS "dlls/*.dll")
if(WIN32)
  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
endif(WIN32)

set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/")
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/")

include_directories(${CMAKE_SOURCE_DIR}/dependencies)
include_directories(${CMAKE_SOURCE_DIR}/include)
