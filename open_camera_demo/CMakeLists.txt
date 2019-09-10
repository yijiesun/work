PROJECT(OpenCV_Example)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE( OpenCV REQUIRED )

# Declare the target (an executable)
ADD_EXECUTABLE(OpenCV_Example  opencv_test.cpp)

TARGET_LINK_LIBRARIES(OpenCV_Example ${OpenCV_LIBS})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")     //好了，就修改这么点东西，保存，关闭。
