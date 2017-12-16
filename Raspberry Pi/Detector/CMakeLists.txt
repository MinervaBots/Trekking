cmake_minimum_required(VERSION 2.8.9)
project(TrekkingDetection)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	list(APPEND ThirdParty_LIBS ${OpenCV_LIBS})
	include_directories(${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} ${SOURCES})


#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)