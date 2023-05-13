cmake_minimum_required(VERSION 2.8)
PROJECT ( Stego )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( HideMessage storeMessage.cpp )
add_executable( RetrieveMessage getMessage.cpp )

target_link_libraries( HideMessage ${OpenCV_LIBS} )
target_link_libraries( RetrieveMessage ${OpenCV_LIBS} )