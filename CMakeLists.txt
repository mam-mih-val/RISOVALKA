cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(flow_drawing_tools)
set(CMAKE_CXX_FLAGS "-Wall")
set (QnDrawer_VERSION_MAJOR 1)
set (QnDrawer_VERSION_MINOR 0)
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

list(APPEND CMAKE_PREFIX_PATH $ENV{Qn})

find_package(Qn REQUIRED)

include_directories(
        QnDrawer/include
        ${CMAKE_SOURCE_DIR}
        ${Base_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}

)
set(SOURCES
        src/file_manager.cc
        src/canvas_manager.cc
        src/correlation_manager.cc
        src/flow_method.cc
        src/rnd_not_ext_method.cc
        src/computation_manager.cc
        )

add_executable(draw src/draw.cc ${SOURCES})
target_link_libraries(draw Qn::Base ${ROOT_LIBRARIES} ${Boost_PROPERTY_TREE} )

add_executable(flow src/flow.cc ${SOURCES})
target_link_libraries(flow Qn::Base ${ROOT_LIBRARIES} ${Boost_PROPERTY_TREE} )
