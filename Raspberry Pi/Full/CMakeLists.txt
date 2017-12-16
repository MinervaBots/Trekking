cmake_minimum_required(VERSION 2.8.9)
project(TrekkingFull)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	list(APPEND ThirdParty_LIBS ${OpenCV_LIBS})
	include_directories(${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

# Inclui os headers (.h, .hpp)
include_directories(include)

#For the shared library:
set (PROJECT_LINK_LIBS libTrekkingDetection.so)
link_directories(../Detector/build)
 
#For the static library:
#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )
 
include_directories(../Detector/include)
 
file(GLOB SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})