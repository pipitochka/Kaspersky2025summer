cmake_minimum_required(VERSION 3.10)
project(mangen C)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        main.c
        src/hash.c
        src/directory_utils.c
)

add_executable(mangen ${SOURCES})

enable_testing()

add_executable(tests test/test.c)

add_test(NAME tests COMMAND tests)
