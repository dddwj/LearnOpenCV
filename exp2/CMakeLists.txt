cmake_minimum_required(VERSION 3.12)
project(exp2)

# 默认为3.4.2版本 地址：/Users/dddwj/anaconda3/share/OpenCV/OpenCVConfig.cmake  由Anaconda安装
# 如果要用2.4.13版本，需要设置以下路径。/usr/local/opt/opencv@2/share/OpenCV/OpenCVConfig.cmake   由Homebrew安装
set(OpenCV_DIR /usr/local/opt/opencv@2/share/OpenCV)
find_package(OpenCV 2.4.13 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(exp2 main.cpp)
add_executable(projection projection.cpp)

target_link_libraries(exp2 ${OpenCV_LIBS})
target_link_libraries(projection ${OpenCV_LIBS})

MESSAGE(STATUS "OpenCV Version is " ${OpenCV_LIBS})