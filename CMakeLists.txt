cmake_minimum_required(VERSION 3.10...3.27)
project(tape-sorter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tmp)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(SOURCES
    src/config/config.cpp
    src/tape/file_tape.cpp
    src/sort/sorter.cpp
)

configure_file(
    ${CMAKE_SOURCE_DIR}/config.json
    ${CMAKE_BINARY_DIR}/config.json
    COPYONLY
)

add_executable(tape-sorter src/Main.cpp ${SOURCES})
target_include_directories(tape-sorter PRIVATE src)
target_link_libraries(tape-sorter PRIVATE nlohmann_json::nlohmann_json)

option(BUILD_TESTS "tests" OFF)
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()

    set(TEST_SOURCES
        tests/tape_tests.cpp
    )

    add_executable(tape_tests ${TEST_SOURCES} ${SOURCES})
    target_include_directories(tape_tests PRIVATE src)
    target_link_libraries(tape_tests PRIVATE nlohmann_json::nlohmann_json GTest::GTest GTest::Main)
    add_test(NAME SorterTest COMMAND tape_tests)
endif()