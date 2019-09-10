PROJECT(cam_cap)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(SRC 
 ${PROJECT_SOURCE_DIR}/src/functions.cpp
 ${PROJECT_SOURCE_DIR}/src/cam_cap.cpp
 ${PROJECT_SOURCE_DIR}/src/cam_cap.hpp
 ${PROJECT_SOURCE_DIR}/src/SqQueue.cpp
 ${PROJECT_SOURCE_DIR}/src/SqQueue.hpp
)


FIND_PACKAGE( OpenCV REQUIRED )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
# Declare the target (an executable)

ADD_EXECUTABLE(cam_cap  ${SRC})

TARGET_LINK_LIBRARIES(cam_cap ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
