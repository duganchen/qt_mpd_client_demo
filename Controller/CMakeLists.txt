cmake_minimum_required(VERSION 3.6)

project(Controller LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMPDCLIENT REQUIRED IMPORTED_TARGET libmpdclient)

add_library(Controller SHARED
  controller.cpp
)

target_include_directories(Controller PRIVATE ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(Controller PRIVATE Qt5::Widgets Qt5::Network PkgConfig::LIBMPDCLIENT)
target_compile_definitions(Controller PRIVATE CONTROLLER_LIBRARY)
