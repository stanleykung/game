cmake_minimum_required(VERSION 3.4.5)
project( randomdirection_collisionball )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( randomdirection_collisionball randomdirection_collisionball.cpp )
target_link_libraries( randomdirection_collisionball ${OpenCV_LIBS} )
