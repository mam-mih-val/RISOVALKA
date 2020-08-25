cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(flow_drawing_tools)
set(CMAKE_CXX_FLAGS "-Wall")
set (QnDrawer_VERSION_MAJOR 0)
set (QnDrawer_VERSION_MINOR 9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE debug)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package( Boost REQUIRED COMPONENTS program_options regex )
find_package(ROOT REQUIRED)

if (NOT ROOT_FOUND)
    message(WARNING "ROOT was not found with CMAKE_PREFIX_PATH...")
    list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
    find_package(ROOT REQUIRED)
endif ()

if (ROOT_USE_FILE)
    message("-- ROOT_USE_FILE ${ROOT_USE_FILE}")
    include(${ROOT_USE_FILE})
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{QnTools})

find_package(QnTools REQUIRED)

include_directories(
        QnDrawer/include
        ${CMAKE_SOURCE_DIR}
        ${Base_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
)

add_executable(draw src/main.cc src/draw_functions.cc  src/json_reader.cc src/file_manager.cc)
target_link_libraries(draw QnTools::Base ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
