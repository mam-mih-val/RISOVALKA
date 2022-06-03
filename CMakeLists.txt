cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(RISOVALKA)
set (RISOVALKA_VERSION_MAJOR 1)
set (RISOVALKA_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE debug)

find_package( Boost REQUIRED COMPONENTS program_options)
find_package(ROOT REQUIRED)
find_package(QnTools REQUIRED)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${ROOT_INCLUDE_DIRS}
        ${QnTools_INCLUDE_DIR}/QnTools
)

message(STATUS ${QnTools_INCLUDE_DIR})

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
        draw/palette.cpp
        draw/book.cpp
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
        draw/palette.h
        draw/book.h
        )

set(PCM_FILE_NAME libDraw)
ROOT_GENERATE_DICTIONARY(G__Draw ${HEADERS}
        LINKDEF
        draw/DrawLinkDef.h
        OPTIONS
        -I ${QnTools_INCLUDE_DIR}/QnTools
        -I ${CMAKE_SOURCE_DIR}
        )
add_library(Draw SHARED ${SOURCES} G__Draw.cxx )
target_link_libraries(Draw PRIVATE QnTools::Base ROOT::RIO)

add_executable(PNCOBATb draw/draw_picture.cpp)
target_link_libraries(PNCOBATb QnTools::Base ROOT::RIO Draw)