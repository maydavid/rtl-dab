cmake_minimum_required(VERSION 2.6)
project(rtl-dab C)


ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-O3)
ADD_DEFINITIONS(-Wextra)
ADD_DEFINITIONS(-Wno-unused)
ADD_DEFINITIONS(-Wsign-compare)
ADD_DEFINITIONS(-g)
ADD_DEFINITIONS(-pg)

#http://gcc.gnu.org/wiki/Visibility
add_definitions(-fvisibility=hidden)

find_package(Threads)
#find_package(FFTW)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

