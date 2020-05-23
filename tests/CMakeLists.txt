cmake_minimum_required(VERSION 3.6)
project(qt_mpd_client_demo LANGUAGES CXX)

enable_testing()

file(COPY test_resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(connectionTest tst_testcontroller.cpp)
add_test(NAME connectionTest COMMAND connectionTest)
target_include_directories(connectionTest PUBLIC "${PROJECT_SOURCE_DIR}/Controller" PRIVATE Qt5::Test ${LIBMPDCLIENT_INCLUDE_DIRS})
target_link_libraries(connectionTest PUBLIC Controller PRIVATE Qt5::Widgets PkgConfig::LIBMPDCLIENT Qt5::Test)

