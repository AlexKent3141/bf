cmake_minimum_required (VERSION 3.16.3)
project(brainfuck_interpreter LANGUAGES C)

set(ARGPARSE_SHARED OFF CACHE BOOL "Only want the static version of argparse" FORCE)

include(FetchContent)

FetchContent_Declare(
  argparse
  GIT_REPOSITORY "https://github.com/cofyc/argparse.git"
  GIT_TAG master)

FetchContent_MakeAvailable(argparse)

# Looks like the argparse library doesn't set up its include directory.
target_include_directories(argparse
  PUBLIC
    ${argparse_SOURCE_DIR})

add_executable(bf
  main.c)

target_link_libraries(bf
  PRIVATE
    argparse)
