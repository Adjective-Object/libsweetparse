cmake_minimum_required (VERSION 3.4)

project (libsweetparse)
set (libsweetparse_VERSION_MAJOR 0)
set (libsweetparse_VERSION_MINOR 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# add libsweetparse to search path
include_directories(AFTER ./src)

set(CMAKE_C_FLAGS "-fPIC -g -Wall -std=c11")

##########################
# project source objects #
##########################
set(LIB_SRCS
        src/sweetexpressions.c
        src/sweetexpressions.h
        src/sweetexpressions/charclasses.c
        src/sweetexpressions/charclasses.h
        src/sweetexpressions/datatypes.c
        src/sweetexpressions/datatypes.h
        src/sweetexpressions/parser.c
        src/sweetexpressions/parser.h
        src/sweetexpressions/stream.c
        src/sweetexpressions/stream.h)
add_library(SWEET_LIB OBJECT ${LIB_SRCS})


####################
# output libraries #
####################
add_library(sweetparse-shared
    SHARED $<TARGET_OBJECTS:SWEET_LIB>)
set_target_properties(sweetparse-shared
    PROPERTIES OUTPUT_NAME "sweetparse")

add_library(sweetparse-static
    STATIC $<TARGET_OBJECTS:SWEET_LIB>)
set_target_properties(sweetparse-static
    PROPERTIES OUTPUT_NAME "sweetparse")


###########
# testing #
###########
add_executable(sweet_test
    tests/sweet_test.c
    $<TARGET_OBJECTS:SWEET_LIB>)
set_target_properties(sweet_test
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./tests)

add_custom_target(run_tests 
    COMMAND ./run_tests.sh
    WORKING_DIRECTORY tests
    DEPENDS sweet_test)


########################################
# Code formatting and linting commands #
########################################
add_custom_target(lint
    COMMAND cppcheck 
        --std=c11
        --enable=warning
        --enable=missingInclude
        --enable=performance
        -I ./src 
        ${LIB_SRCS})

add_custom_target(format
    COMMAND clang-format -i ${LIB_SRCS})
    

