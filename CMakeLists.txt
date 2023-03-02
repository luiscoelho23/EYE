cmake_minimum_required(VERSION 3.14)

project(project_opengl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3")
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )

add_executable(project_opengl
  main.cpp
  controlImage.cpp
  controls.cpp
  camerawindow.cpp
)
target_link_libraries(project_opengl Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(project_opengl Qt5::Widgets)
target_link_libraries(project_opengl ${OpenCV_LIBS} )
target_link_libraries(project_opengl Threads::Threads)
