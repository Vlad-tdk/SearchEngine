cmake_minimum_required(VERSION 3.29)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

if(MINGW)
    set(gtest_disable_pthreads on)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/headers)

add_subdirectory(nlohmann_json)

set(SOURCE_FILES
        main.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

add_executable(SearchEngine ${SOURCE_FILES})

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)

add_custom_command(TARGET SearchEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying requests.json and config.json to binary directory"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/requests.json
        ${CMAKE_BINARY_DIR}/requests.json
)

add_custom_command(TARGET SearchEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying config.json to binary directory"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config.json
        ${CMAKE_BINARY_DIR}/config.json
)

add_executable(TestRunner
        tests/test_main.cpp
        tests/test_inverted_index.cpp
        tests/test_search_server.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

target_link_libraries(TestRunner PRIVATE gtest_main nlohmann_json::nlohmann_json)

enable_testing()

include(GoogleTest)
gtest_discover_tests(TestRunner)
