cmake_minimum_required(VERSION 2.8)
project( pose_estimation2d2d_demo)

# OpenCV
find_package( OpenCV REQUIRED )

#
include_directories(
 include
${OpenCV_INCLUDE_DIRS}
)


add_executable( pose_estimation2d2d_demo ./pose_estimation.cpp )
target_link_libraries( pose_estimation2d2d_demo ${OpenCV_LIBS} )
