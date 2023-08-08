cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(RISOVALKA)
set (RISOVALKA_VERSION_MAJOR 1)
set (RISOVALKA_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE debug)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(ROOT REQUIRED)
find_package(QnTools QUIET)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${QnTools_INCLUDE_DIR}/QnTools)


if(QnTools_FOUND)
    message(STATUS ${QnTools_INCLUDE_DIR})
    include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${QnTools_INCLUDE_DIR}/QnTools)
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
        draw/file_manager.cc
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
        draw/file_manager.h
            )
else()
    message( "QnTools is not found and the project will be compiled without related to drawing correlations" )
    include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} )
    set(SOURCES
            draw/readable_object.cpp
            draw/drawable_object.cpp
            draw/graph.cpp
            draw/histogram_1_d.cpp
            draw/histogram_2_d.cpp
            draw/picture.cpp
            draw/picture_2_d.cpp
            draw/heap_picture.cpp
            draw/ratio.cpp
            draw/palette.cpp
            draw/book.cpp
            )
    set(HEADERS
            draw/readable_object.h
            draw/drawable_object.h
            draw/graph.h
            draw/histogram_1_d.h
            draw/histogram_2_d.h
            draw/picture.h
            draw/picture_2_d.h
            draw/heap_picture.h
            draw/ratio.h
            draw/double_differential_histogram.h
            draw/palette.h
            draw/book.h
            )
endif()

set(PCM_FILE_NAME libDraw)
add_library(Draw SHARED ${SOURCES} G__Draw.cxx )

if(QnTools_FOUND)
    ROOT_GENERATE_DICTIONARY(G__Draw ${HEADERS} LINKDEF draw/DrawLinkDef.h OPTIONS -DUSE_QN_TOOLS)
    target_link_libraries(Draw PUBLIC QnTools::Base ROOT::RIO ROOT::ProofBench ROOT::Postscript ROOT::ASImage ROOT::Graf ROOT::Ged)
    target_compile_definitions(Draw PUBLIC -DUSE_QN_TOOLS)
else()
    ROOT_GENERATE_DICTIONARY(G__Draw ${HEADERS} LINKDEF draw/DrawLinkDef.h)
    target_link_libraries(Draw PUBLIC ROOT::RIO ROOT::ProofBench ROOT::Postscript)
endif()

add_executable(PNCOBATb draw/draw_picture.cpp)
target_link_libraries(PNCOBATb ROOT::RIO Draw)