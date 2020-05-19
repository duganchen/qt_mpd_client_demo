cmake_minimum_required(VERSION 3.6)

project(qt_mpd_client_demo LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMPDCLIENT REQUIRED IMPORTED_TARGET libmpdclient)

add_executable(qt_mpd_client_demo
  controller.cpp
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mpdconnection.cpp
  mpdconnectionfactory.cpp
  connectionmanager.cpp
)

target_include_directories(qt_mpd_client_demo PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(qt_mpd_client_demo PRIVATE Qt5::Widgets PkgConfig::LIBMPDCLIENT)
