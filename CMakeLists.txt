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
        ${QnTools_INCLUDE_DIR}
)

#add_executable(draw src/main.cc src/draw_functions.cc  src/json_reader.cc src/file_manager.cc)
#target_link_libraries(draw QnTools::Base ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
#
set(SOURCES
        draw/readable_object.cpp
        draw/drawable_object.cpp
        draw/graph.cpp
        draw/histogram_1_d.cpp
        draw/histogram_2_d.cpp
        draw/correlation.cpp
        draw/picture.cpp
        draw/picture_2_d.cpp
        draw/heap_picture.cpp
        draw/ratio.cpp
        draw/dv1dy_container.cpp
        draw/double_differential_correlation.cpp
        draw/double_differential_histogram.cpp
        draw/multi_correlation.cpp
        )
set(HEADERS
        draw/readable_object.h
        draw/drawable_object.h
        draw/graph.h
        draw/histogram_1_d.h
        draw/histogram_2_d.h
        draw/correlation.h
        draw/picture.h
        draw/picture_2_d.h
        draw/heap_picture.h
        draw/ratio.h
        draw/dv1dy_container.h
        draw/double_differential_correlation.h
        draw/double_differential_histogram.h
        draw/multi_correlation.h
        )

set(PCM_FILE_NAME libDraw)
message(STATUS "QnTools include ${QnTools_INCLUDE_DIR}")
ROOT_GENERATE_DICTIONARY( G__Draw ${HEADERS}
        LINKDEF
        draw/DrawLinkDef.h
        OPTIONS
#        -s ${PCM_FILE_NAME}
        -I ${QnTools_INCLUDE_DIR}/QnTools
        -I ${CMAKE_SOURCE_DIR}
        )
add_library(Draw SHARED ${SOURCES} G__Draw.cxx )
target_link_libraries(Draw PRIVATE QnTools::Base ${ROOT_LIBRARIES})

add_executable(draw_picture draw/draw_picture.cpp)
target_link_libraries(draw_picture QnTools::Base ${ROOT_LIBRARIES} Draw )