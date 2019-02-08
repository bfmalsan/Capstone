cmake_minimum_required(VERSION 2.8)
project( Capstone2d )

#####################################
# Function for finding packages (in particular for Windows)
#####################################

function(myFindPackage VARIABLE DEBUG_NAME RELEASE_NAME HINT_DIR)
	find_library(LIBRARY_RELEASE_${RELEASE_NAME} "${RELEASE_NAME}" HINTS ${HINT_DIR})
	find_library(LIBRARY_DEBUG_${DEBUG_NAME} "${DEBUG_NAME}" HINTS ${HINT_DIR})
	set(ALL_LIBRARY optimized ${LIBRARY_RELEASE_${RELEASE_NAME}} debug ${LIBRARY_DEBUG_${DEBUG_NAME}})	
	set(${VARIABLE} ${ALL_LIBRARY} PARENT_SCOPE)	
endfunction(myFindPackage)

#####################################
# Set whether we are on Linux
#####################################

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

#####################################
# Add directory to list of places CMake looks for config files
#####################################

if(WIN32)
	set(MASTER_DEPEND "" CACHE PATH "Set this to your directory for dependencies.")	
elseif(APPLE)
	set(MASTER_DEPEND "/usr/local")
endif()

set(CMAKE_PREFIX_PATH ${MASTER_DEPEND} ${CMAKE_PREFIX_PATH})

#####################################
# LIST OF DEPENDENCIES:
# - OpenGL
# - GLFW (static)
# - GLEW (static)
# - GLM
# - Assimp
# - stb_image
# - stb_image_write
#####################################

#####################################
# OpenGL
#####################################

find_package(OpenGL REQUIRED)

#####################################
# GLFW (static) 
#####################################

if(WIN32 AND NOT MINGW)	
	myFindPackage("GLFW_LIBRARY" "glfw3_d" "glfw3" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("GLFW_LIBRARY" "glfw3" "glfw3" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	set(GLFW_LIBRARY ${GLFW_STATIC_LIBRARIES})
endif()

#####################################
# GLEW (static) 
#####################################

if(WIN32 AND NOT MINGW)	
	myFindPackage("GLEW_LIBRARY" "libglew32d" "libglew32" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("GLEW_LIBRARY" "libglew32" "libglew32" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(GLEW REQUIRED)	
	set(GLEW_LIBRARY ${GLEW_LIBRARIES})
endif()

add_definitions(-DGLEW_STATIC)

#####################################
# Assimp 
#####################################

if(WIN32 AND NOT MINGW)	
	if(MSVC12)
		set(ASSIMP_MSVC_VERSION "-vc120")
	elseif(MSVC14)	
		set(ASSIMP_MSVC_VERSION "-vc140")
	endif(MSVC12)

	myFindPackage("ASSIMP_LIBRARY" "assimp${ASSIMP_MSVC_VERSION}-mt_d" "assimp${ASSIMP_MSVC_VERSION}-mt" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("ASSIMP_LIBRARY" "assimp" "assimp" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(assimp REQUIRED)
	set(ASSIMP_LIBRARY ${ASSIMP_LIBRARIES})
endif()

#####################################
# Require C++11
#####################################

if(WIN32)	
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(WIN32 AND MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(LINUX)	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")	
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")	
endif()



#####################################
# Include any includes and libraries we missed
#####################################

include_directories(${MASTER_DEPEND}/include)
link_directories(${MASTER_DEPEND}/lib)

#####################################
# Get general sources
#####################################

include_directories(".")

file(GLOB GENERAL_SOURCES
    "*.h"    
    "*.cpp"  
    "*.hpp"  
)

#####################################
# Set general libraries
#####################################

set(GENERAL_LIBRARIES ${OPENGL_LIBRARY} ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY})

#####################################
# Extra setup
#####################################

if(APPLE)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	set(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
	set(GENERAL_LIBRARIES ${GENERAL_LIBRARIES} ${EXTRA_LIBS})
endif()

if(NOT WIN32)
	set(GENERAL_LIBRARIES ${GENERAL_LIBRARIES} stdc++fs atomic)
elseif(WIN32 AND MINGW)
	set(GENERAL_LIBRARIES ${GENERAL_LIBRARIES} stdc++fs atomic)
endif()

#####################################
# Create executable
#####################################

# Create executable and link libraries
add_executable(Capstone2d ${GENERAL_SOURCES})
target_link_libraries(Capstone2d ${GENERAL_LIBRARIES})

#####################################
# Set install target 
#####################################

install(TARGETS Capstone2d RUNTIME DESTINATION bin)
