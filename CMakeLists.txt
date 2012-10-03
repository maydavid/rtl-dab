cmake_minimum_required(VERSION 2.6)
project(rtl-dab C)


ADD_DEFINITIONS(-Wall)
ADD_DEFINITIONS(-Wextra)
ADD_DEFINITIONS(-Wno-unused)
ADD_DEFINITIONS(-Wsign-compare)
#ADD_DEFINITIONS(-g)
#ADD_DEFINITIONS(-pg)

#http://gcc.gnu.org/wiki/Visibility
add_definitions(-fvisibility=hidden)

find_package(Threads)
#find_package(FFTW)


include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

