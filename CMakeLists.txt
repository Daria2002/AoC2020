cmake_minimum_required(VERSION 3.10)

project(aoc2020)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB_RECURSE aoc2020_SOURCES src/*.cpp )
file(GLOB_RECURSE aoc2020_HEADERS include/*.hpp)

include_directories(include)

foreach( testsourcefile ${aoc2020_SOURCES} )

    get_filename_component(testname ${testsourcefile} NAME)
    string( REPLACE ".cpp" "" testname ${testname} )

    add_executable( ${testname} ${testsourcefile} )
    target_compile_features(${testname} PRIVATE cxx_std_17)
    target_link_libraries(${testname} Threads::Threads mysqlcppconn)

endforeach( testsourcefile ${aoc2020_SOURCES} )