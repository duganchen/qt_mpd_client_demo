cmake_minimum_required(VERSION 3.6)
project(qt_mpd_client_demo LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Test REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMPDCLIENT REQUIRED IMPORTED_TARGET libmpdclient)

add_executable(qt_mpd_client_demo
  main.cpp
  mainwindow.cpp
  mainwindow.h
)

add_subdirectory(Controller)
target_link_libraries(qt_mpd_client_demo PUBLIC Controller PRIVATE Qt5::Widgets PkgConfig::LIBMPDCLIENT)
target_include_directories(qt_mpd_client_demo PUBLIC "${PROJECT_SOURCE_DIR}/Controller" PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})


enable_testing()

file(COPY test_resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(connectionTest tst_testcontroller.cpp)
add_test(NAME connectionTest COMMAND connectionTest)
target_include_directories(connectionTest PUBLIC "${PROJECT_SOURCE_DIR}/Controller" PRIVATE Qt5::Test ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(connectionTest PUBLIC Controller PRIVATE Qt5::Widgets PkgConfig::LIBMPDCLIENT Qt5::Test)

