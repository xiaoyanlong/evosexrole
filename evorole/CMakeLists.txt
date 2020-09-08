#==============================================================================
# evorole CMake file
#============================================================================== 
cmake_minimum_required(VERSION 3.10)

project(evorole)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
if (WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "-DNOMINMAX -DNDEBUG ${RELEASE_WARNING_FLAGS} ${WARNING_FLAGS} ${OTHER_FLAGS} /Oi /GL /fp:fast")
else()
	set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG ${RELEASE_WARNING_FLAGS} ${WARNING_FLAGS} ${OTHER_FLAGS} -O2 -ffast-math -funroll-loops")
endif()

add_subdirectory(src)
