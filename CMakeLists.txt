cmake_minimum_required(VERSION 3.31.6)

project(gr-ini VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME})
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

option(GR_BUILD_TEST "Build test" ON)

set(source
    ${CMAKE_SOURCE_DIR}/src/config.c
)

add_library(gr-ini SHARED ${source})

if(GR_BUILD_TEST)
    add_subdirectory(test)

    target_include_directories(gr-test PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

    target_link_libraries(gr-test PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/lib${PROJECT_NAME}.so
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


