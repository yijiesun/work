PROJECT(save_cam)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.1)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE( OpenCV REQUIRED )

# Declare the target (an executable)
ADD_EXECUTABLE(save_cam  save_cam.cpp)

TARGET_LINK_LIBRARIES(save_cam ${OpenCV_LIBS})

